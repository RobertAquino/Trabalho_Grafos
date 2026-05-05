#ifndef SETUP_PARSER_HPP
#define SETUP_PARSER_HPP

#include "Estruturas.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class SetupParser
{
public:
    void carregarSetup(string caminho, vector<vector<vector<int>>> &matriz_setup)
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

                matriz_setup[mach][job1][job2] = tempo;
            }
        }
        arquivo.close();
    }
};

#endif