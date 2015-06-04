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
#include <string.h>
#include <getopt.h>

#include "config.h"
#include "ui.h"
#include "cfg.h"

extern cfg *conf;


/**
 * Configuration init with default values
 */

void cfg_init() {
    size_t ln;

    conf = (cfg *) malloc(sizeof(cfg));

    conf->debug_level = DEFAULT_UI_DEBUG_LEVEL;
    conf->log_file_enabled = DEFAULT_LOG_FILE_ENABLED;

    ln = strlen(DEFAULT_LOG_FILE_NAME) + 1;
    conf->log_file = (char *) calloc(sizeof(char), ln);
    strcpy(conf->log_file, DEFAULT_LOG_FILE_NAME);

    conf->inv_model = DEFAULT_INVERTER_MODEL;

    ln = strlen(DEFAULT_INVERTER_TCP_IP_ADDR) + 1;
    conf->inv_tcp_addr = (char *) calloc(sizeof(char), ln);
    strcpy(conf->inv_tcp_addr, DEFAULT_INVERTER_TCP_IP_ADDR);

    conf->inv_tcp_port = DEFAULT_INVERTER_TCP_IP_PORT;

    ln = strlen(DEFAULT_INVERTER_SERIAL_PORT) + 1;
    conf->inv_serial_port = (char *) calloc(sizeof(char), ln);
    strcpy(conf->inv_serial_port, DEFAULT_INVERTER_SERIAL_PORT);

    conf->inv_serial_speed = DEFAULT_INVERTER_SERIAL_SPEED;

    conf->inv_num = DEFAULT_INVERTER_NUM;

    conf->lgr_interval = DEFAULT_LGR_INTERVAL;

    ln = strlen(DEFAULT_SERVER_ADDR) + 1;
    conf->server_addr = (char *) calloc(sizeof(char), ln);
    strcpy(conf->server_addr, DEFAULT_SERVER_ADDR);

    conf->server_port = DEFAULT_SERVER_PORT;
    conf->server_inv_id = DEFAULT_SERVER_INVERTER_ID;

    ln = strlen(DEFAULT_SERVER_INVERTER_TOKEN) + 1;
    conf->server_inv_token = (char *) calloc(sizeof(char), ln);
    strcpy(conf->server_inv_token, DEFAULT_SERVER_INVERTER_TOKEN);
}


/**
 * Configuration print
 */

void cfg_print() {
    ui_message(UI_INFO, "CFG", "debug-level = %d", conf->debug_level);
    ui_message(UI_INFO, "CFG", "log-file-enabled = %d", conf->log_file_enabled);
    ui_message(UI_INFO, "CFG", "log-file = %s", conf->log_file);
    ui_message(UI_INFO, "CFG", "inv-model = %d", conf->inv_model);
    ui_message(UI_INFO, "CFG", "inv-tcp-addr = %s", conf->inv_tcp_addr);
    ui_message(UI_INFO, "CFG", "inv-tcp-port = %d", conf->inv_tcp_port);
    ui_message(UI_INFO, "CFG", "inv-serial-port = %s", conf->inv_serial_port);
    ui_message(UI_INFO, "CFG", "inv-serial-speed = %d", conf->inv_serial_speed);
    ui_message(UI_INFO, "CFG", "inv-num = %d", conf->inv_num);
    ui_message(UI_INFO, "CFG", "lgr-interval = %d", conf->lgr_interval);
    ui_message(UI_INFO, "CFG", "server-addr = %s", conf->server_addr);
    ui_message(UI_INFO, "CFG", "server-port = %d", conf->server_port);
    ui_message(UI_INFO, "CFG", "server-inv-id = %ld", conf->server_inv_id);
    ui_message(UI_INFO, "CFG", "server-inv-token = %s", conf->server_inv_token);
}


/**
 * Configuration free
 */

void cfg_free() {
    free(conf->log_file);
    free(conf->inv_tcp_addr);
    free(conf->inv_serial_port);
    free(conf->server_addr);
    free(conf->server_inv_token);

    free(conf);
}


/**
 * Parsing options
 */

