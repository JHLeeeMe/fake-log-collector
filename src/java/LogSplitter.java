import java.util.HashMap;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class LogSplitter {
    private static final Pattern nginx_pattern = Pattern.compile(
        "(?<remoteAddr>.+) - (?<remoteUser>.+) \\[(?<timestamp>.+)\\] \"(?<request>.+)\" (?<statusCode>\\d+) (?<bodyBytesSent>\\d+) \"(?<httpReferer>.+)\" \"(?<userAgent>.+)\"");
    private static final Pattern[] apache_patterns = {
        Pattern.compile(
            "(?<remoteAddr>.+) - (?<remoteUser>.+) \\[(?<timestamp>.+)\\] \"(?<request>.+)\" (?<statusCode>\\d+) (?<bodyBytesSent>\\d+)"),
        Pattern.compile(
            "(?<remoteAddr>.+) - (?<remoteUser>.+) \\[(?<timestamp>.+)\\] \"(?<request>.+)\" (?<statusCode>\\d+) (?<bodyBytesSent>\\d+) \"(?<httpReferer>.+)\" \"(?<userAgent>.+)\"")
    };
    private static final Pattern flask_pattern = Pattern.compile(
        "\\[(?<asctime>.+)\\] (?<levelname>.+) in (?<module>.+): (?<message>.+) \\[in (?<filepath>.+):(?<lineno>\\d+)\\]");

    public static Map<String, String> nginxSplit(String value) {
        Map<String, String> ret = new HashMap<>();

        Matcher matcher = nginx_pattern.matcher(value);
        if (matcher.find()) {
            ret.put("remoteAddr", matcher.group("remoteAddr"));
            ret.put("remoteUser", matcher.group("remoteUser"));
            ret.put("timestamp", matcher.group("timestamp"));
            ret.put("request", matcher.group("request"));
            ret.put("statusCode", matcher.group("statusCode"));
            ret.put("bodyBytesSent", matcher.group("bodyBytesSent"));
            ret.put("httpReferer", matcher.group("httpReferer"));
        }

        return ret;
    }

    public static Map<String, String> apacheSplit(String format, String value) {
        assert (format.equals("common") || format.equals("combined"));

        Map<String, String> ret = new HashMap<>();

        Matcher matcher;
        if (format.equals("common")) {
            matcher = apache_patterns[0].matcher(value);
        } else {
            matcher = apache_patterns[1].matcher(value);
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

        Matcher matcher = flask_pattern.matcher(value);
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