#ifndef JOBINFO_PARSER_HPP // <-- Nome arrumado!
#define JOBINFO_PARSER_HPP

#include "Estruturas.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class JobParser
{
public:
    vector<JobInfo> inicializaParser(std::string caminho)
    {
        vector<JobInfo> lista_job;

        ifstream arquivo(caminho);

        if (!arquivo.is_open())
        {
            cout << "Erro na abertura do arquivo: " << caminho << std::endl;
            return lista_job;
        }

        double temp;
        string linha;

        // Pula o cabeçalho
        getline(arquivo, linha);

        while (getline(arquivo, linha))
        {
            stringstream separador(linha);
            string dado;
            JobInfo job_atual;

            getline(separador, dado, ',');
            job_atual.id_job = std::stoi(dado);

            getline(separador, dado, ',');
            job_atual.release_date = std::stoi(dado);

            getline(separador, dado, ',');
            job_atual.due_date = std::stod(dado);

            getline(separador, dado, ',');
            job_atual.earliness_penalty = std::stod(dado);

            getline(separador, dado, ',');
            job_atual.tardiness_penalty = std::stod(dado);

            getline(separador, dado, ',');
            temp = std::stod(dado);
            temp++;

            lista_job.push_back(job_atual);
        }

        arquivo.close();
        return lista_job;
    }
};

#endif