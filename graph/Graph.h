//
// Created by Mike on 16.02.2019.
//

#ifndef NIR_GRAPH_H
#define NIR_GRAPH_H

#include <vector>
#include <string>


class Graph {
    // std::vector<std::vector<int> > graph;
public:
    explicit Graph(std::string g);
    explicit Graph(std::vector<std::vector<int> > g);

    virtual ~Graph();

    static std::vector<std::vector<int> > g6_to_matrix(std::string g6);
    static std::vector<std::vector<int> > g6_to_adjacency_list(std::string g6);

private:

};


#endif //NIR_GRAPH_H
