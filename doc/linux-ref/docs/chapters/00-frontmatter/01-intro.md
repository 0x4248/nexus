# Introduction and Front Matter

Welcome to the 0x4248 Linux Reference Manual. This manual is a very technical 
Linux Reference for practitioners working close to kernel internals, user-space
interfaces, and Linux system behavior. 

$$
\color{red}
\text{\huge{\textbf{{This is NOT a beginners's manual.}}}} \\
$$

If you are new to Linux, I strongly
recommend starting with a more beginner-friendly resource, such as the
[Linux Journey](https://linuxjourney.com/) or 
[The Linux Command Line](http://linuxcommand.org/tlcl.php). Although this manual
 is not intended for beginners, there may be some beginner-friendly sections,
but this is mostly scarce and more of a quick reminder for practitioners.

## Architecture

This manual will primarialy focus on IBM-PC compatible computers using the 
x86-64 architecture, as this is the most common architecture for Linux. 
This manual may also go into older systems; such as i386, i486, and i686; and
may rarely cover ARM, SPARC, PowerPC.

### System I am using for examples in this manual

I will be using a standard QEMU x86-64 virtual machine with Arch Linux. Arch
Linux is good for this because it very minimal and allows easy configuration and
customization. Unlike Ubuntu and Mint with its many pre-installed packages and 
services.

I am also doing this in a virtual machine because it allows me to revert changes
easily.

### HTML 

If you are reading this manual online, you are likely reading the HTML version.
The HTML is split into multiple files, with one file per chapter. 
This allows for faster loading for people on slower lines such as dial-up or 
mobile telephone data connections. Check the `contents.html` file for the full
list of chapters.

I have also made it so the HTML version does not require JavaScript or intense
CSS to work, users using netscape navigator 4.0, Internet Explorer 5.0, and
even Lynx should be able to read this manual without much issue.

### PDF

PDF is a single file format almost exactly like the print version.

### Print

Print versions of this manual may be available for purchase for a low cost.

**Why?**

This manual is CC BY-SA licensed, which means you are free to share and adapt
the material for any purpose, even commercially, as long as you give appropriate
credit, provide a link to the license, and indicate if changes were made.

Therefore, print versions are at a low cost to cover printing, shipping, and
contributions to help develop the manual further.

### Updates and Errata
This manual is also a continuously evolving work in progress. I will be adding 
more content over time, and if you have purchased this manual in print it may be 
outdated by the time you receive it. However, you can always access the latest 
version of the manual online.

## License

The 0x4248 Linux Reference Manual is licensed under the Creative Commons 
Attribution-ShareAlike 4.0 International License (CC BY-SA 4.0).

Adaptation of the manual for commercial purposes is allowed under this license, 
as long as you give appropriate credit, provide a link to the license, and 
indicate if changes were made.

## Copyright

Copyright (c) 2024 by 0x4248. CC BY-SA 4.0 License applies to all content in 
this manual, including text, images, and other media.

## Credits

### Authors and Contributors

- 0x4248 (Main Author and Maintainer)

### Acknowledgments

- [Linux Kernel Documentation](https://www.kernel.org/doc/html/latest/)
- [Arch Wiki](https://wiki.archlinux.org/)
- [Linux From Scratch](http://www.linuxfromscratch.org/)
- [syscall.sh](https://syscall.sh/)
- [man7.org](https://man7.org/)

Huge thanks to the [All Things Linux](https://discord.gg/linux) for the welcoming and helpful community.