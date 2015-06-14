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


#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

#include "inverter/inverter.h"
#include "queue/queue.h"

#include "invdaemon.h"
#include "ui.h"
#include "cfg.h"
#include "server.h"
#include "log.h"

char *smlgr_program_name;
cfg *conf;

static volatile int keep_running = 1;

int main(int argc, char **argv) {
    smlgr_program_name = argv[0];

    signal(SIGINT, signal_handler);

    cfg_init();

    if (cfg_parse(argc, argv)) {
        log_restart();
        cfg_print();
        inv_daemon();
    }

    cfg_free();

    return 0;
}


/**
 * Signal handler
 */

void signal_handler(int signal) {
    if (signal == SIGINT) {
        keep_running = 0;
    }
}


/**
 * Main program function
 */

void inv_daemon() {
    pthread_t pth;
    pthread_attr_t attr;
    int running = 0;

    if (conf->queue_size > 0) {
        queue_init();
    }

    while (keep_running == 1) {
        ui_message(UI_DEBUG, "", "Interval");

        ui_message(UI_DEBUG, "", "Running = %d", running);
        if (running == 0) {
            ui_message(UI_DEBUG, "", "Running new query thread");
            pthread_attr_init(&attr);
            pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
            pthread_create(&pth, &attr, query_thread, (void *) &running);
        }

        sleep((unsigned int) conf->lgr_interval);
    }

    if (conf->queue_size > 0) {
        queue_destroy();
    }
}


/**
 * Main function for thread
 */

void *query_thread(void *args) {
    int *running = (int *) args;
    invdata *data;

    *running = 1;
    ui_message(UI_DEBUG, "THREAD", "Thread running = %d", *running);

    data = inv_init();

    inv_query(data);

    if (data->valid == 0) {
        if (conf->queue_size > 0) {
            ui_message(UI_DEBUG, "THREAD", "Adding element to queue");
            queue_add(data);
            queue_send();
        } else {
            if (server_send(data) == 0) {
                ui_message(UI_INFO, "THREAD", "Thread Data sent");
            } else {
                ui_message(UI_WARNING, "THREAD", "Thread Error sending data");
            }
        }
    } else {
        ui_message(UI_WARNING, "THREAD", "Thread Inverter query not valid");
    }

    ui_message(UI_DEBUG, "THREAD", "Thread free data");
    inv_free(data);

    *running = 0;
    ui_message(UI_DEBUG, "THREAD", "Thread running = %d", *running);

    return NULL;
}
