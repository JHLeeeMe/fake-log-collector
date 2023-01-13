from kafka.admin import KafkaAdminClient, ConfigResource, ConfigResourceType, NewTopic


def create_topic(topic: str, num_partitions: int = 3, replica: int = 1):
    admin_client = KafkaAdminClient(bootstrap_servers=['kafka-single-node:9092'])

    topic_config = {
            'retention.ms': 1 * 3600 * 1000,                # 1 hour
            'delete.retention.ms': int(0.5 * 3600 * 1000),  # 0.5 hour
            'retention.bytes': 5 * 1024 * 1024              # 5GB
    }
    new_topic = NewTopic(name=topic,
                         num_partitions=num_partitions,
                         replication_factor=replica,
                         topic_configs=topic_config)
    admin_client.create_topics(new_topics=[new_topic])

    admin_client.close()


def describe_configs(topic: str):
    admin_client = KafkaAdminClient(bootstrap_servers=['kafka-single-node:9092'])
    assert (topic in admin_client.list_topics())

    resource = ConfigResource(resource_type=ConfigResourceType.TOPIC, name=topic)
    configs = admin_client.describe_configs(config_resources=[resource])
    print(configs)
    admin_client.close()


def alter_retention_policy(topic: str, hour: float = 0.5):
    assert (hour > 0)

    admin_client = KafkaAdminClient(bootstrap_servers=['kafka-single-node:9092'])
    assert (topic in admin_client.list_topics())

    retention_ms = hour * 3600 * 1000                    # 1 hour
    delete_retention_ms = int((hour / 2) * 3600 * 1000)  # defualt 0.5 hour
    retention_bytes = 5 * 1024 * 1024                    # 5GB

    resource = ConfigResource(resource_type=ConfigResourceType.TOPIC, name=topic)
    resource.configs = {'retention.ms': str(retention_ms),
                        'delete.retention.ms': str(delete_retention_ms),
                        'retention.bytes': str(retention_bytes)}

    admin_client.alter_configs(config_resources=[resource])
    admin_client.close()

