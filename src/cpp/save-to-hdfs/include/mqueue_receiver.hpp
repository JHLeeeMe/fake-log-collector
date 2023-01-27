#pragma once

#include <iostream>
#include <memory>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <spdlog/sinks/daily_file_sink.h>

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
    MQReceiver(const std::string&);
    ~MQReceiver();
public:
    void                           recv_msg();
    std::unique_ptr<struct MsgBuf> get_msg();
private:
    key_t         _key;
    int           _msg_id;
    struct MsgBuf _msg;
};

