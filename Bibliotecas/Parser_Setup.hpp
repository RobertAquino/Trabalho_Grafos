#ifndef PARSER_SETUP_HPP
#define PARSER_SETUP_HPP

#include "Estruturas.hpp"

class SetupParser
{
public:
    void carregarSetup(string caminho, TabelaSetup &tabela)
    {
        ifstream arquivo(caminho);
        string linha;
        getline(arquivo, linha);

        while (getline(arquivo, linha))
        {
            if (linha.empty())
                continue;
            if (linha.back() == '\r')
                linha.pop_back();

            stringstream ss(linha);
            string m, j1, j2, t;
            getline(ss, m, ',');
            getline(ss, j1, ',');
            getline(ss, j2, ',');
            getline(ss, t);

            tabela.transicoes[stoi(m)][stoi(j1)][stoi(j2)] = stoi(t);
        }
    }

    void carregarInitialSetup(string caminho, TabelaSetup &tabela)
    {
        ifstream arquivo(caminho);
        string linha;
        getline(arquivo, linha);

        while (getline(arquivo, linha))
        {
            if (linha.empty())
                continue;
            if (linha.back() == '\r')
                linha.pop_back();

            stringstream ss(linha);
            string m, j;
            getline(ss, m, ',');
            getline(ss, j);

            tabela.estado_inicial[stoi(m)] = stoi(j);
        }
    }
};
#endif