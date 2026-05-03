/* Compiled C
 * A collection of C programs that are compiled to assembly 
 * code for understanding how C and assembly work.
 * GitHub: https://wwww.github.com/0x4248/Compiled_C
 * Licence: GNU General Public Licence v3.0
 * By: 0x4248
 * 
 * This program has two strings and concatenates them.
*/

#include <string.h>

int main() {
    char x[10] = "Hello";
    char y[5] = "World";
    strcat(x, y); // x = x + y
    return 0;
}