/* AArch64 kernel
 * Licence: GNU General Public License v3.0
 * By: 0x4248
*/

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "power.h"

volatile uint8_t *uart = (uint8_t *)0x09000000;

void putchar(char c)
{
    *uart = c;
}

void printk(const char *s)
{
    while (*s != '\0')
    {
        putchar(*s);
        s++;
    }
}



/**
 * kmain - main kernel function
*/
void kmain(void)
{
    printk("Welcome to this simple aarch64 OS!\n");
    kernel_call_poweroff();
}
