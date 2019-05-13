#include <iostream>
#include "graph/Graph.h"
#include "data.h"
#include "task_queue.h"
#include "utils/utils.h"
#include <thread>
#include <mutex>
#include <ctime>
#include <fstream>
#include <time.h>
#include <map>
#include <fileapi.h>

void producer(data_t * data) {
    std::ifstream infile(data->config->filename);
    if (!infile) {
        std::cout << "param -f: filename is not correct" << std::endl;
        raise(-1);
    }
    std::string s;
    int cnt = 1;
    for (infile >> s; !infile.eof(); infile >> s, cnt++) {
        if (cnt % 10000000 == 0) {
            std::cout << cnt << " graphs has been processed." << std::endl;
            cnt = 0;
        }
        task_t task = {
                .task = s,
        };
        queue_push(&data->queue, &task);
    }
    data->queue.stop_push = 1;

    {
        while (!data->queue.q.empty()) {
            // TODO(menc): look at this shit. maybe rewrite this later..
        }
        //for (int i = 0; i < 4; i++)
        data->queue.sem_full.notify();
    }
}


void set_statistic(data_t *data, Graph *graph, const std::string &s, int hams) {
    data->hamilton_cycles[hams]++;
    if (graph->is_euler())
        data->count_euler_cycles++;
    if (graph->is_hamilton())
        data->count_hamilton_cycles++;

    if (graph->is_hamilton() && graph->is_euler()) {
        data->h_e.push_back(s);
        data->ham_cyc_h_e[hams]++;
    }
    else if (!graph->is_hamilton() && graph->is_euler())
        data->nh_e.push_back(s);
    else if (graph->is_hamilton() && !graph->is_euler()) {
        data->h_ne.push_back(s);
        data->ham_cyc_h_ne[hams]++;
    }
    else if (!graph->is_hamilton() && !graph->is_euler())
        data->nh_ne.push_back(s);
}


void consumer(data_t * data, int num) {
    for (;;) {
        task_t task;
        if (!queue_pop(&data->queue, &task)) {
            break;
        }
        Graph graph(task.task);
        int hams = int(graph.get_hamilton_paths().size());
        graph.is_euler();   // call firstly for save lock-time
        {
            std::lock_guard<std::mutex> lock(data->queue.cnt_elem_lock);
            set_statistic(data, &graph, task.task, hams);
        }
    }
    queue_cancel(&data->queue);
    std::cout << "thread " << num << " ended" << std::endl;
}


void thread_mod(data_t * data) {
    std::cout << "Thread mod start" << std::endl;
    int nthreads = std::thread::hardware_concurrency();
    if(nthreads == 0)
        nthreads = 2;
    // nthreads = 4;
    std::cout << "Count of threads: " << nthreads << std::endl;
    std::vector<std::thread> threads;
    for(int i = 0; i < nthreads; i++) {
        std::thread thr(consumer, data, i);
        threads.emplace_back(std::move(thr));
    }
    producer(data);
    data->queue.stop_push = 1;

    for(auto& thr : threads) {
        thr.join();
    }

    std::cout << "Done!" << std::endl;
}


void single_mod(data_t * data) {
    std::cout << "Single mod start" << std::endl;

    std::ifstream infile(data->config->filename);
    if (!infile) {
        std::cout << "param -f: filename is not correct" << std::endl;
        raise(-1);
    }
    if (data->config->input_type == IT_G6) {
        std::string s;
        for (infile >> s; !infile.eof(); infile >> s) {
            Graph graph(s);

            set_statistic(data, &graph, s, graph.get_hamilton_paths().size());
        }
    }
    if (data->config->input_type == IT_MATRIX) {
        // used for testing
        int n;
        infile >> n;
        std::vector<std::vector<int> > mat(n, std::vector<int>(n, 0));

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                infile >> mat[i][j];
            }
        }
        Graph graph(mat);
        std::cout << graph.is_hamilton() << std::endl;
        auto paths = graph.get_hamilton_paths();
        if (paths.empty()) {
            std::cout << "path not found" << std::endl;
            return;
        }
        for (const auto& item: paths) {
            for (int i: item) {
                std::cout << i << " ";
            }
            std::cout << std::endl;
        }
    }
}


void run(data_t * data) {
    switch (data->config->multithread_mode) {
        case TM_MULTI:
            thread_mod(data);
            break;
        case TM_SINGLE:
            single_mod(data);
            break;
    }
    std::string context = split(std::string(data->config->filename), "/").back();
    std::string p = "../result/";
    std::string dir = split(context, ".")[0];
    std::string path = p + dir + '/';
    CreateDirectory(path.c_str(), NULL);
    // FULL STAT
    std::ofstream out(path + "hamilton_stat_FULL.txt");
    for (auto item: data->hamilton_cycles) {
        out << item.first << "\t" << item.second << std::endl;
    }
    out.close();
    // ONLY HAMILTON AND EULER
    out.open(path + "hamilton_stat_HE.txt");
    for (auto item: data->ham_cyc_h_e) {
        out << item.first << "\t" << item.second << std::endl;
    }
    out.close();
    // ONLY HAMILTON AND NOT EULER
    out.open(path + "hamilton_stat_HNE.txt");
    for (auto item: data->ham_cyc_h_ne) {
        out << item.first << "\t" << item.second << std::endl;
    }
    out.close();

    save_vector_into_file(data->h_e, path + "h_e.txt");   // hamilton + euler
    save_vector_into_file(data->nh_e, path + "nh_e.txt"); // not hamilton + euler
    save_vector_into_file(data->h_ne, path + "h_ne.txt"); // hamilton + not euler
    save_vector_into_file(data->nh_ne, path + "nh_ne.txt");   // not hamilton + not euler
}


int main(int argc, char *argv[]) {
    clock_t start_time = clock();
    config_t config;
    data_t data;

    config_init (&config, argc, argv);
    data_init (&data, &config);

    run(&data);

    std::cout << "\ntime: " << ((float)(clock() - start_time)) / CLOCKS_PER_SEC << std::endl;
    return 0;
}