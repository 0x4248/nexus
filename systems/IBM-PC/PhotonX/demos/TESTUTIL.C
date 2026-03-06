#include <conio.h>
#include <dos.h>
#include "VGA.H"

static void red_lines_test(void)
{
	int x;
	int y;
	int i;

	vga_gfx_setclscolor(0);
	vga_gfx_cls();

	for (i = 0; i < 64; i++)
		vga_set_palette((uint8_t)i, (uint8_t)i, 0, 0);

	for (x = 0; x < 320; x++)
		for (y = 0; y < 200; y++)
			vga_gfx_putpixel((uint16_t)x, (uint16_t)y, (uint8_t)(x % 64));

	vga_gfx_puts("RED TEST", 8, 8, 63);
	vga_wait_key();
}

static void green_lines_test(void)
{
	int x;
	int y;
	int i;

	vga_gfx_setclscolor(0);
	vga_gfx_cls();

	for (i = 0; i < 64; i++)
		vga_set_palette((uint8_t)i, 0, (uint8_t)i, 0);

	for (x = 0; x < 320; x++)
		for (y = 0; y < 200; y++)
			vga_gfx_putpixel((uint16_t)x, (uint16_t)y, (uint8_t)(x % 64));

	vga_gfx_puts("GREEN TEST", 8, 8, 63);
	vga_wait_key();
}

static void blue_lines_test(void)
{
	int x;
	int y;
	int i;

	vga_gfx_setclscolor(0);
	vga_gfx_cls();

	for (i = 0; i < 64; i++)
		vga_set_palette((uint8_t)i, 0, 0, (uint8_t)i);

	for (x = 0; x < 320; x++)
		for (y = 0; y < 200; y++)
			vga_gfx_putpixel((uint16_t)x, (uint16_t)y, (uint8_t)(x % 64));

	vga_gfx_puts("BLUE TEST", 8, 8, 63);
	vga_wait_key();
}

static void grid_test(void)
{
	int x;
	int y;

	vga_gfx_setclscolor(0);
	vga_gfx_cls();

	for (x = 0; x < 320; x++)
		for (y = 0; y < 200; y++)
			if (x % 10 == 0 || y % 10 == 0)
				vga_gfx_putpixel((uint16_t)x, (uint16_t)y, 63);
			else
				vga_gfx_putpixel((uint16_t)x, (uint16_t)y, 10);

	vga_gfx_puts("GRID TEST", 8, 8, 63);
	vga_wait_key();
}

static void full_color_test(void)
{
	int x;
	int y;
	int i;

	vga_gfx_setclscolor(0);
	vga_gfx_cls();

	for (i = 0; i < 64; i++)
		vga_set_palette((uint8_t)i, (uint8_t)i, 0, 0);

	for (i = 0; i < 64; i++)
		vga_set_palette((uint8_t)(i + 64), 63, (uint8_t)i, 0);

	for (i = 0; i < 64; i++)
		vga_set_palette((uint8_t)(i + 128), (uint8_t)(63 - i), 63, (uint8_t)i);

	for (i = 0; i < 64; i++)
		vga_set_palette((uint8_t)(i + 192), 0, (uint8_t)(63 - i), 63);

	for (x = 0; x < 320; x++)
		for (y = 0; y < 200; y++)
			vga_gfx_putpixel((uint16_t)x, (uint16_t)y, (uint8_t)(x % 256));

	vga_gfx_puts("FULL COLOR TEST", 8, 8, 15);
	vga_wait_key();
}

static void math_spiral_test(void)
{
	int x = 160;
	int y = 100;
	int nx = x;
	int ny = y;
	int len = 4;
	int dir = 0;
	uint8_t color = 1;

	vga_gfx_setclscolor(0);
	vga_gfx_cls();

	while (len < 220)
	{
		switch (dir)
		{
			case 0: nx = x + len; ny = y; break;
			case 1: nx = x; ny = y - len; break;
			case 2: nx = x - len; ny = y; break;
			default: nx = x; ny = y + len; break;
		}

		vga_gfx_line((uint16_t)x, (uint16_t)y, (uint16_t)nx, (uint16_t)ny, color);

		x = nx;
		y = ny;
		dir = (dir + 1) & 3;
		color = (uint8_t)(color + 2);
		if (dir == 0 || dir == 2)
			len += 6;
	}

	vga_gfx_puts("MATH SPIRAL", 8, 8, 63);
	vga_gfx_puts("PRESS KEY", 8, 18, 63);
	vga_wait_key();
}

int main()
{
	char start_key;

	vga_set_mode(VGA_MODE_TEXT_80x25);
	vga_text_setcolor(15, 1);
	vga_text_cls();
	vga_text_gotoxy(0, 0);
	vga_text_puts("PhotonX VGA Library Demo\r\n");
	vga_text_setcolor(14, 1);
	vga_text_puts("Whitepaper API: text + graphics\r\n");
	vga_text_setcolor(15, 0);
	vga_text_puts("Press N to cancel, any other key to start...\r\n");

	vga_beep(1200, 80);
	vga_beep(1400, 80);
	start_key = getch();
	if (start_key == 'N' || start_key == 'n')
	{
		vga_text_puts("\r\nCancelled.\r\n");
		vga_beep(300, 160);
		return 0;
	}

	vga_set_mode(VGA_MODE_320x200x256);
	vga_gfx_setclscolor(0);
	vga_gfx_cls();

	vga_gfx_rect(10, 10, 309, 189, 12);
	vga_gfx_line(10, 10, 309, 189, 10);
	vga_gfx_line(309, 10, 10, 189, 10);
	vga_gfx_circle(160, 100, 50, 14);
	vga_gfx_puts("PhotonX VGA", 108, 96, 15);
	vga_gfx_puts("Press any key", 104, 112, 11);

	vga_wait_key();
	red_lines_test();
	green_lines_test();
	blue_lines_test();
	grid_test();
	full_color_test();
	math_spiral_test();

	vga_set_mode(VGA_MODE_TEXT_80x25);
	vga_text_setcolor(10, 0);
	vga_text_cls();
	vga_text_puts("Demo complete.\r\n");
	vga_beep(1000, 120);
	vga_beep(1400, 120);

	return 0;
}
