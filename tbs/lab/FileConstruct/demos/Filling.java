/* FileConstruct
 * Construct custom files with ease.
 *
 * Examples/Filling.java
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

public class Filling {

    public static void main(String[] args) {
        FileConstructor FC = new FileConstructor();
        FC.filename = "demo.dat";

        FC.Fill(100, new byte[] { 0x00 });
        FC.Fill(100, new byte[] { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05 });

        FC.Dump();
    }
}
