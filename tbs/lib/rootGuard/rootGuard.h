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


enum rootGaurd_status
{
    RG_OK = 0,
    RG_SEMI_PASS = 9,
    RG_VIOLATION = -1
};


/** rootGaurd_check
 * Internal function to perform root guard checks, works same as 
 * rootGaurd_perform_checks but without output control
 */
int rootGaurd_check();

/** rootGaurd_perform_checks
 * @brief Perform root guard checks
 * 
 * @param silent 1 to suppress output, 0 to allow output
 * @return int 0 on pass, 9 on semi-pass, -1 on violation
 */
int rootGaurd_perform_checks(int silent);