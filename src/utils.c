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
#include <ctype.h>
#include <math.h>

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

void str_trim_doubles(char *dst, char *src, char c) {
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

void str_replace_char(char *dst, char *src, char from, char to) {
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


/**
 * Function used to convert an integer in a fixed-lenght hexadecimal string
 */

void int2hex(char *dst, unsigned int input, size_t dim) {
    char pattern[8];

    memset(dst, '\0', sizeof(dst));

    sprintf(pattern, "%%0%dX", (int) dim);
    sprintf(dst, pattern, input);
}


/**
 * Function to convert an HEX value to unsigned integer
 */

unsigned int hex2uint(char *input) {
    char values[] = "0123456789ABCDEF";
    unsigned int output;
    size_t ln;
    int i;
    int j;
    char c;
    char d;

    output = 0;
    ln = strlen(input) - 1;
    i = 0;

    do {
        c = (char) toupper(*(input + ln));

        j = 0;

        do {
            d = values[j];

            if (c == d) {
                output += pow(16, i) * j;
                break;
            }

            j++;
        } while (c != d);

        ln--;
        i++;
    } while (ln >= 0 && toupper(*(input + ln)) != 'X');

    return output;
}


/**
 * Function used to compute a standard 16-bit checksum
 */

__uint16_t checksum16(char *input) {
    unsigned int sum;

    sum = 0;

    while (*input != '\0') {
        sum += *input;
        sum %= (unsigned int) 0xFFFF;
        input++;
    }

    return (__uint16_t) sum;
}
