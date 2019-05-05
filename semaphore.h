//
// Created by Mike on 16.02.2019.
//

#ifndef NIR_SEMAPHORE_H
#define NIR_SEMAPHORE_H


#include <mutex>
#include <condition_variable>

class semaphore {
public:
    explicit semaphore (int count_ = 0)
            : count(count_) {}

    inline void notify()
    {
        std::unique_lock<std::mutex> lock(mtx);
        count++;
        cv.notify_one();
    }

    inline void wait()
    {
        std::unique_lock<std::mutex> lock(mtx);

        while(count == 0){
            cv.wait(lock);
        }
        count--;
    }

    inline void init(int count_ = 0) {
        this->count = count_;
    }
    int count;
private:
    std::mutex mtx;
    std::condition_variable cv;

};


#endif //NIR_SEMAPHORE_H
