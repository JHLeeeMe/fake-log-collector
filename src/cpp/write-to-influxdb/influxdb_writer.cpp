#include "influxdb_writer.hpp"

const auto logger{spdlog::daily_logger_st("InfluxDBWriter", "/workspace/src/cpp/write-to-influxdb/logs/log.txt", 0, 0)};

InfluxDBWriter::InfluxDBWriter(const std::string& ip, const int port)
    : _port(port)
{
    set_ip(ip.c_str());
    set_sockaddr_info();
    _sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (_sockfd < 0)
    {
        logger->error("socket(...) failed...");
        exit(1);
    }
}

InfluxDBWriter::~InfluxDBWriter()
{
    ::close(_sockfd);
}

const int InfluxDBWriter::send(const std::string& data) const
{
    return ::sendto(_sockfd, data.c_str(), data.length(), 0,
                    (struct sockaddr*)&_sockaddr_info, sizeof(_sockaddr_info));
}

void InfluxDBWriter::set_data(std::string* data, const std::string& msg) const
{
    const std::string key{msg.substr(0, msg.find(','))};
    make_line_protocol(data, "log", key, msg);
}

void InfluxDBWriter::set_ip(const char* ip)
{
    struct hostent* host_entry{gethostbyname(ip)};
    if (!host_entry)
    {
        logger->error("gethostbyname({}) failed...", ip);
        exit(1);
    }

    _ip = inet_ntoa(*((struct in_addr*)host_entry->h_addr_list[0]));
}

void InfluxDBWriter::set_sockaddr_info()
{
    std::memset(&_sockaddr_info, 0x00, sizeof(_sockaddr_info));

    _sockaddr_info.sin_family = AF_INET;
    _sockaddr_info.sin_addr.s_addr = inet_addr(_ip);
    _sockaddr_info.sin_port = htons(_port);
}

void InfluxDBWriter::make_line_protocol(std::string* data,
        const std::string& measurement, const std::string& tag, const std::string& value) const
{
    data->append(measurement);
    data->append(",key=" + tag);

    std::vector<std::string> vec{};
    split(&vec, value);
    if (tag == "flask")
    {
        data->append(" logLevel=\"" + vec[2] + "\"");
        data->append(",msg=\"" + vec[3] + "\"");
    }
    else
    {
        data->append(" remoteAddr=\"" + vec[2] + "\"");
        data->append(",request=\"" + vec[3] + "\"");
        data->append(",statusCode=" + vec[4]);
        data->append(",bodyBytesSent=" + vec[5]);
    }
    data->push_back('\n');
}

void InfluxDBWriter::split(std::vector<std::string>* vec, const std::string& value) const
{
    vec->clear();

    std::istringstream iss{value};
    std::string buf;
    while (std::getline(iss, buf, ','))
    {
        vec->push_back(buf);
    }
}

