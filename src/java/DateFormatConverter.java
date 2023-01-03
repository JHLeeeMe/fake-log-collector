import java.util.Locale;
import java.time.LocalDateTime;
import java.time.ZoneId;
import java.time.ZonedDateTime;
import java.time.format.DateTimeFormatter;

public class DateFormatConverter {
    private static final DateTimeFormatter DEFAULT_INPUT_FORMATTER = DateTimeFormatter.ofPattern("dd/MMM/yyyy:HH:mm:ss Z").withLocale(Locale.US);
    private static final DateTimeFormatter FLASK_INPUT_FORMATTER = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");
    private static final DateTimeFormatter OUTPUT_FORMATTER = DateTimeFormatter.ofPattern("yyyy-MM-dd'T'HH:mm:ssZ");

    public static String convert(String key, String inputTimestamp) {
        if (key.equals("flask")) {
            LocalDateTime dateTime = LocalDateTime.parse(inputTimestamp, FLASK_INPUT_FORMATTER);
            ZonedDateTime seoulDateTime = dateTime.atZone(ZoneId.of("Asia/Seoul"));

            return seoulDateTime.format(OUTPUT_FORMATTER);
        }

        ZonedDateTime dateTime = ZonedDateTime.parse(inputTimestamp, DEFAULT_INPUT_FORMATTER);

        return dateTime.format(OUTPUT_FORMATTER);
    }
}
