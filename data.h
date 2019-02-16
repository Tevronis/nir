//
// Created by Mike on 16.02.2019.
//

#ifndef NIR_DATA_H
#define NIR_DATA_H

#include <mutex>
#include <condition_variable>
#include "task_queue.h"

typedef enum {
    TM_SINGLE,
    TM_MULTI,
} multithread_mode_t;

typedef struct config_t {
    multithread_mode_t multithread_mode;
} config_t;

typedef struct data_t {
    config_t * config;
    std::mutex count_elem_lock;
    std::mutex gen_lock;
    std::condition_variable check_cond;
    queue_t queue;
} data_t;


int data_init (data_t * data, config_t * config);
int config_init (config_t * config, int argc, char * argv[]);
int config_create (config_t * config, char * alph, int length, char * hash);

#endif //NIR_DATA_H
