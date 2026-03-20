# The Linux Kernel Basics

This section will cover the Linux Kernel basics, including what it is, 
how to obtain it, and how to build it. We will dive later into the kernel
workings and developing for it in later sections.

## Linux Kernel Overview

The Kernel in short is a monolithic operating system kernel that is responsible
for managing the system's resources and providing an interface between the 
hardware and software.

$$
\text{User Space} \leftrightarrow \text{Kernel Space} \leftrightarrow \text{Hardware}
$$

### Kernel Space vs User Space

The kernel operates in a privileged mode called kernel space, while user 
applications run in user space. Unlike older systems such as MS-DOS, the Linux 
kernel enforces protection by controlling access to hardware and critical system 
resources. User-space applications cannot directly access these resources; instead,
they interact with the kernel through system calls, which provide a controlled 
interface for requesting services.

### Kernel design

The Linux kernel is designed to be modular and extensible, allowing for a wide
range of hardware and software configurations. It supports a variety of
architectures, including x86, ARM, PowerPC, and more. The kernel is also
designed to be highly configurable, this manual will extensively explore the 
kernel configuration system and how you can customize the kernel for your 
specific needs.


## Kernel Branches

*For more details about the kernel source code see the* **Kernel Source Code** *chapter.*

Across many Linux distributions, there are several kernel branches:

- Stable: This is the mainline kernel that is considered stable and is used in most
    production environments.
- Long-Term Support (LTS): These are kernels that receive long-term support and
    are maintained for several years.
- Hardened: These kernels are modified to include additional security features and
    patches to enhance security.
- Real-Time: These kernels are modified to provide real-time capabilities for
    applications that require low latency and deterministic behavior.

## A Crash Course on Compiling the Kernel

Compiling the kernel can be a complex process, but it allows you to fully 
customize it for your specific hardware and needs.

### Dependencies

*This section has been tested to work with kernel version 7.0.0.*

To compile the kernel, you will need to install the necessary dependencies.
Below are some of the common package names for the required dependencies on 
Debian-based distributions. The exact package names may vary depending on your
distribution and version.

```
# Debian/Trixie
build-essential
git
bc
bison
flex
libelf-dev
dwarves
ccache
pkg-config
cmake
ninja-build
python3
python3-pip
rsync
wget
curl
xz-utils
file
libncurses-dev
```

### Obtaining the Kernel Source Code

Getting a copy of the kernel source code is non-trivial, but there are several 
ways to do it. The most common way is to clone the kernel source code from the 
official Linux kernel repository on GitHub:

```bash
git clone https://github.com/torvalds/linux.git
```

Cloning the mainline repository gives you the latest development version
of the kernel. While it is generally usable, it is not considered a stable
release and may contain experimental changes. For production systems,
it is recommended to use a tagged stable or LTS release.

*For this example we will be using version 6.8, but you can choose any version 
you want.*

```bash
VERSION=6.8
git clone --depth 1 --branch v${VERSION} https://github.com/torvalds/linux.git
```

### Basic compilation

Now for the fun part, compiling the kernel. This section is not an in-depth 
guide to kernel compilation, but will cover the basic steps.


#### Configuration

If you have just cloned the kernel source code, there will be no configuration
file, but creating one is simple.

```bash
make defconfig
```

This will generate a `.config` file in the root of the kernel source code
directory. It works by enabling or disabling configuration options, which are
translated into C preprocessor macros used during compilation.

The kernel does this by mostly using `#ifdef` and `#ifndef` preprocessor 
directives. For example:

```c
#define CONFIG_EXAMPLE 1

#ifdef CONFIG_EXAMPLE
pr_log("Example is enabled\n");
#else
pr_log("Example is disabled\n");
#endif
```

This means if you have turned off for example `networking support` in the kernel
configuration, the kernel will not compile any of the code related to networking
and will not include it in the final kernel image. With this knowledge, you can
see how customizable the kernel is, and how you can optimize it for your
specific hardware.

##### Menuconfig and Xconfig

Now we have a basic configuration file, you can edit it using `vim` or `nano`
but this can be trivial and long. Many of the config names are abbreviated and 
not very descriptive. To make it easier to edit the configuration, you can use 
`menuconfig` or `xconfig`.

