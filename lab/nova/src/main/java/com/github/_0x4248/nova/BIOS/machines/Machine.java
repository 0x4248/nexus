package com.github._0x4248.nova.BIOS.machines;

public class Machine {

    public final String id;
    public final String biosLabel;
    public final int screenScale;
    public final boolean supportsSound;
    public final boolean supportsKeyboard;

    public Machine(String id, String biosLabel, int screenScale, boolean supportsSound, boolean supportsKeyboard) {
        this.id = id;
        this.biosLabel = biosLabel;
        this.screenScale = screenScale;
        this.supportsSound = supportsSound;
        this.supportsKeyboard = supportsKeyboard;
    }
}
