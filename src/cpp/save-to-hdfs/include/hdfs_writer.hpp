#pragma once

#include <iostream>

#include <curl/curl.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/daily_file_sink.h>

#include "rapidjson/document.h"

class HDFSWriter
{
public:
    HDFSWriter(const std::string& addr = "master",
               const std::string& port = "50070");
    ~HDFSWriter();
public:
    void create_file(const std::string& path, const std::string& header);
    void rename_file(const std::string& path, const std::string& dst);
    void append_msg(const std::string& path, const std::string& msg);
private:
    static size_t write_callback(char* response_data, size_t size, size_t nmemb, void* user_data);
    void set_url(const std::string& op_str, const std::string& path);
    void set_curl_opt(const std::string& method);
    void set_location();
    void curl_perform();
private:
    std::unique_ptr<CURL, decltype(&curl_easy_cleanup)> _curl;
    CURLcode _code;
    long _response_code;

    const std::string _addr;
    const std::string _port;

    std::string _url;
    std::string _response;
    std::string _location;
};
