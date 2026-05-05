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

// Esta função é responsável por avaliar a heurística escolhida. Ela basicamente utiliza listas
// de adjacência para descobrir qual é o tempo final de todas as operações(makespan) e também o tempo
// final de cada job indivídual, isso será muito importante na hora do cálculo das penalidades
vector<double> avaliador(vector<int> &lista_ordenacao, vector<int> &predecessor_critico, int n_jobs, const Instancia &instancia,
                         const vector<JobInfo> &lista_job, const vector<Operacao> &lista_operacoes, double &makespan)
{
    // CORREÇÃO: Limpa a lista para garantir que não acumule a ordem das rodadas anteriores
    lista_ordenacao.clear();

    // Primeiro declaramos as estruturas de dados
    int total_operations = lista_operacoes.size();

    predecessor_critico.assign(total_operations, -1);

    // Esse vetor será utilizado para representar a quantidade de pendencias que uma operação tem
    vector<int> grau_entrada;
    grau_entrada.resize(total_operations, 0);
    // A fila conterá todos as operações que não dependem de uma operação anterior ou máquina
    queue<int> fila;
    // O tempo final da última operação de cada job
    vector<double> tempo_final_job(n_jobs, 0);
    // O tempo inicial de cada operação
    vector<double> tempo_inicio_operacao(total_operations, 0);
    // O tempo final de cada operação
    vector<double> tempo_fim_operacao(total_operations, 0);

    // O 'for' passará pelas duas listas de adjância, gerando as dependencias por job anterior
    // ou máquina, expressa na forma de arestas de entrada
    for (int i = 0; i < total_operations; i++)
    {
        if (instancia.jobAntecessor[i] != -1)
            grau_entrada[i]++;

        if (instancia.machAntecessor[i] != -1)
            grau_entrada[i]++;
    }

    // Esse 'for' colocará na fila todos as operação que estão prontas para serem executadas
    for (int i = 0; i < total_operations; i++)
    {
        if (instancia.jobAntecessor[i] == -1 && instancia.machAntecessor[i] == -1)
        {
            fila.push(i);
        }
    }

    int operacoes_processadas = 0;

    // Enquanto houver uma operação a ser executada, somaremos o tempo inicial de cada operação
    // com sua duração para saber quando uma operação específica terminará. Se esta operação for
    // a última de seu job, adicionamos o seu tempo no vetor 'tempo_final_job'
    while (!fila.empty())
    {
        // Tiramos o elemento da fila
        int atual = fila.front();
        fila.pop();

        lista_ordenacao.push_back(atual);

        double inicio_op_atual = 0;
        int culpado_job = -1;

        // Se o operação atual possuir um antecessor, o tempo inicial dele é o tempo final do antecessor
        if (instancia.jobAntecessor[atual] != -1)
        {
            inicio_op_atual = tempo_fim_operacao[instancia.jobAntecessor[atual]];
            culpado_job = instancia.jobAntecessor[atual];
        }
        // Se o operação não possui um antecessor, o tempo inicial dele é o tempo que a primeira
        // operação do job chega, o 'release_date'.
        else
        {
            inicio_op_atual = lista_job[lista_operacoes[atual].id_job].release_date;
        }

        double desocupa_maquina_atual = 0;
        int culpado_mach = -1;

        // Uma vez que a operação atual pode não depender apenas de uma operação anterior de mesmo job,
        // mas de uma máquina que foi usada anteriormente por uma operação de outro job,
        // o tempo inicial é o valor máximo entre o tempo final da operação do mesmo job ou da
        // operação de job diferente
        if (instancia.machAntecessor[atual] != -1)
        {
            desocupa_maquina_atual = tempo_fim_operacao[instancia.machAntecessor[atual]];
            culpado_mach = instancia.machAntecessor[atual];
        }

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

        // O tempo final da operação atual é a soma do tempo inicial do mesmo + o tempo de processamento
        //+ o tempo de setup, se o tempo setup existir
        double duracao = instancia.tempoProcessamento[atual];

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

        // O mesmo if de cima, porém para a lista de operações em cada máquina
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

    if (operacoes_processadas < total_operations)
    {
        makespan = 999999999.0;                             // Diz para a main que o makespan é absurdo
        vector<double> tempo_invalido(n_jobs, 999999999.0); // Retorna tempos gigantes
        return tempo_invalido;                              // O calcula_custo_total vai gerar uma multa bilionária
    }
    // Calcula o makespan

    for (int i = 0; i < n_jobs; i++)
    {
        if (makespan < tempo_final_job[i])
            makespan = tempo_final_job[i];
    }
    return tempo_final_job;
}

// CORREÇÃO: "const" adicionado nos dois últimos parâmetros para bater certinho com o .hpp e com a main
vector<double> calcula_custo_total(vector<int> &lista_ordenacao, vector<int> &predecessor_critico, Instancia &instancia, double &makespan, const vector<JobInfo> &lista_job, const vector<Operacao> &lista_operacoes)
{
    // Declara as estruturas de dados
    vector<double> tempo_final_job;
    int n_jobs = lista_job.size();
    tempo_final_job = avaliador(lista_ordenacao, predecessor_critico, n_jobs, instancia, lista_job, lista_operacoes, makespan);
    vector<double> tempo_multas_job;
    tempo_multas_job.resize(tempo_final_job.size(), 0);

    if (makespan >= 999999990.0)
    {
        for (int i = 0; i < n_jobs; i++)
        {
            tempo_multas_job[i] = 999999999.0; // Multa bilionária
        }
        return tempo_multas_job;
    }

    // Esse 'for' passará pelo tempo de cada job e calculará o tempo real com penalidades, através
    // diferença do tempo total para o tempo esperado
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

        // Se houver adiamento, calculamos a multa em relação o periodo de adiamento
        else
        {
            double temp = diferenca * lista_job[i].earliness_penalty;
            tempo_multas_job[i] = tempo_final_job[i] + temp;
        }
    }
    return tempo_multas_job;
}
void resolve_grafo_professor()
{
    int n = 15;

    vector<vector<int>> adj(n + 1);
    vector<int> grau_entrada(n + 1, 0);

    // ARESTAS CORRIGIDAS DE ACORDO COM O GABARITO VISUAL
    vector<pair<int, int>> arestas = {
        {4, 3}, {4, 1}, {7, 10}, {7, 8}, {10, 3}, {10, 1}, {8, 2}, {8, 14}, // <--- Aresta longa corrigida
        {3, 5},
        {1, 11},
        {1, 2}, // <--- Dependência do 2 corrigida
        {2, 12},
        {5, 15},
        {5, 14},
        {11, 15},
        {11, 14},
        {14, 13},
        {12, 13},
        {12, 9},
        {15, 6},
        {15, 9}, // <--- Dependência do 9 corrigida
        {13, 6}};

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
            // Se encontrar um caminho MAIOR, ele atualiza a culpa e o tamanho.
            // É isso que vai fazer ele pular de 4 para 5 saltos no nó 9!
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

        arquivo_relatorio << "[ REQUISITO 1 ] Ordem Topologica dos Vertices:\n -> ";
        for (size_t i = 0; i < ordem_topologica.size(); i++)
        {
            arquivo_relatorio << ordem_topologica[i];
            if (i < ordem_topologica.size() - 1)
                arquivo_relatorio << ", ";
        }
        arquivo_relatorio << "\n\n";

        arquivo_relatorio << "[ REQUISITO 2 ] Comprimento do Caminho Maximo: " << comprimento_maximo << " saltos (arestas)\n\n";

        arquivo_relatorio << "[ REQUISITO 3 ] Caminho Maximo Global (Minimal ao Maximal):\n -> ";
        for (size_t i = 0; i < caminho_global.size(); i++)
        {
            arquivo_relatorio << caminho_global[i];
            if (i < caminho_global.size() - 1)
                arquivo_relatorio << " -> ";
        }
        arquivo_relatorio << "\n\n";

        arquivo_relatorio << "[ REQUISITO 4 ] Caminhos Maximos para o Final de Cada Linha do Desenho:\n";
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