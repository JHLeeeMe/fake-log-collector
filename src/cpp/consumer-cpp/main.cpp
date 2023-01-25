#include <iostream>
#include <sstream>
#include <memory>
#include <vector>

#include <rdkafkacpp.h>
#include <spdlog/sinks/daily_file_sink.h>

#include "consumer_utils.hpp"
#include "consumer_configs.hpp"
#include "mqueue_sender.hpp"

int main()
{
    const auto logger{spdlog::daily_logger_st("main", "/workspace/src/cpp/consumer-cpp/logs/log.txt", 0, 0)};

    const std::string brokers{"kafka-single-node:9092"};
    const std::string topic_names{"transformed"};  // e.g. "a,b,c,..."
    std::string       err_str{};

    // Create consumer conf
    std::unique_ptr<RdKafka::Conf> config{configs::create_consumer_config(brokers, "consumer-cpp-group")};

    // Create Kafka consumer
    std::unique_ptr<RdKafka::KafkaConsumer> consumer{RdKafka::KafkaConsumer::create(config.get(), err_str)};
    if (!consumer)
    {
        logger->error("Failed to create consumer: {}", err_str);
        exit(1);
    }

    // Subscribe to topic
    std::vector<std::string> topics{};
    utils::split_topics(&topics, topic_names, ',');

    RdKafka::ErrorCode err{consumer->subscribe(topics)};
    if (err)
    {
        logger->error("Failed to subscribe to {}: {}", topic_names, RdKafka::err2str(err));
        exit(2);
    }

    // Create Message Queue Senders
    const std::string cpp_home{"/workspace/src/cpp"};
    MQSender hdfs_sender{cpp_home + "/save-to-hdfs"};
    MQSender influxdb_sender{cpp_home + "/write-to-influxdb"};

    std::unique_ptr<RdKafka::Message> msg{};
    while (true)
    {
        msg = consumer->consume(1000);
        if (!msg)
        {
            continue;
        }

        if (msg->err())
        {
            if (msg->err() == RdKafka::ERR__PARTITION_EOF)
            {
                logger->info("Reached end of topic {} [{}] at offset {}",
                             msg->topic_name(), msg->partition(), msg->offset());
            }
            continue;
        }

        std::stringstream ss{};
        ss << *(msg->key()) << "," << static_cast<const char*>(msg->payload());

        influxdb_sender.send_msg(ss.str().c_str());
        hdfs_sender.send_msg(ss.str().c_str());
    }

    return 0;
}

