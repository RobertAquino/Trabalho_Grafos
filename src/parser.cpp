/*#include "../Bibliotecas/parser.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>

vector<Operation> readOperations(string FileName)
{
    vector<Operation> list;geraJobOperation()#ifndef GERAR_INSTANCIAS_HPP
#define GERAR_INSTANCIAS_HPP

#include "Estruturas.hpp"

void Instancia::configurar(int n_maquinas, int n_jobs)
{
    matriz_setup.resize(n_maquinas, vector<vector<int>>(n_jobs, vector<int>(n_jobs, 0)));
    estado_inicial.resize(n_maquinas, 0);
}
void geraJobOperation();

#endif
    ifstream file(FileName);
    string line;

    if (!file.is_open())
    {
        cerr << "Erro ao abrir " << FileName << endl;
        return list;
    }

    int jobIdCount = 0;
    int globalIdCount = 0;

    while (getline(file, line))
    {
        if (line.empty())
            continue;

        stringstream ss(line);
        string mId, pTime;

        while (getline(ss, mId, ',') && getline(ss, pTime, ','))
        {
            Operation op;
            op.id = globalIdCount++;
            op.jobId = to_string(jobIdCount);
            op.machineId = mId;
            op.processTime = stoi(pTime);
            list.push_back(op);
        }
        jobIdCount++;
    }
    file.close();
    return list;
}

SetupTable readSetups(string fileName)
{
    SetupTable table;
    ifstream file(fileName);
    string line;
    getline(file, line);

    while (getline(file, line))
    {
        if (line.empty())
            continue;
        stringstream ss(line);
        string m, j1, j2, t;
        getline(ss, m, ',');
        getline(ss, j1, ',');
        getline(ss, j2, ',');
        getline(ss, t, ',');
        table[m][j1][j2] = stoi(t);
    }
    return table;
}

map<string, string> readInitialJobs(string fileName)
{
    map<string, string> table;
    ifstream file(fileName);
    string line;
    if (!file.is_open())
        return table;

    getline(file, line);

    while (getline(file, line))
    {
        if (line.empty())
            continue;
        stringstream ss(line);
        string mId, initialJob;

        getline(ss, mId, ',');
        getline(ss, initialJob);

        // limpeza simples de espaços
        initialJob.erase(0, initialJob.find_first_not_of(" "));
        table[mId] = initialJob;
    }
    return table;
}*/