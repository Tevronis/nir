//
// Created by Mike on 16.02.2019.
//

#ifndef NIR_GRAPH_H
#define NIR_GRAPH_H

#include <vector>
#include <string>

using namespace std;

class graph {
public:
    explicit graph(string g);
    explicit graph(vector<vector<int>> g);

    virtual ~graph();

    static vector<vector<int>> g6_to_matrix(string g6);
    static vector<vector<int>> g6_to_adjacency_list(string g6);

private:

};


#endif //NIR_GRAPH_H
