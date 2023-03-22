package swufe.zpchen.simdutf.benchmark;

import org.junit.jupiter.api.Tag;
import org.junit.jupiter.api.Test;
import org.openjdk.jmh.annotations.*;
import org.openjdk.jmh.runner.Runner;
import org.openjdk.jmh.runner.RunnerException;
import org.openjdk.jmh.runner.options.Options;
import org.openjdk.jmh.runner.options.OptionsBuilder;
import swufe.zpchen.simdutf.naive.ASCIIVerify;

import java.util.Random;
import java.util.concurrent.ThreadLocalRandom;
import java.util.concurrent.TimeUnit;

@BenchmarkMode(Mode.AverageTime)
@OutputTimeUnit(TimeUnit.MICROSECONDS)
@State(Scope.Benchmark)
@Measurement(iterations = 5)
@Fork(value = 2, warmups = 1)
@Warmup(iterations = 5)
public class ASCIIBenchmarkTest {
    @Param({"10000000"})
    int size;
    byte[] data;

    @Setup
    public void setup() {
        Random random = ThreadLocalRandom.current();
        data = new byte[size];
        for (int i = 0; i < size; i++) {
            data[i] = (byte) random.nextInt(0, 128);
        }
    }

    @Benchmark
    public void checkWithException() {
        ASCIIVerify.checkByException(data);
    }

    @Benchmark
    public void checkWithRange() {
        ASCIIVerify.checkByRange(data);
    }

    @Benchmark
    public void checkWithSWAR() {
        ASCIIVerify.checkBySWAR(data);
    }

    @Benchmark
    public void checkWithSIMD() {
        ASCIIVerify.checkBySIMD(data);
    }

    @Test
    @Tag("benchmark")
    void benchmarkRandomASCII() throws RunnerException {
        Options options = new OptionsBuilder().include(ASCIIBenchmarkTest.class.getSimpleName())
                .build();
        new Runner(options).run();
    }
}
