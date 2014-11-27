#include "nzgs.h"

int main(int argc, char *argv[])
{
    nz::Application app(argc, argv);

    nz::Application::addLog("");
    nz::Application::addLog("--------------------------------------");
    nz::Application::addLog("NullZen Team - GameServer");
    nz::Application::addLog("--------------------------------------");
    nz::Application::addLog("@ http://github.com/nzteam");
    nz::Application::addLog("--------------------------------------");
    nz::Application::addLog("");

    nz::Configuration::loadFile("config.ini");
    nz::Configuration::loadFile("data/common.ini");

    // == Database == //
    std::string hostname = nz::Configuration::getString("DATABASE", "hostname", "127.0.0.1:3306");
    std::string username = nz::Configuration::getString("DATABASE", "username", "root");
    std::string password = nz::Configuration::getString("DATABASE", "password", "");
    std::string database = nz::Configuration::getString("DATABASE", "database", "nzserver");
    nz::Database::getInstance().start(hostname, username, password, database);

    // == GameServer == //
    std::string address = nz::Configuration::getString("GAMESERVER", "address", "0.0.0.0");
    int port = nz::Configuration::getInt("GAMESERVER", "port", 55901);
    int maxclients = nz::Configuration::getInt("GAMESERVER", "maxclients", 5);
    int timeout = nz::Configuration::getInt("GAMESERVER", "timeout", 30);
    GameServer::getInstance().start(address, port, maxclients, timeout);

    return app.exec();
}
