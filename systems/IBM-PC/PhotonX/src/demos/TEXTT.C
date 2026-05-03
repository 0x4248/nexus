#include "VGA.H"
#include <conio.h>

int main()
{
    unsigned char i;
    unsigned char j;
    const char *color_names[16] = {
        "Black",
        "Blue",
        "Green",
        "Cyan",
        "Red",
        "Magenta",
        "Brown",
        "Light Gray",
        "Dark Gray",
        "Light Blue",
        "Light Green",
        "Light Cyan",
        "Light Red",
        "Light Magenta",
        "Yellow",
        "White"
    };
    const char *logo[11] = {
        "               ",
        "               ",
        "    `7MMF'     ",
        " ,mmmmMMmmmm.  ",
        "6MP   MM   YMb ",
        "8M    MM    M8 ",
        "YMb   MM   dM9 ",
        " `YmmmMMmmmP'  ",
        "    .JMML.     ",
        "               ",
        "               "
    };

    vga_set_mode(VGA_MODE_TEXT_80x25);
    vga_text_setcolor(15, 1);
    vga_text_cls();
    vga_text_gotoxy(0, 0);
    vga_text_puts("PhotonX Demo: Text Colors\r\n");

    vga_text_setcolor(14, 0);
    vga_text_puts("Index shown with matching foreground color:\r\n\r\n");

    vga_text_setcolor(15, 0);
    for (j = 0; j < 11; ++j)
    {
        vga_text_gotoxy(60, 5 + j);
        vga_text_puts(logo[j]);
    }

    vga_text_gotoxy(0, 3);

    for (i = 0; i < 16; ++i)
    {
        if (i == 0)
            vga_text_setcolor(i, 7);
        else
            vga_text_setcolor(i, 0);

        vga_text_puts("Color ");

        if (i < 10)
            vga_text_putch('0' + i);
        else
        {
            vga_text_putch('1');
            vga_text_putch('0' + (i - 10));
        }

        vga_text_puts(": ");
        vga_text_puts(color_names[i]);
        vga_text_puts("\r\n");
    }

    vga_text_setcolor(15, 0);
    vga_text_puts("\r\nPress any key to exit...\r\n");
    getch();

    vga_set_mode(VGA_MODE_TEXT_80x25);
    vga_text_setcolor(15, 0);
    vga_text_cls();

    return 0;
}
