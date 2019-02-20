//
// Created by Mike on 16.02.2019.
//

#ifndef NIR_GRAPH_H
#define NIR_GRAPH_H

#include <vector>
#include <string>


class Graph {
public:
    unsigned long long size;
    std::vector<std::vector<int> > graph;
public:
    explicit Graph(std::string g);
    explicit Graph(std::vector<std::vector<int> > g);

    virtual ~Graph();

    static std::vector<std::vector<int> > g6_to_matrix(std::string g6);
    static std::vector<std::vector<int> > g6_to_adjacency_list(std::string g6);

    bool is_euler();
    bool is_gamilton();
    std::vector<int> components();


private:
    int _is_gamilton = -1;
    int _is_euler = -1;
    int count_of_gamilton_cycle = 0;
    int count_of_euler_cycle = 0;
};


#endif //NIR_GRAPH_H
