; SPDX-License-Identifier: GPL-3.0
; Phoenix
;
; Kernel entry point
;
; COPYRIGHT NOTICE
; Copyright (C) 2024-2026 0x4248 and phoenix contributors
; Redistribution and use in source and binary forms, with or without
; modification, are permitted provided that the license is not changed.
;
; This software is free and open source. Licensed under the GNU general
; public license version 3.0 as published by the Free Software Foundation.

bits 16

start:
    cli
    xor ax, ax
    mov ss, ax
    mov sp, 0x9000
    mov ax, 0x0800
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    sti
    jmp main

%include "src/lib/print.s"
%include "src/lib/colorscreen.s"
%include "src/lib/keyboard.s"

reboot:
    mov ah, 0
    mov al, 3
    int 0x10
    int 0x19
    hlt

; --- Main ---
main:
    mov si, msg0
    call print_string
    call new_line
    mov si, msg1
    call print_string
    call new_line

    mov ah, 0x0E
    mov al, 0x20

    int 0x10

.loop:
    mov si, msg2
    call print_string
    call wait_key
    cmp al, 'r'
    je reboot
    cmp al, 'b'
    je bluesc
    call .loop


msg0 db "Phoenix OS, System running!", 0
msg1 db "Copyright (C) 2024-2026 0x4248 GNU GPL v3", 0
msg2 db "System running, press r to reboot", 0
