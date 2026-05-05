#include "../Bibliotecas/BuscaTabu.hpp"
#include "../Bibliotecas/Estruturas.hpp"
#include "../Bibliotecas/Avaliador.hpp"
#include <iostream>
#include <vector>

using namespace std;

struct Movimento
{
    Solucao solucao;
    int op1;
    int op2;
};

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
void transformaSolucaoSPT(const Instancia &instancia, const vector<JobInfo> &lista_jobs, const vector<Operacao> &lista_operacoes, Solucao &solucao, int n_maquinas)
{
    // Eu inicializo a matriz
    solucao.matriz_solucao.clear();
    solucao.matriz_solucao.resize(n_maquinas);

    int n_jobs = lista_jobs.size();

    // Esse vetor é responsável por guardar a quantidade de operações de cada job que já foram analisadas
    vector<int> progresso_job(n_jobs, 0);

    // Variáveis de controle para o loop
    int total_inseridas = 0;
    int total_ops = lista_operacoes.size();

    while (total_inseridas < total_ops)
    {

        // Variáveis usadas para armazenar o melhor tempo,job e operação
        int melhor_op = -1;
        int melhor_job = -1;
        double menor_tempo = 999999999.0;

        // O for passa por todas as primeiras i operacoes de cada job, tentando encontrar a melhor operacao
        for (int j = 0; j < n_jobs; j++)
        {

            // Esse 'if' compara se ainda existe uma operação de um job que não foi analisada
            if (progresso_job[j] < (int)instancia.jobOperation[j].size())
            {

                // Pegamos o ID da operação que está na atual do job a ser analisada
                int op_id = instancia.jobOperation[j][progresso_job[j]];

                // Calculamos o tempo da operacao
                double tempo_op = lista_operacoes[op_id].tempo_processamento;

                // Comparamos se a operacao atual e a melhor encontrada até agora
                if (tempo_op < menor_tempo)
                {
                    menor_tempo = tempo_op;
                    melhor_op = op_id;
                    melhor_job = j;
                }
            }
        }

        // Cria a fila baseando na operação de menor tempo;
        int maq = lista_operacoes[melhor_op].maquina;
        solucao.matriz_solucao[maq].push_back(melhor_op);

        progresso_job[melhor_job]++;
        total_inseridas++;
    }
}
double somatorioJob(vector<double> multas)
{
    double soma = 0;

    for (double c : multas)
        soma += c;

    return soma;
}
vector<Movimento> geraVizinhos(Solucao &atual)
{
    vector<Movimento> vizinhos;
    for (size_t maquina_atual = 0; maquina_atual < atual.matriz_solucao.size(); maquina_atual++)
    {
        int operacoes = atual.matriz_solucao[maquina_atual].size();

        for (int i = 0; i < operacoes - 1; i++)
        {
            Movimento mov;
            mov.solucao = atual;
            mov.op1 = mov.solucao.matriz_solucao[maquina_atual][i];
            mov.op2 = mov.solucao.matriz_solucao[maquina_atual][i + 1];

            swap(mov.solucao.matriz_solucao[maquina_atual][i], mov.solucao.matriz_solucao[maquina_atual][i + 1]);

            vizinhos.push_back(mov);
        }
    }
    return vizinhos;
}

