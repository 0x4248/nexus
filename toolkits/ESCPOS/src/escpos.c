/* SPDX-License-Identifier: GPL-3.0
 * ESCPOS 4248 Print Standard 
 *
 * Main parser
 *
 * COPYRIGHT NOTICE
 * Copyright (C) 2026 0x4248, Sparky and contributors
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the license is not changed.
 *
 * This software is free and open source. Licensed under the GNU general
 * public license version 3.0 as published by the Free Software Foundation.
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>


#include "escpos_commands.h"

#define PRINTER_COLS 48

static int nl_enabled = 1;

/* ---- helpers ---- */

#define EMIT(cmd) do {                         \
    static const unsigned char _c[] = { cmd }; \
    fwrite(_c, 1, sizeof(_c), stdout);         \
} while (0)

static void hr(void)
{
    for (int i = 0; i < PRINTER_COLS; i++)
        putchar('-');
    putchar('\n');
}

static void box(int height)
{
    putchar('+');
    for (int i = 0; i < PRINTER_COLS - 2; i++)
        putchar('-');
    puts("+");

    for (int h = 0; h < height; h++) {
        putchar('|');
        for (int i = 0; i < PRINTER_COLS - 2; i++)
            putchar(' ');
        puts("|");
    }

    putchar('+');
    for (int i = 0; i < PRINTER_COLS - 2; i++)
        putchar('-');
    puts("+");
}

static void set_normal(void)
{
    EMIT(CMD_ALIGN_LEFT);
    EMIT(CMD_SIZE_NORMAL);
    EMIT(CMD_BOLD_OFF);
    EMIT(CMD_UNDERLINE_OFF);
    EMIT(CMD_INVERT_OFF);
    EMIT(CMD_EMPHASIS_OFF);
    EMIT(CMD_VERTICAL_OFF);
    EMIT(CMD_FONT_A);
}

/* ---- token handling ---- */

enum token_type {
    TOK_SIMPLE,
    TOK_ARG,
};

struct token {
    const char *name;
    enum token_type type;
    void (*handler)(const char *arg);
};


static void t_bold_on(const char *arg)    { (void)arg; EMIT(CMD_BOLD_ON); }
static void t_normal(const char *arg)    { (void)arg; set_normal(); }
static void t_center(const char *arg)    { (void)arg; EMIT(CMD_ALIGN_CENTER); }
static void t_left(const char *arg)      { (void)arg; EMIT(CMD_ALIGN_LEFT); }
static void t_wide(const char *arg)      { (void)arg; EMIT(CMD_SIZE_WIDE); }

static void t_nl_off(const char *arg)     { (void)arg; nl_enabled = 0; }
static void t_nl_on(const char *arg)      { (void)arg; nl_enabled = 1; }
static void t_nl(const char *arg)         { (void)arg; putchar('\n'); }

static void t_hr(const char *arg)         { (void)arg; hr(); }
static void t_cut(const char *arg)        { (void)arg; EMIT(CMD_CUT_SAFE); }


static void t_font(const char *arg)
{
    if (!arg) return;

    if (!strcmp(arg, "A"))
        EMIT(CMD_FONT_A);
    else if (!strcmp(arg, "B"))
        EMIT(CMD_FONT_B);
    else if (!strcmp(arg, "C"))
        EMIT(CMD_FONT_C);
}

static void t_invert(const char *arg)
{
    if (!arg) return;

    if (!strcmp(arg, "ON"))
        EMIT(CMD_INVERT_ON);
    else if (!strcmp(arg, "OFF"))
        EMIT(CMD_INVERT_OFF);
}

static void t_box(const char *arg)
{
    int h;

    if (!arg)
        return;

    h = atoi(arg);
    if (h <= 0 || h > 20)
        return;

    box(h);
}

static const struct token tokens[] = {
    { "BOLD",    TOK_SIMPLE, t_bold_on },
    { "NORMAL",  TOK_SIMPLE, t_normal },
    { "HR",      TOK_SIMPLE, t_hr },
    { "CUT",     TOK_SIMPLE, t_cut },
    { "CENTER",  TOK_SIMPLE, t_center },
    { "LEFT",    TOK_SIMPLE, t_left },
    { "WIDE",    TOK_SIMPLE, t_wide },
    { "NL_OFF",  TOK_SIMPLE, t_nl_off },
    { "NL_ON",   TOK_SIMPLE, t_nl_on },
    { "NL",      TOK_SIMPLE, t_nl },

    { "FONT",    TOK_ARG,    t_font },
    { "INVERT",  TOK_ARG,    t_invert },
    { "BOX",     TOK_ARG,    t_box },
};

static int try_token(const char *s, size_t *consumed)
{
    char name[16] = {0};
    char arg[16]  = {0};
    size_t i = 1, n = 0, a = 0;

    if (s[0] != '[')
        return 0;

    /* parse NAME */
    while (s[i] && s[i] != ' ' && s[i] != ']' && n < sizeof(name) - 1)
        name[n++] = s[i++];

    /* parse optional arg */
    if (s[i] == ' ') {
        i++;
        while (s[i] && s[i] != ']' && a < sizeof(arg) - 1)
            arg[a++] = s[i++];
    }

    if (s[i] != ']')
        return 0;

    i++; /* include ']' */

    for (size_t t = 0; t < sizeof(tokens)/sizeof(tokens[0]); t++) {
        if (strcmp(name, tokens[t].name) == 0) {
            tokens[t].handler(a ? arg : NULL);
            *consumed = i;
            return 1;
        }
    }

    return 0;
}

/* ---- main ---- */

int main(void)
{
    char buf[512];

    EMIT(CMD_INIT);
    set_normal();

    while (fgets(buf, sizeof(buf), stdin)) {
        size_t i = 0;

        while (buf[i]) {
            size_t consumed;

            if (buf[i] == '[' && try_token(&buf[i], &consumed)) {
                i += consumed;
                continue;
            }

            if (buf[i] == '\n') {
                if (nl_enabled)
                    putchar('\n');
                i++;
                continue;
            }

            putchar(buf[i++]);
        }
    }

    EMIT(CMD_BEEP(1, 100));
    return 0;
}
