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
        String[] fields = value.split(" ");
        switch (key) {
            case "nginx":
                for (String field : fields) {
                    retVal += ("|---nginx---|" + field);
                }
                break;
            case "apache":
                for (String field : fields) {
                    retVal += ("|---apache---|" + field);
                }
                break;
            case "flask":
                for (String field : fields) {
                    retVal += ("|---flask---|" + field);
                }
                break;
            default:
                System.out.println("default");
                break;
        }
        return retVal;
    }
}
