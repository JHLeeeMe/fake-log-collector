#include "mqueue_receiver.hpp"
#include "influxdb_writer.hpp"

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

        key = msg.substr(0, msg.find(','));
        msg = msg.substr(key.length() + 1);

        std::cout << "Message: " << msg << std::endl;

        data.append("log,tag1=" + key);
        data.append(" from=\"write-to-influxdb\"");
        data += '\n';

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

