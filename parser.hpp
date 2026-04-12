#ifndef PARSER_HPP
#define PARSER_HPP

#include "graph.hpp"
#include <map>

using namespace std;

/*Maquina -> (JobAnterior -> (JobAtual -> Tempo))*/
typedef map<string, map<string, map<string, int >>> SetupTable;

vector<Operation> readOperations(string FileName);
SetupTable readSetups(string fileName);
map<string, string> readInitialSetups(string fileName); 

#endif