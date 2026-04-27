# Bootloaders

The Linux kernel is not a standalone executable. It must be loaded into memory
by a bootloader, which performs early system initialization and transfers
control to the kernel. On BIOS-based systems, this typically involves a boot
sector, while on UEFI systems the kernel may be loaded as an EFI executable via
the EFI stub.

Common Bootloaders are GRUB and Systemd-boot.

Here is a latex diagram of the boot process:

$$
\text{BIOS/UEFI} \rightarrow \text{Bootloader} \rightarrow \text{Kernel} \rightarrow \text{Initramfs (loaded by bootloader)} \rightarrow \text{Userspace}
$$



## Comparison of bootloaders

Below is a comparison of some of the most common bootloader's used for Linux, 
along with their support:

| BIOS                 | UEFI | MBR     | GPT     | Multiboot | Filesystem support |
|----------------------|------|---------|---------|-----------|--------------------|
| GRUB                 | Yes  | Yes     | Yes     | Yes       | Built in           |
| systemd-boot         | No   | Yes     | Yes     | Yes       | Built in           |
| EFI boot stub        | No   | Yes     | Yes     | Yes       | Firmware           |
| Limine               | Yes  | Yes     | Yes     | Yes       | Limited            |
| rEFInd               | No   | Yes     | Yes     | Yes       | Extensible         |
| Syslinux             | Yes  | Partial | Yes     | Partial   | Limited            |
| Clover               | Yes  | Yes     | No      | Yes       | Extensible         |
| GRUB Legacy          | Yes  | No      | Yes     | No        | Limited            |
| LILO                 | Yes  | No      | Partial | Yes       | Limited            |
| Unified kernel image | No   | Yes     | Yes     | Yes       | Firmware           |


## What a bootloader does

A bootloader is responsible for:

1. Locating a kernel image on disk or other media
2. Loading the kernel into memory
3. Loading an initramfs (if present)
4. Providing kernel command line arguments
5. Transferring control to the kernel entry point

On modern systems, the bootloader may also:

- Provide a menu for selecting kernel versions
- Handle multiple operating systems
- Load additional modules or microcode updates

## BIOS vs UEFI boot

### BIOS (Legacy)

- Uses MBR (Master Boot Record)
- Bootloader resides in the first sectors of the disk
- Limited space → multi-stage bootloaders (e.g. GRUB stage1/stage2)
- 16-bit real mode at boot

### UEFI

- Uses EFI System Partition (ESP)
- Bootloaders are regular `.efi` executables
- Firmware can directly load the kernel (EFI stub)
- No strict size limitations like MBR