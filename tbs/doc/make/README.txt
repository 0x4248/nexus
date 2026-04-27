MAKE SYSTEM
===========
Welcome to the Nexus Makefile

The primary makefile in this repository is not to build anything (crazy), tho
it contains many tools and points to other makefiles to build other things.

Think of the makefile here more of a phone-book to all the helpful scripts and
tools.

CONFIGURATION
===========
The Nexus has a lot of configuration involved, hence why there is a
menuconfig system thanks to the kconfiglib project. To get started configuring this
simply run:

    make menuconfig

Then you will see a TUI allowing to to modify the options. It is very similar to
the kernel config.

ALL TARGETS
===========

HELP TARGETS
------------
    help-targets    - Lists all targets
    help            - Prints this message
