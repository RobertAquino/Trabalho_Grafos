#include "../Bibliotecas/Estruturas.hpp"
#include <queue>

void avaliador(int total_operations, string caminho, int n_jobs, int n_maquinas)
{
    Instancia instancia;
    instancia.inicializaEstruturas(caminho, n_maquinas, n_jobs);
    vector<int> grau_entrada;
    grau_entrada.resize(total_operations, 0);
    queue<int> fila;
    vector<int> tempo_fim_job(n_jobs, 0);
    vector<int> tempo_inicio_job(n_jobs, 0);
    int duracao;
    int tempo_total_instancia = 0;
    int tempo_atual;
    int fim_atual;
    vector<bool> visitados;
    visitados.resize(total_operations, false);

    for (int i = 0; i < total_operations; i++)
    {
        if (instancia.jobAntecessor[i] != -1)
            grau_entrada[i]++;

        if (instancia.machAntecessor[i] != -1)
            grau_entrada[i]++;
    }

    for (int i = 0; i < total_operations; i++)
    {
        if (instancia.jobAntecessor[i] == -1 && instancia.machAntecessor[i] == -1)
            fila.push(i);
    }

    while (!fila.empty())
    {
        int atual = fila.front();
        fila.pop();
        visitados[atual] = true;

        duracao = instancia.tempoProcessamento[atual];

        if (instancia.machAntecessor[atual] != -1)
            duracao += instancia.matriz_setup[instancia.operToMach[atual]][instancia.machAntecessor[atual]][atual];

        fim_atual = duracao + tempo_inicio_job[atual];
        tempo_fim_job[atual] = fim_atual;

        // falta lidar com os tempos de inicial e final de cada job

        int jobSuc = instancia.jobSucessor[atual];
        int machSuc = instancia.machSucessor[atual];

        if (jobSuc != -1)
        {
        }

        if (machSuc != -1)
        {
        }

        tempo_inicio_job[jobSuc] = max if (instancia.jobSucessor[atual] != -1)
            grau_entrada[instancia.jobSucessor[atual]]--;

        if (instancia.machSucessor[atual] != -1)
            grau_entrada[instancia.machSucessor[atual]]--;

        // falta criar uma forma de conjunto fechado para os jobs
        for (int i = 0; i < total_operations; i++)
        {
            if (grau_entrada[i] == 0 && visitados[i] != true)
                fila.push(i);
        }
    }
}
int max(int a, int b)
{
    return (a > b) ? a : b;
}