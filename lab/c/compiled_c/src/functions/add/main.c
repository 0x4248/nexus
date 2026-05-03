/* Compiled C
 * A collection of C programs that are compiled to assembly
 * code for understanding how C and assembly work.
 * GitHub: https://wwww.github.com/0x4248/Compiled_C
 * Licence: GNU General Public Licence v3.0
 * By: 0x4248
 *
 * This program has a function that returns the value of A + B.
*/

int add(int a, int b) {
    int c = a + b;
    return c;
}

int main() {
    int a = 1;
    int b = 2;
    int c = add(a, b);
    return 0;
}
