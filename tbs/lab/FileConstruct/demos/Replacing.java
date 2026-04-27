/* FileConstruct
 * Construct custom files with ease.
 *
 * Examples/Replacing.java
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

public class Replacing {

    public static void main(String[] args) {
        FileConstructor FC = new FileConstructor();
        FC.filename = "demo.dat";

        // 01 02 03 04
        FC.PutByte((byte) 0x01);
        FC.PutByte((byte) 0x02);
        FC.PutByte((byte) 0x03);
        FC.PutByte((byte) 0x04);

        // 01 02 05 04
        FC.ReplaceByte(2, (byte) 0x05);

        FC.Dump();
    }
}
