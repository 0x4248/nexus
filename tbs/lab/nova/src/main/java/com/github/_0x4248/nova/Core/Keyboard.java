package com.github._0x4248.nova.Core;

import java.awt.event.KeyEvent;
import java.util.concurrent.ConcurrentLinkedQueue;

public class Keyboard {

    private final ConcurrentLinkedQueue<Integer> keyQueue = new ConcurrentLinkedQueue<>();

    public boolean hasKeyPress() {
        return !keyQueue.isEmpty();
    }

    public Integer pollKeyCode() {
        return keyQueue.poll();
    }

    public void onKeyPressed(KeyEvent event) {
        keyQueue.offer(event.getKeyCode());
    }
}
