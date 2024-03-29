#include "mqueue_receiver.hpp"

const auto logger{spdlog::daily_logger_st("MQReceiver", "/workspace/src/cpp/save-to-hdfs/logs/log.txt", 0, 0)};

MQReceiver::MQReceiver()
    : _key(ftok("/dev/mqueue/", 255))
{
    _msg_id = msgget(_key, IPC_CREAT | 0644);
    if (_msg_id < 0)
    {
        logger->error("msgget() failed...");
        exit(1);
    }
}

MQReceiver::MQReceiver(const std::string& path)
    : _key(ftok(path.c_str(), 255))
{
    _msg_id = msgget(_key, IPC_CREAT | 0644);
    if (_msg_id < 0)
    {
        logger->error("msgget() failed...");
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
        logger->error("msgctl(..., IPC_RMID, ...) failed...");
        exit(-1);
    }
}

void MQReceiver::recv_msg()
{
    if (msgrcv(_msg_id, &_msg, sizeof(struct MsgBuf), 0, 0) < 0)
    {
        logger->error("msgrcv() failed...");
        exit(2);
    }
}

std::unique_ptr<struct MsgBuf> MQReceiver::get_msg()
{
    return std::make_unique<struct MsgBuf>(_msg);
}

