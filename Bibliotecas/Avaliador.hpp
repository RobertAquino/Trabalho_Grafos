#ifndef AVALIADOR_HPP
#define AVALIADOR_HPP

#include "Estruturas.hpp"
#include <vector>
#include <string>

void calculaQuantidadesComponentes(int &n_jobs, int &n_maquinas, const std::vector<Operacao> &lista_operacoes);

// Adicionado o predecessor_critico e os const nas listas
std::vector<double> avaliador(std::vector<int> &lista_ordenacao, std::vector<int> &predecessor_critico, int n_jobs, const Instancia &instancia,
                              const std::vector<JobInfo> &lista_job, const std::vector<Operacao> &lista_operacoes, double &makespan);

// Adicionado o predecessor_critico e os const nas listas
std::vector<double> calcula_custo_total(std::vector<int> &lista_ordenacao, std::vector<int> &predecessor_critico, Instancia &instancia, double &makespan,
                                        const std::vector<JobInfo> &lista_job, const std::vector<Operacao> &lista_operacoes);

// Nova função para gerar o .txt dos Caminhos e Makespan
void extrai_relatorio_caminhos(std::string nome_instancia, double makespan_final, const std::vector<int> &ordem_topologica, int n_jobs, const Instancia &instancia, const std::vector<JobInfo> &lista_job, const std::vector<Operacao> &lista_operacoes);

void resolve_grafo_professor();
#endif