/* FileConstruct
 * Construct custom files with ease.
 *
 * Examples/HelloWorld.java
 *
 * COPYRIGHT NOTICE
 * Copyright (C) 2024-2026 0x4248 and contributors
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the license is not changed.
 *
 * This software is free and open source. Licensed under the GNU general
 * public license version 3.0 as published by the Free Software Foundation.
 */

package lab.FileConstruct.demos;

import lab.FileConstruct.FileConstructor;

public class HelloWorld {

    public static void main(String[] args) {
        // Basic strings
        String HELLO = "Hello";
        String COMMA = ",";

        // Single byte
        byte SPACE = (byte) 0x20;

        // Byte array
        byte[] WORLD = new byte[] {
            (byte) 0x57,
            (byte) 0x6F,
            (byte) 0x72,
            (byte) 0x6C,
            (byte) 0x64,
        };

        // Hex string
        String ENDING = "21 0A";

        // Create a new file constructor
        FileConstructor FC = new FileConstructor();
        FC.filename = "hello_world.txt";

        // Add the bytes to the file
        FC.PutASCII(HELLO);
        FC.PutASCII(COMMA);
        FC.PutByte(SPACE);
        FC.PutBytes(WORLD);
        FC.PutHexStr(ENDING);

        // Write the file
        FC.Dump();
    }
}
