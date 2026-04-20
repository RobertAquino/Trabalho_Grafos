/*#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include "./Bibliotecas/Estruturas.hpp"
#include "./Bibliotecas/Parser_jobInfo.hpp"
#include "./Bibliotecas/Parser_Operation.hpp"
#include "./Bibliotecas/Parser_Setup.hpp"
#include "./Bibliotecas/Graph_Builder.hpp"

using namespace std;

int main()
{
    JobParser jobParser;
    OperationsParser opParser;
    TabelaSetup setup;
    SetupParser sParser;

    vector<JobInfo> listaJobs = jobParser.inicializaParser("jobs.csv");
    vector<Operacao> listaOps = opParser.inicializaParser("operations.csv");

    sParser.carregarSetup("setup.csv", setup);
    sParser.carregarInitialSetup("initial_setup.csv", setup);

    return 0;
}
    */