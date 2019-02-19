//
// Created by Mike on 16.02.2019.
//

#ifndef NIR_TASK_QUEUE_H
#define NIR_TASK_QUEUE_H

#include <queue>
#include <stdbool.h>
#include <mutex>
#include <thread>

#include "graph/Graph.h"
#include "semaphore.h"


#define QUEUE_SIZE (4)

typedef struct task_t {
    // Graph graph_f;
    int test_num = 0;
} task_t;

typedef struct queue_t {
    int max_size = QUEUE_SIZE;
    std::queue<task_t> q;
    unsigned char stop_push;
    unsigned char stop_pop;
    std::mutex cnt_elem_lock;
    std::mutex lock_tail, lock_head;
    semaphore sem_empty, sem_full;
} queue_t;


void queue_init (queue_t * queue);
void queue_destroy (queue_t * queue);
void queue_cancel (queue_t * queue);
bool queue_push (queue_t * queue, task_t * value);
bool queue_pop (queue_t * queue, task_t * value);

#endif //NIR_TASK_QUEUE_H
