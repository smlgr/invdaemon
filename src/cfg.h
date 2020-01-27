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


#ifndef __CFG_H
#define __CFG_H

#include <stdint.h>

struct cfg_t {
    int debug_level;
    int log_file_level;
    char *log_file;

    int inv_model;

    char *inv_tcp_addr;
    uint16_t inv_tcp_port;

    char *inv_serial_port;
    int inv_serial_speed;

    unsigned int inv_num;

    int lgr_interval;

    char *server_addr;
    uint16_t server_port;
    char *server_inv_name;
    char *server_inv_token;

    int queue_size;
};

typedef struct cfg_t cfg;

void cfg_init();

void cfg_free();

void cfg_print();

int cfg_parse(int, char **);

int cfg_file_parse(char *);

#endif
