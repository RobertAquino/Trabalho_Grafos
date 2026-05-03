#include <iostream>
#include <vector>
#include <string>
#include <dirent.h>
#include <fstream>
#include <iomanip> // Para formatar a saída no terminal (deixar alinhadinho)
#include "./Bibliotecas/Estruturas.hpp"
#include "./Bibliotecas/Parser_JobInfo.hpp"
#include "./Bibliotecas/Parser_Operation.hpp"
#include "./Bibliotecas/Parser_Setup.hpp"
#include "./Bibliotecas/Avaliador.hpp"
#include "./Bibliotecas/BuscaLocal.hpp"

using namespace std;

int main()
{
    string caminho_base = "Pasta do git_Brandimarte/just-in-time-jss-setup-times/instancias_100/";

    DIR *dir;
    struct dirent *ent;

    // Abre o arquivo CSV e cria o novo cabeçalho (com colunas separadas para FIFO e BL)
    ofstream arquivo_saida("resultados_simulacao.csv");
    if (!arquivo_saida.is_open())
    {
        cout << "Erro: Nao foi possivel criar o arquivo de resultados!" << endl;
        return 1;
    }

    arquivo_saida << "Instancia,Makespan_FIFO,Custo_FIFO,Makespan_BL,Custo_BL,N_Jobs,N_Maquinas,Total_Operacoes\n";

    cout << "=========================================================" << endl;
    cout << "           INICIANDO PROCESSAMENTO EM LOTE               " << endl;
    cout << "=========================================================\n"
         << endl;

    if ((dir = opendir(caminho_base.c_str())) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            string nome_pasta = ent->d_name;

            if (nome_pasta.find("instancia_") != string::npos)
            {
                string caminho_completo = caminho_base + nome_pasta;
                string arquivo_operacoes = caminho_completo + "/operations.csv";
                string arquivo_setup = caminho_completo + "/setup.csv";
                string arquivo_jobs = caminho_completo + "/jobs.csv";

                // --- TRAVA DE SEGURANÇA ---
                // Evita que o programa exploda se achar uma pasta sem o operations.csv
                ifstream testa_arquivo(arquivo_operacoes);
                if (!testa_arquivo.is_open())
                {
                    continue;
                }
                testa_arquivo.close();

                // ========================================================
                // 1. LEITURA E CONFIGURAÇÃO DA INSTÂNCIA
                // ========================================================
                int n_jobs = 0, n_maquinas = 0;

                JobParser jobParser;
                vector<JobInfo> lista_jobs = jobParser.inicializaParser(arquivo_jobs);

                OperationsParser operationsParser;
                vector<Operacao> lista_operacoes = operationsParser.inicializaParser(arquivo_operacoes);

                calculaQuantidadesComponentes(n_jobs, n_maquinas, lista_operacoes);

                Instancia instancia;
                instancia.inicializaEstruturas(arquivo_operacoes, arquivo_setup, n_maquinas, n_jobs);

                int total_operacoes = n_jobs * n_maquinas;

                // ========================================================
                // 2. AVALIAÇÃO DA SOLUÇÃO INICIAL (FIFO)
                // ========================================================
                double makespan_fifo = 0;
                vector<double> custos_fifo = calcula_custo_total(instancia, makespan_fifo, lista_jobs, lista_operacoes);

                cout << makespan_fifo << endl;
                double custo_total_fifo = 0;
                for (double c : custos_fifo)
                {
                    custo_total_fifo += c;
                }
                cout << custo_total_fifo << endl;
                // ========================================================
                // 3. OTIMIZAÇÃO (BUSCA LOCAL)
                // ========================================================
                double makespan_bl = 0;
                int iteracoes_bl = 0;
                double custo_total_bl = motorBusca(instancia, lista_jobs, lista_operacoes, makespan_bl, iteracoes_bl, n_maquinas);

                // ========================================================
                // 4. SAÍDA DE DADOS (Terminal e CSV)
                // ========================================================
                cout << ">>> " << nome_pasta << endl;
                cout << "    [Config] Jobs: " << n_jobs << " | Maquinas: " << n_maquinas << " | Op: " << total_operacoes << endl;
                cout << fixed << setprecision(2); // Formata os números quebres com 2 casas decimais
                cout << "    [FIFO]   Makespan: " << makespan_fifo << " | Custo: " << custo_total_fifo << endl;
                cout << "    [B.L.]   Makespan: " << makespan_bl << " | Custo: " << custo_total_bl << " | Passos: " << iteracoes_bl << "\n"
                     << endl;

                // Salva os dados no CSV na ordem correta do cabeçalho
                arquivo_saida << nome_pasta << ","
                              << makespan_fifo << ","
                              << custo_total_fifo << ","
                              << makespan_bl << ","
                              << custo_total_bl << ","
                              << n_jobs << ","
                              << n_maquinas << ","
                              << total_operacoes << "\n";
            }
        }
        closedir(dir);
    }
    else
    {
        cout << "Erro: Nao foi possivel abrir a pasta: " << caminho_base << endl;
    }

    arquivo_saida.close();
    cout << "=========================================================" << endl;
    cout << " PROCESSAMENTO CONCLUIDO! Salvo em 'resultados_simulacao.csv'" << endl;
    cout << "=========================================================" << endl;

    return 0;
}