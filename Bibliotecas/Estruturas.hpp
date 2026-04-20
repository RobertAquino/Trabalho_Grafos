#ifndef ESTRUTURAS_HPP
#define ESTRUTURAS_HPP
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;

// 4. O Contrato Financeiro (Arquivo jobs.csv)
class JobInfo
{
    int id_job;
    int release_date;
    double due_date;
    double earliness_penalty;
    double tardiness_penalty;
    double flow_time_penalty;
};
class Operacao
{
    JobInfo job;
    int maquina;
    int tempo_processamento;
    int tempo_setup;
};

/*// 1. Etiqueta da Seta (Crucial para a Heurística da Fase 5)
enum TipoSeta
{
    ARESTA_ORIGINAL, // Aresta fixa (imutável)
    ARESTA_FILA      // Aresta variável (pode apagar e inverter)
};

// 2. A Aresta Inteligente (Substitui o vector<vector<int>>)
struct Aresta
{
    int destino;
    int peso;      // Tempo de processo da origem + Setup (se houver)
    TipoSeta tipo; // Diz para o C++ se ele pode mexer nessa seta
};

// 3. O Vértice (Operação)
struct Operacao
{
    int id;         // ID global da operação (ex: 0 a 99)
    int id_job;     // INT puro (muito mais rápido que string)
    int id_maquina; // INT puro
    int tempo_processamento;

    // Variáveis exclusivas para o Algoritmo de Kahn
    int grau_entrada = 0;
    int tempo_mais_cedo = 0; // Onde você vai anotar o EndTime calculado

    std::vector<Aresta> vizinhos; // As setas que saem deste vértice
};

// 4. O Contrato Financeiro (Arquivo jobs.csv)
struct JobInfo
{
    int id_job;
    int release_date;
    double due_date;
    double earliness_penalty;
    double tardiness_penalty;
    double flow_time_penalty;
};

// 5. O Grafo Completo (O Mapa da Fábrica)
struct Grafo
{
    int num_vertices;
    std::vector<Operacao> vertices;

    // Construtor simples que já cria o tamanho certo do Grafo
    Grafo(int n)
    {
        num_vertices = n;
        vertices.resize(n);
    }
};

// Tabela de Setup
struct TabelaSetup
{
    // [Maquina][Job_Anterior][Job Atual]
    int transicoes[10][10][10];

    // [Maquina] -> Job inicial que ela contém
    int estado_inicial[10];

    // Inicializa tudo com zero para evitar lixo de memória
    TabelaSetup()
    {
        memset(transicoes, 0, sizeof(transicoes));
        memset(estado_inicial, 0, sizeof(estado_inicial));
    }
};

*/
#endif