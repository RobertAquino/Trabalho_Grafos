#include "../Bibliotecas/BuscaLocal.hpp"
#include "../Bibliotecas/Estruturas.hpp"
#include "../Bibliotecas/Avaliador.hpp"
#include <iostream>
#include <vector>

using namespace std;

void swap(int &a, int &b)
{
    int temp = a;
    a = b;
    b = temp;
}
void alteraMachAntecessor(Instancia &instancia, Solucao &solucao)
{
    for (const auto &fila : solucao.matriz_solucao)
    {
        for (size_t i = 1; i < fila.size(); i++)
        {
            instancia.machAntecessor[fila[i]] = fila[i - 1];
        }
    }
}
void alteraMachSucessor(Instancia &instancia, Solucao &solucao)
{
    for (const auto &fila : solucao.matriz_solucao)
    {
        for (size_t i = 0; i < fila.size() - 1; i++)
        {
            instancia.machSucessor[fila[i]] = fila[i + 1];
        }
    }
}
void transformaSolucaoInicial(vector<Operacao> lista_operacoes, Solucao &solucao)
{
    for (size_t i = 0; i < lista_operacoes.size(); i++)
    {
        solucao.matriz_solucao[lista_operacoes[i].maquina].push_back(lista_operacoes[i].id);
    }
}
double somatorioJob(vector<double> multas)
{
    double soma = 0;

    for (double c : multas)
        soma += c;

    return soma;
}
vector<Solucao> geraVizinhos(Solucao atual)
{
    vector<Solucao> vizinhos;
    for (size_t maquina_atual = 0; maquina_atual < atual.matriz_solucao.size(); maquina_atual++)
    {
        int operacoes = atual.matriz_solucao[maquina_atual].size();

        for (int i = 0; i < operacoes; i++)
        {
            Solucao nova = atual;
            swap(atual.matriz_solucao[maquina_atual][i], atual.matriz_solucao[maquina_atual][i + 1]);
            vizinhos.push_back(atual);
        }
    }
    return vizinhos;
}

double motorBusca(Instancia &instancia, const vector<JobInfo> &lista_jobs,
                  const vector<Operacao> lista_operacoes, double &makespan, int &iteracao)
{
    vector<Solucao> vizinhança;
    bool melhorou = true;
    vector<double> multas_inicial = calcula_custo_total(instancia, makespan, lista_jobs, lista_operacoes);
    double custo_melhor_solucao = somatorioJob(multas_inicial);
    Solucao inicial;
    transformaSolucaoInicial(lista_operacoes, inicial);
    Solucao melhor_solucao = inicial;
    double soma_melhor_atual;
    while (melhorou)
    {
        melhorou = false;
        vizinhança = geraVizinhos(inicial);

        Solucao melhor_solucao_atual = vizinhança[0];
        alteraMachAntecessor(instancia, melhor_solucao_atual);
        alteraMachSucessor(instancia, melhor_solucao_atual);
        vector<double> multas_melhor_atual = calcula_custo_total(instancia, makespan, lista_jobs, lista_operacoes);
        soma_melhor_atual = somatorioJob(multas_melhor_atual);

        for (size_t i = 1; vizinhança.size(); i++)
        {
            Solucao proxima_solucao = vizinhança[i];
            alteraMachAntecessor(instancia, proxima_solucao);
            alteraMachSucessor(instancia, proxima_solucao);
            vector<double> multas_proxima_solucao = calcula_custo_total(instancia, makespan, lista_jobs, lista_operacoes);
            double soma_proxima_solucao = somatorioJob(multas_melhor_atual);

            if (soma_melhor_atual > soma_proxima_solucao)
            {
                melhor_solucao_atual = proxima_solucao;
                soma_melhor_atual = soma_proxima_solucao;
            }
        }

        if (custo_melhor_solucao > soma_melhor_atual)
        {
            melhor_solucao = melhor_solucao_atual;
            melhorou = true;
            iteracao++;
        }
    }
    return soma_melhor_atual;
}