#include "VGA.H"

int main()
{
    vga_set_mode(VGA_MODE_TEXT_80x25);
    vga_text_setcolor(15, 1);
    vga_text_cls();
    vga_text_gotoxy(0, 0);
    vga_text_puts("PhotonX Demo: Hello World\r\n");
    vga_text_setcolor(14, 0);
    vga_text_puts("Press any key to continue...\r\n");
    vga_wait_key();

    vga_set_mode(VGA_MODE_320x200x256);
    vga_gfx_setclscolor(1);
    vga_gfx_cls();
    vga_gfx_puts("HELLO WORLD", 108, 96, 15);
    vga_gfx_puts("PRESS KEY", 116, 110, 14);
    vga_wait_key();

    vga_set_mode(VGA_MODE_TEXT_80x25);
    vga_text_setcolor(15, 0);
    vga_text_cls();

    return 0;
}
