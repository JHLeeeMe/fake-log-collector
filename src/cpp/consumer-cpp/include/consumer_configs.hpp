#pragma once

#include <iostream>
#include <memory>

#include <rdkafkacpp.h>
#include <spdlog/sinks/daily_file_sink.h>

namespace configs
{

enum class AutoOffsetReset
{
    EARLIEST,
    LATEST,
    NONE
};

std::unique_ptr<RdKafka::Conf> create_consumer_config(
        const std::string& brokers,
        const std::string& group_id = "consumer-cpp-group",
        AutoOffsetReset auto_offset_reset = AutoOffsetReset::LATEST,
        bool enable_auto_commit = true);

}

