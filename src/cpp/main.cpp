#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <memory>

#include <rdkafkacpp.h>

class ExampleConsumeCb : public RdKafka::ConsumeCb
{
public:
    void consume_cb (RdKafka::Message &msg, void *opaque)
    {
        std::cout << "Received message with value: " << msg.len() << " bytes" << std::endl;
    }
};

int main (int argc, char **argv)
{
    std::string brokers = "kafka-single-node:9092";
    std::string topic_str = "transformed";
    std::string errstr;

    // Create Kafka consumer
    std::unique_ptr<RdKafka::Conf> conf{RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL)};
    conf->set("metadata.broker.list", brokers, errstr);
    conf->set("group.id", "test-group", errstr);
    conf->set("enable.auto.commit", "false", errstr);

    ExampleConsumeCb ex_consume_cb;
    conf->set("consume_cb", &ex_consume_cb, errstr);

    std::unique_ptr<RdKafka::KafkaConsumer> consumer{RdKafka::KafkaConsumer::create(conf.get(), errstr)};
    if (!consumer)
    {
        std::cerr << "Failed to create consumer: " << errstr << std::endl;
        exit(1);
    }

    // Subscribe to topic
    std::vector<std::string> topics;
    topics.push_back(topic_str);
    RdKafka::ErrorCode err = consumer->subscribe(topics);
    if (err)
    {
        std::cerr << "Failed to subscribe to " << topic_str << ": " << RdKafka::err2str(err) << std::endl;
        exit(2);
    }

    while (true)
    {
        std::unique_ptr<RdKafka::Message> msg(consumer->consume(1000));
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
            else
            {
                std::cerr << "Error consuming message: " << msg->errstr() << std::endl;
            }
            continue;
        }

        std::cout << "Read message at offset " << msg->offset() << std::endl;
    }
}
