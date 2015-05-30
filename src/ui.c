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
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>

#include "config.h"
#include "ui.h"
#include "cfg.h"

extern cfg *conf;
extern const char *smlgr_program_name;


/**
 * Usage information message
 */

void uiUsage() {
    fprintf(stderr, "Usage:\n");
    fprintf(stderr, "    %s [<option> <value> ...]\n", smlgr_program_name);
}


/**
 * Version information
 */

void uiVersion() {
    fprintf(stderr, "SMLGR Inverter Daemon\n");
}


/**
 * Help information message with all command line options
 */

void uiHelp() {
    fprintf(stderr, "SMLGR Inverter Daemon\n");
    fprintf(stderr, "\n");

    uiUsage();

    fprintf(stderr, "\n");
    fprintf(stderr, "Options list with default values:\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "    -c | --config            Config file (overwrite all other options)\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "    -h | --help              This message\n");
    fprintf(stderr, "    -V | --version           Print Version and exit\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "    -q | --quiet             Disable output (debug level 0)\n");
    fprintf(stderr, "    -v | --verbose           Verbose output (debug level 4)\n");
    fprintf(stderr, "    -d | --debug             Debug level (%d)\n", DEFAULT_UI_DEBUG_LEVEL);
    fprintf(stderr, "                             0 DISABLE (quiet)\n");
    fprintf(stderr, "                             1 ERROR\n");
    fprintf(stderr, "                             2 WARNING\n");
    fprintf(stderr, "                             3 INFO\n");
    fprintf(stderr, "                             4 DEBUG (verbose)\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "    -m | --inv-model         Inverter Model (%d)\n", DEFAULT_INVERTER_MODEL);
    fprintf(stderr, "                             1. SolarMax S3000 TCP\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "    -a | --inv-tcp-addr      Inverter (via TCP) IP Address (%s)\n", DEFAULT_INVERTER_TCP_IP_ADDR);
    fprintf(stderr, "    -p | --inv-tcp-port      Inverter (via TCP) TCP Port (%d)\n", DEFAULT_INVERTER_TCP_IP_PORT);
    fprintf(stderr, "\n");
    fprintf(stderr, "    -e | --inv-serial-port   Inverter (via Serial Port) Port Name (%s)\n",
            DEFAULT_INVERTER_SERIAL_PORT);
    fprintf(stderr, "    -b | --inv-serial-speed  Inverter (via Serial Port) TCP Port (%d)\n",
            DEFAULT_INVERTER_SERIAL_SPEED);
    fprintf(stderr, "\n");
    fprintf(stderr, "    -n | --inv-num           Inverter number (%d)\n", DEFAULT_INVERTER_NUM);
    fprintf(stderr, "\n");
    fprintf(stderr, "    -i | --interval          Logger interval in seconds (%d)\n", DEFAULT_LGR_INTERVAL);
    fprintf(stderr, "\n");
    fprintf(stderr, "    -s | --server-addr       Stat Server IP Address (%s)\n", DEFAULT_SERVER_ADDR);
    fprintf(stderr, "    -r | --server-port       Stat Server TCP port (%d)\n", DEFAULT_SERVER_PORT);
    fprintf(stderr, "    -u | --server-inv-id     Stat Server Inverter ID (%ld)\n", DEFAULT_SERVER_INVERTER_ID);
    fprintf(stderr, "    -t | --server-inv-token  Stat Server Inverter Token (%s)\n", DEFAULT_SERVER_INVERTER_TOKEN);
    fprintf(stderr, "\n");
    fprintf(stderr, "All outputs are on Standard Error.\n");
}


/**
 * Debugging function
 * @param[in] level Level of the messages
 * @param[in] text Text message
 */

void ui_message(int level, char *input, ...) {
    va_list args;
    char datetime[20];
    time_t rawtime;
    struct tm *timeinfo;

    if (level <= conf->debug_level) {
        rawtime = time(NULL);
        timeinfo = localtime(&rawtime);
        strftime(datetime, 20, "%Y-%m-%d %H:%M:%S", timeinfo);

        va_start(args, input);

        if (level == UI_ERROR)
            fprintf(stderr, "%s [ERROR] ", datetime);
        if (level == UI_WARNING)
            fprintf(stderr, "%s [WARN]  ", datetime);
        if (level == UI_INFO)
            fprintf(stderr, "%s [INFO]  ", datetime);
        if (level == UI_DEBUG)
            fprintf(stderr, "%s [DEBUG] ", datetime);

        vfprintf(stderr, input, args);
        fprintf(stderr, "\n");

        va_end(args);
    }
}


/**
 * Error message function
 * @param[in] text Generic message
 * @param[in] errnum Error code
 * @param[in] errtext Error text
 */

void uiError(char *text, int errnum, char *errtext) {
    ui_message(UI_ERROR, "%s (%d): %s", text, errnum, errtext);
}