int cfg_parse(int argc, char **argv) {
    int ret = 0;
    int option_index = 0;
    int c;
    size_t ln;
    int conf_file = 0;
    char *config_file;

    static struct option long_options[] = {
            {"config",           required_argument, 0, 'c'},
            {"help",             no_argument,       0, 'h'},
            {"version",          no_argument,       0, 'V'},
            {"quiet",            no_argument,       0, 'q'},
            {"verbose",          no_argument,       0, 'v'},
            {"debug-level",      required_argument, 0, 'd'},
            {"log-file-enabled", required_argument, 0, 'k'},
            {"log-file",         required_argument, 0, 'l'},
            {"inv-model",        required_argument, 0, 'm'},
            {"inv-tcp-addr",     required_argument, 0, 'a'},
            {"inv-tcp-port",     required_argument, 0, 'p'},
            {"inv-serial-port",  required_argument, 0, 'e'},
            {"inv-serial-speed", required_argument, 0, 'b'},
            {"inv-num",          required_argument, 0, 'n'},
            {"lgr-interval",     required_argument, 0, 'i'},
            {"server-addr",      required_argument, 0, 's'},
            {"server-port",      required_argument, 0, 'r'},
            {"server-inv-id",    required_argument, 0, 'u'},
            {"server-inv-token", required_argument, 0, 't'},
            {0, 0,                                  0, 0}
    };

    config_file = (char *) malloc(sizeof(char));
    *config_file = '\0';

    while (1) {
        c = getopt_long(argc, argv, "c:hVqvd:k:l:m:a:p:e:b:n:i:s:r:u:t:", long_options, &option_index);

        if (c == -1) {
            ret = 1;
            break;
        }

        if (c == '?') {
            ui_help();
            break;
        }

        if (c == 'c') {
            conf_file = 1;

            ln = strlen(optarg) + 1;
            config_file = (char *) realloc(config_file, ln * sizeof(char));
            strcpy(config_file, optarg);
        }

        if (c == 'h') {
            ui_help();
            break;
        }

        if (c == 'V') {
            ui_version();
            break;
        }

        if (c == 'q') {
            conf->debug_level = 0;
        }

        if (c == 'v') {
            conf->debug_level = 4;
        }

        if (c == 'd') {
            conf->debug_level = atoi(optarg);
        }

        if (c == 'k') {
            conf->log_file_enabled = atoi(optarg);
        }

        if (c == 'l') {
            ln = strlen(optarg) + 1;
            conf->log_file = (char *) realloc((void *) conf->log_file, sizeof(char) * ln);
            strcpy(conf->log_file, optarg);
        }

        if (c == 'm') {
            conf->inv_model = atoi(optarg);
        }

        if (c == 'a') {
            ln = strlen(optarg) + 1;
            conf->inv_tcp_addr = (char *) realloc((void *) conf->inv_tcp_addr, sizeof(char) * ln);
            strcpy(conf->inv_tcp_addr, optarg);
        }

        if (c == 'p') {
            conf->inv_tcp_port = (uint16_t) atoi(optarg);
        }

        if (c == 'e') {
            ln = strlen(optarg) + 1;
            conf->inv_serial_port = (char *) realloc((void *) conf->inv_serial_port, sizeof(char) * ln);
            strcpy(conf->inv_serial_port, optarg);
        }

        if (c == 'b') {
            conf->inv_serial_speed = atoi(optarg);
        }

        if (c == 'n') {
            conf->inv_num = (unsigned int) atoi(optarg);
        }

        if (c == 'i') {
            conf->lgr_interval = atoi(optarg);
        }

        if (c == 's') {
            ln = strlen(optarg) + 1;
            conf->server_addr = (char *) realloc((void *) conf->server_addr, sizeof(char) * ln);
            strcpy(conf->server_addr, optarg);
        }

        if (c == 'r') {
            conf->server_port = (uint16_t) atoi(optarg);
        }

        if (c == 'r') {
            conf->server_inv_id = atol(optarg);
        }

        if (c == 't') {
            ln = strlen(optarg) + 1;
            conf->server_inv_token = (char *) realloc((void *) conf->server_inv_token, sizeof(char) * ln);
            strcpy(conf->server_inv_token, optarg);
        }
    }

    if (conf_file == 1)
        ret = cfg_file_parse(config_file);

    free(config_file);

    return ret;
}


/**
 * Parsing options
 * @param[in] config_file Path of a config file to parse
 * @param[out] ret Returns 0 in case of error, 1 if not
 */

