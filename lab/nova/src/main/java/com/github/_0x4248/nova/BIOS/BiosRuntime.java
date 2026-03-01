package com.github._0x4248.nova.BIOS;

import com.github._0x4248.nova.BIOS.VGA.VGA;
import com.github._0x4248.nova.BIOS.machines.Machine;
import com.github._0x4248.nova.Core.Gui;

public class BiosRuntime {

    private final Machine machine;
    private final VGA vga;
    private final Gui screen;

    public BiosRuntime(Machine machine) {
        this.machine = machine;
        this.vga = new VGA();
        this.screen = new Gui(vga.getWidth() * machine.screenScale, vga.getHeight() * machine.screenScale);
    }

    public Machine getMachine() {
        return machine;
    }

    public int getWidth() {
        return vga.getWidth();
    }

    public int getHeight() {
        return vga.getHeight();
    }

    public int getTextColumns() {
        return getWidth() / 8;
    }

    public void clear(int colorIndex) {
        vga.clear(colorIndex);
    }

    public void drawText(int x, int y, String text, int foregroundColor, int backgroundColor, boolean transparentBackground) {
        vga.drawText(x, y, text, foregroundColor, backgroundColor, transparentBackground);
    }

    public void setPixel(int x, int y, int colorIndex) {
        vga.setPixel(x, y, colorIndex);
    }

    public void present() {
        vga.blitToGui(screen, machine.screenScale);
    }

    public void beep(int lengthMs, int pitchHz) {
        if (!machine.supportsSound) {
            return;
        }
        screen.biosBeep(lengthMs, pitchHz);
    }

    public boolean hasKeyPress() {
        if (!machine.supportsKeyboard) {
            return false;
        }
        return screen.hasKeyPress();
    }

    public Integer pollKeyCode() {
        if (!machine.supportsKeyboard) {
            return null;
        }
        return screen.pollKeyCode();
    }
}
