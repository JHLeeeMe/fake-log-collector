#pragma once

#include <iostream>

#include <curl/curl.h>

#include "rapidjson/document.h"

enum class OP
{
    CREATE,
    APPEND,
};

class HDFSWriter
{
public:
    HDFSWriter(const std::string& addr = "master",
               const std::string& port = "50070");
    ~HDFSWriter();
public:
    void write(const enum OP& op, const std::string& path, const std::string* msg);
private:
    static size_t write_callback(char* response_data, size_t size, size_t nmemb, void* user_data);

    void request_put(const std::string& path);
    void request_post(const std::string& path, const std::string& msg);
    void set_url(const std::string op, const std::string path);
    void set_curl_opt(const std::string method);
    void set_location();
    void curl_perform();
private:
    std::unique_ptr<CURL, decltype(&curl_easy_cleanup)> _curl;
    CURLcode _code;

    std::string _addr;
    std::string _port;

    std::string _url;
    std::string _response;
    std::string _location;
};