For this guide we will use `menuconfig` as it is more widely supported and allows
for virtual console users to edit the configuration. 

You will need an X11 graphical environment. If you are using a virtual machine 
using QEMU serial console, you will not be able to use `xconfig` and will need 
to use `menuconfig` instead.

```bash
make menuconfig
```

You can also use the much nicer `nconfig` which looks better and has function
key navigation.

```bash
make nconfig
```

This will open a `ncurses` interface that allows you to navigate through the 
configuration options much like the `dialog` command seen on many Linux 
distribution installers. You can use the arrow keys to navigate, `Enter` to 
select, and `Space` to toggle options.

*For now we are not going to change any options, but feel free to explore and 
change any options you want.*

The kernel uses the `Kconfig` system to manage its configuration and build 
process. In this section we wont look too much into it now.

#### Building the Kernel

Now that we have a configuration file, we can build the kernel. This is done
using the `make` command. The kernel uses a custom build system that is based on
`make`, but it has some additional features and optimizations. To build the kernel, you can simply run:

```bash
make -j$(nproc)
```
 
The `-j` flag tells `make` to use multiple threads to speed up the build
process. The `$(nproc)` command returns the number of CPU cores available, 
which allows `make` to use all available cores for the build.

I recommend using `ccache` to speed up subsequent builds. `ccache` is a compiler
cache that stores the results of previous compilations and reuses them when the same
compilation is requested again. This can significan notly reduce the build time for
subsequent builds, especially when making small changes to the kernel configuration.
To use `ccache`, you can set the `CC` environment variable to `ccache gcc` before

running the `make` command:

```bash
export CC="ccache gcc"
make -j$(nproc)
```

## Troubleshooting common issues

When testing your custom kernel, you may encounter some issues.

### Tips on testing your kernel

Before you test your kernel, you should keep the old kernel in the `/boot` 
directory. This way, if your new kernel does not boot, you can easily revert 
to the old kernel. *A big life-saver*

### Kernel panics

*In this section we are not going over the details of `panic.c` but look into how
to read such panics*

Here is an example from the **ArchWiki**:

```
kernel: BUG: unable to handle kernel NULL pointer dereference at (null) 1
kernel: IP: fw_core_init+0x18/0x1000 [firewire_core] 2
kernel: PGD 718d00067
kernel: P4D 718d00067
kernel: PUD 7b3611067
kernel: PMD 0
kernel:
kernel: Oops: 0002 [#1] PREEMPT SMP
kernel: Modules linked in: firewire_core(+) crc_itu_t cfg80211... 3
kernel: CPU: 6 PID: 1438 Comm: modprobe Tainted: P           O    4.13.3-1-ARCH #1
kernel: Hardware name: Gigabyte Technology Co., Ltd. H97-D3H/H97-D3H-CF, BIOS F5 06/26/2014
kernel: task: ffff9c667abd9e00 task.stack: ffffb53b8db34000
kernel: RIP: 0010:fw_core_init+0x18/0x1000 [firewire_core]
kernel: RSP: 0018:ffffb53b8db37c68 EFLAGS: 00010246
kernel: RAX: 0000000000000000 RBX: 0000000000000000 RCX: 0000000000000000
kernel: RDX: 0000000000000000 RSI: 0000000000000008 RDI: ffffffffc16d3af4
kernel: RBP: ffffb53b8db37c70 R08: 0000000000000000 R09: ffffffffae113e95
kernel: R10: ffffe93edfdb9680 R11: 0000000000000000 R12: ffffffffc16d9000
kernel: R13: ffff9c6729bf8f60 R14: ffffffffc16d5710 R15: ffff9c6736e55840
kernel: FS:  00007f301fc80b80(0000) GS:ffff9c675dd80000(0000) knlGS:0000000000000000
kernel: CS:  0010 DS: 0000 ES: 0000 CR0: 0000000080050033
kernel: CR2: 0000000000000000 CR3: 00000007c6456000 CR4: 00000000001406e0
kernel: Call Trace:
kernel:  do_one_initcall+0x50/0x190 4
kernel:  ? do_init_module+0x27/0x1f2
kernel:  do_init_module+0x5f/0x1f2
kernel:  load_module+0x23f3/0x2be0
kernel:  SYSC_init_module+0x16b/0x1a0
kernel:  ? SYSC_init_module+0x16b/0x1a0
kernel:  SyS_init_module+0xe/0x10
kernel:  entry_SYSCALL_64_fastpath+0x1a/0xa5
kernel: RIP: 0033:0x7f301f3a2a0a
kernel: RSP: 002b:00007ffcabbd1998 EFLAGS: 00000246 ORIG_RAX: 00000000000000af
kernel: RAX: ffffffffffffffda RBX: 0000000000c85a48 RCX: 00007f301f3a2a0a
kernel: RDX: 000000000041aada RSI: 000000000001a738 RDI: 00007f301e7eb010
kernel: RBP: 0000000000c8a520 R08: 0000000000000001 R09: 0000000000000085
kernel: R10: 0000000000000000 R11: 0000000000000246 R12: 0000000000c79208
kernel: R13: 0000000000c8b4d8 R14: 00007f301e7fffff R15: 0000000000000030
kernel: Code: <c7> 04 25 00 00 00 00 01 00 00 00 bb f4 ff ff ff e8 73 43 9c ec 48
kernel: RIP: fw_core_init+0x18/0x1000 [firewire_core] RSP: ffffb53b8db37c68
kernel: CR2: 0000000000000000
kernel: ---[ end trace 71f4306ea1238f17 ]---
kernel: Kernel panic - not syncing: Fatal exception 5
kernel: Kernel Offset: 0x80000000 from 0xffffffff810000000...
kernel: ---[ end Kernel panic - not syncing: Fatal exception
```

