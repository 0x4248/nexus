package com.github._0x4248.nova.Machine.core;

import com.github._0x4248.nova.Machine.audio.Speaker;
import com.github._0x4248.nova.Machine.floppy.Floppy;

import java.util.LinkedHashMap;
import java.util.Map;

public class Machine {

    private final Map<String, Hardware> hardware;

    public Machine() {
        this.hardware = new LinkedHashMap<>();
        Video video = new Video();
        attach(video);
        attach(new Floppy());
        attach(new Speaker());
        attach(new Keyboard(video));
    }

    public static Machine basic() {
        return new Machine();
    }

    public static Machine empty() {
        return new Machine(false);
    }

    private Machine(boolean withDefaults) {
        this.hardware = new LinkedHashMap<>();
        if (withDefaults) {
            Video video = new Video();
            attach(video);
            attach(new Floppy());
            attach(new Speaker());
            attach(new Keyboard(video));
        }
    }

    public Machine attach(Hardware device) {
        if (device == null) {
            throw new IllegalArgumentException("device cannot be null");
        }

        Hardware previous = hardware.put(device.id(), device);
        if (previous != null) {
            previous.onDetach(this);
        }
        device.onAttach(this);
        System.out.println("Attached hardware: " + device.id());
        return this;
    }

    public <T extends Hardware> T get(String id, Class<T> type) {
        Hardware device = hardware.get(id);
        if (device == null) {
            return null;
        }
        if (!type.isInstance(device)) {
            throw new IllegalStateException("Hardware '" + id + "' is not a " + type.getSimpleName());
        }
        return type.cast(device);
    }

    public boolean has(String id) {
        return hardware.containsKey(id);
    }

    public Machine detach(String id) {
        Hardware removed = hardware.remove(id);
        if (removed != null) {
            removed.onDetach(this);
        }
        return this;
    }

    private <T extends Hardware> T require(String id, Class<T> type) {
        T device = get(id, type);
        if (device == null) {
            throw new IllegalStateException("Missing required hardware: " + id);
        }
        return device;
    }

    public Video video() {
        return require("video", Video.class);
    }

    public Floppy floppy() {
        return require("floppy", Floppy.class);
    }

    public Speaker speaker() {
        return require("speaker", Speaker.class);
    }

    public Keyboard keyboard() {
        return require("keyboard", Keyboard.class);
    }
}