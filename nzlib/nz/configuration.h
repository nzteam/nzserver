#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <fstream>
#include <sstream>
#include <map>
#include <list>
#include "application.h"

namespace nz {

class Configuration
{

    struct DataStruct
    {
        std::string variable;
        std::string value;
    };

private:
    static std::map<std::string, std::list<DataStruct>> s_data;

    static void parse(std::string buffer);
    static std::string trim(const std::string &s);

public:
    static void loadFile(std::string filePath);
    static std::string getString(std::string group, std::string variable, std::string def);
    static int getInt(std::string group, std::string variable, int def);

};

}

#endif // CONFIGURATION_H
