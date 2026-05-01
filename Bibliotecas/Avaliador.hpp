#ifndef AVALIADOR_HPP
#define AVALIADOR_HPP

#include <vector>
#include <string>

// Avisa ao programa que essa função existe em algum lugar (no .cpp)
std::vector<double> calcula_custo_total(std::string arquivo_jobs,
                                        std::string arquivo_operacoes,
                                        std::string arquivo_setup,
                                        double &makespan, int &n_jobs,
                                        int &n_maquinas);
void calculaQuantidadesComponentes(int &n_jobs, int &n_maquinas, const vector<Operacao> &lista_operacoes);
#endif