double motorBusca(std::vector<int> &lista_ordenacao, Instancia &instancia, const std::vector<JobInfo> &lista_jobs,
                  const std::vector<Operacao> &lista_operacoes, double &makespan, int &iteracao,
                  Solucao solucao_inicial)
{
    // Esse vetor será usado para passar por toda a possíveis soluções(vizinhança) de uma solucao atual
    vector<Movimento> vizinhanca;
    // Criamos um makespan temporário
    double makespan_temp = 0;

    vector<int> predecessor_temporario;

    // Cria a STP (Shortest Processing Time)

    // Calculamos o custo total da nossa fifo e o makespan, que inicialmente é a melhor solucao
    vector<double> multas_inicial = calcula_custo_total(lista_ordenacao, predecessor_temporario, instancia, makespan_temp, lista_jobs, lista_operacoes);
    double melhor_custo_global = somatorioJob(multas_inicial);
    double makespan_melhor_global = makespan_temp;

    // A partir da FIFO geramos a nossa solucao inicial e atribuimos o rotulo de melhor solucao
    Solucao melhor_solucao_global = solucao_inicial;
    Solucao solucao_atual = solucao_inicial;

    int total_operacoes = lista_operacoes.size();

    // Criamos a nossa lista tabu(matriz), que será responsável por restringir alguns movimentos, por um certo
    // período de tempo. Fazemos isso para tentar fugir dos mínimos locais
    vector<vector<int>> lista_tabu(total_operacoes, vector<int>(total_operacoes, 0));

    // Defino os parametros da nossa Busca Tabu, a supensão de cada movimento, e quantidade de passos que ele poderá
    // dar sem conseguir uma melhora.
    int tempo_suspensao = 10;
    int max_iteracoes_sem_melhora = 150;
    int iteracoes_sem_melhora = 0;

    iteracao = 0;

    while (iteracoes_sem_melhora < max_iteracoes_sem_melhora)
    {
        iteracao++;

        // Gero os vizinhos da solucao atual
        vizinhanca = geraVizinhos(solucao_atual);

        if (vizinhanca.empty())
            break;

        // Crio os parametros da rodada atual
        double melhor_custo_rodada = 99999999999.0;
        double melhor_makespan_rodada = 0;
        Solucao melhor_solucao_rodada;
        int op1_escolhida = -1;
        int op2_escolhida = -1;
        bool achou_movimento_valido = false;

        for (size_t i = 0; i < vizinhanca.size(); i++)
        {
            // Passo por todos os vizinhos e calculo o custo e makespan de cada um
            Solucao proxima_solucao = vizinhanca[i].solucao;
            int o1 = vizinhanca[i].op1;
            int o2 = vizinhanca[i].op2;

            alteraMachAntecessor(instancia, proxima_solucao);
            alteraMachSucessor(instancia, proxima_solucao);

            vector<double> multas_proxima_solucao = calcula_custo_total(lista_ordenacao, predecessor_temporario, instancia, makespan_temp, lista_jobs, lista_operacoes);
            double custo_vizinho = somatorioJob(multas_proxima_solucao);

            // Se o movimento gerar um ciclo eu nem analiso
            if (custo_vizinho >= 999999990.0)
                continue;

            // Analisamos se o movimento está suspenso
            bool eh_tabu = (lista_tabu[o1][o2] >= iteracao);

            // Se o movimento estiver suspenso mais ele ainda sim melhor o cust_global, nós forçamos a Busca Tabu permitir o movimento
            if (eh_tabu && custo_vizinho < melhor_custo_global)
                eh_tabu = false;

            // Se o movimento não fizer parte da lista tabu, atualizamos o melhor vizinho
            if (!eh_tabu && custo_vizinho < melhor_custo_rodada)
            {
                melhor_custo_rodada = custo_vizinho;
                melhor_solucao_rodada = proxima_solucao;
                melhor_makespan_rodada = makespan_temp;
                op1_escolhida = o1;
                op2_escolhida = o2;
                achou_movimento_valido = true;
            }
        }

        // Se ele não achar nenhum vizinho válido, eu testo a vizinha de outra solucao
        if (!achou_movimento_valido)
            break;

        // Pegamos o melhor vizinho e restringimos o movimento da solução por algum tempo
        solucao_atual = melhor_solucao_rodada;

        lista_tabu[op1_escolhida][op2_escolhida] = iteracao + tempo_suspensao;
        lista_tabu[op2_escolhida][op1_escolhida] = iteracao + tempo_suspensao;

        // Se a melhor solucao da vizinha for melhor do que a melhor solucao global, atualizamos a solução global
        if (melhor_custo_rodada < melhor_custo_global)
        {
            melhor_custo_global = melhor_custo_rodada;
            melhor_solucao_global = melhor_solucao_rodada;
            makespan_melhor_global = melhor_makespan_rodada;
            iteracoes_sem_melhora = 0;
        }
        else
        {
            iteracoes_sem_melhora++;
        }
    }

    // DEVOLVE PARA A MAIN O MAKESPAN LIMPO
    makespan = makespan_melhor_global;

    alteraMachAntecessor(instancia, melhor_solucao_global);
    alteraMachSucessor(instancia, melhor_solucao_global);

    calcula_custo_total(lista_ordenacao, predecessor_temporario, instancia, makespan, lista_jobs, lista_operacoes);

    return melhor_custo_global;
}