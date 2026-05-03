#include "../Bibliotecas/Estruturas.hpp"
#include "../Bibliotecas/Parser_Operation.hpp"
#include "../Bibliotecas/Parser_JobInfo.hpp"
#include "../Bibliotecas/Parser_Setup.hpp"
#include <queue>

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
vector<double> avaliador(int n_jobs, const Instancia &instancia,
                         const vector<JobInfo> &lista_job, const vector<Operacao> &lista_operacoes, double &makespan)
{
    // Primeiro declaramos as estruturas de dados
    int total_operations = lista_operacoes.size();
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

    // Enquanto houver uma operação a ser executada, somaremos o tempo inicial de cada operação
    // com sua duração para saber quando uma operação específica terminará. Se esta operação for
    // a última de seu job, adicionamos o seu tempo no vetor 'tempo_final_job'
    while (!fila.empty())
    {
        // Tiramos o elemento da fila
        int atual = fila.front();
        fila.pop();

        double inicio_op_atual = 0;

        // Se o operação atual possuir um antecessor, o tempo inicial dele é o tempo final do antecessor
        if (instancia.jobAntecessor[atual] != -1)
        {
            inicio_op_atual = tempo_fim_operacao[instancia.jobAntecessor[atual]];
        }
        // Se o operação não possui um antecessor, o tempo inicial dele é o tempo que a primeira
        // operação do job chega, o 'release_date'.
        else
        {
            inicio_op_atual = lista_job[lista_operacoes[atual].id_job].release_date;
        }

        double desocupa_maquina_atual = 0;

        // Uma vez que a operação atual pode não depender apenas de uma operação anterior de mesmo job,
        // mas de uma máquina que foi usada anteriormente por uma operação de outro job,
        // o tempo inicial é o valor máximo entre o tempo final da operação do mesmo job ou da
        // operação de job diferente
        if (instancia.machAntecessor[atual] != -1)
            desocupa_maquina_atual = tempo_fim_operacao[instancia.machAntecessor[atual]];

        tempo_inicio_operacao[atual] = max(inicio_op_atual, desocupa_maquina_atual);

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
    }
    // Calcula o makespan
    makespan = 0;

    for (int i = 0; i < n_jobs; i++)
    {
        if (makespan < tempo_final_job[i])
            makespan = tempo_final_job[i];
    }
    return tempo_final_job;
}

vector<double> calcula_custo_total(Instancia &instancia, double &makespan, vector<JobInfo> lista_job, vector<Operacao> lista_operacoes)
{
    // Declara as estruturas de dados
    vector<double> tempo_final_job;
    int n_jobs = lista_job.size();
    tempo_final_job = avaliador(n_jobs, instancia, lista_job, lista_operacoes, makespan);
    vector<double> tempo_multas_job;
    tempo_multas_job.resize(tempo_final_job.size(), 0);

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