#include "VGA.H"

int main()
{
    int x;
    int y;

    vga_set_mode(VGA_MODE_320x200x256);

    for (x = 0; x < 64; x++)
        vga_set_palette((uint8_t)x, (uint8_t)x, 0, 0);

    vga_gfx_setclscolor(63);
    vga_gfx_cls();

    for (x = 0; x < 320; x++)
        for (y = 0; y < 200; y++)
            vga_gfx_putpixel((uint16_t)x, (uint16_t)y, 63);

    vga_wait_key();

    vga_set_mode(VGA_MODE_TEXT_80x25);

    return 0;
}
