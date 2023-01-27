#include <vector>
#include <sstream>

#include "mqueue_receiver.hpp"
#include "influxdb_writer.hpp"

int main()
{
    const std::string path = "/workspace/src/cpp/write-to-influxdb";
    const auto logger{spdlog::daily_logger_st("main", path + "/logs/log.txt", 0, 0)};

    MQReceiver receiver{path};
    InfluxDBWriter influxdb_writer{"influxdb", 8089};

    std::unique_ptr<struct MsgBuf> msg_buf;
    std::string msg;
    std::string data;
    while (true)
    {
        receiver.recv_msg();
        msg_buf = receiver.get_msg();
        msg = (msg_buf->payload).data;

        influxdb_writer.set_data(&data, msg);

        if (influxdb_writer.send(data) < 0)
        {
            logger->error("influxdb_writer.sendto(data) failed...");
            return 1;
        }

        data.clear();

        std::cout << "UDP request sent successfully." << std::endl;
    }

    return 0;
}

