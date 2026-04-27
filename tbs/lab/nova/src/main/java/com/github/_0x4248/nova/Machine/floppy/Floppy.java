package com.github._0x4248.nova.Machine.floppy;

import com.github._0x4248.nova.Machine.core.Hardware;

import java.nio.file.Files;
import java.nio.file.Path;

public class Floppy implements Hardware {

    @Override
    public String id() {
        return "floppy";
    }

    public boolean isInserted(Path mediumPath) {
        return mediumPath != null && Files.exists(mediumPath) && Files.isRegularFile(mediumPath);
    }
}