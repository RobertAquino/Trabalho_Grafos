#ifndef OPERATIONS_PARSER_HPP
#define OPERATIONS_PARSER_HPP

#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include "Estruturas.hpp"

using namespace std;

class OperationsParser
{
public:
    vector<Operacao> inicializaParser(string caminho)
    {
        vector<Operacao> lista_operacoes;

        ifstream arquivo(caminho);

        if (!arquivo.is_open())
        {
            cout << "Erro na abertura do arquivo: " << caminho << endl;
            return lista_operacoes;
        }

        string linha;

        int id_operacao = 0;
        int job = 0;

        while (getline(arquivo, linha))
        {
            stringstream separator(linha);

            string id_maquina;
            string tempo;

            while (getline(separator, id_maquina, ',') && getline(separator, tempo, ','))
            {
                Operacao operacao_atual;

                operacao_atual.maquina = stoi(id_maquina);
                operacao_atual.tempo_processamento = stoi(tempo);
                operacao_atual.id_job = job;
                operacao_atual.id = id_operacao;
                operacao_atual.tempo_setup = 0;

                lista_operacoes.push_back(operacao_atual);
                id_operacao++;
            }

            job++;
        }

        arquivo.close();

        return lista_operacoes;
    }
};
#endif