#include <iostream>
#include <vector>
#include <string>
#include <dirent.h>
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

                // Chama sua função de cálc
                vector<double> custos = calcula_custo_total(
                    arquivo_jobs,
                    arquivo_operacoes,
                    arquivo_setup,
                    makespan);

                double soma = 0;
                for (double c : custos)
                    soma += c;

                cout << "  Makespan: " << makespan << " | Custo: " << soma << endl;
            }
        }
        closedir(dir);
    }
    else
    {
        // Se cair aqui, o caminho_base está errado
        cout << "Erro: Nao foi possivel abrir a pasta: " << caminho_base << endl;
    }

    return 0;
}