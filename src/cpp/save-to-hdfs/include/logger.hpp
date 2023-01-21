#pragma once

#include <iostream>
#include <ctime>
#include <map>

class Logger
{
public:
    Logger();
    Logger(std::time_t&& t);
    Logger(const Logger& l)=delete;
    Logger(Logger&& l)=delete;
    ~Logger()=default;

    void        set_date(const std::string& key, const std::time_t& t);
    std::time_t get_date(const std::string& key);
private:
    void init_date(std::time_t* t);
private:
    std::map<std::string, std::time_t> _date_map;
};

