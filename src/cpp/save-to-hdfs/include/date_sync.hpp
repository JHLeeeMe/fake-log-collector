#pragma once

#include <iostream>
#include <ctime>
#include <map>

class DateSync
{
public:
    DateSync();
    DateSync(std::time_t&& t);
    DateSync(const DateSync& l)=delete;
    DateSync(DateSync&& l)=delete;
    ~DateSync()=default;

    void        set_date(const std::string& key, const std::time_t& t);
    std::time_t get_date(const std::string& key);
private:
    void init_date(std::time_t* t);
private:
    std::map<std::string, std::time_t> _date_map;
};

