/*
 * Hello world module
 *
 * This is really basic, mainly for a starting point for modules.
 *
 * 0x4248
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kern_levels.h>

#include "lib/log.h"


static int demo_init(void)
{
    LOG("base", KERN_INFO, "Hello");
	return 0;
}

static void demo_exit(void)
{
    LOG("base", KERN_INFO, "Bye");
}

module_init(demo_init);
module_exit(demo_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("0x4248");
MODULE_DESCRIPTION("A hello world driver");
