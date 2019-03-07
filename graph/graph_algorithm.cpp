//
// Created by Mike on 19.02.2019.
//

#include "graph_algorithm.h"


int galgo::dfs(Graph *graph, int vertex, std::vector<bool> *used) {
    (*used)[vertex] = true;
    int deep = 1;
    for (const auto &neighbour: graph->graph[vertex]) {
        if (!(*used)[neighbour]) {
            deep += dfs(graph, neighbour, used);
        }
    }
    return deep;
}