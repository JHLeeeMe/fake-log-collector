#include <vector>
#include <sstream>

#include "mqueue_receiver.hpp"
#include "influxdb_writer.hpp"

void set_data(std::string* data, std::string* msg);
void make_line_protocol(std::string* data,
        const std::string& measurement, const std::string& tag, const std::string& value);
void split(std::vector<std::string>* vec, const std::string& value);

int main()
{
    const auto logger{spdlog::daily_logger_st("main", "/workspace/src/cpp/write-to-influxdb/logs/log.txt", 0, 0)};

    std::unique_ptr<char> cwd{get_current_dir_name()};
    std::string cwd_str{cwd.get()};

    MQReceiver receiver{cwd_str.substr(0, cwd_str.find_last_of('/'))};
    InfluxDBWriter influxdb_writer{"influxdb", 8089};

    std::unique_ptr<struct MsgBuf> msg_buf;
    std::string msg;
    std::string key;
    std::string data;
    while (true)
    {
        receiver.recv_msg();
        msg_buf = receiver.get_msg();
        msg = (msg_buf->payload).data;

        set_data(&data, &msg);

        std::cout << "Message: " << msg << std::endl;
        std::cout << "Data: " << data << std::endl;

        if (influxdb_writer.sendto(data) < 0)
        {
            logger->error("influxdb_writer.sendto(data) failed...");
            return 1;
        }

        data.clear();

        std::cout << "UDP request sent successfully." << std::endl;
    }

    return 0;
}

void set_data(std::string* data, std::string* msg)
{
    std::string key{msg->substr(0, msg->find(','))};
    *msg = msg->substr(key.length() + 1);

    make_line_protocol(data, "log", key, *msg);
}

void make_line_protocol(std::string* data,
        const std::string& measurement, const std::string& tag, const std::string& value)
{
    data->append(measurement);
    data->append(",key=" + tag);

    std::vector<std::string> vec{};
    split(&vec, value);
    if (tag == "flask")
    {
        data->append(" logLevel=\"" + vec[1] + "\"");
        data->append(",msg=\"" + vec[2] + "\"");
    }
    else
    {
        data->append(" remoteAddr=\"" + vec[1] + "\"");
        data->append(",request=\"" + vec[2] + "\"");
        data->append(",statusCode=\"" + vec[3] + "\"");
        data->append(",bodyBytesSent=\"" + vec[4] + "\"");
    }
    data->push_back('\n');
}

void split(std::vector<std::string>* vec, const std::string& value)
{
    vec->clear();

    std::istringstream iss{value};
    std::string buf;
    while (std::getline(iss, buf, ','))
    {
        vec->push_back(buf);
    }
}

