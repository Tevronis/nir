cmake_minimum_required(VERSION 3.12)
project(nir)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")

add_executable(nir main.cpp graph.cpp graph.h task_queue.cpp task_queue.h semaphore.cpp semaphore.h data.cpp data.h)