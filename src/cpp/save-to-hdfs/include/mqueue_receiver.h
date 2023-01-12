#pragma once

#include <iostream>
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

class MQReceiver
{
public:
    MQReceiver();
    MQReceiver(std::string&);
    ~MQReceiver();
public:
    void recv_msg();
private:
    key_t         _key;
    int           _msg_id;
    struct MsgBuf _msg;
};

