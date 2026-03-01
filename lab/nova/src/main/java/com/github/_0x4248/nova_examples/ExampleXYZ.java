package com.github._0x4248.nova_examples;

import com.github._0x4248.nova.BIOS.BIOS;
import com.github._0x4248.nova.BIOS.BiosRuntime;
import com.github._0x4248.nova.BIOS.machines.StandardMachine;

public class ExampleXYZ {
    public static final int BOOTFLAG = 1;

    public static void main(String[] args) {
        System.out.println("Launching Example XYZ...");
        BiosRuntime bios = BIOS.getRuntime();
        if (bios == null) {
            bios = new BiosRuntime(new StandardMachine());
        }

        bios.clear(1);
        bios.drawText(24, 32, "HELLO WORLD", 15, 1, false);
        bios.drawText(24, 48, "NOVA EXAMPLE XYZ", 14, 1, false);

        if (!bios.getMachine().supportsSound) {
            bios.drawText(24, 64, "NO SOUND DEVICE", 12, 1, false);
        }

        bios.present();
        try{
            Thread.sleep(2500);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        while (true) {
                bios.clear(0);
                bios.beep(200, 440);
                bios.present();
                try {
                    Thread.sleep(1000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                bios.drawText(0, 0, "This is just an example", 15, 0, false);
                bios.present();
                try {
                    Thread.sleep(1000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
        }
    }
}
