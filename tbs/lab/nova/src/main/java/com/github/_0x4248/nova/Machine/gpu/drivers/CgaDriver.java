package com.github._0x4248.nova.Machine.gpu.drivers;

import com.github._0x4248.nova.Machine.gpu.GpuAdapter;
import com.github._0x4248.nova.Machine.gpu.GpuOutputMode;
import com.github._0x4248.nova.Machine.gpu.VideoMode;
import com.github._0x4248.nova.Machine.gpu.fonts.VgaFonts;

import java.util.Arrays;

public class CgaDriver implements VideoDriver {

    private final VideoMode mode;
    private final byte[] framebuffer;
    private final int[] palette;

    public CgaDriver(VideoMode mode) {
        if (mode.adapter() != GpuAdapter.CGA) {
            throw new IllegalArgumentException("CgaDriver requires a CGA mode");
        }
        this.mode = mode;
        this.framebuffer = new byte[mode.width() * mode.height()];
        this.palette = new int[16];
        initializePalette();
    }

    @Override
    public int getWidth() {
        return mode.width();
    }

    @Override
    public int getHeight() {
        return mode.height();
    }

    @Override
    public int getColorCount() {
        return mode.colors();
    }

    @Override
    public GpuOutputMode getOutputMode() {
        return mode.outputMode();
    }

    @Override
    public void clear(int colorIndex) {
        Arrays.fill(framebuffer, (byte) sanitizeColor(colorIndex));
    }

    @Override
    public void drawText(int x, int y, String text, int foregroundColor, int backgroundColor, boolean transparentBackground) {
        int cursorX = x;
        int cursorY = y;

        for (int i = 0; i < text.length(); i++) {
            char character = text.charAt(i);
            if (character == '\n') {
                cursorX = x;
                cursorY += 8;
                continue;
            }

            drawChar(cursorX, cursorY, character, foregroundColor, backgroundColor, transparentBackground);
            cursorX += 8;
        }
    }

    @Override
    public void setPixel(int x, int y, int colorIndex) {
        if (mode.outputMode() != GpuOutputMode.VIDEO) {
            throw new IllegalStateException("Current mode is TEXT. Switch to a VIDEO mode to use pixels.");
        }
        if (x < 0 || y < 0 || x >= mode.width() || y >= mode.height()) {
            return;
        }
        framebuffer[y * mode.width() + x] = (byte) sanitizeColor(colorIndex);
    }

    @Override
    public void setPaletteEntry(int index, int r, int g, int b) {
        throw new IllegalStateException("CGA palette is fixed in this driver");
    }

    @Override
    public int[] toRgbBuffer() {
        int[] rgb = new int[framebuffer.length];
        for (int i = 0; i < framebuffer.length; i++) {
            rgb[i] = palette[framebuffer[i] & 0x0F];
        }
        return rgb;
    }

    private void drawChar(int x, int y, char character, int foregroundColor, int backgroundColor, boolean transparentBackground) {
        int glyphIndex = character & 0x7F;
        if (glyphIndex >= VgaFonts.LATIN8x8.length) {
            glyphIndex = '?';
        }

        byte[] glyph = VgaFonts.LATIN8x8[glyphIndex];
        int foreground = sanitizeColor(foregroundColor);
        int background = sanitizeColor(backgroundColor);

        for (int row = 0; row < 8; row++) {
            int rowBits = glyph[row] & 0xFF;
            for (int col = 0; col < 8; col++) {
                boolean on = ((rowBits >> col) & 1) == 1;
                if (on) {
                    writePixel(x + col, y + row, foreground);
                } else if (!transparentBackground) {
                    writePixel(x + col, y + row, background);
                }
            }
        }
    }

    private void writePixel(int x, int y, int colorIndex) {
        if (x < 0 || y < 0 || x >= mode.width() || y >= mode.height()) {
            return;
        }
        framebuffer[y * mode.width() + x] = (byte) sanitizeColor(colorIndex);
    }

    private int sanitizeColor(int colorIndex) {
        return Math.floorMod(colorIndex, mode.colors());
    }

    private void initializePalette() {
        int[] cga16 = {
                0x000000, 0x0000AA, 0x00AA00, 0x00AAAA,
                0xAA0000, 0xAA00AA, 0xAA5500, 0xAAAAAA,
                0x555555, 0x5555FF, 0x55FF55, 0x55FFFF,
                0xFF5555, 0xFF55FF, 0xFFFF55, 0xFFFFFF
        };

        System.arraycopy(cga16, 0, palette, 0, cga16.length);
    }
}
