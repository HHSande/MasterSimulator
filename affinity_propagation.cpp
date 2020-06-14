#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <algorithm>
#include "affinity_propagation.h"
#include <limits>
#include <map>

using namespace std;


static const char* dataFileName = "/Users/HansHenrikSande/CLionProjects/MasterSimulator/test.txt";
static const char* bwFileName = "/Users/HansHenrikSande/CLionProjects/MasterSimulator/bw.txt";
static const char* ltFileName = "/Users/HansHenrikSande/CLionProjects/MasterSimulator/lt.txt";
typedef vector<vector<double> > DoubleMatrix;
typedef vector<vector<string> > StringMatrix;
DoubleMatrix listLookUp;
DoubleMatrix bwLookUp;
DoubleMatrix ltLookUp;

class AffinityPropgatation{
    int numberOfDataPoints;
    DoubleMatrix S; //Simalairty matrix
    DoubleMatrix R;
    DoubleMatrix A;
    int iter = 2000;
    double lambda = 0.5;
    int convergence = 10;
    double median = 0.;
    vector<int> assignment;
public:
    class ClusterIds {
    public:
        vector<int> dataCenterIdList;

        ClusterIds(vector<int> val){
            dataCenterIdList = val;
        }
    };

    std::map<int, std::vector<int> > run () {
        updatePreferences();
        int inactive = 0;
        for(int i = 0; i < iter && inactive < convergence; i++){
            for(int j = 0; j < numberOfDataPoints; j++){
                vector<double> aj = A[j];
                vector<double> * rj = &R[j];
                vector<double> sj = S[j];

                double max1(-std::numeric_limits<double>::infinity());
                double max2(-std::numeric_limits<double>::infinity());


                int maxk = -1;

                for(int k = 0; k < numberOfDataPoints; k++){
                    double val = aj[k] + sj[k];

                    if(val > max1){
                        max2 = max1;
                        max1 = val;
                        maxk = k;

                    }else if(val > max2){

                        max2 = val;

                    }


                }


                for(int k = 0; k < numberOfDataPoints; k++){
                    double val = sj[k] - ((k != maxk) ? max1 : max2);
                    (*rj)[k] = (*rj)[k] * lambda + val * (1. - lambda);
                }

            }

            for(int j = 0; j < numberOfDataPoints; j++){
                double colposum = 0;

                for(int k = 0; k < numberOfDataPoints; k++){
                    if(k == j || R[k][j] > 0.){
                        colposum += R[k][j];
                    }
                }

                for (int k = 0; k < numberOfDataPoints; k++) {
                    double val = colposum;
                    // adjust column sum by one extra term
                    if (k == j || R[k][j] > 0.)
                        val -= R[k][j];
                    if (k != j && val > 0.)
                        val = 0.;


                    A[k][j] = A[k][j] * lambda + val * (1 - lambda);

                }
            }
            int changed = 0;
            for (int k = 0; k < numberOfDataPoints; k++) {
                vector<double> ak = A[k];
                vector<double> rk = R[k];
                double max(-std::numeric_limits<double>::infinity());
                int maxj = -1;
                for (int j = 0; j < numberOfDataPoints; j++) {
                    double v = ak[j] + rk[j];
                    if (v > max || (k == j && v >= max)) {
                        max = v;
                        maxj = j;
                    }
                }

                if (assignment[k] != maxj) {
                    changed += 1;
                    assignment[k] = maxj;
                }

            }
            inactive = (changed > 0) ? 0 : (inactive + 1);
        }

        map<int, vector<int>> idClusteredMap;
        int position = 0;
        for (int item : assignment) {
            vector<int> valList;
            if (idClusteredMap.find(item) != idClusteredMap.end())
                valList = idClusteredMap.at(item);
            valList.push_back(position);
            idClusteredMap[item] = valList;
            position++;
        }

        map<int, vector<int>> clusterIdList;

        for(pair<int, vector<int>> x : idClusteredMap){
            clusterIdList.insert(std::make_pair(x.first, x.second));
        }

        return clusterIdList;
    }

