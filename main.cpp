#include <iostream>
#include "graph/Graph.h"
#include "data.h"
#include <thread>
#include <mutex>
#include <ctime>
#include <fstream>
#include <map>

void producer(data_t * data) {
    std::ifstream infile(data->config->filename);
    if (!infile) {
        std::cout << "param -f: filename is not correct" << std::endl;
        raise(-1);
    }
    std::string s;
    for (infile >> s; !infile.eof(); infile >> s) {
        // std::cout << "Compute line: " << s << std::endl;
        task_t task = {
                .task = s,
        };
        queue_push(&data->queue, &task);
    }
    data->queue.stop_push = 1;
    // Чтение из файла
    // Генерация слова для перебора хэша
    // Создание таска
    // Запись таска в очередь
}


void consumer(data_t * data, int num) {
    for (;;) {
        if (data->queue.stop_push) {
            break;
        }
        task_t task;
        queue_pop(&data->queue, &task);
        {
            Graph graph(task.task);
            int result = int(graph.get_gamilton_paths().size());
            std::lock_guard<std::mutex> lock(data->queue.cnt_elem_lock);
            data->gam[result]++;
        }
    }
    {
        std::lock_guard<std::mutex> lock(data->queue.cnt_elem_lock);
        std::cout << "thread " << num << " ended" << std::endl;
    }
    // чтение из очереди
    // обработка таска

}


void thread_mod(data_t * data) {
    std::cout << "Thread mod start" << std::endl;
    int nthreads = std::thread::hardware_concurrency();
    if(nthreads == 0)
        nthreads = 2;
    // nthreads = 1;
    std::cout << "Count of threads: " << nthreads << std::endl;
    std::vector<std::thread> threads;
    for(int i = 0; i < nthreads; i++) {
        std::thread thr(consumer, data, i);
        threads.emplace_back(std::move(thr));
    }

    producer(data);

    for(auto& thr : threads) {
        thr.join();
    }

    for (auto item: data->gam) {
        std::cout << item.first << " " << item.second << std::endl;
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
        std::map<int, int> gam;
        for (infile >> s; !infile.eof(); infile >> s) {
            // std::cout << "Compute line: " << s << std::endl;
            Graph graph(s);
            gam[graph.get_gamilton_paths().size()]++;
        }
        for (auto item: gam) {
            std::cout << item.first << " " << item.second << std::endl;
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
        std::cout << graph.is_gamilton() << std::endl;
        auto paths = graph.get_gamilton_paths();
        if (paths.empty()) {
            std::cout << "path not found" << std::endl;
            return;
        }
        for (auto item: paths) {
            for (int i: item) {
                std::cout << i << " ";
            }
            std::cout << std::endl;
        }
        // graph.print();
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
}


int main(int argc, char *argv[]) {
    clock_t start_time = clock();
    config_t config;
    data_t data;

    config_init (&config, argc, argv);
    data_init (&data, &config);

    run(&data);

    std::cout << "time: " << (clock() - start_time) / 60.0 << std::endl;
    return 0;
}