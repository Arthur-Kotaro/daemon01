#include <config.hpp>

bool Config::load(const std::string& _filename)
{
    filename = _filename;
    return reload();
}


bool Config::reload()
{
    std::ifstream file(filename);
    if(!file.is_open())
    {
        syslog(LOG_ERR, "Cannot open config file: %s", filename.c_str());
        return false;
    }
    std::map<std::string, std::string> newData;
    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#') continue;
        std::istringstream iss(line);
        std::string key, value;
        if(std::getline(iss, key, '=') && std::getline(iss, value))
        {
        //Delete whitespaces
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_first_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_first_not_of(" \t") + 1);
            newData[key] = value;
        }
    }
    data.swap(newData);
    syslog(LOG_INFO, "Config reloaded from %s", filename.c_str());
    return true;
}
        

std::string Config::get(const std::string& key, const std::string defaultValue) const
{
    auto it = data.find(key);
    return (it != data.end()) ? it->second : defaultValue;
}


int Config::getInt(const std::string& key, int defaultValue)const
{
    std::string val = get(key);
    if (val.empty()) return defaultValue;
    return std::stoi(val);
}
