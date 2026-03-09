#include <dos.h>
#include <conio.h>
#include "VGA.H"

unsigned char far *vga = (unsigned char far *)MK_FP(VGA_SEG, 0);
static uint8_t g_cls_color = 0;
extern char font8x8_basic[128][8];

static int in_gfx_bounds(int x, int y)
{
    if (x < 0 || x >= 320)
        return 0;
    if (y < 0 || y >= 200)
        return 0;
    return 1;
}

static void swap_u16(uint16_t *a, uint16_t *b)
{
    uint16_t temp = *a;
    *a = *b;
    *b = temp;
}

void vga_set_mode(uint8_t mode)
{
    union REGS regs;
    regs.h.ah = 0x00;
    regs.h.al = mode;
    int86(0x10, &regs, &regs);
}


void vga_text_setcolor(uint8_t fg, uint8_t bg)
{
    textcolor(fg & 0x0F);
    textbackground(bg & 0x0F);
}

void vga_text_gotoxy(uint8_t x, uint8_t y)
{
    gotoxy((int)x + 1, (int)y + 1);
}

void vga_text_putch(char c)
{
    putch(c);
}

void vga_text_puts(const char *str)
{
    cputs(str);
}

void vga_text_cls(void)
{
    clrscr();
}

void vga_set_palette(uint8_t index, uint8_t r, uint8_t g, uint8_t b)
{
    outp(0x3C8, index);
    outp(0x3C9, r);
    outp(0x3C9, g);
    outp(0x3C9, b);
}

void vga_gfx_setclscolor(uint8_t color)
{
    g_cls_color = color;
}

void vga_gfx_cls(void)
{
    int i;
    for (i = 0; i < 320 * 200; i++)
        vga[i] = g_cls_color;
}

void vga_gfx_putpixel(uint16_t x, uint16_t y, uint8_t color)
{
    if (!in_gfx_bounds((int)x, (int)y))
        return;

    vga[(unsigned int)y * 320u + (unsigned int)x] = color;
}

void vga_gfx_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t color)
{
    int dx, dy, sx, sy, err, e2;
    int cx = (int)x1;
    int cy = (int)y1;
    int tx = (int)x2;
    int ty = (int)y2;

    dx = (tx > cx) ? (tx - cx) : (cx - tx);
    dy = (ty > cy) ? (ty - cy) : (cy - ty);
    sx = (cx < tx) ? 1 : -1;
    sy = (cy < ty) ? 1 : -1;
    err = dx - dy;

    for (;;)
    {
        if (in_gfx_bounds(cx, cy))
            vga[cy * 320 + cx] = color;

        if (cx == tx && cy == ty)
            break;

        e2 = err << 1;
        if (e2 > -dy)
        {
            err -= dy;
            cx += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            cy += sy;
        }
    }
}

void vga_gfx_rect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t color)
{
    if (x2 < x1)
        swap_u16(&x1, &x2);
    if (y2 < y1)
        swap_u16(&y1, &y2);

    vga_gfx_line(x1, y1, x2, y1, color);
    vga_gfx_line(x2, y1, x2, y2, color);
    vga_gfx_line(x2, y2, x1, y2, color);
    vga_gfx_line(x1, y2, x1, y1, color);
}

void vga_gfx_circle(uint16_t x, uint16_t y, uint16_t radius, uint8_t color)
{
    int cx = (int)x;
    int cy = (int)y;
    int r = (int)radius;
    int px = r;
    int py = 0;
    int err = 0;

    while (px >= py)
    {
        vga_gfx_putpixel((uint16_t)(cx + px), (uint16_t)(cy + py), color);
        vga_gfx_putpixel((uint16_t)(cx + py), (uint16_t)(cy + px), color);
        vga_gfx_putpixel((uint16_t)(cx - py), (uint16_t)(cy + px), color);
        vga_gfx_putpixel((uint16_t)(cx - px), (uint16_t)(cy + py), color);
        vga_gfx_putpixel((uint16_t)(cx - px), (uint16_t)(cy - py), color);
        vga_gfx_putpixel((uint16_t)(cx - py), (uint16_t)(cy - px), color);
        vga_gfx_putpixel((uint16_t)(cx + py), (uint16_t)(cy - px), color);
        vga_gfx_putpixel((uint16_t)(cx + px), (uint16_t)(cy - py), color);

        if (err <= 0)
        {
            py++;
            err += (py << 1) + 1;
        }
        if (err > 0)
        {
            px--;
            err -= (px << 1) + 1;
        }
    }
}

void vga_gfx_putch(char c, int x, int y, uint8_t color)
{
    int row;
    int col;
    uint8_t bits;
    uint8_t glyph = (uint8_t)c;

    if (glyph >= 128)
        glyph = (uint8_t)'?';

    for (row = 0; row < 8; row++)
    {
        bits = (uint8_t)font8x8_basic[glyph][row];
        for (col = 0; col < 8; col++)
        {
            if (bits & (1u << col))
                vga_gfx_putpixel((uint16_t)(x + col), (uint16_t)(y + row), color);
        }
    }
}

void vga_gfx_puts(const char *str, int x, int y, uint8_t color)
{
    int cx = x;

    while (*str)
    {
        if (*str == '\n')
        {
            cx = x;
            y += 8;
        }
        else
        {
            vga_gfx_putch(*str, cx, y, color);
            cx += 8;
        }
        str++;
    }
}

void set_mode(unsigned char mode)
{
    vga_set_mode((uint8_t)mode);
}

void set_palette(unsigned char index, unsigned char r, unsigned char g, unsigned char b)
{
    vga_set_palette((uint8_t)index, (uint8_t)r, (uint8_t)g, (uint8_t)b);
}

void clear_screen(unsigned char color)
{
    vga_gfx_setclscolor((uint8_t)color);
    vga_gfx_cls();
}

void put_pixel(int x, int y, unsigned char color)
{
    if (x < 0 || y < 0)
        return;

    vga_gfx_putpixel((uint16_t)x, (uint16_t)y, (uint8_t)color);
}

void vga_reset(void)
{
    vga_set_mode(VGA_MODE_TEXT_80x25);
    clrscr();
    textcolor(15);
    textbackground(0);
}