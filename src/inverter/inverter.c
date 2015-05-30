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

#include "inverter.h"
#include "solarmax.h"
#include "../cfg.h"

extern cfg *conf;

/**
 * Inverter data init
 */

invdata *inv_init() {
    invdata *ret;

    ret = (invdata *) malloc(sizeof(invdata));

    ret->valid = -1;

    ret->ac_power = 0;
    ret->ac_voltage = 0;
    ret->ac_current = 0;
    ret->ac_frequency = 0;

    ret->dc1_voltage = 0;
    ret->dc1_current = 0;
    ret->dc2_voltage = 0;
    ret->dc2_current = 0;

    ret->temperature = 0;
    ret->production = 0;

    return ret;
}


/**
 * Inverter data free
 */

void inv_free(invdata *data) {
    free(data);
}


/**
 * Inverter call
 */

void inv_query(invdata *data) {
    if (conf->inv_model == INVERTER_MODEL_SOLARMAX)
        inv_query_solarmax_s3000_tcp(data);
}
