#include <iostream>
#include <sstream>
#include <memory>
#include <vector>

#include <rdkafkacpp.h>

#include "consumer_config.h"
#include "mqueue_sender.h"

void split_topics(std::vector<std::string>&, const std::string&, const char);

int main()
{
    const std::string brokers      = "kafka-single-node:9092";
    const std::string topic_names  = "transformed";  // e.g. "a,b,c,..."
    std::string       err_str;

    // Create consumer conf
    std::unique_ptr<RdKafka::Conf> config{create_consumer_config(brokers, "consumer-cpp-group")};

    // Create Kafka consumer
    std::unique_ptr<RdKafka::KafkaConsumer> consumer{RdKafka::KafkaConsumer::create(config.get(), err_str)};
    if (!consumer)
    {
        std::cerr << "Failed to create consumer: " << err_str << std::endl;
        exit(1);
    }

    // Subscribe to topic
    std::vector<std::string> topics;
    split_topics(topics, topic_names, ',');
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

        //std::cout << "Key: " << *(msg->key()) << std::endl
        //          << "Value: " << std::string(static_cast<char*>(msg->payload()), msg->len())
        //<< std::endl;

        //std::string tmp{std::string(*(msg->key())) + "," + std::string(static_cast<char*>(msg->payload()))};
        //std::cout << tmp << std::endl;

        std::stringstream ss;
        ss << *(msg->key()) << "," << static_cast<const char*>(msg->payload());
        std::cout << ss.str().c_str() << std::endl;
        sender.send_msg(ss.str().c_str());
    }

    consumer->close();

    return 0;
}

void split_topics(std::vector<std::string>& vec, const std::string& str, const char delim)
{
    std::string token;
    std::istringstream token_stream{str};
    while (std::getline(token_stream, token, delim))
    {
        vec.push_back(token);
    }
}