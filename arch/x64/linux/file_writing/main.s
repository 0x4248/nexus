; SPDX-License-Identifier: GPL-3.0
; file_writing
;
; main.asm
;
; Hello world in assembly for x86_64 Linux
;
; Many thanks to https://syscall.sh/, Linus Torvalds and kernel developers.
;
; COPYRIGHT NOTICE
; Copyright (C) 2024-2026 0x4248 and contributors
; Redistribution and use in source and binary forms, with or without
; modification, are permitted provided that the license is not changed.
; 
; This software is free and open source. Licensed under the GNU general
; public license version 3.0 as published by the Free Software Foundation.

; TODO: Still a work in progress.

; Syscalls:
; 2 - open
; 1 - write

global _start 
section .text

_start:
	mov rax, 2			; open syscall
	mov rdi, filename	; filename
	mov rsi, 1			; O_WRONLY

	syscall 			; Call the kernel

	mov rdi, filename		; File descriptor
	mov rax, 1			; write syscall
	mov rsi, message	; Message

	syscall 			; Call the kernel

	mov rax, 60			; exit syscall
	mov rdi, 0			; Set 0 on return

	syscall 			; Call the kernel

filename db 'file.txt', 0
message db 'Hello, world!', 0