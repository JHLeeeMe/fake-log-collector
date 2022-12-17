from kafka import KafkaConsumer

import configs


# 'raw' topic delete.retention.ms = 1 hour
configs.alter_retention_policy(topic='raw', hour=1)


if __name__ == '__main__':
    consumer = KafkaConsumer(
        'raw',
        group_id='test-consumer-group',
        bootstrap_servers=['kafka-single-node:9092'],
        auto_offset_reset='earliest',
        enable_auto_commit=True,
        # consumer_timeout_ms=1000
    )

    for consumer_record in consumer:
        key = consumer_record.key.decode('utf8')
        value = consumer_record.value.decode('utf8')
        print(f'Key: {key}')
        print(f'Value: {value[:90]}...')

#    while True:
#        batch_data = consumer.poll()
#        for topic_partition, consumer_records in batch_data.items():
#            for consumer_record in consumer_records:
#                print(consumer_record)
#                print(consumer_record.key)
#                print(f'Offset: {consumer_record.offset}')
#                print(f'Message: {consumer_record.value.decode("utf8")[:90]}...')

