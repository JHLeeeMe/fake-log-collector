#include <curl/curl.h>

#include "rapidjson/document.h"
#include "mqueue_receiver.hpp"
#include "hdfs_writer.hpp"

int main()
{
    MQReceiver receiver;
    HDFSWriter hdfs_writer{"master", "50070"};

    hdfs_writer.request(OP::CREATE, "tmp", "");

    while (true)
    {
        receiver.recv_msg();
        std::unique_ptr<struct MsgBuf> msg = receiver.get_msg();
        //std::cout << "Type: " << msg->type << std::endl;
        //std::cout << "Message: " << (msg->payload).data << std::endl;
        std::string tmp = std::string((msg->payload).data) + "\n";

        hdfs_writer.request(OP::APPEND, "tmp", tmp);
    }

    return 0;
}

