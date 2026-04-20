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

// class TempoProcesso
// {
// public:
//     vector<int> tempoProcesso;
// };

class Instancia
{
public:
    // matriz_setup[maquina][job_anterior][job_atual]
    vector<vector<vector<int>>> matriz_setup;
    vector<int> estado_inicial;
    vector<int> tempoProcesso;
    vector<int> operToJob;
    vector<int> operToMach;
    vector<vector<unsigned int>> jobOperation;
    vector<vector<unsigned int>> machOperation;

    // Prepara o espaço na memória
    void configurar(int n_maquinas, int n_jobs)
    {
        matriz_setup.resize(n_maquinas, vector<vector<int>>(n_jobs, vector<int>(n_jobs, 0)));
        estado_inicial.resize(n_maquinas, 0);
    }
};

// class OperToJob
// {
// public:
//     vector<int> operToJob;
// };

// class OperToMach
// {
// public:
//     vector<int> operToMach;
// };

// class JobOperation
// {
// public:
//     vector<vector<unsigned int>> jobOperation;
// };

// class MachOperation
// {
// public:
//     vector<vector<unsigned int>> machOperation;
// };

#endif