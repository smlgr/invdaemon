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
#include <stdint.h>
#include <signal.h>
#include <string.h>

#include <http.h>
#include <response.h>

#include "invdaemon.h"
#include "ui.h"
#include "cfg.h"
#include "utils.h"

char *smlgr_program_name;
cfg *conf;

static volatile int keep_running = 1;

int main(int argc, char **argv) {
    smlgr_program_name = argv[0];

    signal(SIGINT, signal_handler);

    cfgInit();

    if (cfgParse(argc, argv)) {
        cfgPrint();
        invdaemon();
    }

    cfgFree();

    return 0;
}


/**
 * Signal handler
 */

void signal_handler(int signal) {
    keep_running = 0;
}


/**
 * Main program function
 */

void invdaemon() {
    pthread_t pth;
    pthread_attr_t attr;
    int running = 0;

    while (keep_running == 1) {
        uiMessage(UI_DEBUG, "Interval");

        if (running == 0) {
            uiMessage(UI_DEBUG, "Running new query thread");
            pthread_attr_init(&attr);
            pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
            pthread_create(&pth, &attr, query_thread, (void *) &running);
        }

        sleep((unsigned int) conf->lgr_interval);
    }
}


/**
 * Main function for thread
 */

void *query_thread(void *args) {
    int *running = (int *) args;
    *running = 1;

    test_query();

    *running = 0;
    return NULL;
}


/**
 * Test function
 */

void test_query() {
    char *rawres;
    response *res;

    res = resinit();

    rawres = http_call("localhost", 80, "/index.html", "GET", "name=text&surname=retest", "application/json",
                       "{\"param\": \"value\", \"index\": 3}");
    resparser(res, rawres);
    free(rawres);

    uiMessage(UI_INFO, "Data: %s", res->data);

    resfree(res);
}