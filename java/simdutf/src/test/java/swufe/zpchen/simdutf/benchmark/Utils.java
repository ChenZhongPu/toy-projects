package swufe.zpchen.simdutf.benchmark;

import java.time.Duration;
import java.time.Instant;
import java.util.Map;
import java.util.function.Supplier;

record BenchmarkFunction(String title, Supplier<Boolean> supplier) {
}

public class Utils {

    public static final int ITERATION = 5;
    public static void printStats(double volumeMB, Map<String, Long> results) {
        for (var result : results.entrySet()) {
            System.out.printf("%-27s : %8.2f MB/s\n",
                    result.getKey(), volumeMB * 1e9 / result.getValue());
        }
    }
    public static long measure(Supplier<Boolean> supplier) {
        Instant start = Instant.now();
        for (int i = 0; i < ITERATION; i++) {
            supplier.get();
        }
        Instant end = Instant.now();
        return Duration.between(start, end).toNanos() / ITERATION;
    }
}
