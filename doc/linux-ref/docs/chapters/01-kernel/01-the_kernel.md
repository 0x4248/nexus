# The Linux Kernel

This section will cover the Linux Kernel. In this chapter, we will look at
the Linux kernel branches, how to build, and an overview of common issues and
topics related to the kernel. Then we will look at other topics such as the 
kernel's boot process and its command line parameters.

## The /boot Directory

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
    a compressed cpio archive that is loaded into memory and used as the initial
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

**Note**: The kernel cant load the initramfs if it is compressed with a 
compression algorithm that the kernel has not been compiled with support for. 
Ensure you enable support for the compression algorithm using `menuconfig`.