#include <ctime>
#include <cstdlib>

#include "rapidjson/document.h"
#include "mqueue_receiver.hpp"
#include "hdfs_writer.hpp"

void set_time(std::time_t* time, std::tm* tm, std::time_t* date_time);

int main()
{
    MQReceiver receiver;
    HDFSWriter hdfs_writer{"master", "50070"};

    std::time_t date_t;
    std::tm     date_tm{};
    set_time(&date_t, &date_tm, nullptr);

    std::string msg_date;
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
        msg_date = msg.substr(key_len + 1, msg.find('T', key_len + 1) - (key_len + 1));
        strptime(msg_date.c_str(), "%Y-%m-%d", &msg_tm);
        msg_t = std::mktime(&msg_tm);

        if (msg_t == date_t)
        {
            hdfs_writer.append_msg(path, msg);
        }
        else
        {
            const std::string dst = key + "/" + msg_date + "_log.csv";

            if (msg_t > date_t)
            {
                // and reset date_t
                set_time(&date_t, nullptr, &msg_t);

                hdfs_writer.rename_file(path, dst);
                hdfs_writer.append_msg(path, msg);
                continue;
            }

            hdfs_writer.append_msg(dst, msg);
        }
    }

    return 0;
}

void set_time(std::time_t* t, std::tm* tm, std::time_t* date_t)
{
    if (date_t)
    {
        *t = *date_t;
        return;
    }

    *t = std::time(nullptr);

    tm = std::localtime(t);
    tm->tm_hour = 0;
    tm->tm_min = 0;
    tm->tm_sec = 0;
    tm->tm_isdst = -1;

    *t = std::mktime(tm);
}

