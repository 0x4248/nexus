/* AArch64 kernel
 * Licence: GNU General Public License v3.0
 * By: 0x4248
*/

void kernel_call_poweroff()
{
    asm volatile(
        "ldr x0, =0x84000008\n"
        "hvc #0\n");
}
