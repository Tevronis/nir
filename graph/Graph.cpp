//
// Created by Mike on 16.02.2019.
//

#include <iostream>
#include <algorithm>
#include "Graph.h"
#include "graph_algorithm.h"


/*
 * Constructor/Destructor and Transform
 */

Graph::Graph(std::string g) {
    // std::cout << "Graph constructor from string" << std::endl;
    this->graph = g6_to_adjacency_list(g);
    this->size = this->graph.size();
}

Graph::Graph(std::vector<std::vector<int> > g) {
    // Read graph from matrix to adjacency list
    this->size = g.size();
    this->graph = std::vector<std::vector<int> >(g.size());
    for (int i = 0; i < g.size(); ++i) {
        for (int j = 0; j < g.size(); ++j) {
            if (g[i][j] == 1) {
                this->graph[i].push_back(j);
                // this->graph[j].push_back(i);
            }
        }
    }
}

Graph::Graph(Graph *g) {
    this->graph = g->graph;
    this->size = g->size;
    this->_is_hamilton = -1;
    this->_is_euler = -1;
}


Graph::~Graph() = default;

std::vector<std::vector<int> > Graph::g6_to_matrix(std::string &g6) {
    // std::cout << "Call function g6_to_matrix" << std::endl;
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
    // std::cout << "Call function g6_to_adjacency_list" << std::endl;
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
    // std::cout << "Call adjacency_list_to_matrix" << std::endl;
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
    // std::cout << "Call matrix_to_adjacency_list" << std::endl;
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

std::vector<std::vector<int> > Graph::get_euler_paths() {

    return std::vector<std::vector<int>>();
}

/*
void hamilton_paths(int v, Graph *graph, std::vector<int> *current_path, std::vector<bool> *used,
                    std::vector<std::vector<int> > *paths) {
    if (current_path->size() == graph->size + 1 && (*current_path)[0] == current_path->back()) {
        // Если 1 == back и путь состоит из всех вершин и замыкается на первой
        paths->push_back(*current_path);
        return;
    }
    (*used)[v] = true;
    for (int i = 0; i < graph->graph[v].size(); ++i) {
        auto &item = graph->graph[v][i];
        if (!(*used)[item] || (current_path->size() == graph->size && item == (*current_path)[0])) {
            current_path->push_back(item);
            hamilton_paths(item, graph, current_path, used, paths);
            current_path->pop_back();
        }
    }
    (*used)[v] = false;
}
 * */

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

std::vector<int> sequence_degree_vertex(Graph *graph) {
    /*
     * return: sorted vector with number of vertex | deg(result[i]) > deg(result[j]) : i < j
     * */
    std::vector<int> result;
    std::vector<std::pair<int, int> > pair_result;
    for (int idx = 0; idx < graph->size; ++idx) {
        auto &vlist = graph->graph[idx];
        std::pair<int, int> pair = std::make_pair(vlist.size(), idx);
        pair_result.push_back(pair);
    }

    sort(pair_result.begin(), pair_result.end());
    result.reserve(pair_result.size());
    for (auto p: pair_result) {
        result.push_back(p.second);
    }
    reverse(result.begin(), result.end());
    return result;
}

std::vector<int> get_not_adjacent_vertexes(Graph *graph, int v) {
    std::vector<int> result;
    for (int i = 0; i < graph->size; ++i) {
        bool candidate = true;
        for (auto ne: graph->graph[v]) {
            if (i == v or ne == i) {
                candidate = false;
                break;
            }
        }
        if (candidate) {
            result.push_back(i);
        }
    }
    return result;
}

Graph graph_closure(Graph *graph) {
    /*
     * Source: http://freeusermanuals.com/backend/web/manuals/1521810604HamiltonBondyAndChvatal.pdf
     * */
    Graph closure(graph);
    for (int j = 0; j < graph->size; ++j) {
        bool stop = true;
        for (auto v_source: sequence_degree_vertex(&closure)) {
            std::vector<int> not_adjacent = get_not_adjacent_vertexes(&closure, v_source);
            for (auto v_destination: not_adjacent) {
                // condition number 3: deg(v_destination) + deg(v_source) >= |graph.vertexes|
                if (closure.graph[v_destination].size() + closure.graph[v_source].size() >= closure.size) {
                    closure.graph[v_source].push_back(v_destination);
                    closure.graph[v_destination].push_back(v_source);
                    stop = false;
                }
            }
            if (!stop)
                break;
        }
        if (stop)
            break;
    }
    return closure;
}

Graph Graph::get_closure() {
    return graph_closure(this);
}

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

bool th_ore(Graph *graph) {
    if (graph->size >= 3) {
        for (int i = 0; i < graph->size; ++i) {
            auto &current_vertex = graph->graph[i];
            for (int j = 0; j < graph->size; ++j) {
                // Element j not in neighbourhood of current_vertex
                if(std::find(current_vertex.begin(), current_vertex.end(), j) == current_vertex.end()) {
                    // check condition: deg u + deg v >= n
                    if (current_vertex.size() + graph->graph[j].size() < graph->size) {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

bool th_posh(Graph *graph) {
    if (graph->size >= 3) {
        for (int k = 0; k < graph->size / 2; ++k) {
            for (const auto &vertex: graph->graph) {
                if (vertex.size() < k + 1) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool th_bondi_chvatal(Graph *graph) {
    if (graph->size >= 3) {
        Graph closure = graph_closure(graph);
        if (dirac(&closure) or th_ore(&closure)) {
            return true;
        }
    }
    return false;
}

bool Graph::is_hamilton() {
    if (this->_is_hamilton != -1)
        return bool(this->_is_hamilton);

    this->_is_hamilton = th_bondi_chvatal(this);

    return bool(this->_is_hamilton);
}

void hamilton_paths(int v, Graph *graph, std::vector<int> *current_path, std::vector<bool> *used,
                    std::vector<std::vector<int> > *paths) {
    if (current_path->size() == graph->size + 1 && (*current_path)[0] == current_path->back()) {
        // Если 1 == back и путь состоит из всех вершин и замыкается на первой
        paths->push_back(*current_path);
        return;
    }
    (*used)[v] = true;
    for (int i = 0; i < graph->graph[v].size(); ++i) {
        auto &item = graph->graph[v][i];
        if (!(*used)[item] || (current_path->size() == graph->size && item == (*current_path)[0])) {
            current_path->push_back(item);
            hamilton_paths(item, graph, current_path, used, paths);
            current_path->pop_back();
        }
    }
    (*used)[v] = false;
}


std::vector<std::vector<int> > Graph::get_hamilton_paths() {
    std::vector<std::vector<int> > result;
    if (this->is_hamilton()) {
        std::vector<int> current_path(1, 0);
        std::vector<bool> used(this->size);
        hamilton_paths(0, this, &current_path, &used, &result);
    }

    return result;
}

void Graph::print() {
    std::vector<std::vector<int> > g = this->adjacency_list_to_matrix(graph);

    for (int i = 0; i < g.size(); ++i, std::cout << std::endl) {
        for (int j = 0; j < g.size(); ++j) {
            std::cout << g[i][j] << " ";
        }
    }
}
