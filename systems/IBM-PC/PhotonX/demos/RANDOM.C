#include "VGA.H"
#include <time.h>
#include <stdlib.h>

int rand_range(int min, int max)
{
    return (rand() % (max - min + 1)) + min;
}

int main()
{
    int i = 0;
    int x;
    int y;
    int lastx = 160;
    int lasty = 100;
    srand((unsigned int)time(NULL));
    vga_set_mode(VGA_MODE_320x200x256);
    vga_gfx_setclscolor(1);
    vga_gfx_cls();

    while (i++ < 100)
    {
        x = rand_range(0, 20);
        y = rand_range(0, 20);
        vga_gfx_line((uint16_t)lastx, (uint16_t)lasty, (uint16_t)x, (uint16_t)y, (uint8_t)(i % 256));
        lastx = x;
        lasty = y;
        vga_beep(rand_range(100, 500), 40);
        delay(40);
        if (kbhit())
            break;
    }

    return 0;
}