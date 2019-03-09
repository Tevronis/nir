//
// Created by Mike on 16.02.2019.
//

#ifndef NIR_GRAPH_H
#define NIR_GRAPH_H

#include <vector>
#include <string>

/*
 * By default, class Graph contains a graph in adjacency list
 *
 * */
class Graph {
public:
    unsigned long long size;
    std::vector<std::vector<int> > graph;
public:
    explicit Graph(Graph *g);
    explicit Graph(std::string g);
    explicit Graph(std::vector<std::vector<int> > g);

    virtual ~Graph();
    // Transform
    static std::vector<std::vector<int> > g6_to_matrix(std::string &g6);
    static std::vector<std::vector<int> > g6_to_adjacency_list(std::string &g6);
    static std::vector<std::vector<int> > matrix_to_adjacency_list(std::vector<std::vector<int> > &graph);
    static std::vector<std::vector<int> > adjacency_list_to_matrix(std::vector<std::vector<int> > &graph);
    // Algorithms
    bool is_euler();
    bool is_gamilton();
    std::vector<int> components();
    Graph get_closure();
    // Visualization
    void print();


private:
    // Flags
    int _is_gamilton = -1;
    int _is_euler = -1;
    // Statistic
    int count_of_gamilton_cycle = 0;
    int count_of_euler_cycle = 0;
};


#endif //NIR_GRAPH_H
