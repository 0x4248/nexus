package com.github._0x4248.nova_examples;

import com.github._0x4248.nova.Machine.core.Keyboard;
import com.github._0x4248.nova.Machine.core.Machine;
import com.github._0x4248.nova.Machine.core.MachineProgram;
import com.github._0x4248.nova.Machine.core.Video;
import com.github._0x4248.nova.Machine.gpu.GPU;

import java.awt.event.KeyEvent;

public class ExampleXYZ implements MachineProgram {

    public static void main(String[] args) {
        new ExampleXYZ().run(Machine.basic(), args);
    }

    @Override
    public void run(Machine machine, String[] args) {
        GPU gpu = machine.video().gpu;
        Keyboard keyboard = machine.keyboard();

        gpu.init(Video.Modes.CGA_TEXT_40x25);
        gpu.clear(0);
        gpu.drawText(8, 8, "NOVA TEXT MODE", 15, 0, false);
        gpu.drawText(8, 20, "Press ENTER for VGA graphics", 14, 0, false);
        gpu.drawText(8, 32, "ESC exits", 10, 0, false);
        gpu.present();

        while (true) {
            Integer keyCode = keyboard.pollKeyCode();
            if (keyCode == null) {
                sleep(10);
                continue;
            }
            if (keyCode == KeyEvent.VK_ESCAPE) {
                break;
            }
            if (keyCode == KeyEvent.VK_ENTER) {
                break;
            }
        }

        gpu.init(Video.Modes.VGA_GRAPHICS_640x480_16);
        gpu.clear(1);
        gpu.drawText(8, 8, "VGA VIDEO MODE", 15, 1, false);

        for (int x = 0; x < gpu.getWidth(); x++) {
            int y = (int) (gpu.getHeight() * 0.5 + Math.sin(x / 20.0) * 50);
            gpu.setPixel(x, y, 14);
        }
        gpu.present();

        while (true) {
            Integer keyCode = keyboard.pollKeyCode();
            if (keyCode != null && keyCode == KeyEvent.VK_ESCAPE) {
                break;
            }
            sleep(10);
        }

        gpu.shutdown();
    }

    private void sleep(long millis) {
        try {
            Thread.sleep(millis);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }
}
