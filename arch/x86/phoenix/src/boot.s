; SPDX-License-Identifier: GPL-3.0
; Phoenix
;
; Main bootloader code
;
; COPYRIGHT NOTICE
; Copyright (C) 2024-2026 0x4248 and phoenix contributors
; Redistribution and use in source and binary forms, with or without
; modification, are permitted provided that the license is not changed.
;
; This software is free and open source. Licensed under the GNU general
; public license version 3.0 as published by the Free Software Foundation.

org 0x7C00
bits 16

start:
    mov ah, 0x02      ; BIOS: Read floppy
    mov al, 25         ; number of sectors to read
    mov ch, 0         ; cylinder
    mov cl, 2         ; sector
    mov dh, 0         ; head
    mov dl, 0x00      ; floppy drive
    mov bx, 0x8000    ; memory address to load kernel
    int 0x13
    jc disk_error     ; if carry flag set, halt

    ; Far jump to kernel
    jmp 0x0000:0x8000

disk_error:
    hlt

; Pad to 510 bytes
times 510 - ($ - $$) db 0
dw 0xAA55
