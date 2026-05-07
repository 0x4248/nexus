; SPDX-License-Identifier: GPL-3.0
; Phoenix
;
; lib/power.s
; Main power management library
;
; COPYRIGHT NOTICE
; Copyright (C) 2024-2026 0x4248 and phoenix contributors
; Redistribution and use in source and binary forms, with or without
; modification, are permitted provided that the license is not changed.
;
; This software is free and open source. Licensed under the GNU general
; public license version 3.0 as published by the Free Software Foundation.


reboot:
    mov ah, 0
    mov al, 3
    int 0x10
    int 0x19
    hlt