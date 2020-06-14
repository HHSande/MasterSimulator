#ifndef AP_INCLUDED
#define AP_INCLUDED

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <map>
#include <sstream>
#include "Framework.h"

std::map<int, std::vector<int> > readS(const char* dfn);

void print(std::vector<int> const &input);
double getClusterDistance(int currentClusterId, int cluterIdToCheck, ApplicationType applicationType);
double getDatacenterDistance(int currentDatacenterId, int datacenterIdToCheck, ApplicationType applicationType);
std::map<int, std::vector<int> > apCluster();

std::map<int, int > getWeights();
std::vector<std::string> tokenizer( const std::string& p_pcstStr, char delim );
int getAntall();

std::tuple<int, int> getExemplarValue(int id);



#endif