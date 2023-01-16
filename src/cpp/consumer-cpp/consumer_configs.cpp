#include "consumer_configs.hpp"

namespace configs
{

std::unique_ptr<RdKafka::Conf> create_consumer_config(
    const std::string& brokers,
    const std::string& group_id,
    AutoOffsetReset auto_offset_reset,
    bool enable_auto_commit)
{
    std::unique_ptr<RdKafka::Conf> config{RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL)};
    std::string err_str;

    if (config->set("bootstrap.servers", brokers, err_str) != RdKafka::Conf::CONF_OK)
    {
        std::cerr << "Error setting bootstrap.servers: " << err_str << std::endl;
        exit(11);
    }

    if (config->set("group.id", group_id, err_str) != RdKafka::Conf::CONF_OK)
    {
        std::cerr << "Error setting group.id: " << err_str << std::endl;
        exit(12);
    }

    std::string state;
    switch (auto_offset_reset)
    {
    case AutoOffsetReset::EARLIEST:
        state = "earliest";
        break;
    case AutoOffsetReset::LATEST:
        state = "latest";
        break;
    default:
        state = "none";
        break;
    }

    if (config->set("auto.offset.reset", state, err_str) != RdKafka::Conf::CONF_OK)
    {
        std::cerr << "Error setting auto.offset.reset: " << err_str << std::endl;
        exit(13);
    }

    if (enable_auto_commit)
    {
        state = "true";
    }
    else
    {
        state = "false";
    }

    if (config->set("enable.auto.commit", state, err_str) != RdKafka::Conf::CONF_OK)
    {
        std::cerr << "Error setting enable.auto.commit: " << err_str << std::endl;
        exit(14);
    }

    return config;
}

}

