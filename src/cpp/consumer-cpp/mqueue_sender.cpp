#include "mqueue_sender.hpp"

const auto logger{spdlog::daily_logger_st("MQSender", "/workspace/src/cpp/consumer-cpp/logs/log.txt", 0, 0)};

MQSender::MQSender()
    : _key(ftok("/dev/mqueue/", 255))
{
    _msg_id = msgget(_key, IPC_CREAT | 0644);
    if (_msg_id < 0)
    {
        logger->error("msgget() failed...");
        exit(1);
    }
}

MQSender::MQSender(const std::string& path)
    : _key(ftok(path.c_str(), 255))
{
    _msg_id = msgget(_key, IPC_CREAT | 0644);
    if (_msg_id < 0)
    {
        logger->error("msgget() failed...");
        exit(1);
    }
}

MQSender::~MQSender()
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

void MQSender::send_msg(const char* value)
{
    std::string s{value};

    memset(&_payload, 0x00, sizeof(struct Data));
    strcpy(_payload.data, value);

    memset(&_msg, 0x00, sizeof(struct MsgBuf));
    _msg.type = 1;
    _msg.payload = _payload;

    if (msgsnd(_msg_id, &_msg, sizeof(struct MsgBuf), 0) < 0)
    {
        logger->error("msgsnd() failed...");
        exit(2);
    }
}

