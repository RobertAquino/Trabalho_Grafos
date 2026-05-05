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
    int release_date;         // Data a partir da qual o Job pode começar a ser processado
    double due_date;          // Prazo de entrega (Data de vencimento)
    double earliness_penalty; // Multa por terminar antes do prazo (estoque)
    double tardiness_penalty; // Multa por terminar após o prazo (atraso)
};

/**
 * Representa uma tarefa individual (operação) que deve ser feita em uma máquina.
 */
class Operacao
{
public:
    int id;     // ID global da operação (ex: 0 a 99)
    int id_job; // A qual Job esta operação pertence
    int maquina;
    int tempo_processamento; // Duração da tarefa na máquina
    int tempo_setup;         // Tempo de preparação (pode ser dinâmico via matriz_setup)
};

class Instancia
{
public:
    std::vector<std::vector<std::vector<int>>> matriz_setup; // matriz_setup[maquina][job_anterior][job_atual]                       // Estado de cada máquina antes de começar

    // --- Mapeamentos Diretos (Acesso O(1)) ---
    // Úteis para quando você tem o ID de uma operação e quer saber um detalhe dela instantaneamente
    std::vector<int> tempoProcessamento; // tempoProcessamento[id_op]
    std::vector<int> operToJob;          // operToJob[id_op] -> Retorna o Job da operação
    std::vector<int> operToMach;         // operToMach[id_op] -> Retorna a Máquina da operação

    // --- Agrupamentos (Estruturas de Adjacência) ---
    // Úteis para percorrer todas as operações de uma mesma "entidade"
    std::vector<std::vector<int>> jobOperation;  // jobOperation[id_job] -> Lista de IDs de operações do Job
    std::vector<std::vector<int>> machOperation; // machOperation[id_mach] -> Lista de IDs de operações da Máquina

    // --- Estrutura do Grafo (Precedência) ---
    // Guardam a sequência lógica do problema
    std::vector<int> jobAntecessor; // Operação anterior dentro do MESMO Job
    std::vector<int> jobSucessor;   // Próxima operação dentro do MESMO Job

    std::vector<int> machAntecessor; // Operação que veio antes na MESMA Máquina
    std::vector<int> machSucessor;   // Próxima operação agendada na MESMA Máquina

    // --- Funções de Construção (Geração de Dados) ---

    // Preenche operToJob usando a lista de operações lida do CSV
    void geraOperToJob(const std::vector<Operacao> &lista_operacoes);

    // Preenche operToMach (mapeia ID da operação para ID da máquina)
    void geraOperToMach(const std::vector<Operacao> &lista_operacoes);

    // Organiza quais operações pertencem a cada Job (preenche jobOperation)
    void geraJobOper(const std::vector<Operacao> &lista_operacoes);

    // Organiza quais operações pertencem a cada máquina (preenche machOperation)
    void geraMachOper(const std::vector<Operacao> &lista_operacoes);

    // Extrai os tempos de processamento para um vetor simples de busca rápida
    void geraTempoProcessamento(const std::vector<Operacao> &lista_operacoes);

    // Conecta as operações dentro do job (Define quem vem antes/depois de quem)
    void geraJobAntecessor();
    void geraJobSucessor();

    // Conecta as operações na máquina (Baseado na sequência da fila de processamento)
    void geraMachAntecessor();
    void geraMachSucessor();

    // Função principal que orquestra a leitura e o preenchimento das listas
    void inicializaEstruturas(std::string caminho_operation, std::string caminho_setup, int n_maquinas, int n_jobs);
    // Prepara o espaço na memória
    void configurar(int n_maquinas, int n_jobs);
};

#endif