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


#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <malloc.h>

#include "http.h"
#include "../ui.h"
#include "../utils.h"
#include "response.h"

char *http_call(char *server, unsigned int port, char *path, char *method, char *query_string, char *type, char *data) {
    char *res;
    char *tmp;
    char *rawdata;
    int sck;
    struct sockaddr_in serv_addr;
    struct hostent *serv_host;
    char url[1025];
    char buff[1025];
    char tmpmsg[1025];
    char dbgmsg[1025];
    char row[1025];
    char *s;
    char *d;
    ssize_t n;
    size_t ln;

    bzero(url, sizeof(url));
    strcpy(url, path);

    if (strlen(query_string) > 0) {
        strcat(url, "?");
        strcat(url, query_string);
    }

    uiMessage(UI_DEBUG, "Creating socket");
    if ((sck = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return NULL;
    }

    uiMessage(UI_DEBUG, "Resolving host");
    if (!(serv_host = gethostbyname(server))) {
        return NULL;
    }

    memset(&serv_addr, '\0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons((uint16_t) port);
    memcpy(&serv_addr.sin_addr, serv_host->h_addr, (size_t) serv_host->h_length);

    uiMessage(UI_DEBUG, "Connecting socket");
    if (connect(sck, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        return NULL;
    }

    memset(buff, '\0', sizeof(buff));
    sprintf(buff, "%s %s HTTP/1.0\r\n", method, url);
    strtrmcrlf(dbgmsg, buff);
    uiMessage(UI_DEBUG, "Socket  --> %s", dbgmsg);
    write(sck, buff, strlen(buff));

    memset(buff, '\0', sizeof(buff));
    sprintf(buff, "User-Agent: %s\r\n", "InvDaemon");
    strtrmcrlf(dbgmsg, buff);
    uiMessage(UI_DEBUG, "Socket  --> %s", dbgmsg);
    write(sck, buff, strlen(buff));

    memset(buff, '\0', sizeof(buff));
    sprintf(buff, "Host: %s\r\n", server);
    strtrmcrlf(dbgmsg, buff);
    uiMessage(UI_DEBUG, "Socket  --> %s", dbgmsg);
    write(sck, buff, strlen(buff));

//    memset(buff, '\0', sizeof(buff));
//    sprintf(buff, "Accept: text/plain,application/json;\r\n");
//    strtrmcrlf(dbgmsg, buff);
//    uiMessage(UI_DEBUG, "Socket  --> %s", dbgmsg);
//    write(sck, buff, strlen(buff));

    memset(buff, '\0', sizeof(buff));
    sprintf(buff, "Content-Type: %s\r\n", type);
    strtrmcrlf(dbgmsg, buff);
    uiMessage(UI_DEBUG, "Socket  --> %s", dbgmsg);
    write(sck, buff, strlen(buff));

    memset(buff, '\0', sizeof(buff));
    sprintf(buff, "Content-Length: %d\r\n", (int) strlen(data));
    strtrmcrlf(dbgmsg, buff);
    uiMessage(UI_DEBUG, "Socket  --> %s", dbgmsg);
    write(sck, buff, strlen(buff));

    memset(buff, '\0', sizeof(buff));
    sprintf(buff, "\r\n");
    strtrmcrlf(dbgmsg, buff);
    uiMessage(UI_DEBUG, "Socket  --> %s", dbgmsg);
    write(sck, buff, strlen(buff));

    if (strlen(data) > 0) {
        memset(buff, '\0', sizeof(buff));
        sprintf(buff, "%s\r\n", data);
        strtrmcrlf(dbgmsg, buff);
        uiMessage(UI_DEBUG, "Socket  --> %s", dbgmsg);
        write(sck, buff, strlen(buff));
    }

    rawdata = (char *) malloc(sizeof(char));
    *rawdata = '\0';

    do {
        memset(buff, '\0', sizeof(buff));
        n = read(sck, buff, sizeof(buff) - 1);

        if (n > 0) {
            ln = strlen(rawdata);
            rawdata = (char *) realloc(rawdata, ln + n + 1);
            strcat(rawdata, buff);
        }
    } while (n > 0);

    close(sck);

    tmp = (char *) calloc(sizeof(char), strlen(rawdata));
    resclr(tmp, rawdata);
    free(rawdata);

    res = (char *) calloc(sizeof(char), strlen(tmp));
    strcpy(res, tmp);
    free(tmp);

    return res;
}