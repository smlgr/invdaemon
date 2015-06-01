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


#ifndef __UTILS_H
#define __UTILS_H

void strtrmcrlf(char *dst, char *src);

void str_trim_doubles(char *dst, char *src, char c);

void str_replace_char(char *dst, char *src, char from, char to);

void int2hex(char *dst, unsigned int input, size_t dim);

unsigned int hex2uint(char *input);

__uint16_t checksum16(char *input);

#endif
