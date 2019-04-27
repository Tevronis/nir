//
// Created by Mike on 16.02.2019.
//

#include "data.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>

int config_init (config_t * config, int argc, char * argv[])
{
    config->multithread_mode = TM_SINGLE;
    int correct = 0;
    const char *opt_string = "msf:t:";
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
            case 'f':
                config->filename = optarg;
                correct = 1;
                break;
            case 't':
                if (strcmp(optarg, "g6") == 0)
                    config->input_type = IT_G6;
                if (strcmp(optarg, "mat") == 0)
                    config->input_type = IT_MATRIX;
                if (strcmp(optarg, "list") == 0)
                    config->input_type = IT_ADJACENCY;
                break;
            default:
                printf("Incorrect arguments.\n! -f = filename with graphs\n  -m = multithread mod\n  -s = single mod\n  -t = [g6, mat, list] graph format in input file\n");
                return (EXIT_FAILURE);
        }
    }
    if (correct == 0) {
        printf("Incorrect arguments.\n! -f = filename with graphs\n  -m = multithread mod\n  -s = single mod\n  -t = [g6, mat, list] graph format in input file\n");
        exit(EXIT_FAILURE);
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
