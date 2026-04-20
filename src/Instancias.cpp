#ifndef GERAR_INSTANCIAS_HPP
#define GERAR_INSTANCIAS_HPP

#include "../Bibliotecas/Estruturas.hpp"
#include "../Bibliotecas/Parser_Operation.hpp"

void Instancia::configurar(int n_maquinas, int n_jobs)
{
    matriz_setup.resize(n_maquinas, vector<vector<int>>(n_jobs, vector<int>(n_jobs, 0)));
    estado_inicial.resize(n_maquinas, 0);

    operToJob.resize(n_jobs);
    operToMach.resize(n_maquinas);
}

void Instancia::geraOperToJob(const vector<Operacao> &lista_operacoes)
{
    int atual = 0;
    operToJob.push_back(atual);

    for (int i = 0; i < lista_operacoes.size(); i++)
    {
        atual = lista_operacoes[i].id_job;
        operToJob.push_back(atual);
    }
}

void Instancia::geraOperToMach(const vector<Operacao> &lista_operacoes)
{
    int atual = 0;
    operToMach.push_back(atual);

    for (int i = 0; i < lista_operacoes.size(); i++)
    {
        atual = lista_operacoes[i].maquina;
        operToJob.push_back(atual);
    }
}

void Instancia::geraJobOper(const vector<int> operToJob, const vector<Operacao> &lista_operacoes)
{
    int job_atual = 0;

    for (int i = 0; i < operToJob.size(); i++)
    {
        if (operToJob[i + 1] == job_atual)
        {
            jobOperation[job_atual].push_back(lista_operacoes[i]);
        }
        else
        {
            job_atual++;
            jobOperation[job_atual].push_back(lista_operacoes[i]);
        }
    }
}

void Instancia::geraMachOper(const vector<int> operToMach, const vector<Operacao> &lista_operacoes)
{
    int mach_atual = 0;

    for (int i = 0; i < operToMach.size(); i++)
    {
        if (operToMach[i + 1] == mach_atual)
        {
            machOperation[mach_atual].push_back(lista_operacoes[i]);
        }
        else
        {
            mach_atual++;
            machOperation[mach_atual].push_back(lista_operacoes[i]);
        }
    }
}

void Instancia::geraTempoProcessamento(const vector<Operacao> &lista_operacoes)
{
    for (int i = 0; i < lista_operacoes.size(); i++)
    {
        tempoProcessamento[i] = lista_operacoes[i].tempo_processamento;
    }
}

#endif