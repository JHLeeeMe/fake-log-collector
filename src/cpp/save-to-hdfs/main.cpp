#include <ctime>

#include "mqueue_receiver.hpp"
#include "hdfs_writer.hpp"
#include "date_sync.hpp"

int main()
{
    MQReceiver receiver{};
    HDFSWriter hdfs_writer{"master", "50070"};
    DateSync   date_sync{};

    std::string msg_date_str;
    std::time_t msg_t;
    std::tm     msg_tm{};

    std::unique_ptr<struct MsgBuf> msg_buf;
    //long type;
    std::string msg;
    std::string key;
    int         key_len;
    std::string path;
    while (true)
    {
        receiver.recv_msg();
        msg_buf = receiver.get_msg();

        //type = msg->type;
        msg = (msg_buf->payload).data;
        msg.push_back('\n');

        key_len = msg.find(',');
        key = msg.substr(0, key_len);
        path = key + "/log.csv";

        // get date from msg
        msg_date_str = msg.substr(key_len + 1, msg.find('T', key_len + 1) - (key_len + 1));
        strptime(msg_date_str.c_str(), "%Y-%m-%d", &msg_tm);
        msg_t = std::mktime(&msg_tm);

        msg = msg.substr(key_len + 1);

        if (msg_t == date_sync.get_date(key))
        {
            hdfs_writer.append_msg(path, msg);
        }
        else
        {
            std::string dst;

            if (msg_t > date_sync.get_date(key))
            {
                char date_str[11];
                date_sync.date_str(date_str, sizeof(date_str), key);
                dst = (key + "/" + date_str + "_log.csv");

                date_sync.set_date(key, msg_t);
                hdfs_writer.rename_file(path, dst);
                hdfs_writer.append_msg(path, msg);

                continue;
            }

            dst = (key + "/" + msg_date_str + "_log.csv");
            hdfs_writer.append_msg(dst, msg);
        }
    }

    return 0;
}

