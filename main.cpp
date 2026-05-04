#include <iostream>
#include <vector>
#include <string>
#include <dirent.h>
#include <fstream>
#include <iomanip>
#include "./Bibliotecas/Estruturas.hpp"
#include "./Bibliotecas/Parser_JobInfo.hpp"
#include "./Bibliotecas/Parser_Operation.hpp"
#include "./Bibliotecas/Parser_Setup.hpp"
#include "./Bibliotecas/Avaliador.hpp"
#include "./Bibliotecas/BuscaTabu.hpp"

using namespace std;

int main()
{
    string caminho_base = "Pasta do git_Brandimarte/just-in-time-jss-setup-times/instancias_100/";

    DIR *dir;
    struct dirent *ent;

    ofstream arquivo_saida("resultados_simulacao.csv");
    if (!arquivo_saida.is_open())
    {
        cout << "Erro: Nao foi possivel criar o arquivo de resultados!" << endl;
        return 1;
    }

    arquivo_saida << "Instancia,N_Maquinas,N_Jobs,Total_Operacoes,Makespan_FIFO,Custo_FIFO,Makespan_SPT,Custo_SPT,Makespan_BT,Custo_BT\n";

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

                ifstream testa_arquivo(arquivo_operacoes);
                if (!testa_arquivo.is_open())
                    continue;
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
                // Inicializa com o Magic Sort (FIFO)
                instancia.inicializaEstruturas(arquivo_operacoes, arquivo_setup, n_maquinas, n_jobs);
                int total_operacoes = n_jobs * n_maquinas;

                // ========================================================
                // 2. AVALIAÇÃO DA SOLUÇÃO INICIAL (FIFO)
                // ========================================================
                double makespan_fifo = 0;
                vector<double> custos_fifo = calcula_custo_total(instancia, makespan_fifo, lista_jobs, lista_operacoes);
                double custo_total_fifo = 0;
                for (double c : custos_fifo)
                    custo_total_fifo += c;

                // ========================================================
                // 3. AVALIAÇÃO DA SOLUÇÃO INICIAL (SPT)
                // ========================================================
                Solucao solucao_spt;
                transformaSolucaoSPT(instancia, lista_jobs, lista_operacoes, solucao_spt, n_maquinas);

                // MUDANÇA CRUCIAL: Atualizamos o Grafo da Instância na Main
                alteraMachAntecessor(instancia, solucao_spt);
                alteraMachSucessor(instancia, solucao_spt);

                double makespan_spt = 0;
                // Calcula o custo com o Grafo já atualizado para o SPT
                vector<double> custos_spt = calcula_custo_total(instancia, makespan_spt, lista_jobs, lista_operacoes);
                double custo_total_spt = 0;
                for (double c : custos_spt)
                    custo_total_spt += c;

                // ========================================================
                // 4. OTIMIZAÇÃO (BUSCA TABU)
                // ========================================================
                double makespan_bt = 0;
                int iteracoes_bt = 0;

                // O Motor recebe a fábrica pronta (SPT) e o Grafo já está alinhado com ela
                double custo_total_bt = motorBusca(instancia, lista_jobs, lista_operacoes, makespan_bt, iteracoes_bt, solucao_spt);

                // ========================================================
                // 5. SAÍDA DE DADOS (Terminal e CSV)
                // ========================================================
                cout << ">>> " << nome_pasta << endl;
                cout << "    [Config] Jobs: " << n_jobs << " | Maquinas: " << n_maquinas << " | Op: " << total_operacoes << endl;
                cout << fixed << setprecision(2);
                cout << "    [FIFO]   Makespan: " << makespan_fifo << " | Custo: " << custo_total_fifo << endl;
                cout << "    [SPT]    Makespan: " << makespan_spt << " | Custo: " << custo_total_spt << endl;
                cout << "    [Tabu]   Makespan: " << makespan_bt << " | Custo: " << custo_total_bt << " | Passos(Total): " << iteracoes_bt << "\n"
                     << endl;

                // Salva os dados no CSV
                // Salva os dados no CSV na exata ordem do cabeçalho
                arquivo_saida << nome_pasta << ","
                              << n_maquinas << ","
                              << n_jobs << ","
                              << total_operacoes << ","
                              << makespan_fifo << ","
                              << custo_total_fifo << ","
                              << makespan_spt << ","
                              << custo_total_spt << ","
                              << makespan_bt << ","
                              << custo_total_bt << "\n";
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