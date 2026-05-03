#ifndef BUSCA_LOCAL_HPP
#define BUSCA_LOCAL_HPP
#include "Estruturas.hpp"
#include <vector>

class Solucao
{
public:
    std::vector<std::vector<int>> matriz_solucao;

    int custo_solucao = 0;

    void imprime_solucao();
};

double motorBusca(Instancia &instancia, const std::vector<JobInfo> &lista_jobs,
                  const std::vector<Operacao> lista_operacoes, double &makespan, int &iteracao);
#endif