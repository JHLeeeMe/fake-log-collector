#include "date_sync.hpp"

DateSync::DateSync()
{
    init_date_map(nullptr);
}

DateSync::DateSync(std::time_t&& t)
{
    init_date_map(&t);
}

void DateSync::set_date(const std::string& key, const std::time_t& t)
{
    _date_map[key] = t;
}

std::time_t DateSync::get_date(const std::string& key)
{
    return _date_map[key];
}

void DateSync::date_str(char* buf, size_t buf_size, const std::string& key)
{
    std::strftime(buf, buf_size, "%Y-%m-%d", std::localtime(&_date_map[key]));
}

void DateSync::init_date_map(std::time_t* t)
{
    std::time_t date_t = std::time(t);

    std::tm tm{*std::localtime(&date_t)};
    tm.tm_hour = 0;
    tm.tm_min = 0;
    tm.tm_sec = 0;
    tm.tm_isdst = -1;

    date_t = std::mktime(&tm);
    _date_map["apache"] = date_t;
    _date_map["nginx"] = date_t;
    _date_map["flask"] = date_t;
}

