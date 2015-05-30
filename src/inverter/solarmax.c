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


#include "solarmax.h"
#include "inverter.h"


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