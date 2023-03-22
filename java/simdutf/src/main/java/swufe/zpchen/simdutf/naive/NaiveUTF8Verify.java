package swufe.zpchen.simdutf.naive;

import java.nio.ByteBuffer;
import java.nio.charset.CharacterCodingException;
import java.nio.charset.StandardCharsets;

/**
 * Java does not have unsigned byte. (from -128 to 127)
 * -1 means 225; -128 means 128
 */
public class NaiveUTF8Verify {
    private static final byte ASCII_START = 0b0000_0000;
    private static final byte LATIN_START = (byte) 0b1100_0010;
    private static final byte LATIN_END = (byte) 0b1101_1111;

    private static final byte ASIA_START1 = (byte) 0b1110_0000;
    private static final byte ASIA_START2 = (byte) 0b1110_1101;
    private static final byte ASIA_START3 = (byte) 0b1110_0001;
    private static final byte ASIA_END3 = (byte) 0b1110_1100;
    private static final byte ASIA_START4 = (byte) 0b1110_1110;
    private static final byte ASIA_END4 = (byte) 0b1110_1111;
    private static final byte OTHER_START = (byte) 0b1111_0000;

    private static final byte OTHER_START2 = (byte) 0b1111_0001;
    private static final byte OTHER_END2 = (byte) 0b1111_0011;
    private static final byte OTHER_START3 = (byte) 0b1111_0100;
    private static final byte OVERLONG_END = (byte) 0b1001_1111;
    private static final byte OVERLONG_END2 = (byte) 0b1000_1111;

    private static final byte SURROGATE_START = (byte) 0b1010_0000;
    private static final byte SURROGATE_END = (byte) 0b1011_1111;

    public static boolean isNotContinuation(byte b) {
        return (b & 0xff) >>> 6 != 2; // not starting with 0b10
    }

    public static boolean checkByException(byte[] bytes) {
        // https://stackoverflow.com/questions/59094164/verifying-a-string-is-utf-8-encoded-in-java
        try {
            StandardCharsets.UTF_8.newDecoder()
                    .decode(ByteBuffer.wrap(bytes));
        } catch (CharacterCodingException e) {
            return false;
        }
        return true;
    }

    public static boolean checkRange(byte[] bytes) {
        // branchy range validator
        for (int i = 0; i < bytes.length; ) {
            byte b = bytes[i];
            if (b >= ASCII_START) {
                // [0b0000_0000, 0b0111_1111]. It is ASCII U+0..7F
                // note that it is can never greater than 0b0111_1111
                i++;
            } else if (b >= LATIN_START && b <= LATIN_END) {
                // [0b1100_0010, 0b1101_1111]. It is Latin (2-byte) U+80..7FF
                // load the next byte, return an error if it is not a continuation starting with 0b10
                if (i + 1 >= bytes.length || isNotContinuation(bytes[i + 1])) return false;
                i += 2;
            } else if (b == ASIA_START1) {
                // It is Asia (3-byte) U+800...FFF (first case)
                // load the next two bytes, return error on EOF or iif not continuation starting with 0b10
                if (i + 2 >= bytes.length || isNotContinuation(bytes[i + 1]) || isNotContinuation(bytes[i + 2]))
                    return false;
                // it may be overlong if the next is in [0b1000_0000, 0b1001_1111]
                // the start 0b1000_0000 (-128) is implicit
                if (bytes[i + 1] <= OVERLONG_END) return false;
                i += 3;
            } else if (b == ASIA_START2) {
                // It is Asia (3-byte) U+D000...D7FF
                // load the next two bytes, return error on EOF or iif not continuation starting with 0b10
                if (i + 2 >= bytes.length || isNotContinuation(bytes[i + 1]) || isNotContinuation(bytes[i + 2]))
                    return false;
                // it many be surrogate if the next is in [0b1010_0000, 0b1011_1111]
                if (bytes[i + 1] >= SURROGATE_START && bytes[i + 1] <= SURROGATE_END)
                    return false;
                i += 3;
            } else if ((b >= ASIA_START3 && b <= ASIA_END3) || (b >= ASIA_START4 && b <= ASIA_END4)) {
                // It is Asia (3-byte) U+1000..CFFF, U+E000...FFFF
                // load the next two bytes, return error on EOF or iif not continuation starting with 0b10
                if (i + 2 >= bytes.length || isNotContinuation(bytes[i + 1]) || isNotContinuation(bytes[i + 2]))
                    return false;
                i += 3;
            } else if (b == OTHER_START) {
                // It is 4 byte U+10000...3FFFF
                // load the next three bytes, return error on EOF or iif not continuation starting with 0b10
                if (i + 3 >= bytes.length || isNotContinuation(bytes[i + 1]) ||
                        isNotContinuation(bytes[i + 2]) || isNotContinuation(bytes[i + 3])) {
                    return false;
                }
                // it may be overlong if the next is in [0b1000_0000, 0b1000_1111]
                if (bytes[i + 1] <= OVERLONG_END2) return false;
                i += 4;
            } else if (b >= OTHER_START2 && b <= OTHER_END2) {
                // it is 4-byte U+40000...FFFFF
                // load the next three bytes, return error on EOF or iif not continuation starting with 0b10
                if (i + 3 >= bytes.length || isNotContinuation(bytes[i + 1]) ||
                        isNotContinuation(bytes[i + 2]) || isNotContinuation(bytes[i + 3])) {
                    return false;
                }
                i += 4;
            } else if (b == OTHER_START3) {
                // it is 4-byte U+100000..10FFFF
                // load the next three bytes, return error on EOF or iif not continuation starting with 0b10
                if (i + 3 >= bytes.length || isNotContinuation(bytes[i + 1]) ||
                        isNotContinuation(bytes[i + 2]) || isNotContinuation(bytes[i + 3])) {
                    return false;
                }
                // it may be too large if the next is in [0b1010_0000, 0b1011_1111]
                if (bytes[i + 1] >= SURROGATE_START && bytes[i + 1] <= SURROGATE_END)
                    return false;
                i += 4;
            } else {
                // too long (5+ byte)
                return false;
            }
        }
        return true;
    }
}
