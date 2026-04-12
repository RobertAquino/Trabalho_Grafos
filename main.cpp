#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include "graph.hpp"
#include "parser.hpp"

using namespace std;

int main()
{

    vector<Operation> operations = readOperations("operations.csv");
    if (operations.empty())
        return 1;
    cout << "Total de operacoes carregadas: " << operations.size() << endl;

    Digraph graph(operations.size());
    graph.nodes = operations;

    /*Adiciona arestas representando a precedência lógica*/
    for (size_t i = 0; i < graph.nodes.size() - 1; i++)
    {
        if (graph.nodes[i].jobId == graph.nodes[i + 1].jobId)
        {
            graph.addEdge(i, i + 1);
        }
    }

    /*Agrupa operações por máquina*/
    map<string, vector<int>> machineUsage;
    for (const auto &op : graph.nodes)
    {
        machineUsage[op.machineId].push_back(op.id);
    }

    return 0;
}