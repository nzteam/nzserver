#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <nz/application.h>
#include <nz/tcpserver.h>
#include <nz/mupacket.h>
#include <nz/configuration.h>

class GameServer : public nz::TCPServer
{

private:
    GameServer() {}
    GameServer(GameServer const&);
    GameServer operator=(GameServer const&);

public:
    static GameServer& getInstance()
    {
        static GameServer instance;
        return instance;
    }

private:
    std::string m_gameServerVersion;

    void before();
    void after();
    void onClientConnect(int id);
    void onClientDisconnect(int id);
    void onClientWrite(int id, unsigned char* buffer, int size);

};

#endif // GAMESERVER_H_H
