//
// Created by Mike on 16.02.2019.
//

#include "Graph.h"
#include "graph_algorithm.h"


/*
 * Constructor/Destructor and Transform
 */

Graph::Graph(std::string g) {}

Graph::Graph(std::vector<std::vector<int>> g) {}

Graph::~Graph() = default;

std::vector<std::vector<int> > Graph::g6_to_matrix(std::string g6) {
    return std::vector<std::vector<int> >();
}

std::vector<std::vector<int> > Graph::g6_to_adjacency_list(std::string g6) {
    return std::vector<std::vector<int> >();
}

/*
 * Check for Euler
 */

bool Graph::is_euler() {
    if (this->_is_euler != -1)
        return bool(this->_is_euler);
    // 1. Количество вершин с нечетной степенью меньше или равно двум.
    for (const auto &vec: this->graph) {
        if (vec.size() % 2 == 1)
            return false;
    }
    // 2. Все компоненты связности кроме, может быть одной, не содержат ребер.
    int flag = false;
    for (auto comp_size: this->components()) {
        if (comp_size != 0) {
            if (flag)
                return false;
            flag = true;
        }
    }

    return true;
}

std::vector<int> Graph::components() {
    std::vector<bool> used(this->size, false);
    std::vector<int> result;
    for (int i = 0; i < this->size; i++) {
        if (!used[i]) {
            int deep = galgo::dfs(this, i, &used);
            result.push_back(deep);
        }
    }
    return result;
}
/*
*** Check for Hamilton ***
*/

bool dirac(Graph *graph) {
    if (graph->size >= 3) {
        for (const auto &vertex: graph->graph) {
            if (vertex.size() < graph->size / 2) {
                return false;
            }
        }
    }
    return true;
}

bool Graph::is_gamilton() {
    bool result;
    // Дирак
    result = dirac(this);
    if (result)
        return result;
    // Оре

    // Поша

    // Редеи-Камиона

    // Гуйя-Ури

    // Хватала

    return false;
}
