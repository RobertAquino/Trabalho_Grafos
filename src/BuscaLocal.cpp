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
        if (fila.empty())
            continue;

        instancia.machAntecessor[fila[0]] = -1;
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
        if (fila.empty())
            continue;

        instancia.machSucessor[fila.back()] = -1;
        for (size_t i = 0; i < fila.size() - 1; i++)
        {
            instancia.machSucessor[fila[i]] = fila[i + 1];
        }
    }
}
void transformaSolucaoInicial(const Instancia &instancia, const vector<Operacao> &lista_operacoes, Solucao &solucao, int n_maquinas)
{
    solucao.matriz_solucao.clear();
    solucao.matriz_solucao.resize(n_maquinas);

    for (size_t i = 0; i < lista_operacoes.size(); i++)
    {
        if (instancia.machAntecessor[i] == -1)
        {
            int maquina_atual = lista_operacoes[i].maquina;
            int op_atual = i;
            int trava_seguranca = 0;

            while (op_atual != -1 && trava_seguranca < (int)lista_operacoes.size())
            {
                solucao.matriz_solucao[maquina_atual].push_back(op_atual);
                op_atual = instancia.machSucessor[op_atual];
                trava_seguranca++;
            }
        }
    }
}
double somatorioJob(vector<double> multas)
{
    double soma = 0;

    for (double c : multas)
        soma += c;

    return soma;
}
vector<Solucao> geraVizinhos(Solucao &atual)
{
    vector<Solucao> vizinhos;
    for (size_t maquina_atual = 0; maquina_atual < atual.matriz_solucao.size(); maquina_atual++)
    {
        int operacoes = atual.matriz_solucao[maquina_atual].size();

        for (int i = 0; i < operacoes - 1; i++)
        {
            Solucao nova = atual;
            swap(nova.matriz_solucao[maquina_atual][i], nova.matriz_solucao[maquina_atual][i + 1]);
            vizinhos.push_back(nova);
        }
    }
    return vizinhos;
}

double motorBusca(Instancia &instancia, const vector<JobInfo> &lista_jobs,
                  const vector<Operacao> lista_operacoes, double &makespan, int &iteracao,
                  int n_maquinas)
{
    vector<Solucao> vizinhanca;
    bool melhorou = true;

    // Variável limpa para não sujar o original
    double makespan_temp = 0;

    vector<double> multas_inicial = calcula_custo_total(instancia, makespan_temp, lista_jobs, lista_operacoes);
    double custo_melhor_solucao = somatorioJob(multas_inicial);
    double makespan_melhor_solucao = makespan_temp;

    Solucao inicial;
    transformaSolucaoInicial(instancia, lista_operacoes, inicial, n_maquinas);
    Solucao melhor_solucao = inicial;

    double soma_melhor_atual;
    double makespan_melhor_atual;

    while (melhorou)
    {
        melhorou = false;
        vizinhanca = geraVizinhos(inicial);

        if (vizinhanca.empty())
            break;

        Solucao melhor_solucao_atual = vizinhanca[0];
        alteraMachAntecessor(instancia, melhor_solucao_atual);
        alteraMachSucessor(instancia, melhor_solucao_atual);
        vector<double> multas_melhor_atual = calcula_custo_total(instancia, makespan_temp, lista_jobs, lista_operacoes);
        soma_melhor_atual = somatorioJob(multas_melhor_atual);
        makespan_melhor_atual = makespan_temp; // <-- SALVA O MAKESPAN AQUI

        for (size_t i = 1; i < vizinhanca.size(); i++)
        {
            Solucao proxima_solucao = vizinhanca[i];
            alteraMachAntecessor(instancia, proxima_solucao);
            alteraMachSucessor(instancia, proxima_solucao);

            // CORREÇÃO: Usa makespan_temp e NÃO a variável makespan global!
            vector<double> multas_proxima_solucao = calcula_custo_total(instancia, makespan_temp, lista_jobs, lista_operacoes);
            double soma_proxima_solucao = somatorioJob(multas_proxima_solucao);

            if (soma_melhor_atual > soma_proxima_solucao)
            {
                melhor_solucao_atual = proxima_solucao;
                soma_melhor_atual = soma_proxima_solucao;
                makespan_melhor_atual = makespan_temp; // <-- CORREÇÃO: SALVA O MAKESPAN DO VIZINHO AQUI
            }
        }

        if (custo_melhor_solucao > soma_melhor_atual)
        {
            melhor_solucao = melhor_solucao_atual;
            custo_melhor_solucao = soma_melhor_atual;
            makespan_melhor_solucao = makespan_melhor_atual;
            melhorou = true;
            iteracao++;
            inicial = melhor_solucao_atual;
        }
    }

    // DEVOLVE PARA A MAIN O MAKESPAN LIMPO
    makespan = makespan_melhor_solucao;

    alteraMachAntecessor(instancia, melhor_solucao);
    alteraMachSucessor(instancia, melhor_solucao);

    calcula_custo_total(instancia, makespan, lista_jobs, lista_operacoes);

    return custo_melhor_solucao;
}