#ifndef AVALIADOR_HPP
#define AVALIADOR_HPP

#include "Estruturas.hpp"
#include <vector>
#include <string>

void calculaQuantidadesComponentes(int &n_jobs, int &n_maquinas, const std::vector<Operacao> &lista_operacoes);

std::vector<double> avaliador(std::vector<int> &lista_ordenacao, std::vector<int> &predecessor_critico, int n_jobs, const Instancia &instancia,
                              const std::vector<JobInfo> &lista_job, const std::vector<Operacao> &lista_operacoes, double &makespan);

std::vector<double> calcula_custo_total(std::vector<int> &lista_ordenacao, std::vector<int> &predecessor_critico, Instancia &instancia, double &makespan,
                                        const std::vector<JobInfo> &lista_job, const std::vector<Operacao> &lista_operacoes);

void resolve_grafo_professor();
#endif