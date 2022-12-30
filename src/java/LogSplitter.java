import java.util.HashMap;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class LogSplitter {
    private static final String COMMON_FORMAT = "(?<remoteAddr>.+) - (?<remoteUser>.+) \\[(?<timestamp>.+)\\] \"(?<request>.+)\" (?<statusCode>\\d+) (?<bodyBytesSent>\\d+)";
    private static final String COMBINED_FORMAT = " \"(?<httpReferer>.+)\" \"(?<userAgent>.+)\"";
    private static final Pattern NGINX_PATTERN = Pattern.compile(COMMON_FORMAT + COMBINED_FORMAT);
    private static final Pattern[] APACHE_PATTERNS = {Pattern.compile(COMMON_FORMAT),
                                                      Pattern.compile(COMMON_FORMAT + COMBINED_FORMAT)};
    private static final Pattern FLASK_PATTERN = Pattern.compile(
        "\\[(?<asctime>.+)\\] (?<levelname>.+) in (?<module>.+): (?<message>.+) \\[in (?<filepath>.+):(?<lineno>\\d+)\\]");

    public static Map<String, String> split(String key, String value) {
        Map<String, String> ret;

        switch (key) {
            case "nginx":
                ret = LogSplitter.nginxSplit(value);
                break;
            case "apache":
                ret = LogSplitter.apacheSplit("combined", value);
                break;
            case "flask":
                ret = LogSplitter.flaskSplit(value);
                break;
            default:
                System.out.println("######### transform() error.");
                ret = new HashMap<String, String>();
                break;
        }

        return ret;
    }

    public static Map<String, String> nginxSplit(String value) {
        Map<String, String> ret = new HashMap<>();

        Matcher matcher = NGINX_PATTERN.matcher(value);
        if (matcher.find()) {
            ret.put("remoteAddr", matcher.group("remoteAddr"));
            ret.put("remoteUser", matcher.group("remoteUser"));
            ret.put("timestamp", matcher.group("timestamp"));
            ret.put("request", matcher.group("request"));
            ret.put("statusCode", matcher.group("statusCode"));
            ret.put("bodyBytesSent", matcher.group("bodyBytesSent"));
            ret.put("httpReferer", matcher.group("httpReferer"));
            ret.put("userAgent", matcher.group("userAgent"));
        }

        return ret;
    }

    public static Map<String, String> apacheSplit(String format, String value) {
        assert (format.equals("common") || format.equals("combined"));

        Map<String, String> ret = new HashMap<>();

        Matcher matcher;
        if (format.equals("common")) {
            matcher = APACHE_PATTERNS[0].matcher(value);
        } else {
            matcher = APACHE_PATTERNS[1].matcher(value);
        }

        if (matcher.find()) {
            ret.put("remoteAddr", matcher.group("remoteAddr"));
            ret.put("remoteUser", matcher.group("remoteUser"));
            ret.put("timestamp", matcher.group("timestamp"));
            ret.put("request", matcher.group("request"));
            ret.put("statusCode", matcher.group("statusCode"));
            ret.put("bodyBytesSent", matcher.group("bodyBytesSent"));
            if (format.equals("combined")) {
                ret.put("httpReferer", matcher.group("httpReferer"));
                ret.put("userAgent", matcher.group("userAgent"));
            }
        }

        return ret;
    }

    public static Map<String, String> flaskSplit(String value) {
        Map<String, String> ret = new HashMap<>();

        Matcher matcher = FLASK_PATTERN.matcher(value);
        if (matcher.find()) {
            ret.put("asctime", matcher.group("asctime"));
            ret.put("levelname", matcher.group("levelname"));
            ret.put("module", matcher.group("module"));
            ret.put("message", matcher.group("message"));
            ret.put("filepath", matcher.group("filepath"));
            ret.put("lineno", matcher.group("lineno"));
        }

        return ret;
    }
}