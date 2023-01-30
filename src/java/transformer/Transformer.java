import java.util.Properties;

import org.apache.kafka.streams.KafkaStreams;
import org.apache.kafka.streams.Topology;

public class Transformer {
    public static void main(String[] args) {
        TransformerConfig transformerConfig = new TransformerConfig();
        Properties props = transformerConfig.getConfig();

        TransformerTopology transformerTopology = new TransformerTopology();
        Topology topology = transformerTopology.createTopology("raw", "transformed") ;

        KafkaStreams streams = new KafkaStreams(topology, props);
        streams.start();

        Runtime.getRuntime().addShutdownHook(new Thread(streams::close));
    }
}