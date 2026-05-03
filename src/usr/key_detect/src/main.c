/**
 * Key detect (main.c)
 * A simple program to detect keypresses.
 * Github: https://www.github.com/0x4248/key_detect
 * Licence: GNU General Public License v3.0
 * By: 0x4248
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    bool binary_mode = false;
    bool hex_mode = false;

    /* Argument parsing */
    if (argc > 1) {
        if (argv[1][0] == '-' && argv[1][1] == 'b') {
            binary_mode = true;
        } else if (argv[1][0] == '-' && argv[1][1] == 'h') {
            printf("Usage: key_detect [-b]\n");
            printf("  -b\tBinary mode. Prints the binary representation of the "
                   "keypress.\n");
            printf("  -x\tHexadecimal mode. Prints the hexadecimal "
                   "representation of the keypress.\n");
            printf("  -h\tPrints this help message.\n");
            return EXIT_SUCCESS;
        } else if (argv[1][0] == '-' && argv[1][1] == '-' &&
                       argv[1][2] == 'h' && argv[1][3] == 'e' &&
                       argv[1][4] == 'x' ||
                   argv[1][0] == '-' && argv[1][1] == 'x') {
            hex_mode = true;
        }
    }

    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    printf("Listening for keypresses.\nPress Ctrl+C to stop listening and "
           "exit.\n");

    while (true) {
        /* Main loop */
        ch = getchar();
        if (binary_mode) {
            for (int i = 7; i >= 0; i--) {
                printf("%d", (ch >> i) & 1);
            }
            printf("\n");
        } else if (hex_mode) {
            printf("Code: 0x%x\tCharacter: %c\n", ch, ch);
            fflush(stdout);
        } else {
            printf("Code: %d\tCharacter: %c\n", ch, ch);
            fflush(stdout);
        }
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return EXIT_SUCCESS;
}