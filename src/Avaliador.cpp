#include "../Bibliotecas/Avaliador.hpp"
#include "../Bibliotecas/Estruturas.hpp"
#include "../Bibliotecas/Parser_Operation.hpp"
#include "../Bibliotecas/Parser_JobInfo.hpp"
#include "../Bibliotecas/Parser_Setup.hpp"
#include <queue>
#include <vector>

using namespace std;

// calcula o máximo entre dois valores
double max(double a, double b)
{
    return (a > b) ? a : b;
}

// Calcula a quantidade de máquinas e jobs de uma instância
void calculaQuantidadesComponentes(int &n_jobs, int &n_maquinas, const vector<Operacao> &lista_operacoes)
{
    int max_job = -1;
    int max_mach = -1;

    for (size_t i = 0; i < lista_operacoes.size(); i++)
    {
        if (lista_operacoes[i].id_job > max_job)
            max_job = lista_operacoes[i].id_job;

        if (lista_operacoes[i].maquina > max_mach)
            max_mach = lista_operacoes[i].maquina;
    }
    n_jobs = max_job + 1;
    n_maquinas = max_mach + 1;
}

/*
Realiza a avaliação da heurística escolhida, utilizando listas de adjacência
para calcular o makespan e tempo final de cada job.
*/
vector<double> avaliador(vector<int> &lista_ordenacao, vector<int> &predecessor_critico, int n_jobs, const Instancia &instancia,
                         const vector<JobInfo> &lista_job, const vector<Operacao> &lista_operacoes, double &makespan)
{
    lista_ordenacao.clear();
    int total_operations = lista_operacoes.size();
    predecessor_critico.assign(total_operations, -1);

    vector<int> grau_entrada;
    grau_entrada.resize(total_operations, 0);
    queue<int> fila;
    vector<double> tempo_final_job(n_jobs, 0);
    vector<double> tempo_inicio_operacao(total_operations, 0);
    vector<double> tempo_fim_operacao(total_operations, 0);

    // Mapeia o grau de entrada de cada operação
    for (int i = 0; i < total_operations; i++)
    {
        if (instancia.jobAntecessor[i] != -1)
            grau_entrada[i]++;

        if (instancia.machAntecessor[i] != -1)
            grau_entrada[i]++;
    }

    // Coloca na fila todas as operações que possuem grau 0 de entrada
    for (int i = 0; i < total_operations; i++)
    {
        if (instancia.jobAntecessor[i] == -1 && instancia.machAntecessor[i] == -1)
        {
            fila.push(i);
        }
    }

    int operacoes_processadas = 0;

    while (!fila.empty())
    {
        int atual = fila.front();
        fila.pop();

        lista_ordenacao.push_back(atual);

        double inicio_op_atual = 0;
        int culpado_job = -1;

        // Decide o tempo inicial de uma operação, sendo o máximo entre o tempo de chegada, o tempo da operação
        // final do mesmo job ou o tempo final da ultima operacao a utilizar a máquina
        if (instancia.jobAntecessor[atual] != -1)
        {
            inicio_op_atual = tempo_fim_operacao[instancia.jobAntecessor[atual]];
            culpado_job = instancia.jobAntecessor[atual];
        }
        else
        {
            inicio_op_atual = lista_job[lista_operacoes[atual].id_job].release_date;
        }

        double desocupa_maquina_atual = 0;
        int culpado_mach = -1;

        if (instancia.machAntecessor[atual] != -1)
        {
            desocupa_maquina_atual = tempo_fim_operacao[instancia.machAntecessor[atual]];
            culpado_mach = instancia.machAntecessor[atual];
        }

        // Atribui o tempo de inicío dos jobs, e mapeia o vetor de predecessores, que será utilizado para
        // se conhecer o caminho máximo
        if (inicio_op_atual > desocupa_maquina_atual)
        {
            tempo_inicio_operacao[atual] = inicio_op_atual;
            predecessor_critico[atual] = culpado_job;
        }
        else
        {
            tempo_inicio_operacao[atual] = desocupa_maquina_atual;
            predecessor_critico[atual] = culpado_mach;
        }

        double duracao = instancia.tempoProcessamento[atual];

        // Calcula o tempo de setup
        if (instancia.machAntecessor[atual] != -1)
            duracao += instancia.matriz_setup[instancia.operToMach[atual]]
                                             [lista_operacoes[instancia.machAntecessor[atual]].id_job][lista_operacoes[atual].id_job];

        double tempo_final = duracao + tempo_inicio_operacao[atual];
        tempo_fim_operacao[atual] = tempo_final;

        // Se a operação atual possuir um sucessor e este tiver grau de entrada
        // 0, coloca ele na fila, senão atribui ao vetor de tempo final o seu valor
        if (instancia.jobSucessor[atual] != -1)
        {
            grau_entrada[instancia.jobSucessor[atual]]--;
            if (grau_entrada[instancia.jobSucessor[atual]] == 0)
            {
                fila.push(instancia.jobSucessor[atual]);
            }
        }
        else
        {
            tempo_final_job[lista_operacoes[atual].id_job] = tempo_final;
        }

        if (instancia.machSucessor[atual] != -1)
        {
            grau_entrada[instancia.machSucessor[atual]]--;
            if (grau_entrada[instancia.machSucessor[atual]] == 0)
            {
                fila.push(instancia.machSucessor[atual]);
            }
        }
        operacoes_processadas++;
    }

    // Se todas operações não forem executadas(ciclos) atribui uma makespan absurdo
    if (operacoes_processadas < total_operations)
    {
        makespan = 999999999.0;
        vector<double> tempo_invalido(n_jobs, 999999999.0);
        return tempo_invalido;
    }
    // Calcula o makespan
    for (int i = 0; i < n_jobs; i++)
    {
        if (makespan < tempo_final_job[i])
            makespan = tempo_final_job[i];
    }
    return tempo_final_job;
}

