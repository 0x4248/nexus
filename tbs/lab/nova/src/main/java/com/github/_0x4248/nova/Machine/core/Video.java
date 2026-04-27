package com.github._0x4248.nova.Machine.core;

import com.github._0x4248.nova.Machine.gpu.GPU;
import com.github._0x4248.nova.Machine.gpu.GpuAdapter;
import com.github._0x4248.nova.Machine.gpu.GpuOutputMode;
import com.github._0x4248.nova.Machine.gpu.VideoMode;

public class Video implements Hardware {

    public final GPU gpu;
    public final Modes modes;

    public static final VideoMode ModesCGA = Modes.CGA_GRAPHICS_320x200;
    public static final VideoMode ModesVGA = Modes.VGA_GRAPHICS_640x480_16;

    public Video() {
        this.modes = new Modes();
        this.gpu = new GPU().init(ModesVGA);
    }

    @Override
    public String id() {
        return "video";
    }

    public static class Modes {
        public static final VideoMode CGA_TEXT_40x25 = VideoMode.of(
                "CGA_TEXT_40x25", GpuAdapter.CGA, GpuOutputMode.TEXT, 320, 200, 3, 16, false
        );

        public static final VideoMode CGA_GRAPHICS_320x200 = VideoMode.of(
                "CGA_GRAPHICS_320x200", GpuAdapter.CGA, GpuOutputMode.VIDEO, 320, 200, 3, 4, false
        );

        public static final VideoMode VGA_TEXT_80x25 = VideoMode.of(
                "VGA_TEXT_80x25", GpuAdapter.VGA, GpuOutputMode.TEXT, 640, 400, 2, 16, false
        );

        public static final VideoMode VGA_GRAPHICS_640x480_16 = VideoMode.of(
                "VGA_GRAPHICS_640x480_16", GpuAdapter.VGA, GpuOutputMode.VIDEO, 640, 480, 2, 16, false
        );

        public static final VideoMode VGA_GRAPHICS_320x200_256 = VideoMode.of(
                "VGA_GRAPHICS_320x200_256", GpuAdapter.VGA, GpuOutputMode.VIDEO, 320, 200, 4, 256, true
        );

        public final VideoMode CGA_TEXT = CGA_TEXT_40x25;
        public final VideoMode CGA = CGA_GRAPHICS_320x200;

        public final VideoMode VGA_TEXT = VGA_TEXT_80x25;
        public final VideoMode VGA = VGA_GRAPHICS_640x480_16;
        public final VideoMode VGA_256 = VGA_GRAPHICS_320x200_256;
    }
}