    AffinityPropgatation(DoubleMatrix matrix) {
        numberOfDataPoints = matrix.size();
        R = DoubleMatrix(numberOfDataPoints, vector<double>(numberOfDataPoints, 0.));
        A = DoubleMatrix(numberOfDataPoints, vector<double>(numberOfDataPoints, 0.));
        assignment = vector<int>(numberOfDataPoints, 0);
        S = matrix;
    }

    void updatePreferences() {
        if (median == 0.)
            computeMedian(S);

        // if some similarity are empty fill it with the median
        S = fillSimilarityMatrix(S, median);

        for (uint i = 0; i < S.size(); i++){
            S[i][i] = median;
        }
    }


    DoubleMatrix fillSimilarityMatrix(DoubleMatrix matrix, double val) {
        DoubleMatrix ret = matrix;
        for (uint i = 0; i < matrix.size(); i++) {
            for (uint j = 0; j < matrix.size(); j++) {
                if (matrix[i][j] == -1) {
                    matrix[i][j] = val;
                }
            }
        }

        return ret;
    }

    void computeMedian(DoubleMatrix s) {
        double ret = 0;
        vector<double> valList;

        for (uint i = 0; i < s.size(); i++) {
            for (uint j = 0; j < s.size(); j++) {
                if (i == j)
                    continue;
                valList.push_back(s[i][j]);
            }
        }

        sort(valList.begin(), valList.end());

        if (valList.size() % 2 == 0)
            ret = (valList.at(valList.size() / 2) + valList.at(valList.size() / 2 - 1)) / 2;
        else
            ret = valList.at(valList.size() / 2);
        median = ret;


    }


};

std::vector<std::string> tokenizer( const std::string& p_pcstStr, char delim )  {
    std::vector<std::string> tokens;
    std::stringstream   mySstream( p_pcstStr );
    std::string         temp;

    while( getline( mySstream, temp, delim ) ) {
        tokens.push_back( temp );
    }

    return tokens;
}

double getClusterDistance(int currentClusterId, int cluterIdToCheck, ApplicationType applicationType){
    switch(applicationType){
        case ApplicationType::Temp1:
            //std::cout << "VANLIG" << std::endl;
            return listLookUp[currentClusterId][cluterIdToCheck];
            break;
        case ApplicationType::Temp2:
            //std::cout << "BW" << std::endl;

            return bwLookUp[currentClusterId][cluterIdToCheck];
            break;
        case ApplicationType::Temp3:
            //std::cout << "LT" << std::endl;

            return ltLookUp[currentClusterId][cluterIdToCheck];
            break;
        case ApplicationType::Temp4:
            return listLookUp[currentClusterId][cluterIdToCheck];
            break;
    }
}

double getDatacenterDistance(int currentDatacenterId, int datacenterIdToCheck, ApplicationType applicationType){
    switch(applicationType){
        case ApplicationType::Temp1:
            //std::cout << "VANLIG" << std::endl;
            return listLookUp[currentDatacenterId][datacenterIdToCheck];
            break;
        case ApplicationType::Temp2:
            //std::cout << "BW" << std::endl;

            return bwLookUp[currentDatacenterId][datacenterIdToCheck];
            break;
        case ApplicationType::Temp3:
            //std::cout << "LT" << std::endl;

            return ltLookUp[currentDatacenterId][datacenterIdToCheck];
            break;
        case ApplicationType::Temp4:
            return listLookUp[currentDatacenterId][datacenterIdToCheck];
            break;
    }
}

