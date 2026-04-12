#include "graph.hpp"

Digraph::Digraph(int num_nodes) : n(num_nodes) {
    adj.resize(n);
    in_degree.assign(n, 0);
}

void Digraph::addEdge(int u, int v) {
    adj[u].push_back(v);
    in_degree[v]++;
}