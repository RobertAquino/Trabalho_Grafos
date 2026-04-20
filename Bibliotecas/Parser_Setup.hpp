#ifndef PARSER_SETUP_HPP
#define PARSER_SETUP_HPP

#include "Estruturas.hpp"

class SetupParser
{
public:
    void carregarSetup(string caminho, Inst &objetoSetup)
    {
        ifstream arquivo(caminho);
        string linha;

        if (!arquivo.is_open())
        {
            cout << "Erro ao abrir: " << caminho << endl;
            return;
        }

        getline(arquivo, linha);

        while (getline(arquivo, linha))
        {
            if (linha.empty() || linha.find_first_not_of(" \t\r\n") == string::npos)
                continue;

            stringstream ss(linha);
            string m, j1, j2, t;

            if (getline(ss, m, ',') && getline(ss, j1, ',') &&
                getline(ss, j2, ',') && getline(ss, t))
            {

                int mach = stoi(m);
                int job1 = stoi(j1);
                int job2 = stoi(j2);
                int tempo = stoi(t);

                objetoSetup.matriz_setup[mach][job1][job2] = tempo;
            }
        }
        arquivo.close();
    }

    void carregarInitialSetup(string caminho, Instancia &objetoSetup)
    {
        ifstream arquivo(caminho);
        string linha;

        if (!arquivo.is_open())
            return;

        getline(arquivo, linha);

        while (getline(arquivo, linha))
        {
            if (linha.empty())
                continue;

            stringstream ss(linha);
            string m, j;

            if (getline(ss, m, ',') && getline(ss, j))
            {
                objetoSetup.estado_inicial[stoi(m)] = stoi(j);
            }
        }
        arquivo.close();
    }
};
#endif