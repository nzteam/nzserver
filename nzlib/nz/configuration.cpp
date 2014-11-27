#include "configuration.h"

namespace nz {

std::map<std::string, std::list<Configuration::DataStruct>> Configuration::s_data;

void Configuration::loadFile(std::string filePath)
{
    std::fstream fileBuffer;
    fileBuffer.open(filePath);

    if(fileBuffer.is_open())
    {
        std::string buffer((std::istreambuf_iterator<char>(fileBuffer)), std::istreambuf_iterator<char>());
        fileBuffer.close();

        Configuration::parse(buffer);

        Application::addLog("[Configuration] Loaded: %s", filePath.c_str());
    }
    else
    {
        Application::addLog("[Configuration] Can not load: %s", filePath.c_str());
    }
}

void Configuration::parse(std::string buffer)
{
    std::istringstream lines(buffer);
    std::string line;
    std::string group;
    std::string variable;
    std::string value;

    while(std::getline(lines, line))
    {
        line = Configuration::trim(line);

        if(line[0] == ';') continue;
        if(line[0] == '#') continue;
        if(line[0] == '/' && line[1] == '/') continue;
        if(line.empty()) continue;

        if(line[0] == '[' && line[line.size()-1] == ']')
        {
            group = line.substr(1, (line.size()-2));
        }
        else if(group.empty())
        {
            continue;
        }
        else
        {
            variable.clear();
            value.clear();

            variable = line.substr(0, line.find('='));
            variable = Configuration::trim(variable);

            value = line.substr(line.find('=')+1, (line.size()-line.find('=')));
            value = Configuration::trim(value);

            DataStruct data;
            data.variable = variable;
            data.value = value;

            Configuration::s_data[group].push_back(data);
        }
    }
}

std::string Configuration::trim(const std::string &s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && isspace(*it))
        it++;

    std::string::const_reverse_iterator rit = s.rbegin();
    while (rit.base() != it && isspace(*rit))
        rit++;

    return std::string(it, rit.base());
}

std::string Configuration::getString(std::string group, std::string variable, std::string def)
{
    auto git = Configuration::s_data.find(group);
    if(git != Configuration::s_data.end())
    {
        auto data = git->second;

        for(auto it = data.begin(); it != data.end(); it++)
        {
            if((*it).variable == variable)
            {
                return (*it).value;
            }
        }
    }
    return def;
}

int Configuration::getInt(std::string group, std::string variable, int def)
{
    auto git = Configuration::s_data.find(group);
    if(git != Configuration::s_data.end())
    {
        auto data = git->second;

        for(auto it = data.begin(); it != data.end(); it++)
        {
            if((*it).variable == variable)
            {
                return atoi((*it).value.c_str());
            }
        }
    }
    return def;
}

}
