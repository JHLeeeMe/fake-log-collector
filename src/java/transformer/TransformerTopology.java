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
        Map<String, String> fieldMap = LogSplitter.split(key, value);

        if (fieldMap.size() == 0) {
            return "";
        }

        if (key.equals("flask")) {
            retVal =
                DateFormatConverter.convert(key, fieldMap.get("asctime")) + ","
                + fieldMap.get("levelname") + ","
                + fieldMap.get("message");
        } else {
            retVal =
                DateFormatConverter.convert(key, fieldMap.get("timestamp")) + ","
                + fieldMap.get("remoteAddr") + ","
                + fieldMap.get("request") + ","
                + fieldMap.get("statusCode") + ","
                + fieldMap.get("bodyBytesSent");
        }

        System.out.println("Transformed Data: " + retVal);

        return retVal;
    }
}