This may look scary and confusing. Lets break it down.

1.  A `BUG` was called, this is a critical error that almost certainly indicates
    the kernel to `panic`. This usually means its a programmers bug or error.
2.  The logs suggest the error came from `fw_core_init` in the `firewire_core` 
    module. This is a good starting point for debugging, you can look at the 
    source code of this function to see what may be causing the issue.
3.  Suggests that the `firewire_core` was the most recent module loaded.
4.  The `do_one_initcall` function was what called the `fw_core_init` function.
5.  Panic was called and system is deadlocked.

With this information we can do two things:
- Change kernel config to not compile the module and disable it.
- Don't recompile the kernel and use `module_blacklist` to prevent the module 
  from loading.

## The /boot Directory

The `/boot` directory is where the kernel and its related files are stored. This
directory is critical for the boot process, as it contains the kernel image and
the initial RAM filesystem (initramfs) that the kernel uses during boot.

If you open this directory, you should see three important files:

- `vmlinuz-<version>`: This is the compressed Linux kernel image.
- `initramfs-<version>.img`: This is the initial RAM filesystem image.
- `initrd.img-<version>`: This is the initial RAM disk image. This is a legacy
    format that is mostly unused now, but some older systems may still use it.

Here is what my `/boot/` directory looks like:

```bash
[root@arch boot]# ls -l
total 24916
drwxr-xr-x 6 root root     4096 Mar 19 19:50 grub
-rwxr-xr-x 1 root root  9038759 Mar 19 19:50 initramfs-linux.img
-rwxr-xr-x 1 root root 16466432 Mar 19 19:50 vmlinuz-linux
[root@arch boot]# 
```

### Vmlinuz

**V**irtual **M**emory **LINU**x g**Z**ip. This is the compressed Linux kernel
image. When the system boots, the bootloader loads this image into memory and
decompresses it to start the operating system.

### RAM Disks

A RAM disk is a virtual disk that resides in the system's RAM. It is used during
the boot process to provide a temporary root filesystem before the actual root
filesystem is mounted. This allows the kernel to load necessary drivers and
modules. This is because things like controller drivers for disks and other 
devices may not work without the RAM disk.

#### Types of RAM Disks

- **initramfs**: This is the modern format for the initial RAM filesystem. It is
    a compressed `CPIO` archive that is loaded into memory and used as the initial
    root filesystem during boot.
- **initrd**: This is the older format for the initial RAM disk.

Linux supports these compressions on the kernel and initramfs images:

- gzip
- bzip2
- xz
- lzma
- lzop
- lz4
- zstd

**Note**: The kernel can not load the initramfs if it is compressed with a 
compression algorithm that the kernel has not been compiled with support for. 
Ensure you enable support for the compression algorithm using `menuconfig`.
