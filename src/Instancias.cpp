#ifndef GERAR_INSTANCIAS_HPP
#define GERAR_INSTANCIAS_HPP

#include "../Bibliotecas/Estruturas.hpp"
#include "../Bibliotecas/Parser_Operation.hpp"
#include "../Bibliotecas/Parser_Setup.hpp"

void Instancia::configurar(int n_maquinas, int n_jobs)
{
    matriz_setup.resize(n_maquinas, vector<vector<int>>(n_jobs, vector<int>(n_jobs, 0)));
    estado_inicial.resize(n_maquinas, 0);

    operToJob.resize(n_jobs);
    operToMach.resize(n_maquinas);
    jobAntecessor.resize((n_maquinas * n_jobs), -1);
    jobSucessor.resize((n_maquinas * n_jobs), -1);
    tempo_final_job.resize(n_jobs, -1);
}

void Instancia::geraOperToJob(const vector<Operacao> &lista_operacoes)
{
    operToJob.push_back(0);

    for (int i = 0; i < lista_operacoes.size() - 1; i++)
    {
        operToJob.push_back(lista_operacoes[i].id_job);
    }
}

void Instancia::geraOperToMach(const vector<Operacao> &lista_operacoes)
{
    operToMach.push_back(0);

    for (int i = 0; i < lista_operacoes.size() - 1; i++)
    {
        operToJob.push_back(lista_operacoes[i].maquina);
    }
}

void Instancia::geraJobOper(const vector<Operacao> &lista_operacoes)
{
    int job_atual = 0;

    for (int i = 0; i < operToJob.size() - 1; i++)
    {
        if (operToJob[i + 1] == job_atual)
        {
            jobOperation[job_atual].push_back(lista_operacoes[i].id_job);
        }
        else
        {
            job_atual++;
            jobOperation[job_atual].push_back(lista_operacoes[i].id_job);
        }
    }
}

void Instancia::geraMachOper(const vector<Operacao> &lista_operacoes)
{
    int mach_atual = 0;

    for (int i = 0; i < operToMach.size() - 1; i++)
    {
        if (operToMach[i + 1] == mach_atual)
        {
            machOperation[mach_atual].push_back(lista_operacoes[i].maquina);
        }
        else
        {
            mach_atual++;
            machOperation[mach_atual].push_back(lista_operacoes[i].maquina);
        }
    }
}

void Instancia::geraTempoProcessamento(const vector<Operacao> &lista_operacoes)
{
    for (int i = 0; i < lista_operacoes.size() - 1; i++)
    {
        tempoProcessamento[i] = lista_operacoes[i].tempo_processamento;
    }
}
void Instancia::geraJobAntecessor()
{
    int id_atual;
    int id_anterior;
    for (const auto &fila_de_job : jobOperation)
    {
        for (int i = 1; i < fila_de_job.size() - 1; i++)
        {
            id_atual = fila_de_job[i];
            id_anterior = fila_de_job[i - 1];
            jobAntecessor[id_atual] = id_anterior;
        }
    }
}
void Instancia::geraJobSucessor()
{
    int id_atual;
    int id_sucessor;
    for (const auto &fila_de_job : jobOperation)
    {
        for (int i = 0; i < fila_de_job.size() - 2; i++)
        {
            id_atual = fila_de_job[i];
            id_sucessor = fila_de_job[i + 1];
            jobSucessor[id_atual] = id_sucessor;
        }
    }
}
void Instancia::geraMachAntecessor()
{
    int id_atual;
    int id_anterior;
    for (const auto &fila_de_maquina : machOperation)
    {
        for (int i = 1; i < fila_de_maquina.size() - 1; i++)
        {
            id_atual = fila_de_maquina[i];
            id_anterior = fila_de_maquina[i - 1];
            machAntecessor[id_atual] = id_anterior;
        }
    }
}
void Instancia::geraMachSucessor()
{
    int id_atual;
    int id_sucessor;
    for (const auto &fila_de_maquina : machOperation)
    {
        for (int i = 1; i < fila_de_maquina.size() - 1; i++)
        {
            id_atual = fila_de_maquina[i];
            id_sucessor = fila_de_maquina[i + 1];
            machSucessor[id_atual] = id_sucessor;
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
    geraJobOper(lista_operacoes);
    geraMachOper(lista_operacoes);
    geraTempoProcessamento(lista_operacoes);
    geraJobAntecessor();
    geraJobSucessor();
    geraMachAntecessor();
    geraMachSucessor();
}
#endif