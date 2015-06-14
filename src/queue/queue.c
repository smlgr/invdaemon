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

QUEUE *queue_first;
QUEUE *queue_last;

pthread_mutex_t queue_lock;


/**
 * Initialize Data Queue
 */

void queue_init() {
    pthread_mutex_init(&queue_lock, NULL);

    pthread_mutex_lock(&queue_lock);

    queue_first = NULL;
    queue_last = NULL;

    pthread_mutex_unlock(&queue_lock);
}


/**
 * Destroy Data Queue
 */

void queue_destroy() {
    QUEUE *q;
    QUEUE *i;

    pthread_mutex_lock(&queue_lock);

    q = queue_first;

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

    queue_first = NULL;
    queue_last = NULL;

    pthread_mutex_unlock(&queue_lock);

    pthread_mutex_destroy(&queue_lock);
}


/**
 * Add an element at the end of the queue
 */

void queue_push(invdata *input) {
    QUEUE *i;

    i = (QUEUE *) malloc(sizeof(QUEUE));

    pthread_mutex_lock(&queue_lock);

    if (queue_last == NULL) {
        queue_last = i;
    } else {
        queue_last->next = i;
        queue_last = queue_last->next;
    }

    queue_last->datetime = time(NULL);

    queue_last->data = (invdata *) malloc(sizeof(invdata));
    inv_data_clone(queue_last->data, input);

    queue_last->next = NULL;

    pthread_mutex_unlock(&queue_lock);
}


/**
 * Return the first element of the queue
 */

QUEUE *queue_offer() {
    QUEUE *ret;

    ret = NULL;

    pthread_mutex_lock(&queue_lock);

    if (queue_first == NULL)
        return ret;

    ret = (QUEUE *) malloc(sizeof(QUEUE));
    ret->datetime = queue_first->datetime;
    ret->next = NULL;

    ret->data = (invdata *) malloc(sizeof(invdata));
    inv_data_clone(ret->data, queue_first->data);

    pthread_mutex_unlock(&queue_lock);

    return ret;
}


/**
 * Remove the first element of the queue
 */

void queue_remove() {
    QUEUE *item;

    pthread_mutex_lock(&queue_lock);

    if (queue_first == NULL)
        return;

    item = queue_first;
    queue_first = queue_first->next;

    inv_free(item->data);
    free(item);
}
