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


#include <malloc.h>
#include <time.h>
#include <pthread.h>
#include <stdlib.h>

#include "queue.h"
#include "../server.h"
#include "../ui.h"

QUEUE *item_first;
QUEUE *item_last;
int items_count;

pthread_mutex_t queue_lock;


/**
 * Initialize Data Queue
 */

void queue_init() {
    pthread_mutex_init(&queue_lock, NULL);

    pthread_mutex_lock(&queue_lock);

    item_first = NULL;
    item_last = NULL;
    items_count = 0;

    pthread_mutex_unlock(&queue_lock);
}


/**
 * Destroy Data Queue
 */

void queue_destroy() {
    QUEUE *q;
    QUEUE *i;

    pthread_mutex_lock(&queue_lock);

    q = item_first;

    if (q == NULL)
        return;

    while (q->next != NULL) {
        i = q;
        q = q->next;

        inv_free(i->data);
        free(i);
    }

    inv_free(q->data);
    free(q);

    item_first = NULL;
    item_last = NULL;
    items_count = 0;

    pthread_mutex_unlock(&queue_lock);

    pthread_mutex_destroy(&queue_lock);
}


/**
 * Add an element at the end of the queue
 */

void queue_add(invdata *input) {
    QUEUE *i;

    i = (QUEUE *) malloc(sizeof(QUEUE));

    pthread_mutex_lock(&queue_lock);

    if (item_last == NULL) {
        item_last = i;
    } else {
        item_last->next = i;
        item_last = item_last->next;
    }

    item_last->datetime = time(NULL);

    item_last->data = (invdata *) malloc(sizeof(invdata));
    inv_data_clone(item_last->data, input);

    item_last->next = NULL;

    items_count++;

    pthread_mutex_unlock(&queue_lock);
}


/**
 * Return the first element of the queue
 */

QUEUE *queue_offer() {
    QUEUE *ret;

    ret = NULL;

    pthread_mutex_lock(&queue_lock);

    if (item_first == NULL)
        return ret;

    ret = (QUEUE *) malloc(sizeof(QUEUE));
    ret->datetime = item_first->datetime;
    ret->next = NULL;

    ret->data = (invdata *) malloc(sizeof(invdata));
    inv_data_clone(ret->data, item_first->data);

    pthread_mutex_unlock(&queue_lock);

    return ret;
}


/**
 * Remove the first element of the queue
 */

void queue_remove() {
    QUEUE *item;

    pthread_mutex_lock(&queue_lock);

    if (item_first == NULL)
        return;

    item = item_first;
    item_first = item_first->next;

    inv_free(item->data);
    free(item);

    items_count--;
}


/**
 * Try to send queue to server
 */

void queue_send() {
    do {
        if (server_send_ts(item_first->data, item_first->datetime) == 0) {
            ui_message(UI_INFO, "QUEUE", "Thread Data sent");
            queue_remove();
        } else {
            ui_message(UI_WARNING, "QUEUE", "Thread Error sending data");
            break;
        }
    } while (item_first != NULL);
}
