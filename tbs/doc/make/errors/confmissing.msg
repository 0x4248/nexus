ERROR
=====

You are missing your .config, which is needed for the makefile system.

How can I make the config?
First lets get the submodules using git:

    git submodule update --init --recursive

Then generate the config using our state of the art menuconfig:

    make menuconfig

Dont worry, the makefile is smart and knows you want to do this to generate the
config. So I wont tell you again, unless you dont do what I say!
