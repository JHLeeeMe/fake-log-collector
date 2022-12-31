import java.time.LocalDateTime;
import java.time.ZoneId;
import java.time.ZonedDateTime;
import java.time.format.DateTimeFormatter;

public class DateFormatConverter {
    private static final DateTimeFormatter INPUT_FORMATTER = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");
    private static final DateTimeFormatter OUTPUT_FORMATTER = DateTimeFormatter.ofPattern("yyyy-MM-dd'T'HH:mm:ssZ");

    public static String convert(String inputTimestamp) {
        LocalDateTime dateTime = LocalDateTime.parse(inputTimestamp, INPUT_FORMATTER);
        ZonedDateTime seoulDateTime = dateTime.atZone(ZoneId.of("Asia/Seoul"));

        return seoulDateTime.format(OUTPUT_FORMATTER);
    }
}
