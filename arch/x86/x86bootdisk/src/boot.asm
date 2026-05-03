; SPDX-License-Identifier: GPL-3.0
; x86bootdisk
;
; boot.asm
;
; COPYRIGHT NOTICE
; Copyright (C) 2024-2026 0x4248 and contributors
; Redistribution and use in source and binary forms, with or without
; modification, are permitted provided that the license is not changed.
; 
; This software is free and open source. Licensed under the GNU general
; public license version 3.0 as published by the Free Software Foundation.
 
[ORG 0x7c00]
[BITS 16]
jmp start               ; Jump to start

start:  
    ; Sertting up the stack
    xor ax, ax           ; clear ax
    mov ds, ax           ; set ds to 0
    cld                  ; clear direction flag
   
    mov si, boot_msg     ; set si to msg
    call bios_print      ; call bios_print
    jmp bios_print       ; jump to bios_print
    hlt                  ; halt the system



bios_print:
    lodsb                ; load byte from si to al
    or al, al            ; check if al is 0
    jz done              ; if al is 0, jump to done
    mov ah, 0x0E          ; set teletype output
    mov bh, 0            ; set page number
   
    int 0x10             ; call bios interrupt
    jmp bios_print       ; jump to bios_print

done:
    ret

boot_msg db 'Booted!', 0

times 510-($-$$) db 0   ; Fill the rest of the sector with 0
db 0x55                 ; Boot signature
db 0xaa                 ; Boot signature
