# The Linux Command Line

The Linux command line is a way to reconfigure parts of the kernel without
having to fully recompile it.

## Editing the command line
sudo virsh net-start default 
You can view your current command line with the following command:

```bash
cat /proc/cmdline
```

*/proc/cmdline*

```
[root@arch blix]# cat /proc/cmdline
BOOT_IMAGE=/vmlinuz-linux root=UUID=2d53026e-e884-4578-89eb-0766c74f6e71 
rw zswap.enabled=0 rootfstype=ext4 loglevel=3 quiet
[root@arch blix]# 
```

The command line is made up of a series of parameters that are separated by a 
space. Some parameter have a value, and some are just flags.

### Updating the command line in GRUB

*In this manual we wont go over every single bootloader but for more information
you can look at this page 
[on the ArchWiki about editing the command line](https://wiki.archlinux.org/title/Kernel_parameters)*

#### Non-persistent changes

When in the GRUB menu, hover over the entry you want to edit and press `e` to 
edit it. This will bring up a simple text editor where you can edit the GRUB
configuration.

Look for the line that starts with `linux`, it will be quite long and you can 
press `end` on the `linux` line to jump to the end of it. Lets add the `splash`
parameter to the end of the line. To boot this configuration press `ctrl + x` or `F10`.

You should now either see a blank screen where you would normally see the boot 
messages, or you should see a splash screen if your distribution has one.

Now cat out the command line again and you should see that the `splash` parameter 
has been added to the command line.

#### Making changes persistent

In your booted system use `vim` or `nano` to edit the file `/etc/default/grub`.
Look for the line:

```
GRUB_CMDLINE_LINUX_DEFAULT=
```

In there you can append or edit kernel configuration parameters. To then install
this configuration use `grub-mkconfig`:

```bash
sudo grub-mkconfig -o /boot/grub/grub.cfg
```

### Updating the command line in systemd-boot

Editing command line parameters in systemd-boot is similar to GRUB but you will
need to edit the entry in the `/boot` folder.

#### Non-persistent changes

When in the systemd-boot menu, hover over the entry you want to edit and 
press `e` to edit it.

#### Making changes persistent

Edit the file `/boot/loader/entries/arch.conf` and add the parameters you want 
to the end of the `options` line. Then save the file and reboot.

## Hijacking the command line

Without using your bootloader, you can force the command line to be something
else. This is done by forcing the `/proc/cmdline` file to use a mounted file instead of the real command line.

```
cp /proc/cmdline /tmp/cmdline
# Then edit it as desired
mount --bind /tmp/cmdline /proc/cmdline
```

**Note**: This will only change the command line for the user space processes,
the kernel will still use the original command line that is in memory.

## Kernel boot parameters

Below is a short list of boot related parameters as a quick guide:

- `init`: This parameter specifies the initial program that the kernel should 
run after fully booting. By default, this is usually set to `/sbin/init`.
- `initrd`: This parameter specifies the initial RAM disk (initrd) image that
the kernel should use during the boot process. The initrd is a temporary file
system that is loaded into memory and used by the kernel during the early stages of booting.
