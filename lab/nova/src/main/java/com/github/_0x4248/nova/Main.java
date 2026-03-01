package com.github._0x4248.nova;

import com.github._0x4248.nova.BIOS.BIOS;
import com.github._0x4248.nova.BIOS.machines.StandardMachine;

import java.nio.file.Path;
import java.nio.file.Paths;

public class Main {
    private static final String DEFAULT_INTERNAL_APP = "com.github._0x4248.nova_examples.ExampleXYZ";

    public static void main(String[] args) {
        BIOS bios = new BIOS(new StandardMachine());
        Path applicationJar = Paths.get("Application.jar");

        boolean booted;
        if (java.nio.file.Files.exists(applicationJar)) {
            booted = bios.bootApplication(applicationJar, args);
        } else {
            booted = bios.bootInternalApplication(DEFAULT_INTERNAL_APP, args);
        }

        if (!booted) {
            System.exit(1);
        }
    }
}