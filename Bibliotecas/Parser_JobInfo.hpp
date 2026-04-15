#ifndef OPERATIONS_PARSER
#define OPERATIONS_PARSER
#include "Estruturas.hpp"

class JobParser
{
public:
    vector<JobInfo> inicializaParser(string caminho);
};

vector<JobInfo> JobParser::inicializaParser(string caminho)
{
    vector<JobInfo> lista_job;

    ifstream arquivo(caminho);

    if (!arquivo.is_open())
    {
        cout << "Erro na abertura do arquivo";
        return lista_job;
    }

    string linha;

    getline(arquivo, linha);

    while (getline(arquivo, linha))
    {
        stringstream separador(linha);
        string dado;
        JobInfo job_atual;

        getline(separador, dado, ',');
        job_atual.id_job = stoi(dado);

        getline(separador, dado, ',');
        job_atual.release_date = stoi(dado);

        getline(separador, dado, ',');
        job_atual.due_date = stod(dado);

        getline(separador, dado, ',');
        job_atual.earliness_penalty = stod(dado);

        getline(separador, dado, ',');
        job_atual.tardiness_penalty = stod(dado);

        getline(separador, dado, ',');
        job_atual.flow_time_penalty = stod(dado);

        lista_job.push_back(job_atual);
    }

    arquivo.close();
    return lista_job;
}

#endif