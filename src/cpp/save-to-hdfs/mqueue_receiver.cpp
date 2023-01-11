#include "mqueue_receiver.h"

MQReceiver::MQReceiver()
    : _key(ftok("/dev/mqueue/", 255))
{
    _msg_id = msgget(_key, IPC_CREAT | 0644);
    if (_msg_id < 0)
    {
        std::cerr << "msgget() failed..." << std::endl;
        exit(1);
    }
}

MQReceiver::MQReceiver(std::string& path)
    : _key(ftok(path.c_str(), 255))
{
    _msg_id = msgget(_key, IPC_CREAT | 0644);
    if (_msg_id < 0)
    {
        std::cerr << "msgget() failed..." << std::endl;
        exit(1);
    }
}

MQReceiver::~MQReceiver()
{
    if (msgget(_msg_id, 0) < 0)
    {
        return;
    }

    if (msgctl(_msg_id, IPC_RMID, nullptr) < 0)
    {
        std::cerr << "msgctl(..., IPC_RMID, ...) failed..." << std::endl;
        exit(2);
    }
}

void MQReceiver::recv_msg()
{
    if (msgrcv(_msg_id, &_msg, sizeof(struct MsgBuf), 0, 0) < 0)
    {
        std::cerr << "msgrcv() failed..." << std::endl;
        exit(2);
    }

    std::cout << "msg type: " << _msg.type         << std::endl
              << "msg     : " << _msg.payload.data << std::endl;
}