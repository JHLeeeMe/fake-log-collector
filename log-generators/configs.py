from kafka.admin import KafkaAdminClient, ConfigResource, ConfigResourceType


def describe_configs(topic: str):
    admin_client = KafkaAdminClient(bootstrap_servers=['kafka-single-node:9092'])
    assert (topic in admin_client.list_topics())

    resource = ConfigResource(resource_type=ConfigResourceType.TOPIC, name=topic)
    configs = admin_client.describe_configs(config_resources=[resource])
    print(configs)
    admin_client.close()


def alter_retention_policy(topic: str, hour: int = 1):
    assert (hour >= 1)

    admin_client = KafkaAdminClient(bootstrap_servers=['kafka-single-node:9092'])
    assert (topic in admin_client.list_topics())

    delete_retention_ms = hour * 3600 * 1000  # defualt 1 hour

    resource = ConfigResource(resource_type=ConfigResourceType.TOPIC, name=topic)
    resource.configs = {'delete.retention.ms': str(delete_retention_ms)}

    admin_client.alter_configs(config_resources=[resource])
    admin_client.close()

