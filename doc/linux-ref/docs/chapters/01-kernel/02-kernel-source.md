# The Linux Kernel Source Code

The Linux Kernel Source code is one of the largest and most complex codebases in
the world. With over 30 million lines of code, it is a massive undertaking to
understand. However, the base Linux kernel source code is relatively small, 
with the majority of the code being drivers and architecture-specific code. 

In the last section we discussed how the Linux kernel can be obtained and built.
Now lets read and go over the source code itself.

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

The rest of this file contains allot of `#include` statements. This is because
this is one of the core kernel init files.
