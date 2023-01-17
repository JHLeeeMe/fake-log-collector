#include <curl/curl.h>

#include "rapidjson/document.h"
#include "mqueue_receiver.hpp"
#include "hdfs_writer.hpp"

int main()
{
    MQReceiver receiver;
    HDFSWriter hdfs_writer{"master", "50070"};

    hdfs_writer.write(OP::CREATE, "tmp", nullptr);

    while (true)
    {
        receiver.recv_msg();
        std::unique_ptr<struct MsgBuf> msg = receiver.get_msg();
        //std::cout << "Type: " << msg->type << std::endl;
        //std::cout << "Message: " << (msg->payload).data << std::endl;
        std::string tmp_msg = std::string((msg->payload).data) + "\n";

        hdfs_writer.write(OP::APPEND, "tmp", &tmp_msg);
    }

    return 0;
}

