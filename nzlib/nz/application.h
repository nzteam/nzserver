#ifndef APPLICATION_H
#define APPLICATION_H

#include <iostream>
#include <exception>
#include <stdexcept>
#include <thread>
#include <chrono>
#include <stdarg.h>

namespace nz {

class Application
{

private:
    static Application* s_instance;

public:
    Application(int argc, char *argv[]);
    static Application* getInstance();
    int exec();
    void close();
    static std::string getFormatedDateTime(std::string format);
    static void addLog(std::string text, ...);

};

}

#endif // APPLICATION_H
