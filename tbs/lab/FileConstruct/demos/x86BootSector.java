/* FileConstruct
 * Construct custom files with ease.
 *
 * Examples/x86BootSector.java
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

class x86BootSector {

    // Boot sector signature
    public static String SIGNATURE = "55 AA";

    public static void main(String[] args) {
        // Create a new file constructor
        FileConstructor FC = new FileConstructor();
        FC.filename = "bootdisk.img";

        // Resize the disk image to 510 bytes
        int fill = 510 - FC.size;
        FC.Fill(fill, new byte[] { 0 });

        // Place in the boot sector signature
        FC.PutHexStr(SIGNATURE);

        // Dump the file
        FC.Dump();
    }
}
