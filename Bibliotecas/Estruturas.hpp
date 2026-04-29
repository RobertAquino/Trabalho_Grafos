#ifndef ESTRUTURAS_HPP
#define ESTRUTURAS_HPP
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

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
    std::vector<std::vector<std::vector<int>>> matriz_setup;
    std::vector<int> estado_inicial;
    std::vector<int> tempoProcessamento;
    std::vector<int> operToJob;
    std::vector<int> operToMach;
    std::vector<std::vector<int>> jobOperation;
    std::vector<std::vector<int>> machOperation;
    std::vector<int> jobAntecessor;
    std::vector<int> jobSucessor;
    std::vector<int> machAntecessor;
    std::vector<int> machSucessor;

    void geraOperToJob(const std::vector<Operacao> &lista_operacoes);
    void geraOperToMach(const std::vector<Operacao> &lista_operacoes);
    void geraJobOper(const std::vector<Operacao> &lista_operacoes);
    void geraMachOper(const std::vector<Operacao> &lista_operacoes);
    void geraTempoProcessamento(const std::vector<Operacao> &lista_operacoes);
    void geraJobAntecessor();
    void geraJobSucessor();
    void geraMachAntecessor();
    void geraMachSucessor();
    void inicializaEstruturas(std::string caminho, int n_maquinas, int n_jobs);
    // Prepara o espaço na memória
    void configurar(int n_maquinas, int n_jobs);
};

#endif