#ifndef CONNECTSERVER_H
#define CONNECTSERVER_H

#include <nz/application.h>
#include <nz/tcpserver.h>
#include <nz/mupacket.h>
#include <nz/configuration.h>
#include "protocol/serverlist.h"

class ConnectServer : public nz::TCPServer
{

private:
    ConnectServer() {}
    ConnectServer(ConnectServer const&);
    ConnectServer operator=(ConnectServer const&);

public:
    static ConnectServer& getInstance()
    {
        static ConnectServer instance;
        return instance;
    }

private:
    void before();
    void after();
    void onClientConnect(int id);
    void onClientDisconnect(int id);
    void onClientWrite(int id, unsigned char* buffer, int size);

};

#endif // CONNECTSERVER_H
