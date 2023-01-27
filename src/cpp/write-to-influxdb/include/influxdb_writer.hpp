#include <sstream>
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
    const int send(const std::string& data) const;
    void set_data(std::string* data, const std::string& msg) const;
private:
    void set_ip(const char* ip);
    void set_sockaddr_info();
    void make_line_protocol(std::string* data,
            const std::string& measurement, const std::string& tag, const std::string& value) const;
    void split(std::vector<std::string>* vec, const std::string& value) const;
private:
    int                _sockfd;

    const char*        _ip;
    int                _port;
    struct sockaddr_in _sockaddr_info;
};

