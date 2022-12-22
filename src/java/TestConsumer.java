import java.time.Duration;
import java.util.Properties;
import java.util.Collections;

import org.apache.kafka.clients.consumer.KafkaConsumer;
import org.apache.kafka.clients.consumer.ConsumerConfig;
import org.apache.kafka.clients.consumer.ConsumerRecord;
import org.apache.kafka.clients.consumer.ConsumerRecords;
import org.apache.kafka.common.serialization.StringDeserializer;

public class TestConsumer {
    public static void main(String[] args) {
        // Set up the configuration for the Kafka consumer
        Properties config = new Properties();
        config.put(ConsumerConfig.BOOTSTRAP_SERVERS_CONFIG, "kafka-single-node:9092");
        config.put(ConsumerConfig.GROUP_ID_CONFIG, "test-consumer-group");
        config.put(ConsumerConfig.AUTO_OFFSET_RESET_CONFIG, "earliest");
        config.put(ConsumerConfig.ENABLE_AUTO_COMMIT_CONFIG, "true");
        config.put(ConsumerConfig.KEY_DESERIALIZER_CLASS_CONFIG, StringDeserializer.class.getName());
        config.put(ConsumerConfig.VALUE_DESERIALIZER_CLASS_CONFIG, StringDeserializer.class.getName());

        // Create the Kafka consumer
        KafkaConsumer<String, String> consumer = new KafkaConsumer<>(config);

        // Subscribe to the 'raw' topic
        consumer.subscribe(Collections.singletonList("raw"));

        // Poll for new messages and process them
        try {
            while (true) {
                ConsumerRecords<String, String> records = consumer.poll(Duration.ofMillis(1000));
                for (ConsumerRecord<String, String> record : records) {
                    String key = record.key();
                    String value = record.value();
                    System.out.println("Key: " + key);
                    System.out.println("Value: " + value.substring(0, 90) + "...");
                }
            }
        } catch (Exception e) {
            System.out.println("error: " + e);
        } finally {
            consumer.close();
        }
    }
}
