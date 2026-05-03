/* Takes in a .st file and reads it to the user like its a storybook */

/*Example file:
@bg <VGA_COLOR>
@fg <VGA_COLOR>
@cls
The quick brown fox jumps over the lazy dog.
@pause
@cls
There was always a lazy dog.
@pause
@cls
The end.
*/

#include <dos.h>
#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/*Borland C style code for MSDOS 5*/

void beep(int freq, int ms)
{
    sound(freq);
    delay(ms);
    nosound();
}


int main(int argc, char *argv[])
{
    FILE *file;
    char line[256];
    int color;
    int waiting;
    int i;
    int beeponlineonly=0;
    char *p;

    if (argc < 2)
    {
        cprintf("Usage: STORY <filename.st>\r\n");
        return 1;
    }

    beep(500, 100);
    beep(700, 100);
    beep(900, 100);
    beep(1100, 100);
    cprintf("Loading story from %s...\r\n", argv[1]);

    file = fopen(argv[1], "r");
    if (!file)
    {
        cprintf("Error: Could not open file %s\r\n", argv[1]);
        return 1;
    }

    clrscr();

    while (fgets(line, sizeof(line), file))
    {
        if (line[0] == '@')
        {
            /* Handle commands */
            if (strncmp(line, "@bg ", 4) == 0)
            {
                /* Set background color */
                color = atoi(line + 4);
                textbackground(color);
            }
            else if (strncmp(line, "@fg ", 4) == 0)
            {
                /* Set foreground color */
                color = atoi(line + 4);
                textcolor(color);
            }
            else if (strncmp(line, "@soundcls", 10) == 0)
            {
                for (i = 0; i < 25; i++)
                {
                    beep(100 + i * 50, 50);
                    cprintf("\r\n");
                }
                clrscr();
            }

            else if (strncmp(line, "@cls", 4) == 0)
            {
                clrscr();
            }


            else if (strncmp(line, "@pause", 6) == 0)
            {
                beep(1000, 200);
                /* animate 3 dots until a key is pressed */
                waiting = 1;
                while (waiting)
                {
                    for (i = 0; i < 3; i++)
                    {
                        beep(100 + i * 100, 100);
                        cprintf(".");
                        delay(500);

                        if (kbhit())
                        {
                            getch();
                            waiting = 0;
                            beep(1200, 200);
                            beep(1000, 200);
                            break;
                        }
                    }
                    cprintf("\b\b\b   \b\b\b"); /* Clear the dots */
                }
            }

            else if (strncmp(line, "@linebeep", 9) == 0)
            {
                beeponlineonly = 1;
            }
            else if (strncmp(line, "@charbeep", 9) == 0)
            {
                beeponlineonly = 0;
            }
        }
        else
        {
            /* print each char with typing delay and punctuation pauses */
            if (beeponlineonly)
            {
                beep(600, 50);
            }

            for (p = line; *p; p++)
            {
                if (*p == '\n')
                {
                    putch('\r');
                    putch('\n');
                    continue;
                }

                if (*p == '\r')
                {
                    continue;
                }

                putch(*p);
                if (beeponlineonly)
                {
                    delay(1);
                }
                if (!beeponlineonly)
                {
                    if (*p == '.' || *p == '!' || *p == '?' || *p == ',')
                    {
                        beep(800, 200);
                        delay(300);
                    }
                    if (*p == ' ')
                    {
                        delay(100);
                    }
                    else
                    {
                        beep(600, 50);
                        delay(50);
                    }
                }
            }
        }
    }

    fclose(file);
    beep(1200, 500);
    clrscr();
    return 0;
}
