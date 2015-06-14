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


#ifndef __QUEUE_H
#define __QUEUE_H

#include <time.h>

#include "../inverter/inverter.h"

struct queue_t {
    time_t datetime;
    invdata *data;
    struct queue_t *next;
};

typedef struct queue_t QUEUE;

void queue_init();

void queue_destroy();

void queue_add(invdata *data);

QUEUE *queue_offer();

void  queue_remove();

void queue_send();

#endif
