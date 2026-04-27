; SPDX-License-Identifier: GPL-3.0
; return
;
; main.asm
;
; Really basic program that returns 0
;
; COPYRIGHT NOTICE
; Copyright (C) 2024-2026 0x4248 and contributors
; Redistribution and use in source and binary forms, with or without
; modification, are permitted provided that the license is not changed.
; 
; This software is free and open source. Licensed under the GNU general
; public license version 3.0 as published by the Free Software Foundation.


global _start 
section .text

main:
	mov rax, 60		; exit syscall
	mov rdi, 0		; Set 0 on return

	syscall 		; Call the kernel