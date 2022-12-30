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

        System.out.println(String.format("------- %s Log -------", key));
        System.out.println(value);
        for (String k : fieldMap.keySet()) {
            System.out.println("\t" + k + ": " + fieldMap.get(k));
            //retVal += (fieldMap.get(k));
        }

        if (fieldMap.size() == 0) {
            return "";
        }

        if (key.equals("flask")) {
            retVal = 
                fieldMap.get("asctime") + ","
                + fieldMap.get("levelname") + ","
                + fieldMap.get("message");
        } else {
            retVal = 
                fieldMap.get("timestamp") + ","
                + fieldMap.get("remoteAddr") + ","
                + fieldMap.get("request") + ","
                + fieldMap.get("statusCode") + ","
                + fieldMap.get("bodyBytesSent");
        }

        return retVal;
    }
}
