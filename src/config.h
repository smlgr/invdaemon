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


#ifndef __CONFIG_H
#define __CONFIG_H

#define DEFAULT_UI_DEBUG_LEVEL 4

#define DEFAULT_INVERTER_MODEL 1

#define DEFAULT_INVERTER_TCP_IP_ADDR "192.168.0.2"
#define DEFAULT_INVERTER_TCP_IP_PORT 12345

#define DEFAULT_INVERTER_SERIAL_PORT "/dev/ttyS0"
#define DEFAULT_INVERTER_SERIAL_SPEED 9600

#define DEFAULT_INVERTER_NUM 1

#define DEFAULT_LGR_INTERVAL 5

#define DEFAULT_SERVER_ADDR "localhost"
#define DEFAULT_SERVER_PORT 8080
#define DEFAULT_SERVER_INVERTER_ID 1l
#define DEFAULT_SERVER_INVERTER_TOKEN "0123456789abcdef"

#define SOCKET_TCP_BUFFER 8193
#define RESPONSE_ROW_MAX_LENGTH 8193
#define URL_MAX_LENGTH 257
#define UI_MESSAGES_OUTPUT stderr
#define UI_MESSAGES_MAX_LENGTH 60

#define SERVER_PATH "/smlgr/api/inverter/%ld/payload"
#define SERVER_QUERY_STRING ""
#define SERVER_METHOD "POST"
#define SERVER_CONTENT_TYPE "application/json"
#define SERVER_DATA "{\"token\":\"%s\",\"data\":{\"ac_power\":%d,\"ac_voltage\":%d,\"ac_current\":%d,\"ac_frequency\":%d,\"dc1_voltage\":%d,\"dc1_current\":%d,\"dc2_voltage\":%d,\"dc2_current\":%d,\"temperature\":%d,\"production\":%d}}"

#endif
