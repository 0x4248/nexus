#ifndef VGA_H
#define VGA_H

#include <dos.h>

#define VGA_SEG 0xA000
#define VGA_MODE_TEXT_80x25 0x03
#define VGA_MODE_320x200x256 0x13
#define VGA_MODE_640x480x16 0x12

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;

extern unsigned char far *vga;

void vga_set_mode(uint8_t mode);
void vga_beep(int freq, int ms);
void vga_wait_key(void);

void vga_text_setcolor(uint8_t fg, uint8_t bg);
void vga_text_gotoxy(uint8_t x, uint8_t y);
void vga_text_putch(char c);
void vga_text_puts(const char *str);
void vga_text_cls(void);

void vga_set_palette(uint8_t index, uint8_t r, uint8_t g, uint8_t b);
void vga_gfx_setclscolor(uint8_t color);
void vga_gfx_cls(void);
void vga_gfx_putpixel(uint16_t x, uint16_t y, uint8_t color);
void vga_gfx_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t color);
void vga_gfx_rect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t color);
void vga_gfx_circle(uint16_t x, uint16_t y, uint16_t radius, uint8_t color);
void vga_gfx_putch(char c, int x, int y, uint8_t color);
void vga_gfx_puts(const char *str, int x, int y, uint8_t color);

void set_mode(unsigned char mode);
void beep(int freq, int ms);
void set_palette(unsigned char index, unsigned char r, unsigned char g, unsigned char b);
void clear_screen(unsigned char color);
void put_pixel(int x, int y, unsigned char color);
void wait_key(void);

#endif
