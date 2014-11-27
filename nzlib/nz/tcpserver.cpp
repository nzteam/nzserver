#include "tcpserver.h"

namespace nz {

void TCPServer::before() {}
void TCPServer::after() {}
void TCPServer::onClientConnect(int id) {}
void TCPServer::onClientDisconnect(int id) {}
void TCPServer::onClientWrite(int id, unsigned char* buffer, int size) {}

void TCPServer::start(std::string address, int port, int maxClients, int timeout)
{
    this->m_address = address;
    this->m_port = port;
    this->m_maxClients = maxClients;
    this->m_timeout = timeout;

    this->before();

    std::thread serverThread(&TCPServer::processServer, this);
    if(serverThread.joinable()) serverThread.detach();
}

void TCPServer::processServer()
{
    Application::addLog("[TCPServer] Starting server (ADDR: %s | PORT: %d | MAX: %d | TIMEOUT: %d)", this->m_address.c_str(), this->m_port, this->m_maxClients, this->m_timeout);

    try
    {
        struct sockaddr_in serverAddr;
        int clientSocket = -1;
        std::thread* clientThread;

        this->m_socket = socket(AF_INET, SOCK_STREAM, 0);

        if(this->m_socket < 0)
        {
            throw std::runtime_error("Can not create socket()");
        }

        memset((char*)&serverAddr, 0x00, sizeof(serverAddr));

        serverAddr.sin_family = AF_INET;

        if(this->m_address == "0.0.0.0")
        {
            serverAddr.sin_addr.s_addr = INADDR_ANY;
        }
        else
        {
            serverAddr.sin_addr.s_addr = inet_addr(this->m_address.c_str());
        }

        serverAddr.sin_port = htons(this->m_port);

        if(bind(this->m_socket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
        {
            throw std::runtime_error("Can not bind()");
        }

        if(listen(this->m_socket, this->m_maxClients) < 0)
        {
            throw std::runtime_error("Can not listen()");
        }

        Application::addLog("[TCPServer] Listening...");

        while(this->m_socket > 0)
        {
            clientSocket = accept(this->m_socket, NULL, 0);

            if(clientSocket > 0)
            {
                clientThread = new std::thread(&TCPServer::processClient, this, clientSocket);
                if(clientThread->joinable()) clientThread->detach();
            }
        }

        Application::addLog("[TCPServer] Working finished");
    }
    catch(std::exception &e)
    {
        Application::addLog("[TCPServer] Error: %s", e.what());
    }

    this->after();

    Application::getInstance()->close();
}

void TCPServer::processClient(int socket)
{
    struct sockaddr_storage socketAddr;
    socklen_t socketSize = sizeof(socketAddr);
    char socketIP[INET6_ADDRSTRLEN];
    int socketPort;
    int socketID;

    getpeername(socket, (struct sockaddr*)&socketAddr, &socketSize);

    if (socketAddr.ss_family == AF_INET)
    {
        struct sockaddr_in *s = (struct sockaddr_in *)&socketAddr;
        socketPort = ntohs(s->sin_port);
        inet_ntop(AF_INET, &s->sin_addr, socketIP, sizeof(socketIP));
    }
    else
    {
        struct sockaddr_in6 *s = (struct sockaddr_in6 *)&socketAddr;
        socketPort = ntohs(s->sin6_port);
        inet_ntop(AF_INET6, &s->sin6_addr, socketIP, sizeof(socketIP));
    }

    if((int)this->m_clients.size() < this->m_maxClients)
    {
        for(int id = 1; id <= ((int)this->m_clients.size() + 1); id++)
        {
            std::map<int, ClientInfo*>::iterator it = this->m_clients.find(id);

            if(it == this->m_clients.end())
            {
                this->m_clients[id] = new ClientInfo;
                this->m_clients[id]->id = id;
                this->m_clients[id]->socket = socket;
                socketID = id;
                break;
            }
        }
    }
    else
    {
        Application::addLog("[TCPServer] Max clients reached! (ADDR: %s | PORT: %d)", socketIP, socketPort);
        this->closeSocket(socket);
        return;
    }

    Application::addLog("[TCPServer] Client connected (ID: %d | ADDR: %s | PORT: %d)", socketID, socketIP, socketPort);

    if(this->m_timeout > 0)
    {
        struct timeval tv;
        tv.tv_sec = this->m_timeout;
        tv.tv_usec = 0;
        setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval));
    }

    this->onClientConnect(socketID);

    int socketStatus;
    char bufferRecv[7024];

    while(true)
    {
        memset(bufferRecv, 0x00, sizeof(bufferRecv));
        socketStatus = recv(socket, bufferRecv, sizeof(bufferRecv), 0);

        if(socketStatus > 0)
        {
            unsigned char buffer[socketStatus];
            memcpy(buffer, bufferRecv, socketStatus);

            this->onClientWrite(socketID, buffer, socketStatus);
        }
        else
        {
            break;
        }
    }

    this->onClientDisconnect(socketID);

    this->closeClient(socketID); // to be sure

    if(socketStatus == 0)
    {
        Application::addLog("[TCPServer] Client disconnected (ID: %d | ADDR: %s | PORT: %d)", socketID, socketIP, socketPort);
    }
    else
    {
        Application::addLog("[TCPServer] Client disconnected by server (ID: %d | ADDR: %s | PORT: %d)", socketID, socketIP, socketPort);
    }

    std::map<int, ClientInfo*>::iterator it = this->m_clients.find(socketID);

    if(it != this->m_clients.end())
    {
        this->m_clients.erase(it);
    }
}

void TCPServer::closeSocket(int socket)
{
    if(socket > 0)
    {
        close(socket);
    }
}

void TCPServer::closeClient(int id)
{
    std::map<int, ClientInfo*>::iterator it = this->m_clients.find(id);

    if(it != this->m_clients.end())
    {
        this->closeSocket(this->m_clients[id]->socket);
    }
}

void TCPServer::writeToClient(int id, unsigned char* buffer, int size)
{
    std::map<int, ClientInfo*>::iterator it = this->m_clients.find(id);

    if(it != this->m_clients.end())
    {
        write(this->m_clients[id]->socket, buffer, size);
    }
}

void TCPServer::writeToAll(unsigned char* buffer, int size)
{
    for(std::map<int, ClientInfo*>::iterator it = this->m_clients.begin(); it != this->m_clients.end(); it++)
    {
        write(it->second->socket, buffer, size);
    }
}

}
