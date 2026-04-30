#include <iostream>
#include <vector>
#include <string>
#include "./Bibliotecas/Estruturas.hpp"
#include "./Bibliotecas/Parser_JobInfo.hpp"
#include "./src/Instancias.cpp"
#include "./src/Avaliador.cpp"

using namespace std;

int main()
{
    int n_jobs = 10;
    int n_machines = 10;
    double makespan = 0;

    string arquivo_jobs = "jobs.csv";
    string arquivo_operacoes = "operations.csv";
    string arquivo_setup = "setup.csv";
    string arquivo_setup_inicial = "initial_setup.csv";

    cout << "--- Iniciando Execucao da Instancia ---" << endl;

    // A função calcula_custo_total já encapsula a chamada do avaliador e dos parsers internos
    vector<double> custos_por_job = calcula_custo_total(
        arquivo_operacoes,
        n_jobs,
        n_machines,
        makespan);

    cout << "\nRESULTADOS DA SIMULACAO:" << endl;
    cout << "Makespan Total (Cmax): " << makespan << " unidades de tempo." << endl;
    cout << "---------------------------------------" << endl;
    cout << "Custo Individual (Tempo + Penalidades):" << endl;

    double custo_total_sistema = 0;
    for (int i = 0; i < n_jobs; i++)
    {
        cout << "Job [" << i << "]: " << custos_por_job[i] << endl;
        custo_total_sistema += custos_por_job[i];
    }

    cout << "---------------------------------------" << endl;
    cout << "CUSTO TOTAL DO SCHEDULING: " << custo_total_sistema << endl;

    return 0;
}