DoubleMatrix convertMatrixManual(StringMatrix list, int num){
    DoubleMatrix ret = DoubleMatrix(list.size(), vector<double>(list.size(), 0));
    unsigned int i = 0;
    std::string::size_type sz;
    for(vector<string> itemlist : list){
        unsigned int j = 0;
        for(string item : itemlist){
            if(i == j){
                ret[i][j] = 0.0;
            }else{
                ret[i][j] = std::stod(item, &sz);
            }
            j++;
        }

        i++;
    }

    switch(num){
        case 0:
            listLookUp = ret;
            break;
        case 1:
            bwLookUp = ret;
            break;
        case 2:
            ltLookUp = ret;
            break;
    }

    return ret;
}

/*void printClusters(hashmap<int, std::vector<int> > clusterIdList){

	foreachpair()
}
*/
std::map<int, std::vector<int>> readS(const char* dfn) {
    //read data
    ifstream myfile(dfn);
    StringMatrix list;
    string line;

    while(getline(myfile, line)){
        list.push_back(tokenizer(line, '\t'));
    }
    ifstream myfile1(bwFileName);
    StringMatrix list1;
    string line1;

    while(getline(myfile1, line1)){
        list1.push_back(tokenizer(line1, '\t'));
    }
    ifstream myfile2(ltFileName);
    StringMatrix list2;
    string line2;

    while(getline(myfile2, line2)){
        list2.push_back(tokenizer(line2, '\t'));
    }

    AffinityPropgatation* temp = new AffinityPropgatation(convertMatrixManual(list, 0));
    AffinityPropgatation* temp1 = new AffinityPropgatation(convertMatrixManual(list1, 1));
    AffinityPropgatation* temp2 = new AffinityPropgatation(convertMatrixManual(list2, 2));

    std::map<int, std::vector<int> >clusterIdList = temp->run();
    delete temp;
    return clusterIdList;
    //printClusters(clusterIdList);

}


std::map<int, std::vector<int> > apCluster()
{
    std::map<int, std::vector<int> > lol = readS(dataFileName);

    /*map<int, std::vector<int>>::iterator it;

    for ( it = lol.begin(); it != lol.end(); it++ )
    {
        std::cout << "Cluster " << it->first << std::endl;
        for(int i = 0; i < it->second.size(); i++){
            std::cout << it->second[i] << " ";
            for(int j = 0; j < it->second.size(); j++){
                std::cout << getDatacenterDistance(it->second[i], it->second[j], ApplicationType::Temp1) << " ";
            }
        }
        std::cout << " "  << std::endl;
    }*/
    return lol;
}

string gotoline(std::ifstream& infile, unsigned int num){
    string st;
    unsigned int n;
    infile.seekg(ios::beg);
    for(n=0;n<num-1;++n){
        infile.ignore(std::numeric_limits<streamsize>::max(), '\n');
    }
    getline(infile,st);
    return st;
}

std::tuple<int,int> getExemplarValue(int id){
    std::ifstream myfile(dataFileName);
    char whiteSpace;
    whiteSpace = ' ';
    string line = gotoline(myfile, id);
    std::vector<string> temp = tokenizer(line, whiteSpace);
    std::vector<int> tempInt;

    for(std::string s : temp){
        int x;
        std::stringstream(s) >> x;
        tempInt.push_back(x);
    }

    return std::make_pair(tempInt[0], tempInt[1]);
}
std::map<int, int>  getWeights(){

    std::ifstream myfile(dataFileName);
    int counter = 0;
    int temp = 1;
    std::map<int, int > hm;
    string STRING;
    char whiteSpace;
    whiteSpace = ' ';

    while(getline(myfile, STRING)){

        vector<string> tokens = tokenizer(STRING, whiteSpace);
        int totalWeight = 0;
        for(string s : tokens){
            int x;
            istringstream(s) >> x;
            totalWeight += x;
        }
        counter++;

        temp++;
    }

    myfile.close();
    return hm;
}


int getAntall(){
    std::ifstream myfile(dataFileName);
    int counter = 0;
    string STRING;
    while(getline(myfile, STRING)){
        counter++;
    }
    return counter;
}
//
// Created by Hans Henrik Sande on 11/04/2020.
//

