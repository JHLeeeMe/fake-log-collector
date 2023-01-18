#include "rapidjson/document.h"
#include "mqueue_receiver.hpp"
#include "hdfs_writer.hpp"

int main()
{
    MQReceiver receiver;
    HDFSWriter hdfs_writer{"master", "50070"};

    std::unique_ptr<struct MsgBuf> msg;
    //long type;
    std::string data;
    std::string key;
    std::string path;
    while (true)
    {
        receiver.recv_msg();
        msg = receiver.get_msg();

        //type = msg->type;
        data = (msg->payload).data;
        data.push_back('\n');

        key = data.substr(0, data.find(','));
        path = key + "/log.csv";

        hdfs_writer.write(OP::APPEND, path, &data);
    }

    return 0;
}

