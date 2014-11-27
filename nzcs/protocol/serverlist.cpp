#include "serverlist.h"

namespace protocol {

std::map<int, ServerList::ServerInfo*> ServerList::s_servers;

void ServerList::bindUpdate(int interval)
{
    std::list<std::string> args;
    args.push_back("SELECT * FROM serverlist");

    while(true)
    {
        nz::Database::getInstance().query(args, std::bind(ServerList::doUpdate, std::placeholders::_1));
        std::this_thread::sleep_for(std::chrono::seconds(interval));
    }
}

void ServerList::doUpdate(sql::ResultSet* result)
{
    ServerList::s_servers.clear();

    int srvCode = 0;

    while(result->next())
    {
        ServerList::ServerInfo* info = new ServerList::ServerInfo;
        info->hash = result->getString("hash");
        info->address = result->getString("address");
        info->port = result->getInt("port");
        info->visible = result->getBoolean("visible");
        info->preparing = result->getBoolean("preparing");
        info->usage = 50; // default 50%, cause @todo update usage...

        srvCode = (result->getInt("gid") * 20) + result->getInt("sid");

        ServerList::s_servers[srvCode] = info;
    }

    nz::Application::addLog("[ServerList] List updated!");
}

void ServerList::doUpdateFromFile(std::string filePath)
{
    nz::Application::addLog("[ServerList] List updated from file: %s", filePath.c_str());
}

void ServerList::getList(int id, nz::MuPacket packet)
{
    packet.prepare(0xC2, 0xF4, 0x06);

    packet.getBody()->writeUShort(ServerList::s_servers.size()); // Count

    // SERVERS //
    for(std::map<int, ServerList::ServerInfo*>::iterator it = ServerList::s_servers.begin(); it != ServerList::s_servers.end(); it++)
    {
        if(it->second->visible == false) continue;

        packet.getBody()->writeShort(it->first);
        if(it->second->preparing == false)
        {
            packet.getBody()->writeShort(it->second->usage);
        }
        else
        {
            packet.getBody()->writeShort(255);
        }
    }

    ConnectServer::getInstance().writeToClient(id, packet.getBuffer(), packet.getSize());
}

void ServerList::getServer(int id, nz::MuPacket packet)
{
    int srvCode = (int)packet.getBody()->readByte();
    int srvID = srvCode % 20;
    int srvGroup = srvCode / 20;

    if(ServerList::s_servers.find(srvCode) != ServerList::s_servers.end())
    {
        nz::Application::addLog("[ServerList] getServer() (ID: %d | SRVID: %d | SRVGROUP: %d)", id, srvID, srvGroup);

        packet.prepare(0xC1, 0xF4, 0x03);

        packet.getBody()->writeString(ServerList::s_servers[srvCode]->address, 16);
        packet.getBody()->writeShort(ServerList::s_servers[srvCode]->port);
    }
    else
    {
        nz::Application::addLog("[ServerList] getServer() - no exists (ID: %d | SRVID: %d | SRVGROUP: %d)", id, srvID, srvGroup);
    }

    ConnectServer::getInstance().writeToClient(id, packet.getBuffer(), packet.getSize());
}

}
