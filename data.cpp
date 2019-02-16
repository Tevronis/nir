//
// Created by Mike on 16.02.2019.
//

#include "data.h"

#include <stdio.h>

#include <stdlib.h>
#include <getopt.h>

int config_init (config_t * config, int argc, char * argv[])
{
    config->multithread_mode = TM_SINGLE;
    const char *opt_string = "ms";
    for (;;) {
        int opt = getopt(argc, argv, opt_string);
        if (-1 == opt)
            break;
        switch (opt) {
            case 'm':
                config->multithread_mode = TM_MULTI;
                break;
            case 's':
                config->multithread_mode = TM_SINGLE;
                break;
            default:
                return (EXIT_FAILURE);
        }
    }

    return (EXIT_SUCCESS);
}

int config_create (config_t * config) {
    config->multithread_mode = TM_SINGLE;

    return (EXIT_SUCCESS);
}

int data_init (data_t * data, config_t * config) {
    data->config = config;
    queue_init (&data->queue);
    data->queue.stop_pop = data->queue.stop_push = 0;

    printf("%s\n", "config and data init successful");
    return (EXIT_SUCCESS);
}
