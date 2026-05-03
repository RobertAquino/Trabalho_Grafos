#ifndef AVALIADOR_HPP
#define AVALIADOR_HPP

#include <vector>
#include <string>

void calculaQuantidadesComponentes(int &n_jobs, int &n_maquinas, const std::vector<Operacao> &lista_operacoes);

std::vector<double> calcula_custo_total(Instancia &instancia, double &makespan, std::vector<JobInfo> lista_job, std::vector<Operacao> lista_operacoes);

#endif