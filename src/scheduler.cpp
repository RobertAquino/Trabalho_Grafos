/*#include <queue>
#include "../Bibliotecas/graph.hpp"

void realizarCaminhadaTopologica(DigrafoJobShop& g) {
    std::queue<int> fila;
    std::vector<int> ordem;

    // Inicia com elementos minimais (in_degree == 0) [cite: 24, 25]
    for(int i=0; i < g.n_vertices; i++) {
        if(g.in_degree[i] == 0) fila.push(i);
    }

    while(!fila.empty()) {
        int u = fila.front();
        fila.pop();
        ordem.push_back(u);

        for(int v : g.adj[u]) {
            // Lógica do Caminho Máximo (Flow Time):
            // tempoTermino[v] = max(tempoTermino[v], tempoTermino[u] + peso)
            g.in_degree[v]--;
            if(g.in_degree[v] == 0) fila.push(v);
        }
    }
    // Mostrar a ordem percorrida conforme pedido [cite: 6]
}*/