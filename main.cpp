#include <iostream>
#include <vector>
#include <string>
#include <dirent.h>
#include <fstream>
#include "./Bibliotecas/Estruturas.hpp"
#include "./Bibliotecas/Parser_JobInfo.hpp"
#include "./Bibliotecas/Parser_Operation.hpp"
#include "./Bibliotecas/Parser_Setup.hpp"
#include "./Bibliotecas/Avaliador.hpp"

using namespace std;

int main()
{
    string caminho_base = "Pasta do git_Brandimarte/just-in-time-jss-setup-times/instancias_100/";

    DIR *dir;
    struct dirent *ent;
    int n_jobs = 0;
    int n_maquinas = 0;
    int total_operacoes = 0;

    ofstream arquivo_saida("resultados_simulacao.csv");
    if (!arquivo_saida.is_open())
    {
        cout << "Erro: Nao foi possivel criar o arquivo de resultados!" << endl;
        return 1;
    }

    arquivo_saida << "Instancia,Makespan,Custo_Total,N_Jobs,N_Maquinas,Total_Operacoes\n";

    cout << "--- INICIANDO PROCESSAMENTO ---" << endl;

    // Abre o diretório principal
    if ((dir = opendir(caminho_base.c_str())) != NULL)
    {
        // Percorre todos os arquivos e pastas dentro dele
        while ((ent = readdir(dir)) != NULL)
        {
            string nome_pasta = ent->d_name;

            // Filtra para pegar apenas pastas que comecem com "instancia_"
            // e ignora os diretórios "." e ".."
            if (nome_pasta.find("instancia_") != string::npos)
            {

                string caminho_completo = caminho_base + nome_pasta;
                double makespan = 0;

                // Monta o caminho dos arquivos
                string arquivo_operacoes = caminho_completo + "/operations.csv";
                string arquivo_setup = caminho_completo + "/setup.csv";
                string arquivo_jobs = caminho_completo + "/jobs.csv";

                cout << "\n>>> Executando: " << nome_pasta << endl;

                // Chama sua função de cálculo
                vector<double> custos = calcula_custo_total(
                    arquivo_jobs,
                    arquivo_operacoes,
                    arquivo_setup,
                    makespan, n_jobs, n_maquinas);

                double soma = 0;
                for (double c : custos)
                    soma += c;

                total_operacoes = n_jobs * n_maquinas;

                cout << "  Jobs: " << n_jobs << " | Maquinas: " << n_maquinas << " | Op: " << total_operacoes << endl;
                cout << "  Makespan: " << makespan << " | Custo: " << soma << endl;

                // Salva os dados processados direto no arquivo CSV
                arquivo_saida << nome_pasta << ","
                              << makespan << ","
                              << soma << ","
                              << n_jobs << ","
                              << n_maquinas << ","
                              << total_operacoes << "\n";
            }
        }
        closedir(dir);
    }
    else
    {
        // Se cair aqui, o caminho_base está errado
        cout << "Erro: Nao foi possivel abrir a pasta: " << caminho_base << endl;
    }

    // Fecha e salva o arquivo de forma segura após o término
    arquivo_saida.close();
    cout << "\n--- PROCESSAMENTO CONCLUIDO! Resultados salvos em 'resultados_simulacao.csv' ---" << endl;

    return 0;
}