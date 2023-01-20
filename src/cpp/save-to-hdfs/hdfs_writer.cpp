#include <curl/curl.h>

#include "hdfs_writer.hpp"

HDFSWriter::HDFSWriter(const std::string& addr, const std::string& port)
    : _curl(curl_easy_init(), &curl_easy_cleanup)
    , _addr(addr)
    , _port(port)
{
    if (!_curl)
    {
        std::cerr << "curl_easy_init() failed..." << std::endl;
        exit(1);
    }
}

HDFSWriter::~HDFSWriter()
{}

void HDFSWriter::create_file(const std::string& path, const std::string& header)
{
first:
    for (int i = 0; i < 2; i++)
    {
        if (i == 0)
        {
            set_url("CREATE", path);
        }
        else
        {
            curl_easy_setopt(_curl.get(), CURLOPT_POSTFIELDS, header.c_str());
        }
        set_curl_opt("PUT");

        curl_perform();

        if (i != 0)
        {
            break;
        }

        if (_response_code != 200)
        {
            std::cerr << "Error Code: " << _response_code << std::endl
                      << _response << std::endl;
            _response.clear();
            _location.clear();
            curl_easy_reset(_curl.get());

            goto first;
        }

        set_location();

        _response.clear();
        curl_easy_reset(_curl.get());
    }
    std::cout << "ok." << std::endl;

    _response.clear();
    _location.clear();
}

void HDFSWriter::rename_file(const std::string& path, const std::string& dst)
{
    set_url("RENAME", path);
    _url.append("&destination=/" + dst);
    set_curl_opt("PUT");

    curl_perform();
    std::cout << "ok." << std::endl;

    _response.clear();
    curl_easy_reset(_curl.get());
}

void HDFSWriter::append_msg(const std::string& path, const std::string& msg)
{
first:
    for (int i = 0; i < 2; i++)
    {
        if (i == 0)
        {
            set_url("APPEND", path);
        }
        else
        {
            curl_easy_setopt(_curl.get(), CURLOPT_POSTFIELDS, msg.c_str());
        }
        set_curl_opt("POST");

        curl_perform();

        if (i != 0)
        {
            break;
        }

        if (_response_code != 200)
        {
            std::cerr << "Error Code: " << _response_code << std::endl
                      << _response << std::endl;
            _response.clear();
            _location.clear();
            curl_easy_reset(_curl.get());

            if (_response_code == 404)  // file not exists.
            {
                std::cout << "Creating `/" << path << "`" << std::endl;

                const std::string key = path.substr(0, path.find('/'));
                std::string header;
                if (key == "nginx" || key == "apache")
                {
                    header = "datetime,ip,request,statusCode,bodyBytes\n";
                }
                else
                {
                    header = "datetime,logLevel,message\n";
                }
                create_file(path, header);
            }

            goto first;
        }

        set_location();

        _response.clear();
        curl_easy_reset(_curl.get());
    }
    std::cout << "ok." << std::endl;

    _response.clear();
    _location.clear();
}

size_t HDFSWriter::write_callback(char* response_data, size_t size, size_t nmemb, void* user_data)
{
    ((std::string*)user_data)->append(response_data, size * nmemb);

    return size * nmemb;
}

void HDFSWriter::set_url(const std::string& op, const std::string& path)
{
    _url.clear();
    _url.append(
        "http://" + _addr + ":" + _port +
        "/webhdfs/v1/" + path +
        "?op=" + op +
        "&noredirect=true"
    );
}

void HDFSWriter::set_curl_opt(const std::string& method)
{
    curl_easy_setopt(_curl.get(), CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(_curl.get(), CURLOPT_WRITEDATA, &_response);
    curl_easy_setopt(_curl.get(), CURLOPT_CUSTOMREQUEST, method.c_str());

    if (_location.empty())
    {
        curl_easy_setopt(_curl.get(), CURLOPT_URL, _url.c_str());
    }
    else
    {
        curl_easy_setopt(_curl.get(), CURLOPT_URL, _location.c_str());
    }
}

void HDFSWriter::set_location()
{
    rapidjson::Document doc;
    doc.Parse(_response.c_str());

    std::string key;
    for (const auto& m : doc.GetObject())
    {
        key = m.name.GetString();
        if (key != "Location")
        {
            std::cerr << m.value.GetString() << std::endl;
            exit(1);
        }
        _location = m.value.GetString();
        break;
    }
}

void HDFSWriter::curl_perform()
{
    _code = curl_easy_perform(_curl.get());
    if (_code != CURLE_OK)
    {
        std::cerr << "perform() failed: " << curl_easy_strerror(_code) << std::endl;
        exit(1);
    }

    curl_easy_getinfo(_curl.get(), CURLINFO_RESPONSE_CODE, &_response_code);
}

