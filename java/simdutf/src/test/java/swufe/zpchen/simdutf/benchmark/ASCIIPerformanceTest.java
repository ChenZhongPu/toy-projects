package swufe.zpchen.simdutf.benchmark;

import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Tag;
import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;
import swufe.zpchen.simdutf.naive.ASCIIVerify;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.*;
import java.util.concurrent.ThreadLocalRandom;

@Tag("ascii")
public class ASCIIPerformanceTest {
    private static byte[] data;

    private static final int SIZE = 100_000_000;
    @BeforeAll
    static void setup() throws IOException {
        String file = System.getProperty("file");
        if (file == null || file.isEmpty()) {
            data = new byte[SIZE];
            Random random = ThreadLocalRandom.current();
            for (int i = 0; i < SIZE; i++) {
                data[i] = (byte) random.nextInt(0, 128);
            }
        } else {
            data = Files.readAllBytes(Path.of(System.getProperty("base"),
                    "data", System.getProperty("file")));
        }
    }
    @Test
    void verify() {
        double volumeMB = data.length / (1024. * 1024);
        List<BenchmarkFunction> functionList = Arrays.asList(
                new BenchmarkFunction("checkASCIIByException", () -> ASCIIVerify.checkByException(data)),
                new BenchmarkFunction("checkASCIIByRange", () -> ASCIIVerify.checkByRange(data)),
                new BenchmarkFunction("checkASCIIBySWAR", () -> ASCIIVerify.checkBySWAR(data)),
                new BenchmarkFunction("checkASCIIBySIMD", () -> ASCIIVerify.checkBySIMD(data))
        );
        // warm-up
        for (int i = 0; i < Utils.ITERATION; i++) {
            assertTrue(functionList.stream().allMatch(b -> b.supplier().get()) ||
                    functionList.stream().noneMatch(b -> b.supplier().get()));
        }
        Map<String, Long> resultMap = new LinkedHashMap<>();
        for (BenchmarkFunction b : functionList) {
            long cost = Utils.measure(b.supplier());
            resultMap.put(b.title(), cost);
        }
        String source = System.getProperty("file") == null || System.getProperty("file").isEmpty() ? "random string" : System.getProperty("file");
        System.out.printf("ASCII Measurement results from %s: \n", source);
        Utils.printStats(volumeMB, resultMap);
    }
}
