# SPDX-License-Identifier: GPL-3.0
# ESC/POS Image experiment
#
# receipt.py
# A simple script to print text and images on an ESC/POS compatible printer.
# 
# Authored: sparkydadoggo <188922951+sparkydadoggo@users.noreply.github.com>
# Co-Authored: 0x4248 <60709927+0x4248@users.noreply.github.com>
# Reviewed by: 0x4248 <60709927+0x4248@users.noreply.github.com>
#
# TESTED HARDWARE:
# - TOSHIBA ???? (I dont know what the actual model is, there is so manty numbers on the printer but 
# all look like serial numbers, has ESC/POS support, "images" as we saw and serial over standard 2.0 USB, RS232)
#
# COPYRIGHT NOTICE
# Copyright (C) 2025-2026 0x4248 and contributors
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the license is not changed.
#
# This software is free and open source. Licensed under the GNU general
# public license version 3.0 as published by the Free Software Foundation.
#
# NOTE:
# made with Sparky's own 2 paws

from PIL import Image
from escpos import *
p = printer.Usb(0x08a6, 0x003d, profile ="default")

while True:
    usertext = input("Enter text to print, enter 'cut' to cut the paper, enter 'exit' to quit: ")
    if usertext.lower() == 'exit':
        break
    elif usertext.lower() == 'cut':
        for i in range(3):
            p.text("\n")
        p.cut()
    elif usertext.lower() == 'image':
        # XXX: If the image size is wrong or too large, expect an entire roll of paper to be lost. 
        # You will get an output of "random characters" and the printer will just keep printing. We
        # assume that its just the printer printing the raw data of the image, but we dont know for 
        # sure.
        p.profile.profile_data['media']['width']['pixels'] = 576  # or 384, test at your own risk 
        img = Image.open(input("Enter the path to the image you want to print: "))
        # HACK: This is the weirdest part of the code, but it works. The printer can only print 
        # images that are 576 pixels wide (or 384 for some models), so we need to resize the image 
        # to fit the printer's width while maintaining the aspect ratio.
        #
        # We really should figure out if there is a better way to do this e.g detecting the 
        # printer's width and resizing the image accordingly, but for now this is a simple solution 
        # that "should" work with most images. 
        printer_width = 576
        aspect_ratio = img.height / img.width
        new_height = int(printer_width * aspect_ratio)
        img = img.resize((printer_width, new_height))
        img = img.convert("1")
        p.image(img, impl="bitImageColumn")
        for i in range(3):
            p.text("\n")
        p.cut()
    else:
        p.text(usertext + "\n")