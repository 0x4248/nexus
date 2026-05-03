# The Linux Kernel Source Code

The Linux Kernel Source code is one of the largest and most complex codebases in
the world. With over 30 million lines of code, it is a massive undertaking to
understand. However, the base Linux kernel source code is relatively small, 
with the majority of the code being drivers and architecture-specific code. 

In the last section we discussed how the Linux kernel can be obtained and built.
Now let's read and go over the source code itself.

## Layout of the source code

The Linux kernel source code is organized into several directories, each containing
different parts of the kernel. The main directories are:

- `arch/`: Architecture-specific code for different CPU architectures.
- `block/`: Block device drivers.
- `certs/`: Certificates for module signing.
- `crypto/`: Cryptographic algorithms and drivers.
- `Documentation/`: Documentation for the kernel.
- `drivers/`: Device drivers for various hardware.
- `fs/`: Filesystem code.
- `include/`: Header files for the kernel.
- `init/`: Initialization code for the kernel.
- `io_uring/`: io_uring subsystem code.
- `ipc/`: Inter-process communication code.
- `kernel/`: Core kernel code.
- `lib/`: Library code used by the kernel.
- `mm/`: Memory management code.
- `net/`: Networking code.
- `rust/`: Development of the Rust language support in the kernel.
- `samples/`: Sample code for various kernel features.
- `scripts/`: Scripts for building and maintaining the kernel.
- `security/`: Security modules and code.
- `sound/`: Sound drivers and ALSA code.
- `usr/`: User-space tools and utilities.
- `virt/`: Virtualization code.

In this book we are not going to go over every single directory and file in the
kernel source code, but we will focus on the core workings.

### How to read the source code

The Linux kernel source code is written in C, with some assembly code for 
architecture-specific parts. It also uses Makefile and Shell scripts for 
building and maintaining the kernel.

When reading the source code, it is important to understand the overall 
structure of the kernel and how the different parts interact with each other.

The C follows the C99 standard with some GNU extensions. The code is heavily 
commented, but it can still be difficult to understand without a good 
understanding of C and the kernel's architecture. The code is also 80-column 
formatted, which makes it easier for CRT and 4:3 users to read; like myself.

> *"Help, I find it hard to follow functions and references in the code!"*

