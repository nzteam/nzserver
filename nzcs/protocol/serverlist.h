#ifndef SERVERLIST_H
#define SERVERLIST_H

#include <nz/application.h>
#include <nz/mupacket.h>
#include <nz/database.h>
#include "../connectserver.h"
#include <map>

namespace protocol {

class ServerList
{

    struct ServerInfo
    {
        std::string hash;
        std::string address;
        int port;
        bool visible;
        bool preparing;
        int usage;
    };

private:
    static std::map<int, ServerInfo*> s_servers;

public:
    static void bindUpdate(int interval);
    static void doUpdate(sql::ResultSet* result);
    static void doUpdateFromFile(std::string filePath);
    void getList(int id, nz::MuPacket packet);
    void getServer(int id, nz::MuPacket packet);

};

}

#endif // SERVERLIST_H
