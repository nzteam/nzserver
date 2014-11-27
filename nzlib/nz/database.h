#ifndef DATABASE_H
#define DATABASE_H

#include "application.h"
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <thread>
#include <list>
#include <functional>

namespace nz {

class Database
{

private:
    Database() {}
    Database(Database const&);
    Database operator=(Database const&);

public:
    static Database& getInstance()
    {
        static Database instance;
        return instance;
    }

private:
    sql::Driver* m_driver;
    sql::Connection* m_connection;

    void process();
    void queryIt(std::list<std::string> args, std::function<void(sql::ResultSet*)> func);
    void queryItWithId(std::list<std::string> args, std::function<void(int id, sql::ResultSet*)> func);

public:
    void start(std::string hostname, std::string username, std::string password, std::string database);
    void query(std::list<std::string> args);
    void query(std::list<std::string> args, std::function<void(sql::ResultSet*)> func);
    void query(std::list<std::string> args, std::function<void(int id, sql::ResultSet*)> func);

};

}

#endif // DATABASE_H
