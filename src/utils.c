/*
 *   SMLGR Inverter Daemon
 *   Copyright (C) 2015  Luca Cireddu
 *   sardylan@gmail.com
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License along
 *   with this program; if not, write to the Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */


#include <malloc.h>
#include <string.h>

#include "utils.h"

/**
 * Removing CR and LF at the end of a string
 */
void strtrmcrlf(char *dst, char *src) {
    size_t ln;

    strcpy(dst, src);

    ln = strlen(dst);

    if (*(dst + (ln - 1)) == '\r' || *(dst + (ln - 1)) == '\n')
        *(dst + (ln - 1)) = '\0';

    if (*(dst + (ln - 2)) == '\r' || *(dst + (ln - 2)) == '\n')
        *(dst + (ln - 2)) = '\0';
}


/**
 * Remove double chars from a string
 */

void strtrmdbl(char *dst, char *src, char c) {
    while (*src != '\0') {
        *dst = *src;

        if (*src == c && *(src + 1) == *src)
            src++;

        src++;
        dst++;
    }

    *dst = '\0';
}


/**
 * Replace char in string
 */

void strrplc(char *dst, char *src, char from, char to) {
    while (*src != '\0') {
        if (*src == from)
            *dst = to;
        else
            *dst = *src;

        dst++;
        src++;
    }

    *dst = '\0';
}
