; SPDX-License-Identifier: GPL-3.0
; Phoenix
;
; lib/colorscreen.s
; Screen colouring
;
; COPYRIGHT NOTICE
; Copyright (C) 2024-2026 0x4248 and phoenix contributors
; Redistribution and use in source and binary forms, with or without
; modification, are permitted provided that the license is not changed.
;
; This software is free and open source. Licensed under the GNU general
; public license version 3.0 as published by the Free Software Foundation.

bluesc:
    mov ax, 0x0600
    mov ax, 0x0B00
    mov bh, 0x00
    mov bl, 0x01
    int 0x10
