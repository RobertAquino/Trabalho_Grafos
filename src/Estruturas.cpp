#ifndef GERAR_INSTANCIAS_HPP
#define GERAR_INSTANCIAS_HPP

#include "../Bibliotecas/Estruturas.hpp"
#include "../Bibliotecas/Parser_Operation.hpp"
#include "../Bibliotecas/Parser_Setup.hpp"

using namespace std;

/**
 * @brief Prepara a memória redimensionando todos os vetores da classe Instancia.
 * É o primeiro passo para evitar erros de "out of range" (acesso a memória inválida).
 */
void Instancia::configurar(int n_maquinas, int n_jobs)
{
    cout << "Configurando memória para " << n_maquinas << " máquinas e " << n_jobs << " jobs..." << endl;
    int total_operations = n_maquinas * n_jobs;

    // Redimensiona a matriz 3D de setup [máquina][job_ant][job_atual]
    matriz_setup.resize(n_maquinas, vector<vector<int>>(n_jobs, vector<int>(n_jobs, 0)));
    estado_inicial.resize(n_maquinas, 0);

    // Mapeamentos diretos: cada posição do vetor corresponde ao ID de uma operação
    operToJob.resize(total_operations, 0);
    operToMach.resize(total_operations, 0);
    tempoProcessamento.resize(total_operations, 0);

    // Estruturas de vizinhança no grafo: inicializadas com -1 (indicando que não existe vizinho)
    jobAntecessor.resize(total_operations, -1);
    jobSucessor.resize(total_operations, -1);
    machAntecessor.resize(total_operations, -1);
    machSucessor.resize(total_operations, -1);

    // Agrupadores: vetores de listas (cada Job/Máquina terá sua própria lista de IDs)
    jobOperation.resize(n_jobs);
    machOperation.resize(n_maquinas);
}

/**
 * @brief Cria um mapa rápido para descobrir o Job de uma operação usando apenas o ID dela.
 */
void Instancia::geraOperToJob(const vector<Operacao> &lista_operacoes)
{
    for (int i = 0; i < lista_operacoes.size(); i++)
    {
        // Ex: operToJob[ID_DA_OP] = ID_DO_JOB
        operToJob[lista_operacoes[i].id] = lista_operacoes[i].id_job;
    }
}

/**
 * @brief Cria um mapa rápido para descobrir em qual máquina uma operação deve rodar.
 */
void Instancia::geraOperToMach(const vector<Operacao> &lista_operacoes)
{
    for (int i = 0; i < lista_operacoes.size(); i++)
    {
        operToMach[lista_operacoes[i].id] = lista_operacoes[i].maquina;
    }
}

/**
 * @brief Armazena o tempo de processamento de cada operação em um vetor indexado pelo ID.
 */
void Instancia::geraTempoProcessamento(const vector<Operacao> &lista_operacoes)
{
    for (int i = 0; i < lista_operacoes.size(); i++)
    {
        tempoProcessamento[lista_operacoes[i].id] = lista_operacoes[i].tempo_processamento;
    }
}

/**
 * @brief Agrupa os IDs das operações dentro de seus respectivos Jobs.
 * Resulta em: jobOperation[0] = {op0, op1, op2...}
 */
void Instancia::geraJobOper(const vector<Operacao> &lista_operacoes)
{
    for (int i = 0; i < lista_operacoes.size(); i++)
    {
        jobOperation[lista_operacoes[i].id_job].push_back(lista_operacoes[i].id);
    }
}

/**
 * @brief Agrupa os IDs das operações dentro de suas respectivas Máquinas.
 * Resulta em: machOperation[0] = {op5, op12, op18...}
 */
void Instancia::geraMachOper(const vector<Operacao> &lista_operacoes)
{
    for (int i = 0; i < lista_operacoes.size(); i++)
    {
        machOperation[lista_operacoes[i].maquina].push_back(lista_operacoes[i].id);
    }
}

/**
 * @brief Define a precedência linear dentro de cada Job.
 */
void Instancia::geraJobAntecessor()
{
    for (const auto &fila : jobOperation)
    {
        // Começa do 1 (a primeira operação não tem antecessor)
        for (int i = 1; i < fila.size(); i++)
        {
            jobAntecessor[fila[i]] = fila[i - 1];
        }
    }
}

/**
 * @brief Define a sucessão linear dentro de cada Job.
 */
void Instancia::geraJobSucessor()
{
    for (const auto &fila : jobOperation)
    {
        // Vai até o penúltimo (a última operação não tem sucessor)
        for (int i = 0; i < fila.size() - 1; i++)
        {
            jobSucessor[fila[i]] = fila[i + 1];
        }
    }
}

/**
 * @brief Define a ordem de processamento na máquina (quem veio antes na fila).
 */
void Instancia::geraMachAntecessor()
{
    for (const auto &fila : machOperation)
    {
        for (int i = 1; i < fila.size(); i++)
        {
            machAntecessor[fila[i]] = fila[i - 1];
        }
    }
}

/**
 * @brief Define a ordem de processamento na máquina (quem será o próximo).
 */
void Instancia::geraMachSucessor()
{
    for (const auto &fila : machOperation)
    {
        for (int i = 0; i < fila.size() - 1; i++)
        {
            machSucessor[fila[i]] = fila[i + 1];
        }
    }
}

/**
 * @brief Função orquestradora: Chama o parser, configura a memória e gera todos os mapas.
 */
void Instancia::inicializaEstruturas(string caminho_operation, string caminho_setup, int n_maquinas, int n_jobs)
{
    // 1. Prepara os vetores
    configurar(n_maquinas, n_jobs);
    // 2. Lê os dados brutos do arquivo CSV
    OperationsParser operationsParser;
    vector<Operacao> lista_operacoes = operationsParser.inicializaParser(caminho_operation);

    SetupParser parserSetup;
    parserSetup.carregarSetup(caminho_setup, matriz_setup);

    // 3. Converte a lista plana de operações em mapas de acesso rápido
    geraOperToJob(lista_operacoes);
    geraOperToMach(lista_operacoes);
    geraTempoProcessamento(lista_operacoes);
    geraJobOper(lista_operacoes);
    geraMachOper(lista_operacoes);

    // 4. Conecta os pontos (define as arestas lógicas do grafo de precedência)
    geraJobAntecessor();
    geraJobSucessor();
    geraMachAntecessor();
    geraMachSucessor();
}

#endif