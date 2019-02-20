#include <iostream>
#include "graph/Graph.h"
#include "data.h"
#include <thread>
#include <mutex>
#include <ctime>

void producer(data_t * data) {
    for (int i = 0; i < 1000; i++) {
        task_t task = {
                .test_num = i,
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
            std::lock_guard<std::mutex> lock(data->queue.cnt_elem_lock);
            std::cout << task.test_num << "\tfrom thread " << num << std::endl;
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

    std::cout << "Done!" << std::endl;
}


void single_mod(data_t * data) {

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

    std::cout << "time: " << clock() - start_time << std::endl;
    return 0;
}