package lab.decomplab.src;

import java.io.*;
import java.nio.*;
import java.nio.file.*;

public class DecompLib {

    private final ByteBuffer buf;

    public DecompLib(byte[] data) {
        buf = ByteBuffer.wrap(data).order(ByteOrder.LITTLE_ENDIAN);
    }

    public byte readUInt8() {
        return buf.get();
    }

    public short readUInt16() {
        return (short) (buf.getShort() & 0xFFFF);
    }

    public int readUInt32() {
        return buf.getInt();
    }

    public long readUInt64() {
        return buf.getLong();
    }

    public char readChar() {
        return (char) buf.get();
    }

    public int readInt() {
        return buf.getInt();
    }

    public void skip(int n) {
        buf.position(buf.position() + n);
    }
}
