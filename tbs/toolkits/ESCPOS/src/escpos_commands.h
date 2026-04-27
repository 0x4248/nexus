/* SPDX-License-Identifier: GPL-3.0
 * ESCPOS X42 Print Standard 
 *
 * ESC / POS Command Definitions
 *
 * COPYRIGHT NOTICE
 * Copyright (C) 2026 0x4248, Sparky and contributors
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the license is not changed.
 *
 * This software is free and open source. Licensed under the GNU general
 * public license version 3.0 as published by the Free Software Foundation.
*/


#ifndef ESCPOS_COMMANDS_H
#define ESCPOS_COMMANDS_H

/* ASCII control chars */
#define CHR_NUL  0x00
#define CHR_LF   0x0A

/* ESC/POS prefixes */
#define CHR_ESC  0x1B
#define CHR_GS   0x1D

/* ---- Printer control ---- */

/* Initialize */
#define CMD_INIT           CHR_ESC, '@'

/* Line feed */
#define CMD_LF             CHR_LF

/* ---- Text formatting ---- */

/* Bold */
#define CMD_BOLD_ON        CHR_ESC, 'E', 0x01
#define CMD_BOLD_OFF       CHR_ESC, 'E', 0x00

/* Underline */
#define CMD_UNDERLINE_ON   CHR_ESC, '-', 0x01
#define CMD_UNDERLINE_OFF  CHR_ESC, '-', 0x00

/* Alignment */
#define CMD_ALIGN_LEFT     CHR_ESC, 'a', 0x00
#define CMD_ALIGN_CENTER   CHR_ESC, 'a', 0x01
#define CMD_ALIGN_RIGHT    CHR_ESC, 'a', 0x02

/* Character size (GS ! n) */
#define CMD_SIZE_NORMAL    CHR_GS, '!', 0x00
#define CMD_SIZE_WIDE      CHR_GS, '!', 0x11  /* 2x width + height */

/* ---- Font selection ---- */
#define CMD_FONT_A         CHR_ESC, 'M', 0x00
#define CMD_FONT_B         CHR_ESC, 'M', 0x01
#define CMD_FONT_C         CHR_ESC, 'M', 0x02

#define CMD_INVERT_ON      CHR_GS, 'B', 0x01
#define CMD_INVERT_OFF     CHR_GS, 'B', 0x00

#define CMD_EMPHASIS_ON    CHR_ESC, 'G', 0x01
#define CMD_EMPHASIS_OFF   CHR_ESC, 'G', 0x00

#define CMD_VERTICAL_ON    CHR_ESC, 'V', 0x01
#define CMD_VERTICAL_OFF   CHR_ESC, 'V', 0x00

/* ---- Paper handling ---- */

/* Feed n lines */
#define CMD_FEED(n)        CHR_ESC, 'd', (n)

/* Cut paper */
#define CMD_CUT_FULL       CHR_GS, 'V', 0x00
#define CMD_CUT_PARTIAL    CHR_GS, 'V', 0x01
#define CMD_CUT_SAFE       CHR_GS, 'V', 'B', 0x00

/* ---- Beeper ---- */
#define CMD_BEEP(n, t)     CHR_ESC, 'B', (n), (t)



#endif /* ESCPOS_COMMANDS_H */
