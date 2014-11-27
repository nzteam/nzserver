#include "nzcs.h"

int main(int argc, char *argv[])
{
    nz::Application app(argc, argv);

    nz::Application::addLog("");
    nz::Application::addLog("--------------------------------------");
    nz::Application::addLog("NullZen Team - ConnectServer");
    nz::Application::addLog("--------------------------------------");
    nz::Application::addLog("@ http://github.com/nzteam");
    nz::Application::addLog("--------------------------------------");
    nz::Application::addLog("");

    nz::Configuration::loadFile("config.ini");

    // == Database == //
    std::string hostname = nz::Configuration::getString("DATABASE", "hostname", "127.0.0.1:3306");
    std::string username = nz::Configuration::getString("DATABASE", "username", "root");
    std::string password = nz::Configuration::getString("DATABASE", "password", "");
    std::string database = nz::Configuration::getString("DATABASE", "database", "nzserver");
    nz::Database::getInstance().start(hostname, username, password, database);

    // == ConnectServer == //
    std::string address = nz::Configuration::getString("CONNECTSERVER", "address", "0.0.0.0");
    int port = nz::Configuration::getInt("CONNECTSERVER", "port", 44405);
    int maxclients = nz::Configuration::getInt("CONNECTSERVER", "maxclients", 5);
    int timeout = nz::Configuration::getInt("CONNECTSERVER", "timeout", 30);
    ConnectServer::getInstance().start(address, port, maxclients, timeout);

    return app.exec();
}
