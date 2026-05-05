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

    ofstream limpa_relatorio("relatorio_grafos.txt");
    limpa_relatorio.close();

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

                vector<int> lista_ordenacao;
                vector<int> rastro_critico;

                Instancia instancia;
                // Inicializa com o Magic Sort (FIFO)
                instancia.inicializaEstruturas(arquivo_operacoes, arquivo_setup, n_maquinas, n_jobs);
                int total_operacoes = n_jobs * n_maquinas;

                // ========================================================
                // 2. AVALIAÇÃO DA SOLUÇÃO INICIAL (FIFO)
                // ========================================================
                double makespan_fifo = 0;
                vector<double> custos_fifo = calcula_custo_total(lista_ordenacao, rastro_critico, instancia, makespan_fifo, lista_jobs, lista_operacoes);
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
                vector<double> custos_spt = calcula_custo_total(lista_ordenacao, rastro_critico, instancia, makespan_spt, lista_jobs, lista_operacoes);
                double custo_total_spt = 0;
                for (double c : custos_spt)
                    custo_total_spt += c;

                // ========================================================
                // 4. OTIMIZAÇÃO (BUSCA TABU)
                // ========================================================
                double makespan_bt = 0;
                int iteracoes_bt = 0;

                // O Motor recebe a fábrica pronta (SPT) e o Grafo já está alinhado com ela
                double custo_total_bt = motorBusca(lista_ordenacao, instancia, lista_jobs, lista_operacoes, makespan_bt, iteracoes_bt, solucao_spt);

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

                // ========================================================
                // 6. RELATÓRIO DE GRAFOS (ARQUIVO .TXT)
                // ========================================================
                lista_ordenacao.clear();
                rastro_critico.clear();
                double makespan_final_txt = 0;

                calcula_custo_total(lista_ordenacao, rastro_critico, instancia, makespan_final_txt, lista_jobs, lista_operacoes);

                // --- REQUISITO 3: CAMINHO GLOBAL ---
                int atual = lista_ordenacao.back();
                vector<int> caminho_global;

                while (atual != -1)
                {
                    caminho_global.push_back(atual);
                    atual = rastro_critico[atual];
                }

                vector<int> caminho_final_certo;
                for (int i = caminho_global.size() - 1; i >= 0; i--)
                {
                    caminho_final_certo.push_back(caminho_global[i]);
                }

                ofstream arquivo_relatorio("relatorio_grafos.txt", ios::app);
                if (arquivo_relatorio.is_open())
                {
                    arquivo_relatorio << "=========================================================\n";
                    arquivo_relatorio << "INSTANCIA: " << nome_pasta << "\n";
                    arquivo_relatorio << "=========================================================\n\n";

                    arquivo_relatorio << "[ REQUISITO 1 ] Ordem Topologica dos Vertices:\n -> ";
                    for (size_t i = 0; i < lista_ordenacao.size(); i++)
                    {
                        arquivo_relatorio << lista_ordenacao[i];
                        if (i < lista_ordenacao.size() - 1)
                            arquivo_relatorio << ", ";
                    }
                    arquivo_relatorio << "\n\n";

                                        arquivo_relatorio << "[ REQUISITO 2 ] Comprimento do Caminho Maximo (Makespan): " << makespan_final_txt << "\n\n";

                    arquivo_relatorio << "[ REQUISITO 3 ] Caminho Maximo Global (Elemento Minimal ao Maximal):\n -> ";
                    for (size_t i = 0; i < caminho_final_certo.size(); i++)
                    {
                        arquivo_relatorio << caminho_final_certo[i];
                        if (i < caminho_final_certo.size() - 1)
                            arquivo_relatorio << " -> ";
                    }
                    arquivo_relatorio << "\n\n";

                    // --- REQUISITO 4: CAMINHOS MAXIMOS POR FINAL DE LINHA (JOB) ---
                    arquivo_relatorio << "[ REQUISITO 4 ] Caminhos Maximos para os Finais de Linha (Ultimas Ops de cada Job):\n";
                    for (int j = 0; j < n_jobs; j++)
                    {

                        int ultima_op_do_job = instancia.jobOperation[j].back();

                        vector<int> caminho_do_job;
                        int rastreio_atual = ultima_op_do_job;

                        while (rastreio_atual != -1)
                        {
                            caminho_do_job.push_back(rastreio_atual);
                            rastreio_atual = rastro_critico[rastreio_atual];
                        }

                        vector<int> caminho_desvirado;
                        for (int i = caminho_do_job.size() - 1; i >= 0; i--)
                        {
                            caminho_desvirado.push_back(caminho_do_job[i]);
                        }

                        arquivo_relatorio << " -> Final da Linha (Job " << j << "): ";
                        for (size_t i = 0; i < caminho_desvirado.size(); i++)
                        {
                            arquivo_relatorio << caminho_desvirado[i];
                            if (i < caminho_desvirado.size() - 1)
                                arquivo_relatorio << " -> ";
                        }
                        arquivo_relatorio << "\n";
                    }
                    arquivo_relatorio << "\n\n";
                    arquivo_relatorio.close();
                }
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

    // CHAMA A RESOLUÇÃO DA PROVA DO PROFESSOR!
    resolve_grafo_professor();
    cout << " RELATORIO EXTRA: 'resposta_grafos_professor.txt' gerado com sucesso!" << endl;

    return 0;
}