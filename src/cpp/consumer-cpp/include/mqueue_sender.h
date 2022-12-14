#pragma once

#include <iostream>
#include <cstring>
#include <sys/ipc.h>
#include <sys/msg.h>

struct Data
{
    char data[300];
};

struct MsgBuf
{
    long        type;
    struct Data payload;
};

class MQSender
{
public:
    MQSender();
    MQSender(std::string&);
    ~MQSender();
public:
    void send_msg(const char*);
private:
    key_t         _key;
    int           _msg_id;
    struct Data   _payload;
    struct MsgBuf _msg;
};