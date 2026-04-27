; SPDX-License-Identifier: PROPRIETARY
; Lost version
;
; I seem to have found this on a random drive I had, I cant remember when I
; made it but it was copyrighted 2024.
;
; This version seems to have the very advanced blue screen colour syscall, fun.
;
; Copyright (C) 2024-2026 0x4248


[ORG 0x7c00]
[BITS 16]
jmp start ; Jump to start

start:
   ; Sertting up the stack
   xor ax, ax ; clear ax
   mov ds, ax ; set ds to 0
   cld        ; clear direction flag

   mov si, boot_msg ; set si to msg
   call bios_print ; call bios_print
   call blue_screen
   call halt_cpu

boot_msg db 'Booted!', 0, 13, 10

bios_print:
   lodsb          ; load byte from si to al
   or al, al      ; check if al is 0
   jz done        ; if al is 0, jump to done
   mov ah, 0x0E   ; set teletype output
   mov bh, 0      ; set page number
   int 0x10       ; call bios interrupt
   jmp bios_print ; jump to bios_print

done:
   ret

blue_screen:
   ; Before halting we set the screen to blue
   mov ax, 0x0600  ; BIOS function to scroll (clear) the screen
   mov ax, 0x0B00  ; BIOS function to set background color
   mov bh, 0x00    ; Page number
   mov bl, 0x01    ; Foreground: white (0xF), Background: blue (0x1)
   int 0x10        ; Call BIOS interrupt to set color


halt_cpu:
   hlt ; halt the CPU

times 510-($-$$) db 0 ; Fill the rest of the sector with 0
db 0x55 ; Boot signature
db 0xAA ; Boot signature
