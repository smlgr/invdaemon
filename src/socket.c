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


#include <stdint.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <malloc.h>
#include <stdlib.h>

#include "ui.h"
#include "config.h"


/**
 * Open a socket, send query, read and return response
 */

char *socket_query(char *hostname, uint16_t port, char *data) {
    char *ret;
    int sck;
    struct sockaddr_in serv_addr;
    struct hostent *serv_host;
    size_t ln;
    ssize_t n;
    char buff[SOCKET_TCP_BUFFER];
    char *c;
    int i;
    struct timeval timeout;
    int flag;

    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    flag = 1;

    ui_message(UI_DEBUG, "SOCKET", "Creating socket");
    if ((sck = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return NULL;

    ui_message(UI_DEBUG, "SOCKET", "Setting RX timeout");
    if (setsockopt(sck, SOL_SOCKET, SO_RCVTIMEO, (char *) &timeout, sizeof(timeout)) < 0)
        return NULL;

    ui_message(UI_DEBUG, "SOCKET", "Setting TX timeout");
    if (setsockopt(sck, SOL_SOCKET, SO_SNDTIMEO, (char *) &timeout, sizeof(timeout)) < 0)
        return NULL;

    ui_message(UI_DEBUG, "SOCKET", "Setting TCP_NODELAY");
    if (setsockopt(sck, IPPROTO_TCP, TCP_NODELAY, (char *) &flag, sizeof(int)) < 0)
        return NULL;

    ui_message(UI_DEBUG, "SOCKET", "Resolving host");
    if (!(serv_host = gethostbyname(hostname)))
        return NULL;

    memset(&serv_addr, '\0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    memcpy(&serv_addr.sin_addr, serv_host->h_addr, (size_t) serv_host->h_length);

    ui_message(UI_DEBUG, "SOCKET", "Connecting socket to %s:%d", hostname, port);
    if (connect(sck, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        return NULL;
    }

    ui_message(UI_DEBUG, "SOCKET", "Sending data");
    write(sck, data, strlen(data));

    ui_message(UI_DEBUG, "SOCKET", "Reading from socket");

    ret = (char *) malloc(sizeof(char));
    *ret = '\0';

    memset(buff, '\0', sizeof(buff));

    n = read(sck, buff, sizeof(buff) - 1);

    if (n > 0) {
        buff[n] = '\0';

        ret = (char *) realloc(ret, (size_t) (n + 1));
        strcpy(ret, buff);
    }

    ui_message(UI_DEBUG, "SOCKET", "Closing socket");
    close(sck);

    ui_message(UI_INFO, "SOCKET", "Return value: %s", ret);
    return ret;
}
