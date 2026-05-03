# VGA Subsystem

PhotonX Graphics SDK — VGA driver interface for 8086/8088 systems.

## Overview

The VGA subsystem exposes two distinct APIs depending on the active video mode:
a text-mode API for 80x25 character output, and a graphics-mode API for direct
pixel rendering in mode 13h (320x200, 256 colors).

<!-- #ifndef VGA_H
#define VGA_H

#include <dos.h>

#define VGA_SEG 0xA000
#define VGA_MODE_TEXT_80x25 0x03
#define VGA_MODE_320x200x256 0x13

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;

extern unsigned char far *vga;

/* Core mode + utility */
void vga_set_mode(uint8_t mode);

/* Text mode API (whitepaper-aligned) */
void vga_text_setcolor(uint8_t fg, uint8_t bg);
void vga_text_gotoxy(uint8_t x, uint8_t y);
void vga_text_putch(char c);
void vga_text_puts(const char *str);
void vga_text_cls(void);

/* Graphics mode API (whitepaper-aligned) */
void vga_set_palette(uint8_t index, uint8_t r, uint8_t g, uint8_t b);
void vga_gfx_setclscolor(uint8_t color);
void vga_gfx_cls(void);
void vga_gfx_putpixel(uint16_t x, uint16_t y, uint8_t color);
void vga_gfx_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t color);
void vga_gfx_rect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t color);
void vga_gfx_circle(uint16_t x, uint16_t y, uint16_t radius, uint8_t color);
void vga_gfx_putch(char c, int x, int y, uint8_t color);
void vga_gfx_puts(const char *str, int x, int y, uint8_t color);

/* Compatibility names for existing code */
void set_mode(unsigned char mode);
void set_palette(unsigned char index, unsigned char r, unsigned char g, unsigned char b);
void clear_screen(unsigned char color);
void put_pixel(int x, int y, unsigned char color);

#endif -->

## Setting mode outputs

To switch between text and graphics modes, use the `vga_set_mode` function with 
the appropriate mode constant:

```c
vga_set_mode(VGA_MODE_TEXT_80x25); // Switch to text mode
vga_set_mode(VGA_MODE_320x200x256); // Switch to graphics mode
```

**WARNING**: It is always important to reset the screen mode back to text mode
before returning the user to the MS-DOS environment, otherwise they will be left
almost like a broken state and could lead to unexpected behavior - It is also
noted that on MS-DOS the `cls` command does not reset the screen mode. For simple
way to reset the screen mode you should call these functions:

```c
vga_set_mode(VGA_MODE_TEXT_80x25); // Reset to text mode
vga_text_setcolor(7, 0); // Set default white on black text color
vga_text_cls(); // Clear the screen
```
## Text Mode

In text mode, you get a character grid of 80 columns and 25 rows and 16 colors.

### Color palette 

The VGA text mode supports a fixed palette of 16 colors, indexed from 0 to 15:

<table>
  <tr><th>Index</th><th>Color name</th><th>Color HTML</th></tr>
  <tr><td>0</td><td>Black</td><td style="background:#000000;color:#ffffff;">#000000</td></tr>
  <tr><td>1</td><td>Blue</td><td style="background:#0000AA;color:#ffffff;">#0000AA</td></tr>
  <tr><td>2</td><td>Green</td><td style="background:#00AA00;color:#ffffff;">#00AA00</td></tr>
  <tr><td>3</td><td>Cyan</td><td style="background:#00AAAA;color:#000000;">#00AAAA</td></tr>
  <tr><td>4</td><td>Red</td><td style="background:#AA0000;color:#ffffff;">#AA0000</td></tr>
  <tr><td>5</td><td>Magenta</td><td style="background:#AA00AA;color:#ffffff;">#AA00AA</td></tr>
  <tr><td>6</td><td>Brown</td><td style="background:#AA5500;color:#ffffff;">#AA5500</td></tr>
  <tr><td>7</td><td>Light Gray</td><td style="background:#AAAAAA;color:#000000;">#AAAAAA</td></tr>
  <tr><td>8</td><td>Dark Gray</td><td style="background:#555555;color:#ffffff;">#555555</td></tr>
  <tr><td>9</td><td>Light Blue</td><td style="background:#5555FF;color:#ffffff;">#5555FF</td></tr>
  <tr><td>10</td><td>Light Green</td><td style="background:#55FF55;color:#000000;">#55FF55</td></tr>
  <tr><td>11</td><td>Light Cyan</td><td style="background:#55FFFF;color:#000000;">#55FFFF</td></tr>
  <tr><td>12</td><td>Light Red</td><td style="background:#FF5555;color:#000000;">#FF5555</td></tr>
  <tr><td>13</td><td>Light Magenta</td><td style="background:#FF55FF;color:#000000;">#FF55FF</td></tr>
  <tr><td>14</td><td>Yellow</td><td style="background:#FFFF55;color:#000000;">#FFFF55</td></tr>
  <tr><td>15</td><td>White</td><td style="background:#FFFFFF;color:#000000;">#FFFFFF</td></tr>
</table>