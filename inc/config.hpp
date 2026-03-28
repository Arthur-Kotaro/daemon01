#pragma once

#include <cstdio>
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <syslog.h>

#define DEFAULT_CONFIG_PATH "/etc/daemon01/default.conf"

class Config
{
    std::string filename;
    std::map<std::string, std::string> data;
    
    public:
    bool load(const std::string& _filename);
    bool reload();
    std::string get(const std::string& key, const std::string defaultValue = "") const;
    int getInt(const std::string& key, int defaultValue       = 0)const;
};
