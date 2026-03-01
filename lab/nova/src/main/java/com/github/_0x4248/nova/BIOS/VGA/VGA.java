package com.github._0x4248.nova.BIOS.VGA;

import com.github._0x4248.nova.Core.Gui;

import java.awt.image.BufferedImage;
import java.util.Arrays;

public class VGA {

	public static final int MODE13H_WIDTH = 320;
	public static final int MODE13H_HEIGHT = 200;

	private final int width;
	private final int height;
	private final byte[] framebuffer;
	private final int[] palette;

	public VGA() {
		this(MODE13H_WIDTH, MODE13H_HEIGHT);
	}

	public VGA(int width, int height) {
		if (width <= 0 || height <= 0) {
			throw new IllegalArgumentException("Width and height must be positive");
		}

		this.width = width;
		this.height = height;
		this.framebuffer = new byte[width * height];
		this.palette = new int[256];

		initializeDefaultPalette();
	}

	public int getWidth() {
		return width;
	}

	public int getHeight() {
		return height;
	}

	public void clear(int colorIndex) {
		Arrays.fill(framebuffer, toPaletteIndex(colorIndex));
	}

	public void setPixel(int x, int y, int colorIndex) {
		if (x < 0 || y < 0 || x >= width || y >= height) {
			return;
		}

		framebuffer[y * width + x] = toPaletteIndex(colorIndex);
	}

	public int getPixel(int x, int y) {
		if (x < 0 || y < 0 || x >= width || y >= height) {
			return 0;
		}

		return framebuffer[y * width + x] & 0xFF;
	}

	public void drawChar(int x, int y, char character, int foregroundColor, int backgroundColor, boolean transparentBackground) {
		int glyphIndex = character & 0x7F;
		if (glyphIndex >= VGAFonts.LATIN8x8.length) {
			glyphIndex = '?';
		}

		byte[] glyph = VGAFonts.LATIN8x8[glyphIndex];

		for (int row = 0; row < 8; row++) {
			int rowBits = glyph[row] & 0xFF;

			for (int col = 0; col < 8; col++) {
				boolean on = ((rowBits >> col) & 1) == 1;
				if (on) {
					setPixel(x + col, y + row, foregroundColor);
				} else if (!transparentBackground) {
					setPixel(x + col, y + row, backgroundColor);
				}
			}
		}
	}

	public void drawText(int x, int y, String text, int foregroundColor, int backgroundColor, boolean transparentBackground) {
		int cursorX = x;
		int cursorY = y;

		for (int i = 0; i < text.length(); i++) {
			char character = text.charAt(i);

			if (character == '\n') {
				cursorX = x;
				cursorY += 8;
				continue;
			}

			drawChar(cursorX, cursorY, character, foregroundColor, backgroundColor, transparentBackground);
			cursorX += 8;
		}
	}

	public void setPaletteEntry(int index, int r, int g, int b) {
		if (index < 0 || index > 255) {
			return;
		}

		int red = clamp8(r);
		int green = clamp8(g);
		int blue = clamp8(b);
		palette[index] = (red << 16) | (green << 8) | blue;
	}

	public int getPaletteEntry(int index) {
		if (index < 0 || index > 255) {
			return 0;
		}

		return palette[index];
	}

	public int[] toRgbBuffer() {
		int[] rgb = new int[framebuffer.length];
		for (int i = 0; i < framebuffer.length; i++) {
			rgb[i] = palette[framebuffer[i] & 0xFF];
		}
		return rgb;
	}

	public BufferedImage toBufferedImage() {
		BufferedImage image = new BufferedImage(width, height, BufferedImage.TYPE_INT_RGB);
		image.setRGB(0, 0, width, height, toRgbBuffer(), 0, width);
		return image;
	}

	public void blitToGui(Gui gui, int scale) {
		if (gui == null || scale <= 0) {
			return;
		}
                try {
            Thread.sleep(1);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
		int[] rgb = toRgbBuffer();

		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				int color = rgb[y * width + x];
				int r = (color >> 16) & 0xFF;
				int g = (color >> 8) & 0xFF;
				int b = color & 0xFF;

				int baseX = x * scale;
				int baseY = y * scale;

				for (int sy = 0; sy < scale; sy++) {
					for (int sx = 0; sx < scale; sx++) {
						gui.putPixel(baseX + sx, baseY + sy, r, g, b);
					}
				}
			}
		}
	}

	public void blitToGui(Gui gui) {
		blitToGui(gui, 1);
	}

	private byte toPaletteIndex(int colorIndex) {
		return (byte) (colorIndex & 0xFF);
	}

	private int clamp8(int value) {
		return Math.max(0, Math.min(255, value));
	}

	private void initializeDefaultPalette() {
		int[] ega16 = {
				0x000000, 0x0000AA, 0x00AA00, 0x00AAAA,
				0xAA0000, 0xAA00AA, 0xAA5500, 0xAAAAAA,
				0x555555, 0x5555FF, 0x55FF55, 0x55FFFF,
				0xFF5555, 0xFF55FF, 0xFFFF55, 0xFFFFFF
		};

		System.arraycopy(ega16, 0, palette, 0, ega16.length);

		for (int i = 16; i < 256; i++) {
			int gray = (int) Math.round(((i - 16) / 239.0) * 255.0);
			palette[i] = (gray << 16) | (gray << 8) | gray;
		}
	}
}
