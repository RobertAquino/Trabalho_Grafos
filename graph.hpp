#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <map>
#include <string>

using namespace std;

struct Operation {
    int id;
    string jobId;
    string machineId;
    int processTime;
    int SetupTime = 0;
    int StartTime = 0;
    int EndTime = 0;
};

struct Digraph {
    int n;
    vector<vector<int>> adj;
    vector<int> in_degree;
    vector<Operation> nodes;

    Digraph(int num_nodes);
    void addEdge(int u, int v);
};

#endif