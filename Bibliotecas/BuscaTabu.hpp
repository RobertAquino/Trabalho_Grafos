#ifndef BUSCA_TABU_HPP
#define BUSCA_TABU_HPP

#include "Estruturas.hpp"
#include <vector>

class Solucao
{
public:
    std::vector<std::vector<int>> matriz_solucao;

    void imprime_solucao();
};

void transformaSolucaoSPT(const Instancia &instancia, const std::vector<JobInfo> &lista_jobs, const std::vector<Operacao> &lista_operacoes, Solucao &solucao, int n_maquinas);

void alteraMachAntecessor(Instancia &instancia, Solucao &solucao);
void alteraMachSucessor(Instancia &instancia, Solucao &solucao);

// Adicionada a lista_ordenacao no começo e garantido o std::vector
double motorBusca(std::vector<int> &lista_ordenacao, Instancia &instancia, const std::vector<JobInfo> &lista_jobs,
                  const std::vector<Operacao> &lista_operacoes, double &makespan, int &iteracao,
                  Solucao solucao_inicial);

#endif