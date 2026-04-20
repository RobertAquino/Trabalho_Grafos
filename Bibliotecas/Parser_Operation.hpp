#ifndef PARSER_ARESTA
#define PARSER_ARESTA
#include "Estruturas.hpp"

class OperationsParser
{
public:
    vector<Operacao> inicializaParser(string caminho);
};

vector<Operacao> OperationsParser::inicializaParser(string caminho)
{
    vector<Operacao> lista_operacoes;

    ifstream arquivo(caminho);

    if (!arquivo.is_open())
    {
        cout << "Erro na abertura do arquivo";
        return lista_operacoes;
    }
    string linha;

    int id_global = 0;
    int id_atual = 0;
    while (getline(arquivo, linha))
    {
        stringstream separator(linha);

        string id_maquina;
        string tempo;

        while (getline(separator, id_maquina, ',') && getline(separator, tempo, ','))
        {
            Operacao operacao_atual;

            operacao_atual.id_maquina = stoi(id_maquina);
            operacao_atual.tempo_processamento = stoi(tempo);
            operacao_atual.id_job = id_atual;
            operacao_atual.id = id_global;
            operacao_atual.grau_entrada = 0;
            operacao_atual.tempo_mais_cedo = 0;
            lista_operacoes.push_back(operacao_atual);
            id_global++;
        }

        id_atual++;
    }

    arquivo.close();
    return lista_operacoes;
}
#endif