// Calcula o custo total dos jobs a partir das penalidades
vector<double> calcula_custo_total(vector<int> &lista_ordenacao, vector<int> &predecessor_critico, Instancia &instancia, double &makespan, const vector<JobInfo> &lista_job, const vector<Operacao> &lista_operacoes)
{
    vector<double> tempo_final_job;
    int n_jobs = lista_job.size();
    tempo_final_job = avaliador(lista_ordenacao, predecessor_critico, n_jobs, instancia, lista_job, lista_operacoes, makespan);
    vector<double> tempo_multas_job;
    tempo_multas_job.resize(tempo_final_job.size(), 0);

    // Se tiver ciclo, o algoritmo dá um tempo de multa absurdo
    if (makespan >= 999999990.0)
    {
        for (int i = 0; i < n_jobs; i++)
        {
            tempo_multas_job[i] = 999999999.0;
        }
        return tempo_multas_job;
    }

    for (int i = 0; i < n_jobs; i++)
    {
        double job_atual = tempo_final_job[i];
        // Se não houver penalidade, o valor permanece o mesmo
        if (job_atual == lista_job[i].due_date)
            tempo_multas_job[i] = tempo_final_job[i];

        double diferenca = lista_job[i].due_date - tempo_final_job[i];

        // Se houver atraso, calculamos a multa em relação o periodo de atraso
        if (diferenca < 0)
        {
            double temp = (diferenca * lista_job[i].tardiness_penalty) * (-1);
            tempo_multas_job[i] = tempo_final_job[i] + temp;
        }

        // Se houver adiamento, calculamos a multa em relação ao periodo de adiamento
        else
        {
            double temp = diferenca * lista_job[i].earliness_penalty;
            tempo_multas_job[i] = tempo_final_job[i] + temp;
        }
    }
    return tempo_multas_job;
}
// Ordenação topológica para o grafo fixo específico
void resolve_grafo_professor()
{
    int n = 15;

    vector<vector<int>> adj(n + 1);
    vector<int> grau_entrada(n + 1, 0);

    vector<pair<int, int>> arestas = {
        {4, 3}, {4, 1}, {7, 10}, {7, 8}, {10, 3}, {10, 1}, {8, 2}, {8, 14}, {3, 5}, {1, 11}, {1, 2}, {2, 12}, {5, 15}, {5, 14}, {11, 15}, {11, 14}, {14, 13}, {12, 13}, {12, 9}, {15, 6}, {15, 9}, {13, 6}};

    for (auto aresta : arestas)
    {
        adj[aresta.first].push_back(aresta.second);
        grau_entrada[aresta.second]++;
    }

    queue<int> fila;
    vector<int> tempo_fim(n + 1, 0);
    vector<int> predecessor(n + 1, -1);
    vector<int> ordem_topologica;

    for (int i = 1; i <= n; i++)
    {
        if (grau_entrada[i] == 0)
            fila.push(i);
    }

    while (!fila.empty())
    {
        int atual = fila.front();
        fila.pop();
        ordem_topologica.push_back(atual);

        for (int vizinho : adj[atual])
        {
            if (tempo_fim[atual] + 1 > tempo_fim[vizinho])
            {
                tempo_fim[vizinho] = tempo_fim[atual] + 1;
                predecessor[vizinho] = atual;
            }
            grau_entrada[vizinho]--;
            if (grau_entrada[vizinho] == 0)
                fila.push(vizinho);
        }
    }

    int comprimento_maximo = 0;
    int ultima_op_global = -1;

    for (int i = 1; i <= n; i++)
    {
        if (tempo_fim[i] > comprimento_maximo)
        {
            comprimento_maximo = tempo_fim[i];
            ultima_op_global = i;
        }
    }

    vector<int> caminho_global;
    int rastreio = ultima_op_global;

    while (rastreio != -1)
    {
        caminho_global.push_back(rastreio);
        rastreio = predecessor[rastreio];
    }
    reverse(caminho_global.begin(), caminho_global.end());

    vector<int> finais_de_linha_visuais = {6, 13, 9};

    ofstream arquivo_relatorio("resposta_grafos_professor.txt");
    if (arquivo_relatorio.is_open())
    {
        arquivo_relatorio << "=========================================================\n";
        arquivo_relatorio << "RESOLUCAO DA PROVA: GRAFO TEORICO\n";
        arquivo_relatorio << "=========================================================\n\n";

        arquivo_relatorio << "Ordem Topologica dos Vertices:\n -> ";
        for (size_t i = 0; i < ordem_topologica.size(); i++)
        {
            arquivo_relatorio << ordem_topologica[i];
            if (i < ordem_topologica.size() - 1)
                arquivo_relatorio << ", ";
        }
        arquivo_relatorio << "\n\n";

        arquivo_relatorio << "Comprimento do Caminho Maximo: " << comprimento_maximo << " saltos (arestas)\n\n";

        arquivo_relatorio << "Caminho Maximo Global (Minimal ao Maximal):\n -> ";
        for (size_t i = 0; i < caminho_global.size(); i++)
        {
            arquivo_relatorio << caminho_global[i];
            if (i < caminho_global.size() - 1)
                arquivo_relatorio << " -> ";
        }
        arquivo_relatorio << "\n\n";

        arquivo_relatorio << "Caminhos Maximos para o Final de Cada Linha do Desenho:\n";
        for (int maximal : finais_de_linha_visuais)
        {
            vector<int> caminho_individual;
            int r = maximal;

            while (r != -1)
            {
                caminho_individual.push_back(r);
                r = predecessor[r];
            }
            reverse(caminho_individual.begin(), caminho_individual.end());

            arquivo_relatorio << " -> Final da Linha (No " << maximal << "): ";
            for (size_t i = 0; i < caminho_individual.size(); i++)
            {
                arquivo_relatorio << caminho_individual[i];
                if (i < caminho_individual.size() - 1)
                    arquivo_relatorio << " -> ";
            }
            arquivo_relatorio << " (Comprimento: " << tempo_fim[maximal] << ")\n";
        }

        arquivo_relatorio << "\n";
        arquivo_relatorio.close();
    }
}