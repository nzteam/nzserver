#include "application.h"

namespace nz {

Application* Application::s_instance = nullptr;

Application::Application(int argc, char *argv[])
{
    if(Application::s_instance == nullptr)
    {
        Application::s_instance = this;
    }
    else
    {
        throw std::runtime_error("[Application] Multiple instance");
        delete this;
    }
}

Application* Application::getInstance()
{
    if(Application::s_instance == nullptr)
    {
        throw std::runtime_error("[Application] No instance");
    }

    return Application::s_instance;
}

int Application::exec()
{
    while(true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    return 0;
}

void Application::close()
{
    exit(0);
}

std::string Application::getFormatedDateTime(std::string format)
{
    time_t rawtime;
    struct tm * timeinfo;

    time (&rawtime);
    timeinfo = localtime (&rawtime);

    char buffer[80];
    std::strftime(buffer, 80, format.c_str(), timeinfo);
    return buffer;
}

void Application::addLog(std::string text, ...)
{
    char buffer[256];
    va_list args;
    va_start(args, text);
    vsprintf(buffer, text.c_str(), args);
    va_end(args);

    std::cout << Application::getFormatedDateTime("[%d-%m-%Y] %T") << "  ##  " << buffer << std::endl;
}

}
