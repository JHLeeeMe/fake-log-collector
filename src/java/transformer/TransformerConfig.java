import java.util.Properties;

import org.apache.kafka.common.serialization.Serde;
import org.apache.kafka.common.serialization.Serdes;
import org.apache.kafka.streams.StreamsConfig;

public class TransformerConfig {
    private Properties config = new Properties();
    private String bootstrapServer = "kafka-single-node:9092";
    private String applicationID = "transformer";
    private Class<? extends Serde> keySerde = Serdes.String().getClass();
    private Class<? extends Serde> valueSerde = Serdes.String().getClass();

    public TransformerConfig() {}

    public TransformerConfig(String bootstrapServer,
                             String applicationID) {
        this.bootstrapServer = bootstrapServer;
        this.applicationID = applicationID;
    }

    public TransformerConfig(Class<? extends Serde> keySerde,
                             Class<? extends Serde> valueSerde) {
        this.keySerde = keySerde;
        this.valueSerde = valueSerde;
    }

    public TransformerConfig(String bootstrapServer,
                             String applicationID,
                             Class<? extends Serde> keySerde,
                             Class<? extends Serde> valueSerde) {
        this.bootstrapServer = bootstrapServer;
        this.applicationID = applicationID;
        this.keySerde = keySerde;
        this.valueSerde = valueSerde;
    }

    public Properties getConfig() {
        config.put(StreamsConfig.BOOTSTRAP_SERVERS_CONFIG, this.bootstrapServer);
        config.put(StreamsConfig.APPLICATION_ID_CONFIG, this.applicationID);
        config.put(StreamsConfig.DEFAULT_KEY_SERDE_CLASS_CONFIG, this.keySerde);
        config.put(StreamsConfig.DEFAULT_VALUE_SERDE_CLASS_CONFIG, this.valueSerde);

        return config;
    }
}
