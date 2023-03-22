package swufe.zpchen.simdutf.unit;

import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Tag;
import org.junit.jupiter.api.Test;
import swufe.zpchen.simdutf.naive.ASCIIVerify;

import java.io.IOException;
import java.net.URISyntaxException;
import java.nio.file.Files;
import java.nio.file.Path;

import static org.junit.jupiter.api.Assertions.*;

@Tag("fast")
class ASCIIVerifyTest {
    @Test
    void testValidASCII() {
        byte[] data = "abcdefg".getBytes();
        Assertions.assertTrue(ASCIIVerify.checkByRange(data));
        assertTrue(ASCIIVerify.checkByException(data));
        assertTrue(ASCIIVerify.checkBySWAR(data));
        assertTrue(ASCIIVerify.checkBySIMD(data));
    }

    @Test
    void testLatinInASCII() {
        byte[] data = "abcdɛefg€".getBytes();
        assertFalse(ASCIIVerify.checkByException(data));
        assertFalse(ASCIIVerify.checkByException(data));
        assertFalse(ASCIIVerify.checkBySWAR(data));
        assertFalse(ASCIIVerify.checkBySIMD(data));
    }
    @Test
    void testEmojiInASCII() {
        byte[] data = "abcd\uD83D\uDE0Aefg".getBytes();
        assertFalse(ASCIIVerify.checkByRange(data));
        assertFalse(ASCIIVerify.checkByException(data));
        assertFalse(ASCIIVerify.checkBySWAR(data));
        assertFalse(ASCIIVerify.checkBySIMD(data));
    }

    @Test
    void testCJKInASCII() {
        byte[] data = "abc你わdef어".getBytes();
        assertFalse(ASCIIVerify.checkByException(data));
        assertFalse(ASCIIVerify.checkByException(data));
        assertFalse(ASCIIVerify.checkBySWAR(data));
        assertFalse(ASCIIVerify.checkBySIMD(data));
    }

    @Test
    void testValidASCIIFromFile() throws URISyntaxException, IOException {
        byte[] data = Files.readAllBytes(Path.of(getClass().getClassLoader().getResource("us.txt").toURI()));
        assertTrue(ASCIIVerify.checkByRange(data));
        assertTrue(ASCIIVerify.checkByException(data));
        assertTrue(ASCIIVerify.checkBySWAR(data));
        assertTrue(ASCIIVerify.checkBySIMD(data));
    }

    @Test
    void testInvalidASCIIFromFile() throws URISyntaxException, IOException {
        byte[] data = Files.readAllBytes(Path.of(getClass().getClassLoader().getResource("emoji.txt").toURI()));
        assertFalse(ASCIIVerify.checkByException(data));
        assertFalse(ASCIIVerify.checkByException(data));
        assertFalse(ASCIIVerify.checkBySWAR(data));
        assertFalse(ASCIIVerify.checkBySIMD(data));
    }
}