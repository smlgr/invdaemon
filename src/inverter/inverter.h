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


#ifndef __INVERTER_H
#define __INVERTER_H

#define INVERTER_MODEL_SOLARMAX 1

struct invdata_t {
    int valid;

    int ac_power;
    int ac_voltage;
    int ac_current;
    int ac_frequency;

    int dc1_voltage;
    int dc1_current;
    int dc2_voltage;
    int dc2_current;

    int temperature;
    int production;
};

typedef struct invdata_t invdata;

invdata *inv_init();

void inv_free(invdata *data);

void inv_query(invdata *data);

#endif
