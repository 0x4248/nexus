package com.github._0x4248.nova.Machine.gpu.drivers;

import com.github._0x4248.nova.Machine.gpu.GpuAdapter;
import com.github._0x4248.nova.Machine.gpu.GpuOutputMode;
import com.github._0x4248.nova.Machine.gpu.VideoMode;
import com.github._0x4248.nova.Machine.gpu.fonts.VgaFonts;

import java.util.Arrays;

public class VgaDriver implements VideoDriver {

    private final VideoMode mode;
    private final byte[] framebuffer;
    private final int[] palette;

    public VgaDriver(VideoMode mode) {
        if (mode.adapter() != GpuAdapter.VGA) {
            throw new IllegalArgumentException("VgaDriver requires a VGA mode");
        }
        this.mode = mode;
        this.framebuffer = new byte[mode.width() * mode.height()];
        this.palette = new int[256];
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
        if (!mode.supportsPaletteChanges()) {
            throw new IllegalStateException("Palette changes are not supported in mode " + mode.name());
        }
        if (index < 0 || index >= mode.colors()) {
            return;
        }
        int red = clamp8(r);
        int green = clamp8(g);
        int blue = clamp8(b);
        palette[index] = (red << 16) | (green << 8) | blue;
    }

    @Override
    public int[] toRgbBuffer() {
        int[] rgb = new int[framebuffer.length];
        for (int i = 0; i < framebuffer.length; i++) {
            rgb[i] = palette[framebuffer[i] & 0xFF];
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

    private int clamp8(int value) {
        return Math.max(0, Math.min(255, value));
    }

    private void initializePalette() {
        int[] ega16 = {
                0x000000, 0x0000AA, 0x00AA00, 0x00AAAA,
                0xAA0000, 0xAA00AA, 0xAA5500, 0xAAAAAA,
                0x555555, 0x5555FF, 0x55FF55, 0x55FFFF,
                0xFF5555, 0xFF55FF, 0xFFFF55, 0xFFFFFF
        };

        System.arraycopy(ega16, 0, palette, 0, ega16.length);

        for (int i = 16; i < 256; i++) {
            int gray = (int) Math.round(((i - 16) / 239.0) * 255.0);
            palette[i] = (gray << 16) | (gray << 8) | gray;
        }
    }
}
