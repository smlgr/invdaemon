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


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "solarmax.h"
#include "../utils.h"
#include "../cfg.h"
#include "../ui.h"

extern cfg *conf;


/**
 * Function used prepare the string to send to SolarMax inverter
 */

void solarmax_query_prepare(char *dst) {
    size_t ln;
    char inverter_num[3];
    char len[3];
    __uint16_t checksum;
    char cs[5];
    char tmp[1025];

    memset(dst, '\0', sizeof(dst));
    memset(tmp, '\0', sizeof(tmp));

    ln = strlen(SOLARMAX_QUERY);

    int2hex(inverter_num, conf->inv_num, 2);
    int2hex(len, (unsigned int) (13 + ln + 6 + 1), 2);

    sprintf(tmp, "%s;%s;%s|64:%s|", SOLARMAX_ID_FROM, inverter_num, len, SOLARMAX_QUERY);

    checksum = checksum16(tmp);
    int2hex(cs, checksum, 4);

    sprintf(dst, "{%s%s}", tmp, cs);

    ui_message(UI_DEBUG, "Inverter query: %s", dst);
}


/**
 * Function used prepare the string to send to SolarMax inverter
 */

void solarmax_response_parse(invdata *data, char *response) {
    char c;
    char param[9];
    char value[17];
    char *p;
    char *v;
    int mode;

    memset(param, '\0', sizeof(param));
    memset(value, '\0', sizeof(value));
    response += 13;
    mode = 0;
    p = param;
    v = value;

    while ((c = *response) != (char) "|") {
        if (c == '=') {
            mode = 1;
        } else if (c == ';') {
            ui_message(UI_DEBUG, "%s = %s (%d)", param, value, atoi(value));

            if (strcmp(param, "PAC") == 0)
                data->ac_power = atoi(value) * 5;

            if (strcmp(param, "UL1") == 0)
                data->ac_voltage = atoi(value);

            if (strcmp(param, "IL1") == 0)
                data->ac_current = atoi(value);

            if (strcmp(param, "TNF") == 0)
                data->ac_frequency = atoi(value);

            if (strcmp(param, "UDC") == 0)
                data->dc1_voltage = atoi(value);

            if (strcmp(param, "IDC") == 0)
                data->dc1_current = atoi(value);

            if (strcmp(param, "TKK") == 0)
                data->temperature = atoi(value);

            if (strcmp(param, "KDY") == 0)
                data->production = atoi(value);

            memset(param, '\0', sizeof(param));
            memset(value, '\0', sizeof(value));
            p = param;
            v = value;
            mode = 0;
        } else {
            if (mode == 0) {
                *p = c;
                p++;
            } else {
                *v = c;
                v++;
            }
        }
        response++;
    }

    data->valid = 1;
}


/**
 * SolarMax Inverter query function
 */

void inv_query_solarmax_s3000_tcp(invdata *data) {
    data->ac_power = 10000;
    data->ac_voltage = 2350;
    data->ac_current = 42;
    data->ac_frequency = 5000;

    data->dc1_voltage = 5000;
    data->dc1_current = 20;
    data->dc2_voltage = 0;
    data->dc2_current = 0;

    data->temperature = 450;
    data->production = 1234;

    data->valid = 0;
}