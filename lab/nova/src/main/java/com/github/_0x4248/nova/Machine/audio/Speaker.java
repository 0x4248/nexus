package com.github._0x4248.nova.Machine.audio;

import com.github._0x4248.nova.Core.Sound;
import com.github._0x4248.nova.Machine.core.Hardware;

public class Speaker implements Hardware {

    private final Sound sound;

    public Speaker() {
        this.sound = new Sound();
    }

    @Override
    public String id() {
        return "speaker";
    }

    public void beep(int lengthMs, int pitchHz) {
        sound.biosBeep(lengthMs, pitchHz);
    }
}