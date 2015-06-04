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

#include "log.h"
#include "cfg.h"

extern cfg *conf;

void log_restart() {
    FILE *logfd;

    if (conf->log_file_level > 0) {
        logfd = fopen(conf->log_file, "a");
        fprintf(logfd, "---LOG RESTART---\n");
        fclose(logfd);
    }
}

void log_file_message(int level, char *prefix, char *row) {
    FILE *logfd;

    if (level <= conf->log_file_level) {
        logfd = fopen(conf->log_file, "a");
        fprintf(logfd, "%s%s\n", prefix, row);
        fclose(logfd);
    }
}
