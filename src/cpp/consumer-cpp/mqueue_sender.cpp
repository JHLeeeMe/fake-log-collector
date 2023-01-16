#include "mqueue_sender.hpp"

MQSender::MQSender()
    : _key(ftok("/dev/mqueue/", 255))
{
    _msg_id = msgget(_key, IPC_CREAT | 0644);
    if (_msg_id < 0)
    {
        std::cerr << "msgget() failed..." << std::endl;
        exit(1);
    }
}

MQSender::MQSender(std::string& path)
    : _key(ftok(path.c_str(), 255))
{
    _msg_id = msgget(_key, IPC_CREAT | 0644);
    if (_msg_id < 0)
    {
        std::cerr << "msgget() failed..." << std::endl;
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
        std::cerr << "msgctl(..., IPC_RMID, ...) failed..." << std::endl;
        exit(2);
    }
}

void MQSender::send_msg(const char* value)
{
    std::string s{value};

    memset(&_payload, 0x00, sizeof(struct Data));
    strcpy(_payload.data, value);
    memset(&_msg, 0x00, sizeof(struct MsgBuf));

    std::string key = s.substr(0, s.find(','));
    if (key == "nginx" || key == "apache")
    {
        _msg.type = 1;
        _msg.payload = _payload;
    }
    else if (key == "flask")
    {
        _msg.type = 2;
    }
    _msg.payload = _payload;

    if (msgsnd(_msg_id, &_msg, sizeof(struct MsgBuf), 0) < 0)
    {
        std::cerr << "msgsnd() failed..." << std::endl;
        exit(2);
    }
}
