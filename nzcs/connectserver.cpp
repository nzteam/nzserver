#include "connectserver.h"

void ConnectServer::before()
{
    std::string updateType = nz::Configuration::getString("CONNECTSERVER", "updatetype", "file");

    if(updateType == "file")
    {
        std::string serverListFilePath = nz::Configuration::getString("CONNECTSERVER", "serverlist", "data/serverlist.lst");
        std::thread serverListUpdateThread(protocol::ServerList::doUpdateFromFile, serverListFilePath);
        if(serverListUpdateThread.joinable()) serverListUpdateThread.detach();
    }
    else if(updateType == "database")
    {
        int interval = nz::Configuration::getInt("CONNECTSERVER", "updateinterval", 60);
        std::thread serverListUpdateThread(protocol::ServerList::bindUpdate, interval);
        if(serverListUpdateThread.joinable()) serverListUpdateThread.detach();
    }
    else
    {
        nz::Application::addLog("[TCPServer] Invalid update type: %s! Can not read server list!", updateType.c_str());
    }
}

void ConnectServer::after()
{
    //
}

void ConnectServer::onClientConnect(int id)
{
    nz::MuPacket packet;
    packet.prepare(0xC1, 0x00, 0x01);
    this->writeToClient(id, packet.getBuffer(), packet.getSize());
}

void ConnectServer::onClientDisconnect(int id)
{
    //
}

void ConnectServer::onClientWrite(int id, unsigned char* buffer, int size)
{
    nz::MuPacket packet;

    if(packet.explain(nz::ByteBuffer(buffer, size)))
    {
        if(packet.getProtocol() == 0xF4 && packet.getSubprotocol() == 0x06)
        {
            protocol::ServerList().getList(id, packet);
        }
        else if(packet.getProtocol() == 0xF4 && packet.getSubprotocol() == 0x03)
        {
            protocol::ServerList().getServer(id, packet);
        }
        else
        {
            nz::Application::addLog("[TCPServer] Client try use unknown method! (ID: %d | PROTOCOL: %02X | SUBPROTOCOL: %02X)", id, packet.getProtocol(), packet.getSubprotocol());
        }
    }
    else
    {
            nz::Application::addLog("[TCPServer] Client send invalid packet! (ID: %d | SIZE: %d)", id, size);
            this->closeClient(id);
    }
}
