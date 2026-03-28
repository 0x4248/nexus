# The Linux Filesystem Hierarchy

The Linux filesystem hierarchy is a standardized directory structure that 
defines the organization of files and directories on a Linux system. It is based
on the Filesystem Hierarchy Standard (FHS) and is used by most Linux 
distributions.

## Standard Hierarchy

According to the Free Desktop Organization, the standard Linux filesystem 
hierarchy is as follows:

- `/bin`: Essential user command binaries
- `/boot`: Files for the boot loader and kernel
- `/dev`: Device files
- `/etc`: Host-specific system configuration
- `/home`: User home directories
- `/lib`: Shared libraries and kernel modules
- `/media`: Mount points for removable media (CD-ROM, Floppy, etc)
- `/mnt`: Mount point for mounting a filesystem temporarily
- `/opt`: Add-on application software packages
- `/proc`: Virtual filesystem providing process and kernel information as files
- `/root`: Home directory for the root user
- `/run`: Runtime variable data
- `/sbin`: System binaries
- `/srv`: Data for services provided by the system
- `/sys`: Virtual filesystem providing system information and configuration
- `/tmp`: Temporary files
- `/usr`: Secondary hierarchy for read-only user data
- `/var`: Variable data files

Each of these directories has a specific purpose and contains specific types of 
files.

### /bin

The `/bin` directory contains executable binaries that are essential for the 
system and can be ran by all users. In here you will find common commands such
as `ls`, `cp`, `mv`, `rm`, and many more.

### /boot

The `/boot` directory is where the kernel and its related files are stored. This
directory is critical for the boot process, as it contains the kernel image and 
the initial RAM filesystem (initramfs) that the kernel uses during boot.

If you open this directory, you should see three important files:

- vmlinuz-<version>: This is the compressed Linux kernel image.
- initramfs-<version>.img: This is the initial RAM filesystem image.
- initrd.img-<version>: This is the initial RAM disk image. This is a legacy format that is
                    mostly unused now, but some older systems may still use it.

Here is what my `/boot` directory looks like:

```
[root@arch boot]# ls -l
total 24916
drwxr-xr-x 6 root root 4096 Mar 19 19:50 grub
-rwxr-xr-x 1 root root 9038759 Mar 19 19:50 initramfs-linux.img
-rwxr-xr-x 1 root root 16466432 Mar 19 19:50 vmlinuz-linux
11
[root@arch boot]#
```

#### Vmlinuz

Virtual Memory LINUx gZip. This is the compressed Linux kernel image. When the system boots, the
bootloader loads this image into memory and decompresses it to start the operating system.

#### RAM Disks

A RAM disk is a virtual disk that resides in the system’s RAM. It is used during the boot process to
provide a temporary root filesystem before the actual root filesystem is mounted. This allows the kernel
to load necessary drivers and modules. This is because things like controller drivers for disks and other
devices may not work without the RAM disk.

##### Types of RAM Disks

- `initramfs`: This is the modern format for the initial RAM filesystem. It is a compressed CPIO
archive that is loaded into memory and used as the initial root filesystem during boot.
- `initrd`: This is the older format for the initial RAM disk.

Linux supports these compressions on the kernel and initramfs images:

- `gzip`
- `bzip2`
- `xz`
- `lzma`
- `lzop`
- `lz4`
- `zstd`


**Note**: The kernel can not load the initramfs if it is compressed with a compression algorithm that the
kernel has not been compiled with support for. Ensure you enable support for the compression algorithm
using `menuconfig`

### /dev

The `/dev` directory contains device files that represent hardware devices and 
virtual devices on the system. These files allow user-space programs to interact
with hardware and system resources without needing to interact with the hardware 
directly. This increases security and stability since you can give file 
permissions to these device files and control who can access them.

#### A brief overview of device files

*In this list, `*` denotes a wildcard matching any characters.*

##### Disks

- `sd*`:
  SCSI disk devices; now used generically for most block storage (SATA, USB).
- `hd*`:
  Legacy IDE disk devices; largely deprecated in favor of `sd*`.
- `nvme*`:
  NVMe solid-state storage devices.

##### Terminals

- `tty*`:
  Terminal devices (virtual consoles and some serial interfaces).
