/* FileConstruct
 * Construct custom files with ease.
 *
 * FileConstructor.java
 *
 * COPYRIGHT NOTICE
 * Copyright (C) 2024-2026 0x4248 and contributors
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the license is not changed.
 *
 * This software is free and open source. Licensed under the GNU general
 * public license version 3.0 as published by the Free Software Foundation.
 */

package lab.FileConstruct;

import java.util.Arrays;

public class FileConstructor {

    public byte[] data;
    public String filename;
    public int size;
    public boolean verbose;

    /**
     * Constructor object for creating a file
     */
    public FileConstructor() {
        data = new byte[0];
        size = 0;
        filename = "";
        verbose = false;
    }

    /**
     * PutByte - Place a single byte into the file
     */
    public void PutByte(byte b) {
        if (verbose) {
            String message = "[ FC ] PLACE: " + b + " LOCATION: " + size;
        }
        byte[] newData = new byte[data.length + 1];
        System.arraycopy(data, 0, newData, 0, data.length);
        newData[data.length] = b;
        size += 1;
        data = newData;
    }

    /**
     * PutBytes - Place an array of bytes into the file
     */
    public void PutBytes(byte[] b) {
        if (verbose) {
            String message =
                "[ FC ] PUTBYTES: " + Arrays.toString(b) + " LOCATION: " + size;
            System.out.println(message);
        }

        byte[] newData = new byte[data.length + b.length];
        System.arraycopy(data, 0, newData, 0, data.length);
        System.arraycopy(b, 0, newData, data.length, b.length);
        size += b.length;
        data = newData;
    }

    /**
     * PutASCII - Place a string into the file
     */
    public void PutASCII(String s) {
        if (verbose) {
            String message = "[ FC ] PUTASCII: " + s + " LOCATION: " + size;
            System.out.println(message);
        }
        size += s.length();
        PutBytes(s.getBytes());
    }

    /**
     * PutString - Place a string into the file
     */
    public void PutString(String s) {
        if (verbose) {
            String message = "[ FC ] PUTSTRING: " + s + " LOCATION: " + size;
            System.out.println(message);
        }

        PutBytes(s.getBytes());
    }

    /**
     * PutInt - Place an integer into the file
     */
    public void PutInt(int i) {
        if (verbose) {
            String message = "[ FC ] PUTINT: " + i + " LOCATION: " + size;
            System.out.println(message);
        }
        PutBytes(
            new byte[] {
                (byte) (i >> 24),
                (byte) (i >> 16),
                (byte) (i >> 8),
                (byte) i,
            }
        );
    }

    /**
     * PutBool - Place a boolean into the file (as a bit)
     */
    public void PutBool(boolean b) {
        if (verbose) {
            String message = "[ FC ] PUTBOOL: " + b + " LOCATION: " + size;
            System.out.println(message);
        }
        PutByte((byte) (b ? 1 : 0));
    }

    /**
     * PutBit - Place a bit into the file
     */
    public void PutBit(int b) {
        PutByte((byte) (b & 1));
    }

    /**
     * PutHexStr - Place a hex string into the file
     * Example: "55 AA" -> 0x55, 0xAA
     */
    public void PutHexStr(String s) {
        if (verbose) {
            String message = "[ FC ] PUTHEXSTR: " + s + " LOCATION: " + size;
            System.out.println(message);
        }
        String[] hex = s.split(" ");
        for (String h : hex) {
            PutByte((byte) Integer.parseInt(h, 16));
        }
    }

    /**
     * Fill - Fill the file with a byte array pattern
     */
    public void Fill(int length, byte[] b) {
        if (verbose) {
            String message =
                "[ FC ] FILL: " +
                Arrays.toString(b) +
                " LENGTH: " +
                length +
                " LOCATION: " +
                size;
            System.out.println(message);
        }
        byte[] newData = new byte[data.length + length];
        System.arraycopy(data, 0, newData, 0, data.length);
        for (int i = 0; i < length; i++) {
            newData[data.length + i] = b[i % b.length];
        }
        size += length;
        data = newData;
    }

