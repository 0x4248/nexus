package com.github._0x4248.nova.Machine.gpu;

import com.github._0x4248.nova.Core.Gui;
import com.github._0x4248.nova.Machine.gpu.drivers.CgaDriver;
import com.github._0x4248.nova.Machine.gpu.drivers.VgaDriver;
import com.github._0x4248.nova.Machine.gpu.drivers.VideoDriver;

public class GPU {

    private VideoDriver driver;
    private Gui screen;
    private int scale;
    private VideoMode mode;

    public GPU init(VideoMode mode) {
        if (mode == null) {
            throw new IllegalArgumentException("mode cannot be null");
        }

        closeOutput();

        this.driver = createDriver(mode);
        this.scale = mode.scale();
        this.mode = mode;
        this.screen = new Gui(driver.getWidth() * scale, driver.getHeight() * scale);
        return this;
    }

    public VideoMode getMode() {
        ensureInitialized();
        return mode;
    }

    public String getModeName() {
        return getMode().name();
    }

    public GpuOutputMode getOutputMode() {
        ensureInitialized();
        return mode.outputMode();
    }

    public int getColorCount() {
        ensureInitialized();
        return mode.colors();
    }

    public int getWidth() {
        ensureInitialized();
        return driver.getWidth();
    }

    public int getHeight() {
        ensureInitialized();
        return driver.getHeight();
    }

    public int getTextColumns() {
        return getWidth() / 8;
    }

    public void clear(int colorIndex) {
        ensureInitialized();
        driver.clear(colorIndex);
    }

    public void drawText(int x, int y, String text, int foregroundColor, int backgroundColor, boolean transparentBackground) {
        ensureInitialized();
        driver.drawText(x, y, text, foregroundColor, backgroundColor, transparentBackground);
    }

    public void setPixel(int x, int y, int colorIndex) {
        ensureInitialized();
        driver.setPixel(x, y, colorIndex);
    }

    public void setPaletteEntry(int index, int r, int g, int b) {
        ensureInitialized();
        driver.setPaletteEntry(index, r, g, b);
    }

    public void present() {
        ensureInitialized();

        int[] rgb = driver.toRgbBuffer();
        int width = driver.getWidth();
        int height = driver.getHeight();

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int color = rgb[y * width + x];
                int r = (color >> 16) & 0xFF;
                int g = (color >> 8) & 0xFF;
                int b = color & 0xFF;

                int baseX = x * scale;
                int baseY = y * scale;

                for (int sy = 0; sy < scale; sy++) {
                    for (int sx = 0; sx < scale; sx++) {
                        screen.putPixel(baseX + sx, baseY + sy, r, g, b);
                    }
                }
            }
        }
    }

    public boolean hasKeyPress() {
        ensureInitialized();
        return screen.hasKeyPress();
    }

    public Integer pollKeyCode() {
        ensureInitialized();
        return screen.pollKeyCode();
    }

    public void shutdown() {
        closeOutput();
        this.driver = null;
        this.mode = null;
    }

    private VideoDriver createDriver(VideoMode mode) {
        if (mode.adapter() == GpuAdapter.CGA) {
            return new CgaDriver(mode);
        }
        if (mode.adapter() == GpuAdapter.VGA) {
            return new VgaDriver(mode);
        }
        throw new IllegalStateException("Unsupported adapter: " + mode.adapter());
    }

    private void ensureInitialized() {
        if (driver == null || screen == null || mode == null) {
            throw new IllegalStateException("GPU not initialized. Call init(mode) first.");
        }
    }

    private void closeOutput() {
        if (screen != null) {
            screen.close();
            screen = null;
        }
    }
}
