#ifndef GERAR_INSTANCIAS_HPP
#define GERAR_INSTANCIAS_HPP

#include "../Bibliotecas/Estruturas.hpp"
#include "../Bibliotecas/Parser_Operation.hpp"
#include "../Bibliotecas/Parser_Setup.hpp"

using namespace std;

void Instancia::configurar(int n_maquinas, int n_jobs)
{
    int total_operations = n_maquinas * n_jobs;

    matriz_setup.resize(n_maquinas, vector<vector<int>>(n_jobs, vector<int>(n_jobs, 0)));
    estado_inicial.resize(n_maquinas, 0);

    // Todos esses vetores precisam ter o tamanho do TOTAL de operações
    operToJob.resize(total_operations, 0);
    operToMach.resize(total_operations, 0);
    tempoProcessamento.resize(total_operations, 0);

    jobAntecessor.resize(total_operations, -1);
    jobSucessor.resize(total_operations, -1);
    machAntecessor.resize(total_operations, -1);
    machSucessor.resize(total_operations, -1);

    jobOperation.resize(n_jobs);
    machOperation.resize(n_maquinas);
}

void Instancia::geraOperToJob(const vector<Operacao> &lista_operacoes)
{
    for (int i = 0; i < lista_operacoes.size(); i++)
    {
        operToJob[lista_operacoes[i].id] = lista_operacoes[i].id_job;
    }
}

void Instancia::geraOperToMach(const vector<Operacao> &lista_operacoes)
{
    for (int i = 0; i < lista_operacoes.size(); i++)
    {
        operToMach[lista_operacoes[i].id] = lista_operacoes[i].maquina;
    }
}

void Instancia::geraTempoProcessamento(const vector<Operacao> &lista_operacoes)
{
    for (int i = 0; i < lista_operacoes.size(); i++)
    {
        tempoProcessamento[lista_operacoes[i].id] = lista_operacoes[i].tempo_processamento;
    }
}

void Instancia::geraJobOper(const vector<Operacao> &lista_operacoes)
{
    for (int i = 0; i < lista_operacoes.size(); i++)
    {
        jobOperation[lista_operacoes[i].id_job].push_back(lista_operacoes[i].id);
    }
}

void Instancia::geraMachOper(const vector<Operacao> &lista_operacoes)
{
    for (int i = 0; i < lista_operacoes.size(); i++)
    {
        machOperation[lista_operacoes[i].maquina].push_back(lista_operacoes[i].id);
    }
}

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

void Instancia::inicializaEstruturas(string caminho, int n_maquinas, int n_jobs)
{
    configurar(n_maquinas, n_jobs);

    OperationsParser operationsParser;
    vector<Operacao> lista_operacoes = operationsParser.inicializaParser(caminho);

    SetupParser parserSetup;
    parserSetup.carregarSetup(caminho, matriz_setup);

    geraOperToJob(lista_operacoes);
    geraOperToMach(lista_operacoes);
    geraTempoProcessamento(lista_operacoes);
    geraJobOper(lista_operacoes);
    geraMachOper(lista_operacoes);

    geraJobAntecessor();
    geraJobSucessor();
    geraMachAntecessor();
    geraMachSucessor();
}

#endif