    /**
     * ReplaceByte - Replace a byte at a specific index
     */
    public void ReplaceByte(int index, byte b) {
        if (verbose) {
            String message = "[ FC ] REPLACEBYTE: " + b + " LOCATION: " + index;
            System.out.println(message);
        }
        data[index] = b;
    }

    /**
     * ReplaceBytes - Replace a byte array at a specific index
     */
    public void ReplaceBytes(int index, byte[] b) {
        if (verbose) {
            String message =
                "[ FC ] REPLACEBYTES: " +
                Arrays.toString(b) +
                " LOCATION: " +
                index;
            System.out.println(message);
        }
        for (int i = 0; i < b.length; i++) {
            data[index + i] = b[i];
        }
    }

    /**
     * ReplaceASCII - Replace a string at a specific index
     */
    public void ReplaceASCII(int index, String s) {
        if (verbose) {
            String message =
                "[ FC ] REPLACEASCII: " + s + " LOCATION: " + index;
            System.out.println(message);
        }
        ReplaceBytes(index, s.getBytes());
    }

    /**
     * ReplaceString - Replace a string at a specific index
     */
    public void ReplaceString(int index, String s) {
        if (verbose) {
            String message =
                "[ FC ] REPLACESTRING: " + s + " LOCATION: " + index;
            System.out.println(message);
        }
        ReplaceBytes(index, s.getBytes());
    }

    /**
     * ReplaceInt - Replace an integer at a specific index
     */
    public void ReplaceInt(int index, int i) {
        if (verbose) {
            String message = "[ FC ] REPLACEINT: " + i + " LOCATION: " + index;
            System.out.println(message);
        }
        ReplaceBytes(
            index,
            new byte[] {
                (byte) (i >> 24),
                (byte) (i >> 16),
                (byte) (i >> 8),
                (byte) i,
            }
        );
    }

    /**
     * ReplaceBool - Replace a boolean at a specific index
     */
    public void ReplaceBool(int index, boolean b) {
        if (verbose) {
            String message = "[ FC ] REPLACEBOOL: " + b + " LOCATION: " + index;
            System.out.println(message);
        }
        ReplaceByte(index, (byte) (b ? 1 : 0));
    }

    /**
     * ReplaceBit - Replace a bit at a specific index
     */
    public void ReplaceBit(int index, int b) {
        if (verbose) {
            String message = "[ FC ] REPLACEBIT: " + b + " LOCATION: " + index;
            System.out.println(message);
        }
        ReplaceByte(index, (byte) (b & 1));
    }

    /**
     * Dump - Write the file to disk
     */
    public void Dump() {
        if (verbose) {
            String message = "[ FC ] DUMP: " + filename;
            System.out.println(message);
        }
        try {
            java.io.FileOutputStream fos = new java.io.FileOutputStream(
                filename
            );
            fos.write(data);
            fos.close();
            if (verbose) {
                String message =
                    "[ FC ] WROTE: " + size + " BYTES TO: " + filename;
                System.out.println(message);
            }
        } catch (java.io.IOException e) {
            e.printStackTrace();
        }
    }

    /**
     * Load - Load a file from disk
     */
    public void Load(String filename) {
        if (verbose) {
            String message = "[ FC ] LOAD: " + filename;
            System.out.println(message);
        }
        try {
            java.io.FileInputStream fis = new java.io.FileInputStream(filename);
            data = fis.readAllBytes();
            size = data.length;
            fis.close();
            if (verbose) {
                String message =
                    "[ FC ] LOADED: " + size + " BYTES FROM: " + filename;
                System.out.println(message);
            }
        } catch (java.io.IOException e) {
            e.printStackTrace();
        }
    }
}
