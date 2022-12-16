from kafka import KafkaConsumer


if __name__ == '__main__':
    consumer = KafkaConsumer(
        'raw',
        group_id='test-consumer-group',
        bootstrap_servers=['kafka-single-node:9092'],
        auto_offset_reset='earliest',
        enable_auto_commit=True,
        # consumer_timeout_ms=1000
    )

    for message in consumer:
        message = message.value.decode('utf8')
        print(message)

