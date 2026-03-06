package com.github._0x4248.nova.Machine.gpu;

public final class VideoMode {

    private final String name;
    private final GpuAdapter adapter;
    private final GpuOutputMode outputMode;
    private final int width;
    private final int height;
    private final int scale;
    private final int colors;
    private final boolean paletteChangeSupported;

    private VideoMode(
            String name,
            GpuAdapter adapter,
            GpuOutputMode outputMode,
            int width,
            int height,
            int scale,
            int colors,
            boolean paletteChangeSupported
    ) {
        if (name == null || name.isBlank()) {
            throw new IllegalArgumentException("name cannot be blank");
        }
        if (adapter == null) {
            throw new IllegalArgumentException("adapter cannot be null");
        }
        if (outputMode == null) {
            throw new IllegalArgumentException("outputMode cannot be null");
        }
        if (width <= 0 || height <= 0) {
            throw new IllegalArgumentException("width and height must be positive");
        }
        if (scale <= 0) {
            throw new IllegalArgumentException("scale must be positive");
        }
        if (colors <= 0 || colors > 256) {
            throw new IllegalArgumentException("colors must be between 1 and 256");
        }

        this.name = name;
        this.adapter = adapter;
        this.outputMode = outputMode;
        this.width = width;
        this.height = height;
        this.scale = scale;
        this.colors = colors;
        this.paletteChangeSupported = paletteChangeSupported;
    }

    public static VideoMode of(
            String name,
            GpuAdapter adapter,
            GpuOutputMode outputMode,
            int width,
            int height,
            int scale,
            int colors,
            boolean paletteChangeSupported
    ) {
        return new VideoMode(name, adapter, outputMode, width, height, scale, colors, paletteChangeSupported);
    }

    public String name() {
        return name;
    }

    public GpuAdapter adapter() {
        return adapter;
    }

    public GpuOutputMode outputMode() {
        return outputMode;
    }

    public int width() {
        return width;
    }

    public int height() {
        return height;
    }

    public int scale() {
        return scale;
    }

    public int colors() {
        return colors;
    }

    public boolean supportsPaletteChanges() {
        return paletteChangeSupported;
    }

    public boolean supportsPixels() {
        return outputMode == GpuOutputMode.VIDEO;
    }

    @Override
    public String toString() {
        return adapter + " " + outputMode + " " + width + "x" + height + " (" + colors + " colors)";
    }
}