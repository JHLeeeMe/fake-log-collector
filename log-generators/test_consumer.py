from kafka import KafkaConsumer


#if __name__ == '__main__':
#    consumer = KafkaConsumer(
#        'raw',
#        group_id='test-consumer-group',
#        bootstrap_servers=['kafka-single-node:9092'],
#        auto_offset_reset='earliest',
#        enable_auto_commit=True,
#        # consumer_timeout_ms=1000
#    )
#
#    for message in consumer:
#        message = message.value.decode('utf8')
#        print(message)
#
#    while True:
#        messages = consumer.poll(timeout_ms=1000)
#        for message in messages:
#            print(message)

if __name__ == '__main__':
    consumer1 = KafkaConsumer(group_id='test-consumers',
                              bootstrap_servers=['kafka-single-node:9092'],
                              auto_offset_reset='earliest',
                              enable_auto_commit=True)
    consumer2 = KafkaConsumer(group_id='test-consumers',
                              bootstrap_servers=['kafka-single-node:9092'],
                              auto_offset_reset='earliest',
                              enable_auto_commit=True)

    consumer1.subscribe(['raw'])
    consumer2.subscribe(['raw'])

    while True:
        messages1 = consumer1.poll()
        for message in messages1:
            print("Consumer 1:", message.topic, message.partition, message.offset, message.key, message.value)

        messages2 = consumer2.poll()
        for message in messages2:
            print("Consumer 2:", message.topic, message.partition, message.offset, message.key, message.value)
