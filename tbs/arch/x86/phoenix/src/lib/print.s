; SPDX-License-Identifier: GPL-3.0
; Phoenix
;
; lib/print.s
; Main printing libary
;
; COPYRIGHT NOTICE
; Copyright (C) 2024-2026 0x4248 and phoenix contributors
; Redistribution and use in source and binary forms, with or without
; modification, are permitted provided that the license is not changed.
;
; This software is free and open source. Licensed under the GNU general
; public license version 3.0 as published by the Free Software Foundation.


print_string:
    .loop:
        lodsb
        or al, al
        jz .done
        mov ah, 0x0E
        int 0x10
        jmp .loop
    .done:
        ret

new_line:
    mov ah, 0x0E
    mov al, 0x0A
    int 0x10
    mov al, 0x0D
    int 0x10
    ret
