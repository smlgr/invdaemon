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
#include <time.h>
#include <string.h>

#include "inverter/inverter.h"
#include "http/response.h"
#include "http/http.h"

#include "cfg.h"
#include "ui.h"
#include "config.h"

extern cfg *conf;


/*
 * Send data to server with timestamp
 */

int server_send_ts(invdata *data, time_t timestamp) {
    char *raw;
    response *res;
    char server_url[URL_MAX_LENGTH];
    char server_data[SOCKET_TCP_BUFFER];
    int code;

    strcpy(server_url, SERVER_PATH);

    sprintf(server_data, SERVER_DATA,
            (long long) timestamp,
            conf->server_inv_token,
            data->ac_power,
            data->ac_voltage,
            data->ac_current,
            data->ac_frequency,
            data->dc1_voltage,
            data->dc1_current,
            data->dc2_voltage,
            data->dc2_current,
            data->temperature,
            data->production);

    raw = http_call(conf->server_addr, conf->server_port, server_url,
                    SERVER_METHOD, SERVER_QUERY_STRING, SERVER_CONTENT_TYPE,
                    server_data);

    if (raw == NULL)
        return -1;

    res = res_init();
    res_parser(res, raw);
    free(raw);
    code = res->code;
    res_free(res);

    ui_message(UI_INFO, "SERVER", "Response code: %d", code);

    if (code == 200)
        return 0;

    return -1;
}


/*
 * Send data to server without timestamp
 */

int server_send(invdata *data) {
    return server_send_ts(data, 0);
}