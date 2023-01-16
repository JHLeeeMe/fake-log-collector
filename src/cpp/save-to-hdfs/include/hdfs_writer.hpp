#pragma once

#include <iostream>

#include <curl/curl.h>

#include "rapidjson/document.h"

class HDFSWriter
{
public:
    HDFSWriter();
    ~HDFSWriter();
public:
    void request();
private:
    size_t write_callback(char* response_data, size_t size, size_t nmemb, void* user_data);
    std::unique_ptr<rapidjson::Document> get_json(const char* str);
    void set_url(std::string* url,
                 const std::string op,
                 const std::string& path = "",
                 const std::string& addr = "master",
                 const std::string& port = "50070");
private:
    std::unique_ptr<CURL, decltype(&curl_easy_cleanup)> _curl;
    CURLcode _res;

    std::string _url;
    std::string _response;
    std::string _location;
    std::unique_ptr<rapidjson::Document> _doc;
};
