
#pragma once
#include <linux/printk.h>


/*
 * LOG(MODULE, LVL, FMT, ...)
 * Produces:
 *   0x4248/<MODULE>: <MSG>
 *
 * LVL is a printk level prefix like KERN_INFO, KERN_ERR, etc.
 *
 * This is really for internal testing so I can see clearly where the logs are
 * being made.
 */
#define LOG(mod, lvl, fmt, ...) \
	_printk(lvl "0x4248/%s: " fmt, mod, ##__VA_ARGS__)
