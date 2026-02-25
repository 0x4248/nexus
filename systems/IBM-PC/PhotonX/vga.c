#include <dos.h>
#include <conio.h>
#include <stdio.h>
#define VGA_SEG 0xA000

unsigned char far *vga = (unsigned char far *)MK_FP(VGA_SEG, 0);

/* Prototypes */
void set_mode(unsigned char mode);
void set_palette(unsigned char index, unsigned char r, unsigned char g, unsigned char b);
void clear_screen(unsigned char color);
void put_pixel(int x, int y, unsigned char color);
void wait_key(void);

void red_lines(void);
void green_lines(void);
void blue_lines(void);
void grid_with_corners(void);
void full_color_test(void);

/* BIOS video mode */
void set_mode(unsigned char mode)
{
    union REGS regs;
    regs.h.ah = 0x00;
    regs.h.al = mode;
    int86(0x10, &regs, &regs);
}

void beep(int freq, int ms)
{
	sound(freq);
	delay(ms);
	nosound();
}

/* VGA DAC */
void set_palette(unsigned char index, unsigned char r, unsigned char g, unsigned char b)
{
    outp(0x3C8, index);
    outp(0x3C9, r);
    outp(0x3C9, g);
    outp(0x3C9, b);
}

void clear_screen(unsigned char color)
{
    int i;
    for (i = 0; i < 320 * 200; i++)
        vga[i] = color;
}

void put_pixel(int x, int y, unsigned char color)
{
    vga[y * 320 + x] = color;
}

void wait_key(void)
{
    getch();
    clear_screen(0);
}

void red_lines(void)
{
    int i, x;

    for (i = 0; i < 64; i++)
        set_palette(i, i, 0, 0);

    for (x = 0; x < 320; x++)
        for (i = 0; i < 200; i++)
            put_pixel(x, i, x % 64);

    wait_key();
}

void green_lines(void)
{
    int i, x;

    for (i = 0; i < 64; i++)
        set_palette(i, 0, i, 0);

    for (x = 0; x < 320; x++)
        for (i = 0; i < 200; i++)
            put_pixel(x, i, x % 64);

    wait_key();
}

void blue_lines(void)
{
    int i, x;

    for (i = 0; i < 64; i++)
        set_palette(i, 0, 0, i);

    for (x = 0; x < 320; x++)
        for (i = 0; i < 200; i++)
            put_pixel(x, i, x % 64);

    wait_key();
}

void grid_with_corners(void)
{
    int x, y;

    for (x = 0; x < 320; x++)
	for (y = 0; y < 200; y++)
	    if (x % 10 == 0 || y % 10 == 0)
                put_pixel(x, y, 63); /* White */
            else
                put_pixel(x, y, 10); /* Light blue */
    wait_key();
}

void full_color_test(void)
{
    int x, y, i;
    for (x = 0; x < 320; x++)
	for (y = 0; y < 200; y++)
		put_pixel(x,y, 0);

    /* Build full rainbow palette */
    for (i = 0; i < 64; i++)
        set_palette(i, i, 0, 0);

    for (i = 0; i < 64; i++)
        set_palette(i + 64, 63, i, 0);

    for (i = 0; i < 64; i++)
        set_palette(i + 128, 63 - i, 63, i);

    for (i = 0; i < 64; i++)
        set_palette(i + 192, 0, 63 - i, 63);

    for (x = 0; x < 320; x++)
        for (y = 0; y < 200; y++)
            put_pixel(x, y, x % 256);

    wait_key();
}



/* ================= MAIN ================= */

int main()
{
	beep(700,100);
    cprintf("\r\n");
    cprintf("    `7MMF'     \r\n");
	beep(750,100);
	cprintf(" ,mmmmMMmmmm.  \r\n");
	beep(800,100);
	cprintf("6MP   MM   YMb \r\n");
	beep(850,100);
	cprintf("8M    MM    M8 \r\n");
	beep(900,100);
	cprintf("YMb   MM   dM9 \r\n");
	beep(950,100);
	cprintf(" `YmmmMMmmmP'  \r\n");
	beep(1000,100);
	cprintf("    .JMML.     \r\n");
	cprintf("=====================================\r\n");
    cprintf(" PhotonX VGA Test\r\n");
    cprintf(" Copyright (C) 2026 4248 Systems\r\n");
    cprintf("=====================================\r\n");
	beep(1500,200);
	cprintf(" Press N to cancel, any other key to start...\r\n");
    if (getch() == 'N' || getch() == 'n')
    {
		beep(1000,200);
		beep(800,200);
		beep(600,200);
		beep(400,200);
		beep(200,200);
		cprintf(" Test cancelled.\r\n");
        return 0;
    }

    set_mode(0x13);
	beep(500,400);
    red_lines();
	beep(600,400);
    green_lines();
	beep(700,400);
    blue_lines();
	beep(800,400);
    grid_with_corners();
	beep(900,400);
    full_color_test();
	beep(1000,1000);
	delay(200);
	beep(700,200);
	beep(800,200);
	beep(900,200);
	beep(1000,200);
	beep(800,100);
	beep(700,100);
	beep(600,100);
	beep(500,100);
	beep(400,100);
	beep(300,100);
	beep(200,100);
	beep(100,100);
	set_mode(0x03);

    return 0;
}