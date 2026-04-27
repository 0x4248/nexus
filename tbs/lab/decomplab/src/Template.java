package lab.decomplab.src;

import java.io.*;
import java.nio.*;
import java.nio.charset.StandardCharsets;
import java.nio.file.*;

public class Template {

    public class BinaryAsciiUtils {

        public static String readAscii(ByteBuffer buf, int len) {
            byte[] bytes = new byte[len];
            buf.get(bytes);
            return new String(bytes, StandardCharsets.US_ASCII);
        }

        public static String shortToAscii(short value) {
            byte low = (byte) (value & 0xFF);
            byte high = (byte) ((value >> 8) & 0xFF);
            return new String(
                new byte[] { low, high },
                StandardCharsets.US_ASCII
            );
        }

        public static String intToAscii(int value) {
            byte b0 = (byte) (value & 0xFF);
            byte b1 = (byte) ((value >> 8) & 0xFF);
            byte b2 = (byte) ((value >> 16) & 0xFF);
            byte b3 = (byte) ((value >> 24) & 0xFF);
            return new String(
                new byte[] { b0, b1, b2, b3 },
                StandardCharsets.US_ASCII
            );
        }

        public static String longToAscii(long value) {
            byte b0 = (byte) (value & 0xFF);
            byte b1 = (byte) ((value >> 8) & 0xFF);
            byte b2 = (byte) ((value >> 16) & 0xFF);
            byte b3 = (byte) ((value >> 24) & 0xFF);
            byte b4 = (byte) ((value >> 32) & 0xFF);
            byte b5 = (byte) ((value >> 40) & 0xFF);
            byte b6 = (byte) ((value >> 48) & 0xFF);
            byte b7 = (byte) ((value >> 56) & 0xFF);
            return new String(
                new byte[] { b0, b1, b2, b3, b4, b5, b6, b7 },
                StandardCharsets.US_ASCII
            );
        }
    }

    static class HeaderA {

        byte flag;
        short version;
        int dataSize;
        long timestamp;

        void read(ByteBuffer buf) {
            flag = buf.get();
            version = buf.getShort();
            dataSize = buf.getInt();
            timestamp = buf.getLong();
        }

        void print() {
            System.out.println("HeaderA:");
            System.out.println("  flag: " + flag);
            System.out.println("  version: " + version);
            System.out.println("  dataSize: " + dataSize);
            System.out.println(
                "  timestamp: " + BinaryAsciiUtils.longToAscii(timestamp)
            );
        }
    }

    static class HeaderB {

        char label;
        int count;
        short mode;
        byte active;

        void read(ByteBuffer buf) {
            label = (char) buf.get();
            count = buf.getInt();
            mode = buf.getShort();
            active = buf.get();
        }

        void print() {
            System.out.println("HeaderB:");
            System.out.println("  label: " + label);
            System.out.println("  count: " + count);
            System.out.println("  mode: " + mode);
            System.out.println("  active: " + active);
        }
    }

    public static void main(String[] args) throws IOException {
        if (args.length != 1) {
            System.out.println("Usage: java Template <file>");
            return;
        }

        byte[] data = Files.readAllBytes(Path.of(args[0]));
        ByteBuffer buf = ByteBuffer.wrap(data).order(ByteOrder.LITTLE_ENDIAN);

        HeaderA headA = new HeaderA();
        headA.read(buf);
        headA.print();

        buf.position(buf.position() + 10);

        HeaderB headB = new HeaderB();
        headB.read(buf);
        headB.print();
    }
}
