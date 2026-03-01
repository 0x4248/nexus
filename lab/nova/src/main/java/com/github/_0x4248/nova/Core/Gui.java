package com.github._0x4248.nova.Core;

import javax.swing.*;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.awt.event.*;

public class Gui extends JPanel implements KeyListener {

    private final BufferedImage framebuffer;
    private final int framebufferWidth;
    private final int framebufferHeight;
    private final Keyboard keyboard;
    private final Sound sound;

    public Gui(int width, int height) {
        framebufferWidth = width;
        framebufferHeight = height;
        framebuffer = new BufferedImage(width, height, BufferedImage.TYPE_INT_RGB);
        keyboard = new Keyboard();
        sound = new Sound();

        JFrame frame = new JFrame("NovaEngine");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setSize(width, height);
        frame.add(this);
        setBackground(Color.BLACK);
        frame.setVisible(true);
        frame.addKeyListener(this);
    }

    // Draw pixel
    public void putPixel(int x, int y, int r, int g, int b) {
        int color = (r << 16) | (g << 8) | b;
        framebuffer.setRGB(x, y, color);
        repaint();
    }

    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);

        int panelWidth = getWidth();
        int panelHeight = getHeight();

        double scale = Math.min(
                (double) panelWidth / framebufferWidth,
                (double) panelHeight / framebufferHeight
        );

        int drawWidth = (int) Math.round(framebufferWidth * scale);
        int drawHeight = (int) Math.round(framebufferHeight * scale);
        int x = (panelWidth - drawWidth) / 2;
        int y = (panelHeight - drawHeight) / 2;

        g.drawImage(framebuffer, x, y, drawWidth, drawHeight, null);
    }

    public boolean hasKeyPress() {
        return keyboard.hasKeyPress();
    }

    public Integer pollKeyCode() {
        return keyboard.pollKeyCode();
    }

    public void biosBeep(int lengthMs, int pitchHz) {
        sound.biosBeep(lengthMs, pitchHz);
    }

    @Override
    public void keyTyped(KeyEvent e) {
    }

    @Override
    public void keyPressed(KeyEvent e) {
        keyboard.onKeyPressed(e);
    }

    @Override
    public void keyReleased(KeyEvent e) {
    }

}