//
// Created by Mike on 16.02.2019.
//

#ifndef NIR_DATA_H
#define NIR_DATA_H

#include <mutex>
#include <condition_variable>
#include <map>
#include "task_queue.h"

typedef enum {
    TM_SINGLE,
    TM_MULTI,
} multithread_mode_t;

typedef enum {
    IT_G6,
    IT_MATRIX,
    IT_ADJACENCY,
} input_type_t;

typedef struct config_t {
    multithread_mode_t multithread_mode;
    char * filename;
    input_type_t input_type = IT_G6;
} config_t;

typedef struct data_t {
    config_t * config;
    std::mutex count_elem_lock;
    std::mutex gen_lock;
    std::condition_variable check_cond;
    queue_t queue;
    std::map<int, int> hamilton_cycles;
    std::map<int, int> ham_cyc_h_e;
    std::map<int, int> ham_cyc_h_ne;
    int count_euler_cycles = 0;
    int count_hamilton_cycles = 0;
    std::vector<std::string> h_e;
    std::vector<std::string> nh_e;
    std::vector<std::string> h_ne;
    std::vector<std::string> nh_ne;
} data_t;


int data_init (data_t * data, config_t * config);
int config_init (config_t * config, int argc, char * argv[]);
int config_create (config_t * config, char * alph, int length, char * hash);

#endif //NIR_DATA_H
