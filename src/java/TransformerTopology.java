import java.util.Map;

import org.apache.kafka.streams.StreamsBuilder;
import org.apache.kafka.streams.kstream.KStream;
import org.apache.kafka.streams.KeyValue;
import org.apache.kafka.streams.Topology;

public class TransformerTopology {
    private StreamsBuilder builder = new StreamsBuilder();

    public Topology createTopology(String inputTopic, String outputTopic) {
        KStream<String, String> inputStream = builder.stream(inputTopic);

        KStream<String, String> transformedStream = inputStream.map((key, value) -> {
            String transformedValue = transform(key, value);
            return new KeyValue<>(key, transformedValue);
        });

        transformedStream.to(outputTopic);

        return builder.build();
    }

    private String transform(String key, String value) {
        String retVal = "";
        Map<String, String> fieldMap;
        switch (key) {
            case "nginx":
                fieldMap = LogSplitter.nginxSplit(value);
                break;
            case "apache":
                fieldMap = LogSplitter.apacheSplit("combined", value);
                break;
            case "flask":
                fieldMap = LogSplitter.flaskSplit(value);
                break;
            default:
                System.out.println("######### transform() error.");
                return "######### transform() error.";
        }

        System.out.println(String.format("------- %s Log -------", key));
        System.out.println(value);
        for (String k : fieldMap.keySet()) {
            System.out.println("\t" + k + ": " + fieldMap.get(k));
            retVal += (fieldMap.get(k));
        }

        return retVal;
    }
}
