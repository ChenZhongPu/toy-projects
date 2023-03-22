package swufe.zpchen.simdutf.naive;

import jdk.incubator.vector.ByteVector;
import jdk.incubator.vector.VectorSpecies;

import java.math.BigInteger;
import java.nio.ByteBuffer;
import java.nio.charset.CharacterCodingException;
import java.nio.charset.StandardCharsets;

public class ASCIIVerify {
    private static final byte ASCII_START = (byte) 0b0000_0000;

    public static boolean checkByRange(byte[] bytes) {
        for (byte b : bytes) {
            if (b < ASCII_START) return false;
        }
        return true;
    }

    public static boolean checkBySWAR(byte[] bytes) {
        // SIMD within a register, every 8 (default) bytes
        // mask is: 1000_0000
        int width = 8;
        long mask = new BigInteger("80".repeat(width), 16).longValue();
        int upperBound = width * (bytes.length / width);
        var i = 0;
        for (; i < upperBound; i += width) {
            var buffer = ByteBuffer.wrap(bytes, i, width).getLong() & mask;
            if (buffer != 0) return false;
        }
        for (; i < bytes.length; i++) {
            if (bytes[i] < ASCII_START) return false;
        }
        return true;
    }

    public static boolean checkBySIMD(byte[] bytes) {
        VectorSpecies<Byte> species = ByteVector.SPECIES_PREFERRED;
        var upperBound = species.loopBound(bytes.length);
        var i = 0;
        for (; i < upperBound; i += species.length()) {
            var buffer = ByteVector.fromArray(species, bytes, i);
            if (!buffer.and((byte) 0x80).eq((byte) 0x00).allTrue()) return false;
        }
        for (; i < bytes.length; i++) {
            if (bytes[i] < ASCII_START) return false;
        }
        return true;
    }

    public static boolean checkByException(byte[] bytes) {
        try {
            StandardCharsets.US_ASCII.newDecoder().decode(ByteBuffer.wrap(bytes));
        } catch (CharacterCodingException e) {
            return false;
        }
        return true;
    }
}
