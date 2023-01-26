#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <spdlog/sinks/daily_file_sink.h>

class InfluxDBWriter
{
public:
    InfluxDBWriter(const std::string& ip, const int port);
    InfluxDBWriter(const InfluxDBWriter&)=delete;
    ~InfluxDBWriter();
public:
    const int sendto(const std::string& data) const;
private:
    void set_ip(const char* ip);
    void set_sockaddr_info();
private:
    int                _sockfd;

    const char*        _ip;
    int                _port;
    struct sockaddr_in _sockaddr_info;
};