Do not worry, a great website called 
[The Bootlin Elixir Cross-Reference](https://elixir.bootlin.com/linux/latest/source) 
allows you to easily navigate the kernel source code and find references to 
functions and variables. It also has a great search feature that allows you to 
find specific functions and variables. Perfect for people new and experienced.

### Standard C file structure

In the kernel source code, C files typically have a standard structure. They 
usually start with a large comment block that describes the file, its purpose, 
and its authors. This is followed by include statements, macro definitions, and 
then the actual code.

*init/main.c*

```c
// SPDX-License-Identifier: GPL-2.0-only
/*
 *  linux/init/main.c
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 *
 *  GK 2/5/95  -  Changed to support mounting root fs via NFS
 *  Added initrd & change_root: Werner Almesberger & Hans Lermen, Feb '96
 *  Moan early if gcc is old, avoiding bogus kernels - Paul Gortmaker, May '96
 *  Simplified starting of init:  Michael A. Griffith <grif@acm.org>
 */

#define DEBUG		/* Enable initcall_debug */
```

1.  Line 1 is a SPDX license identifier, which is a standard way to indicate the
    license of the file. In this case, it indicates that the file is licensed 
    under the GPL-2.0-only license.
2.  Line 3 explains the file path and name.
3.  Lines 5-9 are a comment block that describes the file, its purpose, and its 
    authors. It also includes a history of changes to the file.
4.  Line 11 is a macro definition and where the actual code starts.

The rest of this file contains a lot of `#include` statements. This is because
this is one of the core kernel init files.

Below in this file there will also be functions and variables defined. We won't
go over how to read and understand C code in this book, but if you are interested.
I strongly recommend the book 
[The C Programming Language](https://en.wikipedia.org/wiki/The_C_Programming_Language)

On several functions you may see the `__init` macro. This is a special macro 
that tells the kernel that the function is only used during initialization. Once
the kernel has finished initializing, the memory used by these functions can be
freed up. This is because these functions are only needed once.

To make a function a initialization function, you can simply add the macro to 
the function definition. For example:

```c
static int __init my_init_function(void) {
    return 0;
}
```

### Kbuild and Makefiles

The Linux kernel uses a custom build system known as Kbuild, which is built
on top of make. Due to the size of the codebase, the build system is split
across many smaller Makefiles distributed throughout the source tree.

The top-level Makefile, located at the root of the kernel source, orchestrates
the overall build. It is responsible for invoking the build process and
descending into subdirectories. In practice, it is rarely modified directly.

Most work is done in per-directory Makefiles (often referred to as Kbuild
files). These describe:

- Which source files are compiled
- Whether they are built into the kernel or as modules
- Relationships between objects

Kbuild operates recursively. Each directory contributes objects, which are
aggregated into:

- `vmlinux` (the final kernel image)
- Loadable modules (`*.ko`)

A typical entry:

```
obj-y += main.o
```

This instructs Kbuild to compile `main.c` and link it into the kernel image.

Alternatively:

```
obj-m += my_driver.o
```


This builds `my_driver.c` as a loadable kernel module.

Kbuild works closely with the kernel configuration system (`Kconfig`).
Kconfig determines *what* is built, while Kbuild determines *how* it is built.

### Kconfig

Kconfig (Kernel Configuration) is the system used to manage kernel build
options. The kernel supports a wide range of hardware and features, so it is
not practical to build everything into a single image. Kconfig allows the user
to select exactly which components are included.

Configuration options are defined across many `Kconfig` files throughout the
source tree. These files form a hierarchy, which is presented to the user via
interfaces such as `menuconfig`.

The main `Kconfig` file at the root of the kernel source is minimal. It
primarily includes other `Kconfig` files from subdirectories, forming the full
configuration tree.

*Kconfig*

```c
mainmenu "Linux/$(ARCH) $(KERNELVERSION) Kernel Configuration"

source "scripts/Kconfig.include"

source "init/Kconfig"

source "kernel/Kconfig.freezer"

source "fs/Kconfig.binfmt"

...
```

Lets take a look at a better `Kconfig` file, the one located in `lib/Kconfig.debug`. This is responsible for the kernel hacking and debugging options.

*lib/Kconfig.debug*

```
# SPDX-License-Identifier: GPL-2.0-only
menu "Kernel hacking"

menu "printk and dmesg options"

config PRINTK_TIME
	bool "Show timing information on printks"
	depends on PRINTK
	help
	  Selecting this option causes time stamps of the printk()
	  messages to be added to the output of the syslog() system
	  call and at the console.

	  The timestamp is always recorded internally, and exported
	  to /dev/kmsg. This flag just specifies if the timestamp should
	  be included, not that the timestamp is recorded.

	  The behavior is also controlled by the kernel command line
	  parameter printk.time=1. See Documentation/admin-guide/kernel-parameters.rst

config PRINTK_CALLER
	bool "Show caller information on printks"
	depends on PRINTK
	help
	  Selecting this option causes printk() to add a caller "thread id" (if
	  in task context) or a caller "processor id" (if not in task context)
	  to every message.

	  This option is intended for environments where multiple threads
	  concurrently call printk() for many times, for it is difficult to
	  interpret without knowing where these lines (or sometimes individual
	  line which was divided into multiple lines due to race) came from.

	  Since toggling after boot makes the code racy, currently there is
	  no option to enable/disable at the kernel command line parameter or
	  sysfs interface.

config STACKTRACE_BUILD_ID
	bool "Show build ID information in stacktraces"
	depends on PRINTK
	help
	  Selecting this option adds build ID information for symbols in
	  stacktraces printed with the printk format '%p[SR]b'.

	  This option is intended for distros where debuginfo is not easily
	  accessible but can be downloaded given the build ID of the vmlinux or
	  kernel module where the function is located.

config CONSOLE_LOGLEVEL_DEFAULT
	int "Default console loglevel (1-15)"
	range 1 15
	default "7"
	help
	  Default loglevel to determine what will be printed on the console.

	  Setting a default here is equivalent to passing in loglevel=<x> in
	  the kernel bootargs. loglevel=<x> continues to override whatever
	  value is specified here as well.

	  Note: This does not affect the log level of un-prefixed printk()
	  usage in the kernel. That is controlled by the MESSAGE_LOGLEVEL_DEFAULT
	  option.
```

This can look complex compared to your traditional `JSON` or `YAML` configuration files, but it is actually quite simple if we break it down.

- `menu "Kernel hacking"`: This defines a new menu in the menuconfig
- `menu "printk and dmesg options"`: This defines a submenu under the "Kernel hacking" menu
- `config PRINTK_TIME`: This defines a new configuration option called `PRINTK_TIME` however on build this will be converted to `CONFIG_PRINTK_TIME`
to prevent name clashes.
- `bool "Show timing information on printks"`: This specifies that the option is a boolean (true/false) and provides a description for it.
- `depends on PRINTK`: This specifies that this option can only be enabled if the `PRINTK` option is also enabled.
- `help`: This provides a help message that will be displayed when the user selects this option in the menuconfig interface.

An `endmenu` is further down in the file to leave the submenu and return to the parent menu.

Running the kernel currently will show a timestamp next to the logs:

*QEMU*

```
[    0.000000] Linux version 7.0.0-rc4-gee5389d2ec94
[    0.000000] Command line: console=ttyS0
[    0.000000] BIOS-provided physical RAM map:
```

Now lets disable the `PRINTK_TIME` option in menuconfig:

*QEMU*

```
Linux version 7.0.0-rc4-g0f052016d8b4-dirty
Command line: console=ttyS0
BIOS-provided physical RAM map:
```

Now you can see how the timestamps are gone. This is just one example of how 
Kconfig allows you to easily configure the kernel and enable or disable features 
as needed.

Kconfig writes the configuration options to a file called `.config` in the root 
of the kernel source. This file is then used by Kbuild. Editing this file is not 
recommended, but we can inspect it to see how the options are stored.

*.config*

```
#
# Automatically generated file; DO NOT EDIT.
# Linux/x86 7.0.0-rc4 Kernel Configuration
#
CONFIG_CC_VERSION_TEXT="gcc (Debian 14.2.0-19) 14.2.0"
CONFIG_CC_IS_GCC=y
CONFIG_GCC_VERSION=140200
CONFIG_CLANG_VERSION=0
CONFIG_AS_IS_GNU=y
...and so on
```

We can find for the `CONFIG_PRINTK_TIME` option:

```
#
# printk and dmesg options
#
# CONFIG_PRINTK_TIME is not set
# CONFIG_PRINTK_CALLER is not set
# CONFIG_STACKTRACE_BUILD_ID is not set
CONFIG_CONSOLE_LOGLEVEL_DEFAULT=7
``` 

You will notice it is not set and we can re-enable it by changing the line to:

```
CONFIG_PRINTK_TIME=y
```

Recompiling the kernel will then enable the timestamps again.

#### Hello World Kconfig Example

Now we know how the configuration system works, let's create a simple "Hello World" configuration option.

Going back to the `lib/Kconfig.debug` file, we can add a new option at the start
of the file:

*lib/Kconfig.debug*

```
# SPDX-License-Identifier: GPL-2.0-only
menu "Kernel hacking"

menu "printk and dmesg options"

config HELLO_WORLD
	bool "Hello world"
	depends on PRINTK
	help
	  Demo option to show how config options work. Adds "Hello world!" to the
	  printk output during boot.
```

Now we should see:

```
Kernel hacking  --->
    printk and dmesg options  --->
        [ ] Hello world (NEW)
```

Now enable it, `CONFIG_HELLO_WORLD` now exists in the `.config` and we can make
a simple patch in the `printk` system to append "Hello world!" to the end of 
every printk message.


Now go to this file:

*kernel/printk/printk.c*

```c
static bool printk_time = IS_ENABLED(CONFIG_PRINTK_TIME);
module_param_named(time, printk_time, bool, S_IRUGO | S_IWUSR);

static size_t print_syslog(unsigned int level, char *buf)
{
	return sprintf(buf, "<%u>", level);
}

static size_t print_time(u64 ts, char *buf)
{
	unsigned long rem_nsec = do_div(ts, 1000000000);

	return sprintf(buf, "[%5lu.%06lu]",
		       (unsigned long)ts, rem_nsec / 1000);
}
```

I know this is not the best way to add new features but we are only demonstrating 
how to use Kconfig and the configuration system.

*kernel/printk/printk.c*

```c
static size_t print_time(u64 ts, char *buf)
{
	unsigned long rem_nsec = do_div(ts, 1000000000);

#ifdef CONFIG_HELLO_WORLD
	return sprintf(buf, "Hello World! [%5lu.%06lu]",
		       (unsigned long)ts, rem_nsec / 1000);
#else
	return sprintf(buf, "[%5lu.%06lu]",
	       (unsigned long)ts, rem_nsec / 1000);	
#endif
}
```

So now when on compile it will only keep the "Hello World!" string if `CONFIG_HELLO_WORLD` is enabled.


**Note**: You might want to re-enable the `PRINTK_TIME` option to see the effect 
of this change, since in the last section we disabled it.


*QEMU*

```
Hello World! [    0.000000] Linux version 7.0.0-rc4-g0f052016d8b4-dirty
Hello World! [    0.000000] Command line: console=ttyS0
Hello World! [    0.000000] BIOS-provided physical RAM map:
Hello World! [    0.000000] NX (Execute Disable) protection: active
Hello World! [    0.000000] APIC: Static calls initialized
```

Now we see our "Hello World!" string in the printk output during boot. This is 
just a simple example of how to use Kconfig to add new configuration options to 
the kernel and how to use those options in the code.

### 