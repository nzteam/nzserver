#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "application.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <map>

namespace nz {

class TCPServer
{

    struct ClientInfo
    {
        int id;
        int socket;

        ClientInfo()
        {
            id = 0;
            socket = -1;
        }
    };

private:
    std::string m_address;
    int m_port;
    int m_maxClients;
    int m_timeout;
    int m_socket;
    std::map<int, ClientInfo*> m_clients;

    void processServer();
    void processClient(int socket);
    virtual void before();
    virtual void after();
    virtual void onClientConnect(int id);
    virtual void onClientDisconnect(int id);
    virtual void onClientWrite(int id, unsigned char* buffer, int size);

public:
    void start(std::string address, int port, int maxClients, int timeout);
    void closeSocket(int socket);
    void closeClient(int id);
    void writeToClient(int id, unsigned char* buffer, int size);
    void writeToAll(unsigned char* buffer, int size);

};

}

#endif // TCPSERVER_H
