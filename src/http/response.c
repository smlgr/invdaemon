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

#include <stddef.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#include "response.h"
#include "../utils.h"
#include "../ui.h"

/**
 * Clear Response string
 */
void res_clear(char *dst, char *src) {
    size_t ln;
    char *tmp;

    ln = strlen(src);

    bzero(dst, ln);

    tmp = (char *) calloc(sizeof(char), ln + 1);

    str_replace_char(tmp, src, '\r', '\n');
    str_trim_doubles(dst, tmp, '\n');

    free(tmp);
}


/**
 * Response init
 */

response *res_init() {
    response *res;
    res = (response *) malloc(sizeof(response));

    res->type = (char *) malloc(sizeof(char));
    *res->type = '\0';

    res->data = (char *) malloc(sizeof(char));
    *res->data = '\0';

    return res;
}


/**
 * Prepare Response struct from string
 */

void res_parser(response *res, char *input) {
    char row[8193];
    char *d;
    char *item;
    char *save;
    int empty;
    size_t ln;

    memset(row, '\0', sizeof(row));
    empty = 0;
    d = row;

    do {
        if (*input == '\n') {
            input++;
            d = row;

            ui_message(UI_DEBUG, "Response row: %s", row);

            if (empty > 0) {
                ln = strlen(res->data) + strlen(row) + 1;
                res->data = (char *) realloc(res->data, ln * sizeof(char));
                strcat(res->data, row);
                memset(row, '\0', sizeof(row));
                continue;
            }

            if (strncmp(row, "HTTP", 4) == 0) {
                item = strtok_r(row, " ", &save);
                item = strtok_r(NULL, " ", &save);
                res->code = atoi(item);
                memset(row, '\0', sizeof(row));
                continue;
            }

            if (strncmp(row, "Content-Type", 12) == 0) {
                item = strtok_r(row, " ", &save);
                item = strtok_r(NULL, " ", &save);
                res->type = (char *) realloc(res->type, (strlen(item) + 1) * sizeof(char));
                strcpy(res->type, item);
                memset(row, '\0', sizeof(row));
                continue;
            }

            if (strncmp(row, "Content-Length", 14) == 0) {
                item = strtok_r(row, " ", &save);
                item = strtok_r(NULL, " ", &save);
                res->len = atoi(item);
                memset(row, '\0', sizeof(row));
                continue;
            }

            if (strlen(row) == 0) {
                empty++;
            }

            memset(row, '\0', sizeof(row));
            continue;
        }

        *d = *input;
        input++;
        d++;
    } while (*input != '\0');

    ui_message(UI_DEBUG, "Response CODE: %d", res->code);
    ui_message(UI_DEBUG, "Response TYPE: %s", res->type);
    ui_message(UI_DEBUG, "Response LENGTH: %d", res->len);
    ui_message(UI_DEBUG, "Response DATA: %s", res->data);
}


/**
 * Response free
 */

void res_free(response *res) {
    free(res->type);
    free(res->data);
    free(res);
}