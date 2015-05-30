/*
 *   SolarMax S3000 DB logger
 *   Copyright (C) 2013  Luca Cireddu
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


#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "invdaemon.h"
#include "ui.h"
#include "cfg.h"

char *smlgr_program_name;
cfg *conf;


int main(int argc, char **argv) {
    smlgr_program_name = argv[0];

    cfgInit();

    if (cfgParse(argc, argv)) {
        cfgPrint();
        invdaemon();
    }

    cfgFree();

    return 0;
}


/**
 * Main program function
 */

void invdaemon() {
    pthread_t pth;
    pthread_attr_t attr;
    int running = 0;

    while (1) {
        uiMessage(UI_DEBUG, "Interval");

        if (running == 0) {
            uiMessage(UI_DEBUG, "Running new query thread");
            pthread_attr_init(&attr);
            pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
            pthread_create(&pth, &attr, query_thread, (void *) &running);
        }

        sleep(conf->lgr_interval);
    }
}


/**
 * Main function for thread
 */

void *query_thread(void *args) {
    int *running = (int *) args;
    *running = 1;

    // Query operations

    *running = 0;
    return NULL;
}
