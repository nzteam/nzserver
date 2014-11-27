#include "database.h"

namespace nz {

void Database::start(std::string hostname, std::string username, std::string password, std::string database)
{
    try
    {
        std::string address("tcp://");
        address.append(hostname);

        this->m_driver = get_driver_instance();
        this->m_connection = this->m_driver->connect(address, username, password);
        this->m_connection->setSchema(database);

        Application::addLog("[Database] Connected (HOSTNAME: %s | USERNAME: %s | PASSWORD: *** | DATABASE: %s)", hostname.c_str(), username.c_str(), database.c_str());

        std::thread* processThread = new std::thread(&Database::process, this);
        if(processThread->joinable())
        {
            processThread->detach();
        }
    }
    catch(sql::SQLException &e)
    {
        Application::addLog("[Database] Can not connect! (HOSTNAME: %s | USERNAME: %s | PASSWORD: *** | DATABASE: %s)", hostname.c_str(), username.c_str(), database.c_str());
        Application::addLog("[Database] Error: %s", e.what());
        Application::addLog("[Database] Error no: %d", e.getErrorCode());
        Application::getInstance()->close();
    }
}

void Database::process()
{
    try
    {
        while(true)
        {
            this->m_connection->getWarnings();
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    }
    catch(sql::SQLException &e)
    {
        Application::addLog("[Database] Connection closed!");
        Application::addLog("[Database] Error: %s", e.what());
        Application::getInstance()->close();
    }
}

void Database::queryIt(std::list<std::string> args, std::function<void(sql::ResultSet*)> func)
{
    while(this->m_connection->isClosed() == true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    auto qit = args.begin();
    std::string query(*qit);
    args.pop_front();

    sql::ResultSet* result;
    sql::PreparedStatement* pstmt;
    pstmt = this->m_connection->prepareStatement(query);

    int tempId = 1;
    for(std::list<std::string>::iterator it = args.begin(); it != args.end(); it++)
    {
        pstmt->setString(tempId, (*it).c_str());
        tempId++;
    }

    if(pstmt->execute())
    {
        result = pstmt->executeQuery();

        func(result);
    }
}

void Database::queryItWithId(std::list<std::string> args, std::function<void(int id, sql::ResultSet*)> func)
{
    while(this->m_connection->isClosed() == true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    auto qit = args.begin();
    std::string query(*qit);
    args.pop_front();

    sql::ResultSet* result;
    sql::PreparedStatement* pstmt;
    pstmt = this->m_connection->prepareStatement(query);

    int tempId = 1;
    for(std::list<std::string>::iterator it = args.begin(); it != args.end(); it++)
    {
        pstmt->setString(tempId, (*it).c_str());
        tempId++;
    }

    if(pstmt->execute())
    {
        result = pstmt->executeQuery();

        func(0, result);
    }
}

void Database::query(std::list<std::string> args)
{
    if(args.size() < 1) return;

    std::function<void(sql::ResultSet*)> func = [](sql::ResultSet*) {};

    std::thread queryThread(&Database::queryIt, this, args, func);

    if(queryThread.joinable()) queryThread.detach();
}

void Database::query(std::list<std::string> args, std::function<void(sql::ResultSet*)> func)
{
    if(args.size() < 1) return;

    std::thread queryThread(&Database::queryIt, this, args, func);

    if(queryThread.joinable()) queryThread.detach();
}

void Database::query(std::list<std::string> args, std::function<void(int id, sql::ResultSet*)> func)
{
    if(args.size() < 1) return;

    std::thread queryThread(&Database::queryItWithId, this, args, func);

    if(queryThread.joinable()) queryThread.detach();
}

}
