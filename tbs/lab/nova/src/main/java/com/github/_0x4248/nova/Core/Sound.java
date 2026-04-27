package com.github._0x4248.nova.Core;

import javax.sound.sampled.AudioFormat;
import javax.sound.sampled.AudioSystem;
import javax.sound.sampled.DataLine;
import javax.sound.sampled.LineUnavailableException;
import javax.sound.sampled.SourceDataLine;
import java.awt.Toolkit;

public class Sound {

    private static final float BEEP_SAMPLE_RATE = 44100.0f;
    private static final int BEEP_VOLUME = 64;

    public void biosBeep(int lengthMs, int pitchHz) {
        if (lengthMs <= 0 || pitchHz <= 0) {
            return;
        }

        Thread beepThread = new Thread(() -> playBeep(lengthMs, pitchHz), "bios-beep");
        beepThread.setDaemon(true);
        beepThread.start();
    }

    private void playBeep(int lengthMs, int pitchHz) {
        AudioFormat[] formats = {
                new AudioFormat(BEEP_SAMPLE_RATE, 8, 1, true, false),
                new AudioFormat(BEEP_SAMPLE_RATE, 16, 1, true, false),
                new AudioFormat(22050.0f, 16, 1, true, false)
        };

        for (AudioFormat format : formats) {
            DataLine.Info lineInfo = new DataLine.Info(SourceDataLine.class, format);
            if (!AudioSystem.isLineSupported(lineInfo)) {
                continue;
            }

            try (SourceDataLine line = (SourceDataLine) AudioSystem.getLine(lineInfo)) {
                line.open(format);
                line.start();

                byte[] buffer = createSquareWaveBuffer(lengthMs, pitchHz, format);
                line.write(buffer, 0, buffer.length);
                line.drain();
                return;
            } catch (LineUnavailableException | IllegalArgumentException ignored) {
            }
        }

        Toolkit.getDefaultToolkit().beep();
    }

    private byte[] createSquareWaveBuffer(int lengthMs, int pitchHz, AudioFormat format) {
        int sampleCount = (int) ((format.getSampleRate() * lengthMs) / 1000);
        int periodSamples = Math.max(1, (int) (format.getSampleRate() / pitchHz));

        if (format.getSampleSizeInBits() == 8) {
            byte[] buffer = new byte[sampleCount];
            for (int i = 0; i < sampleCount; i++) {
                buffer[i] = (byte) ((i % periodSamples) < (periodSamples / 2) ? BEEP_VOLUME : -BEEP_VOLUME);
            }
            return buffer;
        }

        byte[] buffer = new byte[sampleCount * 2];
        short amplitude = 12000;
        for (int i = 0; i < sampleCount; i++) {
            short value = (short) (((i % periodSamples) < (periodSamples / 2)) ? amplitude : -amplitude);
            int offset = i * 2;
            buffer[offset] = (byte) (value & 0xFF);
            buffer[offset + 1] = (byte) ((value >> 8) & 0xFF);
        }
        return buffer;
    }
}
