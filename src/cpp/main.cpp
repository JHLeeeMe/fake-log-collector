#include <iostream>
#include <sstream>
#include <memory>
#include <vector>

#include <rdkafkacpp.h>

void split(std::vector<std::string>&, const std::string&, const char&);

int main()
{
    const std::string brokers      = "kafka-single-node:9092";
    const std::string topic_names  = "transformed";  // e.g. "a,b,c,..."
    std::string       err_str;

    // Create consumer conf
    std::unique_ptr<RdKafka::Conf> config{RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL)};
    config->set("bootstrap.servers", brokers, err_str);
    config->set("group.id", "consumer-cpp-group", err_str);
    config->set("auto.offset.reset", "latest", err_str);
    config->set("enable.auto.commit", "true", err_str);

    // Create Kafka consumer
    std::unique_ptr<RdKafka::KafkaConsumer> consumer{RdKafka::KafkaConsumer::create(config.get(), err_str)};
    if (!consumer)
    {
        std::cerr << "Failed to create consumer: " << err_str << std::endl;
        exit(1);
    }

    // Subscribe to topic
    std::vector<std::string> topics;
    split(topics, topic_names, ',');

    RdKafka::ErrorCode err = consumer->subscribe(topics);
    if (err)
    {
        std::cerr << "Failed to subscribe to " << topic_names << ": " << RdKafka::err2str(err) << std::endl;
        exit(2);
    }

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

        std::cout << "Key: " << *(msg->key()) << std::endl
                  << "Value: " << std::string(static_cast<char*>(msg->payload()), msg->len())
        << std::endl;
    }

    consumer->close();

    return 0;
}

void split(std::vector<std::string>& vec, const std::string& str, const char& delim)
{
    std::string token;
    std::istringstream token_stream{str};
    while (std::getline(token_stream, token, delim))
    {
        vec.push_back(token);
    }
}