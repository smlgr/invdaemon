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
void resclr(char *dst, char *src) {
    size_t ln;
    char *tmp;

    ln = strlen(src);

    bzero(dst, ln);

    tmp = (char *) calloc(sizeof(char), ln + 1);

    strrplc(tmp, src, '\r', '\n');
    strtrmdbl(dst, tmp, '\n');

    free(tmp);
}


/**
 * Response init
 */

response *resinit() {
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

void resparser(response *res, char *input) {
    char row[8193];
    char *d;
    char *item;
    char *save;
    int empty;

    memset(row, '\0', sizeof(row));
    empty = 0;
    d = row;

    do {
        if (*input == '\n') {
            input++;
            d = row;

            uiMessage(UI_DEBUG, "Socket <--  %s", row);

            if (empty > 0) {
                res->data = (char *) realloc(res->data, (strlen(row) + 1) * sizeof(char));
                strcpy(res->data, row);
                uiMessage(UI_DEBUG, "Response DATA: %s", res->data);
                continue;
            }

            if (strncmp(row, "HTTP", 4) == 0) {
                item = strtok_r(row, " ", &save);
                item = strtok_r(NULL, " ", &save);
                res->code = atoi(item);
                uiMessage(UI_DEBUG, "Response CODE: %d", res->code);
                continue;
            }

            if (strncmp(row, "Content-Type", 12) == 0) {
                item = strtok_r(row, " ", &save);
                item = strtok_r(NULL, " ", &save);
                res->type = (char *) realloc(res->type, (strlen(item) + 1) * sizeof(char));
                strcpy(res->type, item);
                uiMessage(UI_DEBUG, "Response TYPE: %s", res->type);
                continue;
            }

            if (strncmp(row, "Content-Length", 14) == 0) {
                item = strtok_r(row, " ", &save);
                item = strtok_r(NULL, " ", &save);
                res->len = atoi(item);
                uiMessage(UI_DEBUG, "Response LENGTH: %d", res->len);
                continue;
            }

            if (strlen(row) == 0) {
                empty++;
            }

            bzero(row, sizeof(row));
            continue;
        }

        *d = *input;
        input++;
        d++;
    } while (*input != '\0');
}


/**
 * Response free
 */

void resfree(response *res) {
    free(res->type);
    free(res->data);
    free(res);
}