- `pts/*`:
  Pseudo-terminal slave devices used by terminal emulators and SSH sessions.
- `ttyS*`:
  Traditional serial port devices.
- `ttyUSB*`:
  USB-to-serial converter devices.
- `ttyACM*`:
  USB CDC ACM devices (modems, microcontrollers, etc.).
- `ttyAMA*`:
  ARM-specific serial devices (commonly seen on embedded platforms).

##### Network Interfaces

- `eth*`:
  Traditional Ethernet interface names (legacy naming scheme).
- `wlan*`:
  Wireless LAN interfaces.
- `lo`:
  Loopback interface.
- `en*`:
  Predictable interface names (modern systems, e.g. `enp3s0`).

##### Sound Devices

- `snd/*`:
  ALSA sound subsystem devices.
- `audio*`:
  Legacy audio device interfaces.
- `midi*`:
  MIDI devices.
- `pcm*`:
  PCM audio devices for playback and capture.

##### USB and Input Devices

- `usb/*`:
  USB device hierarchy.
- `input/*`:
  Input devices (keyboard, mouse, controllers).

##### Misc Devices

- `lp*`:
  Parallel port (printer) devices.

##### Non-hardware Devices

- `null`:
  Discards all data written; reads return EOF.
- `zero`:
  Produces an infinite stream of null bytes.
- `random`:
  Blocking source of random data (entropy-based).
- `urandom`:
  Non-blocking random data source.
- `full`:
  Always fails writes with ENOSPC ("disk full").
- `console`:
  System console for kernel and early userspace output.

### /etc

The `/etc` directory contains host-specific system configuration files. Many of 
these are not related to the kernel and more used by user-space programs. 

Privileged users can usually only edit these files

### /home

The `/home` directory contains the home directories for regular users. Folders
are owned by the user and have permissions set to allow the user to read, write, and execute files within their home directory.

User configuration files are stored in the directory with a dot prefix, such as `.bashrc` and are also stored in the `.config` and `.local` directories. These files and directories are hidden by default.

### /lib

The `/lib` directory contains essential shared libraries and kernel modules. These libraries are required for the system to boot and run basic commands.

### /media

The `/media` directory is used for mounting removable media:

- `/media/floppy`: Mount point for floppy disks.
- `/media/cdrom`: Mount point for CD-ROMs.
- `/media/cdrecorder`: Mount point for CD writers.
- `/media/zip`: Mount point for ZIP drives.

You may also see:

- `/media/usb`: Mount point for USB drives.
- `/media/dvd`: Mount point for DVDs.
- `/media/iso`: Mount point for ISO images.

But these are not standardized and may not be present on all systems.

### /mnt

The `/mnt` directory is a temporary mount point for mounting filesystems. It
must not be used for permanent mounts. It is typically used by system administrators when they need to mount a filesystem temporarily for maintenance.

### /opt

Optional application software packages are stored in the `/opt` directory. This is where third-party applications that are not part of the standard distribution can be installed.

### /proc

The `/proc` directory is a virtual filesystem that provides process and kernel information as files. It is used to access information about the system and running processes and is managed by the kernel. 

### /root

The `/root` directory is the home directory for the root user. It is separate from the `/home` directory to ensure that the root user's files are kept separate from regular users' files.

### /var

The `/var` directory contains variable data files. This includes log files, spool files, and temporary files that are expected to grow in size over time.

### /usr

The `/usr` directory is a secondary hierarchy for read-only user data. It contains the majority of user utilities and applications. This includes subdirectories such as:

- `/usr/bin`: Non-essential user command binaries.
- `/usr/lib`: Non-essential shared libraries and kernel modules.
- `/usr/local`: Local hierarchy for system administrator-installed software.
- `/usr/share`: Architecture-independent data (e.g., documentation, icons).
- `/usr/include`: Header files for development.
- `/usr/src`: Source code for the kernel and other software. (Not always present)
- `/usr/games`: Game binaries (not always present)

### /sbin

Administrative system binaries are stored in the `/sbin` directory. Similar to 
`/bin`, but these are intended for use by the system administrator and system
itself.

### /srv

The `/srv` directory contains data for services provided by the system. This can include web server files, FTP server files, and other service-related data.