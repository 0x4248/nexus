/* SPDX-License-Identifier: GPL-3.0
 * Root Guard
 *
 * Root Guard is a simple library to quickly inject a root user check into 
 * C/C++ sbin programs. It does several checks to determine if the program
 * is being run by the root user or even by a user with root privileges.
 * 
 * Since just checking the UID isnt always enough, Root Guard performs
 * multiple checks including:
 * - The UID check
 * - Checking access to /etc/shadow
 * - Checking access to the root directory
 * - I plan on adding /proc checks in the future
 *
 * COPYRIGHT NOTICE
 * Copyright (C) 2025-2026 0x4248 and contributors
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the license is not changed.
 *
 * This software is free and open source. Licensed under the GNU general
 * public license version 3.0 as published by the Free Software Foundation.
*/

#include "rootGuard.h"
#include "checks.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>

int rootGaurd_check() {
        int passing_checks = 0;
    int checks_total = 2; /* Excludes the UUID checks */

    if (root_uuid_check() == RG_TP) {
        /* Why stop here? Well here we assume that you are just root and no 
         * other checks are needed here 
        */
        return RG_OK;
    }

    if (shadow_file_check() == RG_TP) {
        passing_checks++;
    }

    if (root_dir_check() == RG_TP) {
        passing_checks++;
    }

    if (passing_checks == checks_total) {
        /* All checks indicate root access, we can pass this on to the calling
        program as a okay to handle */
        return RG_OK;
    }
    else if (passing_checks > 0) {
        /* Looks like they have root privileges or some kind of root access, we 
        are going to semi pass this time to that the calling program can 
        decide what to do */
        return RG_SEMI_PASS;
    }
    return RG_VIOLATION;
}

int rootGaurd_perform_checks(int silent)
{

    int ret = rootGaurd_check();
    if (ret == RG_OK) {
        return RG_OK;
    }
    else if (ret == RG_SEMI_PASS) {
        return RG_SEMI_PASS;
    }

    else if (ret == RG_VIOLATION) {
        if (!silent){
            printf("[ rootGuard ] RootGuard has performed several checks and found no\n");
            printf("               indication of root access. Please run this program\n");
            printf("               as root or with elevated privileges.\n");
        }
 
        return RG_VIOLATION;
    }
}

int main(int argc, char *argv[]){
    int ret;
    ret = rootGaurd_perform_checks(0);
    if (ret != RG_OK){
        return -1;
    } else {
        // So we passed, now run what was in the argv 

        if (argc < 2){
            if (!ret){
                printf("No command provided to run as root.\n");
            }
            return -1;
        }
        // build command
        char command[1024] = {0};
        for (int i = 1; i < argc; i++){
            strcat(command, argv[i]);
            if (i != argc -1){
                strcat(command, " ");
            }
        }
        pid_t pid = fork();
        if (pid == 0) {
            /* child: exec the requested command */
            execvp(argv[1], &argv[1]);
            perror("execvp"); /* only reached on error */
            _exit(127);
        } else if (pid > 0) {
            /* parent: wait for child and return child's exit status */
            int status;
            if (waitpid(pid, &status, 0) == -1) {
                perror("waitpid");
                return -1;
            }
            if (WIFEXITED(status)) return WEXITSTATUS(status);
            return -1;
        } else {
            perror("fork");
            return -1;
        }
    }
}