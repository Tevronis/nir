//
// Created by Mike on 16.02.2019.
//

#include <iostream>
#include "Graph.h"
#include "graph_algorithm.h"


/*
 * Constructor/Destructor and Transform
 */

Graph::Graph(std::string g) {
    std::cout << "Graph constructor from string" << std::endl;
    this->graph = g6_to_adjacency_list(g);
}

Graph::Graph(std::vector<std::vector<int>> g) {
    // Read graph from matrix to adjacency list
}

Graph::~Graph() = default;

std::vector<std::vector<int> > Graph::g6_to_matrix(std::string &g6) {
    std::cout << "Call function g6_to_matrix" << std::endl;
    int n = (int(g6[0]) - 63);
    std::vector<std::vector<int> > result(n, std::vector<int>(n, 0));

    int row = 0;
    int col = 1;
    for (int i = 1; i < g6.size(); i++) {
        int ch = g6[i] - 63;
        int bit = 0;
        for (int ibit = 5; ibit >= 0; --ibit) {
            bit = (ch >> ibit) & 1;
            result[row][col] = bit;
            result[col][row] = bit;
            row++;
            if (row == col) {
                row = 0;
                col++;
            }
            if (col == n)
                return result;
        }
    }

    return std::vector<std::vector<int> >();
}

std::vector<std::vector<int> > Graph::g6_to_adjacency_list(std::string &g6) {
    std::cout << "Call function g6_to_adjacency_list" << std::endl;
    int n = (int(g6[0]) - 63);
    std::vector<std::vector<int> > result(n);

    int row = 0;
    int col = 1;
    for (int i = 1; i < g6.size(); i++) {
        int ch = g6[i] - 63;
        for (int ibit = 5; ibit >= 0; --ibit) {
            if ((ch >> ibit) & 1) {
                result[row].push_back(col);
                result[col].push_back(row);
            }
            row++;
            if (row == col) {
                row = 0;
                col++;
            }
            if (col == n) {
                return result;
            }
        }
    }
    return std::vector<std::vector<int> >();
}


std::vector<std::vector<int> > Graph::adjacency_list_to_matrix(std::vector<std::vector<int> > &graph) {
    std::cout << "Call adjacency_list_to_matrix" << std::endl;
    std::vector<std::vector<int> > result(graph.size(), std::vector<int>(graph.size()));

    for (int row = 0; row < graph.size(); ++row) {
        for (int col: graph[row]) {
            result[row][col] = 1;
            result[col][row] = 1;
        }
    }

    return result;
}

std::vector<std::vector<int> > Graph::matrix_to_adjacency_list(std::vector<std::vector<int> > &graph) {
    std::cout << "Call matrix_to_adjacency_list" << std::endl;
    std::vector<std::vector<int> > result(graph.size(), std::vector<int>());

    for (int i = 0; i < graph.size(); ++i) {
        for (int j = 0; j < graph.size(); ++j) {
            if (graph[i][j]) {
                result[i].push_back(j);
                result[j].push_back(i);
            }
        }
    }

    return result;
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

void Graph::print() {
    std::vector<std::vector<int> > g = this->adjacency_list_to_matrix(graph);

    for (int i = 0; i < g.size(); ++i, std::cout << std::endl) {
        for (int j = 0; j < g.size(); ++j) {
            std::cout << g[i][j] << " ";
        }
    }
}
