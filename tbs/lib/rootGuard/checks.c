/* SPDX-License-Identifier: GPL-3.0
 * Root Guard
 *
 * checks.c - Root Guard Checks Implementation
 * 
 * COPYRIGHT NOTICE
 * Copyright (C) 2025-2026 0x4248 and contributors
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the license is not changed.
 *
 * This software is free and open source. Licensed under the GNU general
 * public license version 3.0 as published by the Free Software Foundation.
*/


#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <dirent.h>

#include "checks.h"



int root_uuid_check(){
    if (getuid() != 0) {
        return RG_TF;
    }
    return RG_TP;
}

int shadow_file_check(){
    if (faccessat(AT_FDCWD, "/etc/shadow", R_OK, AT_EACCESS) == 0) return RG_TP;
    return RG_TF;
}

int root_dir_check(){
    DIR *dir = opendir("/root");
    if (dir) {
        closedir(dir);
        return RG_TP;
    }
    return RG_TF;
}