int cfg_file_parse(char *config_file) {
    int ret = 0;
    FILE *fd;
    char param[80];
    char value[80];
    size_t ln;
    int linecount = 0;

    fd = fopen(config_file, "r");

    if (fd != NULL) {
        ui_message(UI_INFO, "CFG-FILE", "Parsing config file %s", config_file);

        while (!feof(fd)) {
            linecount++;

            bzero(param, sizeof(param));
            bzero(value, sizeof(value));

            if (fscanf(fd, "%s %s", param, value) != 2) {
                if (strlen(param) != 0 || strlen(value) != 0)
                    ui_message(UI_ERROR, "CFG-FILE", "Unable to parse config file in line %d", linecount);
                continue;
            }

            ui_message(UI_DEBUG, "CFG-FILE", "Param: %s - Value: %s", param, value);

            if (strcmp(param, "debug-level") == 0) {
                conf->debug_level = atoi(value);
                ui_message(UI_DEBUG, "CFG-FILE", "Configuration updated. debug_level = %d", conf->debug_level);
                continue;
            }

            if (strcmp(param, "log-file-enabled") == 0) {
                conf->log_file_enabled = atoi(value);
                ui_message(UI_DEBUG, "CFG-FILE", "Configuration updated. log_file_enabled = %d", conf->log_file_enabled);
                continue;
            }

            if (strcmp(param, "log-file") == 0) {
                ln = strlen(value) + 1;
                conf->log_file = (char *) realloc((void *) conf->log_file, sizeof(char) * ln);
                strcpy(conf->log_file, value);
                ui_message(UI_DEBUG, "CFG-FILE", "Configuration updated. log_file = %s", conf->log_file);
                continue;
            }

            if (strcmp(param, "inv-model") == 0) {
                conf->inv_model = atoi(value);
                ui_message(UI_DEBUG, "CFG-FILE", "Configuration updated. inv_model = %d", conf->inv_model);
                continue;
            }

            if (strcmp(param, "inv-tcp-addr") == 0) {
                ln = strlen(value) + 1;
                conf->inv_tcp_addr = (char *) realloc((void *) conf->inv_tcp_addr, sizeof(char) * ln);
                strcpy(conf->inv_tcp_addr, value);
                ui_message(UI_DEBUG, "CFG-FILE", "Configuration updated. inv_tcp_addr = %s", conf->inv_tcp_addr);
                continue;
            }

            if (strcmp(param, "inv-tcp-port") == 0) {
                conf->inv_tcp_port = (uint16_t) atoi(value);
                ui_message(UI_DEBUG, "CFG-FILE", "Configuration updated. inv_tcp_port = %d", conf->inv_tcp_port);
                continue;
            }

            if (strcmp(param, "inv-serial-port") == 0) {
                ln = strlen(value) + 1;
                conf->inv_tcp_addr = (char *) realloc((void *) conf->inv_tcp_addr, sizeof(char) * ln);
                strcpy(conf->inv_tcp_addr, value);
                ui_message(UI_DEBUG, "CFG-FILE", "Configuration updated. inv_tcp_addr = %s", conf->inv_tcp_addr);
                continue;
            }

            if (strcmp(param, "inv-serial-speed") == 0) {
                conf->inv_serial_speed = atoi(value);
                ui_message(UI_DEBUG, "CFG-FILE", "Configuration updated. inv_serial_speed = %d",
                           conf->inv_serial_speed);
                continue;
            }

            if (strcmp(param, "inv-num") == 0) {
                conf->inv_num = (unsigned int) atoi(value);
                ui_message(UI_DEBUG, "CFG-FILE", "Configuration updated. inv_num = %d", conf->inv_num);
                continue;
            }

            if (strcmp(param, "lgr-interval") == 0) {
                conf->lgr_interval = atoi(value);
                ui_message(UI_DEBUG, "CFG-FILE", "Configuration updated. lgr_interval = %d", conf->lgr_interval);
                continue;
            }

            if (strcmp(param, "server-addr") == 0) {
                ln = strlen(value) + 1;
                conf->server_addr = (char *) realloc((void *) conf->server_addr, sizeof(char) * ln);
                strcpy(conf->server_addr, value);
                ui_message(UI_DEBUG, "CFG-FILE", "Configuration updated. server_addr = %s", conf->server_addr);
                continue;
            }

            if (strcmp(param, "server-port") == 0) {
                conf->server_port = (uint16_t) atoi(value);
                ui_message(UI_DEBUG, "CFG-FILE", "Configuration updated. server_port = %d", conf->server_port);
                continue;
            }

            if (strcmp(param, "server-inv-id") == 0) {
                conf->server_inv_id = atoi(value);
                ui_message(UI_DEBUG, "CFG-FILE", "Configuration updated. server_inv_id = %d", conf->server_inv_id);
                continue;
            }

            if (strcmp(param, "server-inv-token") == 0) {
                ln = strlen(value) + 1;
                conf->server_inv_token = (char *) realloc((void *) conf->server_inv_token, sizeof(char) * ln);
                strcpy(conf->server_inv_token, value);
                ui_message(UI_DEBUG, "CFG-FILE", "Configuration updated. server_inv_token = %s",
                           conf->server_inv_token);
                continue;
            }

            ui_message(UI_ERROR, "CFG-FILE", "Unable to parse config file in line %d", linecount);
        }

        fclose(fd);

        ret = 1;
    } else {
        ui_message(UI_ERROR, "CFG-FILE", "Unable to open config file %s", config_file);
    }

    return ret;
}
