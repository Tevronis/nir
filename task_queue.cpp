//
// Created by Mike on 16.02.2019.
//

#include "task_queue.h"
#include <stdio.h>
#include <mutex>
#include <thread>
#include "semaphore.h"


void queue_init (queue_t * queue)
{
    queue->sem_empty.init(QUEUE_SIZE);
    queue->sem_full.init(0);
}

void queue_destroy (queue_t * queue)
{

}

bool queue_push (queue_t * queue, task_t * value) {
    queue->sem_empty.wait();
    {
        std::lock_guard<std::mutex> lock(queue->lock_head);
        queue->q.push(*value);
    }
    queue->sem_full.notify();

    return true;
}

bool queue_pop (queue_t * queue, task_t * value) {
    queue->sem_full.wait();
    {
        std::lock_guard<std::mutex> lock(queue->lock_head);
        if (queue->q.empty() || queue->stop_pop) {
            queue->sem_full.notify();
            return (false);
        }
        *value = queue->q.front();
        queue->q.pop();
    }
    queue->sem_empty.notify();
    return (true);
}

void queue_cancel (queue_t * queue) {
    queue->stop_pop = 1;
    queue->sem_empty.notify();
}
