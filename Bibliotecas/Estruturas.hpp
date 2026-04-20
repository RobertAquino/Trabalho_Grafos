#ifndef ESTRUTURAS_HPP
#define ESTRUTURAS_HPP
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

// 4. O Contrato Financeiro (Arquivo jobs.csv)
class JobInfo
{
public:
    int id_job;
    int release_date;
    double due_date;
    double earliness_penalty;
    double tardiness_penalty;
    double flow_time_penalty;
};

class Operacao
{
public:
    int id;
    int id_job;
    int maquina;
    int tempo_processamento;
    int tempo_setup;
};

class Instancia
{
public:
    // matriz_setup[maquina][job_anterior][job_atual]
    vector<vector<vector<int>>> matriz_setup;
    vector<int> estado_inicial;
    vector<int> tempoProcessamento;
    vector<int> operToJob;
    vector<int> operToMach;
    vector<vector<Operacao>> jobOperation;
    vector<vector<Operacao>> machOperation;

    void geraOperToJob(const vector<Operacao> &lista_operacoes);
    void geraOperToMach(const vector<Operacao> &lista_operacoes);
    void geraJobOper(const vector<int> operToJob, const vector<Operacao> &lista_operacoes);
    void geraMachOper(const vector<int> operToMach, const vector<Operacao> &lista_operacoes);
    void geraTempoProcessamento(const vector<Operacao> &lista_operacoes);
    // Prepara o espaço na memória
    void configurar(int n_maquinas, int n_jobs);
};

#endif