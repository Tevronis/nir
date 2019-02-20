//
// Created by Mike on 19.02.2019.
//

#ifndef NIR_GRAPH_ALGORITHM_H
#define NIR_GRAPH_ALGORITHM_H

#include "Graph.h"

namespace galgo {
    int dfs(Graph *graph, int vertex, std::vector<bool> *used);
}

#endif //NIR_GRAPH_ALGORITHM_H
