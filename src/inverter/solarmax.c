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
#include "../socket.h"
#include "../config.h"

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
    char tmp[SOCKET_TCP_BUFFER];

    memset(dst, '\0', sizeof(dst));
    memset(tmp, '\0', sizeof(tmp));

    ln = strlen(SOLARMAX_QUERY);

    int2hex(inverter_num, conf->inv_num, 2);
    int2hex(len, (unsigned int) (13 + ln + 6), 2);

    sprintf(tmp, "%s;%s;%s|64:%s|", SOLARMAX_ID_FROM, inverter_num, len, SOLARMAX_QUERY);

    checksum = checksum16(tmp);
    int2hex(cs, checksum, 4);

    sprintf(dst, "{%s%s}", tmp, cs);

    ui_message(UI_DEBUG, "INVERTER-SOLARMAX", "Inverter query: %s", dst);
}


/**
 * Function used prepare the string to send to SolarMax inverter
 */

void solarmax_response_parse(invdata *data, char *response) {
    char param[9];
    char value[17];
    char *p;
    char *v;
    int mode;

    if (strlen(response) < 19)
        return;

    memset(param, '\0', sizeof(param));
    memset(value, '\0', sizeof(value));
    response += 13;
    mode = 0;
    p = param;
    v = value;

    while (*response != '|') {
        if (*response == '=') {
            mode = 1;
        } else if (*response == ';') {
            ui_message(UI_DEBUG, "INVERTER-SOLARMAX", "%s = %s", param, value);

            if (strcmp(param, "PAC") == 0)
                data->ac_power = (int) strtol(value, NULL, 16) * 5;

            if (strcmp(param, "UL1") == 0)
                data->ac_voltage = (int) strtol(value, NULL, 16);

            if (strcmp(param, "IL1") == 0)
                data->ac_current = (int) strtol(value, NULL, 16);

            if (strcmp(param, "TNF") == 0)
                data->ac_frequency = (int) strtol(value, NULL, 16);

            if (strcmp(param, "UDC") == 0)
                data->dc1_voltage = (int) strtol(value, NULL, 16);

            if (strcmp(param, "IDC") == 0)
                data->dc1_current = (int) strtol(value, NULL, 16);

            if (strcmp(param, "TKK") == 0)
                data->temperature = (int) strtol(value, NULL, 16) * 10;

            if (strcmp(param, "KDY") == 0)
                data->production = (int) strtol(value, NULL, 16);

            memset(param, '\0', sizeof(param));
            memset(value, '\0', sizeof(value));
            p = param;
            v = value;
            mode = 0;
        } else {
            if (mode == 0) {
                *p = *response;
                p++;
            } else {
                *v = *response;
                v++;
            }
        }
        response++;
    }

    data->valid = 0;
}


/**
 * SolarMax Inverter query function
 */

void inv_query_solarmax_s3000_tcp(invdata *data) {
    char query[SOCKET_TCP_BUFFER];
    char *inv_resp;

    solarmax_query_prepare(query);

    inv_resp = socket_query(conf->inv_tcp_addr, conf->inv_tcp_port, query);

    if (inv_resp == NULL)
        return;

    solarmax_response_parse(data, inv_resp);

    free(inv_resp);
}
