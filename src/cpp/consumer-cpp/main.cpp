#include <iostream>
#include <sstream>
#include <memory>
#include <vector>

#include <rdkafkacpp.h>

#include "consumer_utils.h"
#include "consumer_configs.h"
#include "mqueue_sender.h"

int main()
{
    const std::string brokers{"kafka-single-node:9092"};
    const std::string topic_names{"transformed"};  // e.g. "a,b,c,..."
    std::string       err_str;

    // Create consumer conf
    std::unique_ptr<RdKafka::Conf> config{configs::create_consumer_config(brokers, "consumer-cpp-group")};

    // Create Kafka consumer
    std::unique_ptr<RdKafka::KafkaConsumer> consumer{RdKafka::KafkaConsumer::create(config.get(), err_str)};
    if (!consumer)
    {
        std::cerr << "Failed to create consumer: " << err_str << std::endl;
        exit(1);
    }

    // Subscribe to topic
    std::vector<std::string> topics;
    utils::split_topics(&topics, topic_names, ',');
    RdKafka::ErrorCode err = consumer->subscribe(topics);
    if (err)
    {
       std::cerr << "Failed to subscribe to " << topic_names << ": " << RdKafka::err2str(err) << std::endl;
       exit(2);
    }

    // Create Message Queue Sender
    MQSender sender{MQSender()};

    while (true)
    {
        std::unique_ptr<RdKafka::Message> msg{consumer->consume(1000)};
        if (!msg)
        {
            continue;
        }

        if (msg->err())
        {
            if (msg->err() == RdKafka::ERR__PARTITION_EOF)
            {
                std::cout << "Reached end of topic "
                          << msg->topic_name()
                          << " [" << msg->partition() << "] at offset " << msg->offset()
                << std::endl;
            }
            continue;
        }

        std::stringstream ss;
        ss << *(msg->key()) << "," << static_cast<const char*>(msg->payload());
        sender.send_msg(ss.str().c_str());
    }

    consumer->close();

    return 0;
}

