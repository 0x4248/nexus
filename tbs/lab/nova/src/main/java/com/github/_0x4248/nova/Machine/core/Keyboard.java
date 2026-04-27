package com.github._0x4248.nova.Machine.core;

public class Keyboard implements Hardware {

    private final Video video;

    public Keyboard(Video video) {
        this.video = video;
    }

    @Override
    public String id() {
        return "keyboard";
    }

    public boolean hasKeyPress() {
        return video.gpu.hasKeyPress();
    }

    public Integer pollKeyCode() {
        return video.gpu.pollKeyCode();
    }
}
