package com.github._0x4248.nova.Machine.gpu.drivers;

import com.github._0x4248.nova.Machine.gpu.GpuOutputMode;

public interface VideoDriver {

    int getWidth();

    int getHeight();

    int getColorCount();

    GpuOutputMode getOutputMode();

    void clear(int colorIndex);

    void drawText(int x, int y, String text, int foregroundColor, int backgroundColor, boolean transparentBackground);

    void setPixel(int x, int y, int colorIndex);

    void setPaletteEntry(int index, int r, int g, int b);

    int[] toRgbBuffer();
}
