//
// Created by Hans Henrik Sande on 09/04/2020.
//
#include <iostream>
#include "Allocator.h"
#include "Resource.h"
#include "affinity_propagation.h"
#include <cstdlib>
#include <algorithm>
#include <unordered_map>


std::map<int, std::vector<int>> cluster;

Resource Allocator::getTotalOfferableResources(Slave slave){
    return slave.getAvailable();
}
std::map<std::string, int> aggresiveBundle(BundleCategory category){
    std::map<std::string, int> tempHm;

    switch(category){
        case BundleCategory ::Small:
            tempHm.insert(std::make_pair("mem", 6000));
            tempHm.insert(std::make_pair("cpus", 6));
            break;
        case BundleCategory ::Medium:
            tempHm.insert(std::make_pair("mem", 8000));
            tempHm.insert(std::make_pair("cpus", 8));
            break;
        case BundleCategory ::Large:
            tempHm.insert(std::make_pair("mem", 10000));
            tempHm.insert(std::make_pair("cpus", 10));
            break;
        case BundleCategory ::Last:
            break;
    }

    return tempHm;
}

std::map<std::string, int> normalBundle(BundleCategory category){
    std::map<std::string, int> tempHm;

    switch(category){
        case BundleCategory ::Small:
            tempHm.insert(std::make_pair("mem", 2000));
            tempHm.insert(std::make_pair("cpus", 2));
            break;
        case BundleCategory ::Medium:
            tempHm.insert(std::make_pair("mem", 4000));
            tempHm.insert(std::make_pair("cpus", 4));
            break;
        case BundleCategory ::Large:
            tempHm.insert(std::make_pair("mem", 8000));
            tempHm.insert(std::make_pair("cpus", 8));
            break;
        case BundleCategory  ::Last:
            break;
    }

    return tempHm;
}

std::map<std::string, int> fineGrainedBundle(Framework framework, BundleCategory category){
    std::map<std::string, int> tempHm;

    if(category == Last){
        return tempHm;
    }
    for(int i = 0; i < framework.numberOfTasks.size(); i++){
        if(!framework.numberOfTasks[i].finished){
            Resource temp = framework.numberOfTasks[i].required;
            tempHm.insert(std::make_pair("cpus", temp.getCpus()));
            tempHm.insert(std::make_pair("mem", temp.getMem()));
            break;
        }
    }

    return tempHm;



}
int getExemplar(int datacenterId);
std::map<std::string, int> Allocator::getResourcesOfBundle(Framework framework, BundleCategory category){


    switch(bundleMode){
        case Aggresive:
            return aggresiveBundle(category);
            break;
        case Normal:
            return normalBundle(category);
            break;
        case FineGrained:
            return fineGrainedBundle(framework, category);
            break;
    }

}
int Allocator::getRandomClusterId(std::vector<int> checked){

    std::vector<int> v;
    for(std::map<int,std::vector<int>>::iterator it = cluster.begin(); it != cluster.end(); ++it) {
        v.push_back(it->first);
    }

    for(int cluster : checked){
        v.erase(std::remove(v.begin(), v.end(), cluster), v.end());
    }

    if(v.size() == 0){
        return -1;
    }

    int randomDatacenter = rand() % v.size();
    return v[randomDatacenter];

}
std::vector<int> getAllExemplars(){
    std::vector<int> exemplars;

    for(std::pair<int, std::vector<int>> tempPair : cluster){
        exemplars.push_back(tempPair.first);
    }

    return exemplars;
}

int Allocator::randomNumberFromMap(std::map<int, Framework> running){
    std::vector<int> tempVec;

    for(std::pair<int, Framework> x : running){
        tempVec.push_back(x.first);
    }

    int randomNumber = rand() % tempVec.size();

    return tempVec[randomNumber];
}

void Allocator::finishFramework(int frameworkId) {
    //std::cout << "Finishing framework " << std::endl;
    Framework* framework = getFramework(frameworkId);
    std::vector<std::pair<int, Resource>> toFree = framework->slavesServingFramework;
    bool freed = false;
    Slave* slave = nullptr;

    for(std::pair<int, Resource> tempPair : toFree){
        //std::cout << "Relasing resources from slave " << tempPair.first << std::endl;
        //std::cout << tempPair.second.getCpus() << " " << tempPair.second.getMem() << std::endl;
        slave = getSlave(tempPair.first);
        slave->increaseAvailable(tempPair.second);
        freed = true;
    }
    framework->slavesServingFramework.clear();
    runningFrameworks.erase(frameworkId);

    framework->numberOfTasks.erase(
            std::remove_if(
                    framework->numberOfTasks.begin(),
                    framework->numberOfTasks.end(),
                    [](Task const & t) { return t.finished; }
            ),
            framework->numberOfTasks.end()
    );


    if(framework->numberOfTasks.size()==0){
        framework->finished = true;
    }
    //framework->offerAccepted = false;

}

int Allocator::getTotalFinishedFrameworks() {
    int counter = 0;
    for(int i = 0; i < registeredFrameworks.size(); i++){
        Framework temp = registeredFrameworks.at(i);

        if(registeredFrameworks.at(i).numberOfTasks.size() == 0){
            counter++;
        }
    }
    return counter;
}
int Allocator::getRandomDatacenterId(int clusterId, std::vector<int> checked){

    std::vector<int> datacentersInCluster = cluster[clusterId];
    for(int datacenter : checked){
        datacentersInCluster.erase(std::remove(datacentersInCluster.begin(), datacentersInCluster.end(), datacenter), datacentersInCluster.end());
    }

    if(datacentersInCluster.size() == 0){
        return -1;
    }

    int randomDatacenter = rand() % datacentersInCluster.size();
    return datacentersInCluster[randomDatacenter];
}

std::vector<int> getAllDatacenters(){
    std::vector<int> temp;

    for(std::pair<int, std::vector<int>> tempPair : cluster){
        for(int tempInt : tempPair.second){
            temp.push_back(tempInt);
        }
    }

    return temp;
}

int Allocator::randomlyPickDatacenter(int currentDatacenter, std::vector<int> checked) {

    //int randomCluster = randomlyPickCluster(currentDatacenter, checked);

    /*if(randomCluster == -1){
        return -1;
    }*/

    std::vector<int> datacentersInCluster = getAllDatacenters();

    for(int datacenter : checked){
        datacentersInCluster.erase(std::remove(datacentersInCluster.begin(), datacentersInCluster.end(), datacenter), datacentersInCluster.end());
    }

    if(datacentersInCluster.size() == 0){
        return -1;
    }

    int randomDatacenter = rand() % datacentersInCluster.size();
    return datacentersInCluster[randomDatacenter];
}

int Allocator::randomlyPickCluster(int currentCluster, std::vector<int> checked) {

    std::vector<int> clusters = getAllExemplars();
    for(int c : checked){
        clusters.erase(std::remove(clusters.begin(), clusters.end(), c), clusters.end());
    }

    if(clusters.size() == 0){
        return -1;
    }

    int randomDatacenter = rand() % clusters.size();
    return clusters[randomDatacenter];
}
void Allocator::registerSlave(Slave slave){
    Allocator::slaves.insert(std::make_pair(slave.slaveId, slave));
}
/*
BundleCategory checkPreviousBundles(BundleCategory category, std::vector<Resource> resourcesOfSlave,
                                    std::map<int, std::map<BundleCategory, std::vector<std::pair<int, std::vector<Resource>>>>> bundles){
    return BundleCategory::Last;
}
 */

//Legge til bundles som param
Resource Allocator::takeResourceFromSlave(Resource resourcesOfSlave, std::map<std::string, int>& remaining){
    Resource temp;

    std::map<std::string, int> remainingCalc;

    remainingCalc.insert(std::make_pair("mem", resourcesOfSlave.getMem()));
    remainingCalc.insert(std::make_pair("cpus", resourcesOfSlave.getCpus()));

    int memTaken = remainingCalc["mem"] - remaining["mem"];
    int cpuTaken = remainingCalc["cpus"] - remaining["cpus"];

    if(memTaken < 0){
        temp.setMem(remainingCalc["mem"]);
    }else{
        temp.setMem(remaining["mem"]);
    }

    if(cpuTaken < 0){
        temp.setCpus(remainingCalc["cpus"]);
    }else{
        temp.setCpus(remaining["cpus"]);
    }

    int remainingMem = remaining["mem"] - remainingCalc["mem"];
    int remainingCpu = remaining["cpus"] - remainingCalc["cpus"];

    if(remainingMem < 0){
        remaining["mem"] = 0;
    }else{
        remaining["mem"] = remainingMem;
    }

    if(remainingCpu < 0){
        remaining["cpus"] = 0;
    }else{
        remaining["cpus"] = remainingCpu;
    }
    return temp;
} //
bool Allocator::isRemainingEmpty(std::map<std::string, int> remaining){
    for(std::pair<std::string, int> tempPair : remaining){
        if(tempPair.second > 0){
            return false;
        }
    }
    return true;
} //hashmap med string og int som param

BundleCategory Allocator::getNextCategory(BundleCategory category){

    int x = 0;
    for(int i = 0; i < BundleCategory::Last; i++){
        if((BundleCategory) i == category){
            x = i+1;
            break;
        }
    }

    return (BundleCategory) x;
}
void Allocator::addFramework(Framework framework){
    std::cout << "ADDED FRAMEWORK" << std::endl;
    registeredFrameworks.insert(std::make_pair(framework.id, framework));
}

int getExemplar(int datacenterId){
    for(std::pair<int, std::vector<int>> tempPair : cluster){
        if(std::find(tempPair.second.begin(), tempPair.second.end(), datacenterId) != tempPair.second.end()){
            return tempPair.first;
        }
    }

    return -1;
}
int Allocator::findClosestDatacenter(int currentDatacenterId, std::vector<int> checkedDatacenters, ApplicationType applicationType){

    int clusterId = getExemplar(currentDatacenterId);
    std::vector<int> datacentersInCluster = cluster[clusterId];

    for(int datacenter : checkedDatacenters){
        datacentersInCluster.erase(std::remove(datacentersInCluster.begin(), datacentersInCluster.end(), datacenter), datacentersInCluster.end());
    }
    if(datacentersInCluster.size() == 0){
        return -1;
    }

    std::pair<int, double> currBest;

    for(int x : datacentersInCluster){
        double temp = getDatacenterDistance(currentDatacenterId, x, applicationType);
        if(currBest.second != 0.0){
            if(currBest.second < temp){
                currBest = std::make_pair(x, temp);
            }
        }else{
            currBest = std::make_pair(x, temp);
        }
    }
    return currBest.first;
}
std::vector<int> Allocator::datacenterSlaves(int currentDatacenterId){
    std::vector<int> temp;
    for(std::pair<int, Slave> tempPair : slaves){
        Slave* slave = getSlave(tempPair.first);
        if(slave->datacenterId == currentDatacenterId){
            temp.push_back(tempPair.first);
        }
    }
    std::random_shuffle(temp.begin(), temp.end());

    return temp;
}

Slave* Allocator::getSlave(int slaveId){

    auto it = slaves.find(slaveId);
    if(it != slaves.end()){
        return &it->second;
    }
    return NULL;
}
Resource Allocator::getTotalResourcesOfSlaves() {
    Resource r;

    int c = 0;
    int m = 0;

    for(int i = 0; i < slaves.size(); i++){
        Slave slave = slaves.at(i);

        c += slave.getAvailable().getCpus();
        m += slave.getAvailable().getMem();
    }

    r.setCpus(c);
    r.setMem(m);

    return r;
}
Framework* Allocator::getFramework(int frameworkId){

    auto it = registeredFrameworks.find(frameworkId);
    if(it != registeredFrameworks.end()){
        return &it->second;
    }
    return NULL;
}
//TODO: GJØRE OM SLIK AT DEN SKIRVER PÅ CSV FORMAT.
void Allocator::writeBundlesToFile(BundleCategory current, int frameworkId, std::vector<std::pair<int, Resource>> resources, bool pickRandomly){
    //std::ofstream myfile;
    //myfile.open("/Users/HansHenrikSande/CLionProjects/MasterSimulator/bundlesDatacenters.txt", std::ios::app);
    std::ofstream tempFile;
    std::string parametersOfSimulation;
    Framework* framework = getFramework(frameworkId);

    //std::cout << avgLatency << " " << avgBw << std::endl;

    int firstDatacenterId = -1;
    int secondDatacenterId = -1;
    int firstClusterId = -1;
    int secondClusterId = -1;
    double distanceCost = 0.0;
    double bwDistanceCost = 0.0;
    double ltDistanceCost = 0.0;

    for(std::pair<int, Resource> res : resources){
        Slave* slave = getSlave(res.first);
        if(firstClusterId == -1){
            firstClusterId = getExemplar(slave->datacenterId);
            firstDatacenterId = slave->datacenterId;
        }else{
            secondClusterId = getExemplar(slave->datacenterId);
            secondDatacenterId = slave->datacenterId;

            /*if(firstClusterId != secondClusterId){
                //std::cout << "Fra cluster " << firstClusterId << " til cluster " << secondClusterId << std::endl;
                distanceCost += getClusterDistance(firstClusterId, secondClusterId, framework->applicationType);
                bwDistanceCost += 0.0;
                ltDistanceCost
            }*/

            //std::cout << "Fra datacenter " << firstDatacenterId << " til datacenter " << secondClusterId << std::endl;
            distanceCost += getDatacenterDistance(firstDatacenterId, secondDatacenterId, framework->applicationType);
            bwDistanceCost += getDatacenterDistance(firstDatacenterId, secondDatacenterId, Temp2);
            ltDistanceCost += getDatacenterDistance(firstDatacenterId, secondDatacenterId, Temp3);
            firstClusterId = secondClusterId;
            firstDatacenterId = secondDatacenterId;
            lines += 1.0;
        }
    }

    if(bwDistanceCost == 0.0 || ltDistanceCost == 0.0) {
        //tempFile << 1 << ";" << 1 << ";" << resources.size() << ";" << wasted << "\n";
        if(bwDistanceCost == 0.0){
            //std::cout << " La til 1 på bw" << std::endl;
            avgBw += 1.0;
        }else{
            avgBw += bwDistanceCost;
        }

        if(ltDistanceCost == 0.0){
            //std::cout << " La til 1 på lt" << std::endl;

            avgLatency += 1.0;
        }else{
            avgLatency += ltDistanceCost;

        }

        lines += 1.0;

    }else{
        //tempFile << bwDistanceCost << ";" << ltDistanceCost << ";" << resources.size() << ";" << wasted << "\n";
        avgBw += bwDistanceCost;
        avgLatency += ltDistanceCost;
        //std::cout << "La til " << bwDistanceCost <<  " på bw " << std::endl;
        //std::cout << "La til " << ltDistanceCost << " på lt " << std::endl;

        lines += 1.0;
    }

    if(avgLatency/lines > 1.0){
        int c = 0;
    }
    if(avgBw/lines > 1.0){
        int c = 0;
    }

    if(avgBw == NAN){
        int x = 0;
    }
    tempFile.close();
}

bool Allocator::offerCallback(int frameworkId, std::vector<std::pair<int, Resource>> res){
    //std::cout << "Sending offer to framework " << frameworkId << std::endl;
    Framework* framework = getFramework(frameworkId);
    Resource temp;
    double totalCpu = 0.0;
    double totalMem = 0.0;
    for(std::pair<int, Resource> tempPair : res){
        totalCpu += tempPair.second.getCpus();
        totalMem += tempPair.second.getMem();
    }

    temp.setMem(totalMem);
    temp.setCpus(totalCpu);

    bool offerAccepted = framework->sendOffer(temp,res);


    if(!offerAccepted){
        std::cout << "OFFER DENINED" << std::endl;
        for(std::pair<int, Resource> tempPair: res){
            Slave* slave = getSlave(tempPair.first);
            slave->increaseAvailable(tempPair.second);
        }
    }else{
        framework->setSlaves(res);

        //TODO: LEGGE TIL LOGIKK SLIK AT DEN KAN KJØRE SÅ MANGE TASKS SOM MULIG (VELGE DET OFFERET DEN FÅR MEST UT AV)
        for(int i = 0; i < framework->numberOfTasks.size(); i++){
            if(!framework->numberOfTasks[i].finished && framework->enoughResourceForTask(temp, framework->numberOfTasks[i])){
                framework->numberOfTasks[i].run();
                temp.setMem(temp.getMem() - framework->numberOfTasks[i].required.getMem());
                temp.setCpus(temp.getCpus() - framework->numberOfTasks[i].required.getCpus());
            }
        }

        double cpuWasted = 0.0;
        double cheekyConvert = 0.0;
        if(temp.getCpus() > 0){
           cheekyConvert = temp.getCpus();
           cpuWasted = cheekyConvert/totalCpu;
        }

        double memWasted = 0.0;
        if(temp.getMem() > 0){
            cheekyConvert = temp.getMem();
            memWasted = cheekyConvert/totalMem;
        }

        double totalWasted = (cpuWasted + memWasted) / 2;
        wasted += totalWasted;
        std::cout << getTotalTasksInQueue() << std::endl;
        //std::cout << "Resources overallocated " << "cpus " << temp.getCpus() << " " << "mem " << temp.getMem() << std::endl;
        //runningFrameworks.insert(std::make_pair(frameworkId, *framework));
    }

    return offerAccepted;
}

int Allocator::findClosestCluster(int currentClusterId, std::vector<int> checkedClusters, ApplicationType applicationType){
    std::vector<int> exemplars = getAllExemplars();

    for(int exemplar : checkedClusters){
        exemplars.erase(std::remove(exemplars.begin(), exemplars.end(), exemplar), exemplars.end());
    }

    if(exemplars.size() == 0){
        return -1;
    }

    std::pair<int, double> currBest;

    for(int x : exemplars){
        double temp = getClusterDistance(getExemplar(currentClusterId), x, applicationType);
        if(currBest.second != 0.0){
            if(currBest.second < temp){
                currBest = std::make_pair(x, temp);
            }
        }else{
            currBest = std::make_pair(x, temp);
        }
    }

    return currBest.first;
}

double Allocator::getSpecificResourceType(Resource toOffer, std::string resourceType){
    //for(Resource r : toOffer){
        //if(r.)
    //}
    return 0.0;
}
bool Allocator::checkResourcesForBundle(BundleCategory currentCategory, Resource toOffer, std::map<std::string, int> remaining, int num){

    double cpusToOffer = toOffer.getCpus();
    double remainingCpus = remaining["cpus"];

    double ramToOffer =  toOffer.getMem();
    double remainingRam = remaining["mem"];

    //TODO: LOGISK FEIL NÅR THRESH ER 0, VELGER FLERE SLAVER SOM KUN OFFERER 0 RAM OG 0 CPU
    double compareCpus = (cpusToOffer/remainingCpus)*100;
    double compareRam = (ramToOffer/remainingRam)*100;
    bool cpuTreshholdSufficient = compareCpus >= this->cpuThresh;
    bool ramTreshholdSufficient = compareRam >= this->memThresh;

    bool satisfiesTreshhold = cpuTreshholdSufficient && ramTreshholdSufficient;

    bool enoughRam = ramToOffer >= remainingRam;
    bool enoughCpu = cpusToOffer >= remainingCpus;
    //std::cout << "Could satisfy entire request? " << (enoughCpu && enoughRam) << std::endl;
    if(satisfiesTreshhold && !(enoughCpu && enoughRam)){
        //std::cout << "Could ONLY satisfy threshhold " << std::endl;

    }
    if(this->bundleSize-1 == num){
        return enoughCpu && enoughRam;
    }

    return (enoughCpu && enoughRam) || satisfiesTreshhold;
}

BundleCategory Allocator::checkPreviousBundles(Framework framework, BundleCategory currentCategory, Resource resourcesOfSlave, std::map<BundleCategory, std::vector<std::pair<int, Resource>>> bundles){
    BundleCategory temp = BundleCategory::Small;

    while(temp != currentCategory){
        if(bundles[temp].size() > 1){
            if(checkResourcesForBundle(temp, resourcesOfSlave, getResourcesOfBundle(framework, temp), this->bundleSize-1)){
                //std::cout << "Fant et bedre alternativ for " << temp << std::endl;
                return temp;
            }
        }

        temp = getNextCategory(temp);
    }

    return BundleCategory::Last;
}

void Allocator::freeResourcesFromBundlesNotUsed(BundleCategory current,
                                                std::map<BundleCategory, std::vector<std::pair<int, Resource>>> bundle) {
    BundleCategory temp = getNextCategory(current);

    while(temp != BundleCategory::Last){
            std::vector<std::pair<int, Resource>> x = bundle[temp];
            for(std::pair<int, Resource> w : x){
                Slave* s = getSlave(w.first);
                s->increaseAvailable(w.second);
            }


        temp = getNextCategory(temp);
    }
}

std::unordered_map<int, Framework> shuffle(std::unordered_map<int, Framework> toShuffle){
    std::vector<std::pair<int, Framework>> temp;

    for(std::pair<int, Framework> ouff : toShuffle){
        temp.push_back(ouff);
    }

    std::random_shuffle(temp.begin(), temp.end());

    std::unordered_map<int, Framework> tempMap;

    for(int i = 0; i < temp.size(); i++){
        tempMap.insert(temp[i]);
    }

    return tempMap;
}
int Allocator::getTotalTasksInQueue() {
    int total = 0;
    for(int i = 0; i < registeredFrameworks.size(); i++){
        total += registeredFrameworks.at(i).getTasksInQueue();
    }

    return total;
}
/*
void Allocator::mappedAlgorithm() {
    cluster = apCluster();
    std::map<int, std::map<BundleCategory, std::vector<std::pair<int, Resource>>>> bundles;
    std::map<BundleCategory, std::vector<std::pair<int, Resource>>> bundle;

    bool done = false;

    std::unordered_map<int, Framework> shuffledFramworks = shuffle(registeredFrameworks);
    for(std::pair<int, Framework> tempFramework : shuffledFramworks) {
        Framework framework = tempFramework.second;
        if(framework.finished)
            continue;
        BundleCategory currentCategory = BundleCategory::Small;

        std::vector<int> slavesToCheck = datacenterSlaves(framework.datacenterMappedTo);
        std::vector<std::pair<int, Resource>> resourcesFromSlaves;
        std::map<std::string, int> remaining = getResourcesOfBundle(framework, currentCategory);

            for (int slaveId : slavesToCheck) {
                Slave *slave = getSlave(slaveId);
                Resource resourcesOfSlave = getTotalOfferableResources(*slave);
                std::vector<std::pair<int, Resource>> test = resourcesFromSlaves;
                int bundleSize = test.size();
                //std::cout << "Checking slave " << slaveId << std::endl;
                bool resourcesForCurrentBundle = checkResourcesForBundle(currentCategory, resourcesOfSlave, remaining,
                                                                         bundleSize);
                if(!resourcesForCurrentBundle){
                    continue;
                }

                Resource resourcesTakenFromSlave = takeResourceFromSlave(resourcesOfSlave, remaining);

                resourcesFromSlaves.push_back(std::make_pair(slaveId, resourcesTakenFromSlave));
                slave->decreaseAvailable(resourcesTakenFromSlave);

                if (isRemainingEmpty(remaining)) {
                    bundle[currentCategory] = resourcesFromSlaves;
                    //bundles.insert(std::make_pair(framework.id, bundle));
                    bundles[framework.id] = bundle;
                    done = currentCategory == BundleCategory::Large;
                    resourcesFromSlaves.clear();
                    currentCategory = getNextCategory(currentCategory);
                    if (currentCategory == BundleCategory::Last)
                        break;
                    remaining = getResourcesOfBundle(framework, currentCategory);

                    test = resourcesFromSlaves;
                    bundleSize = test.size();

                    while (checkResourcesForBundle(currentCategory, getTotalOfferableResources(*slave),
                                                   getResourcesOfBundle(framework, currentCategory), bundleSize)) {

                        resourcesOfSlave = getTotalOfferableResources(*slave);
                        Resource resourcesTakenFromSlave = takeResourceFromSlave(resourcesOfSlave, remaining);
                        resourcesFromSlaves.push_back(std::make_pair(slaveId, resourcesTakenFromSlave));
                        slave->decreaseAvailable(resourcesTakenFromSlave);

                        if (isRemainingEmpty(remaining)) {
                            //std::cout << "Ferdig med en kateogri " << std::endl;
                            bundle[currentCategory] = resourcesFromSlaves;
                            //bundles.insert(std::make_pair(framework.id, bundle));
                            bundles[framework.id] = bundle;

                            done = currentCategory == BundleCategory::Large;
                            if (done)
                                break;
                            resourcesFromSlaves.clear();
                            currentCategory = getNextCategory(currentCategory);
                            if (currentCategory == BundleCategory::Last) {
                                done = true;
                                break;
                            }
                            remaining = getResourcesOfBundle(framework, currentCategory);
                        }

                        test = resourcesFromSlaves;
                        bundleSize = test.size();

                    }

                }

                if (done)
                    break;

        }
        bundle.clear();
    }

    if(bundles.empty()){
        std::cout << "NO BUNDLES" << std::endl;
    }else{
        for(auto const& tempBundle : bundles){
            for(auto const& bc : bundles[tempBundle.first]){
                BundleCategory current;
                std::vector<std::pair<int, Resource>> vec = bundles[tempBundle.first][bc.first];
                switch(bc.first){
                    case BundleCategory::Small:
                        current = BundleCategory::Small;
                        break;
                    case BundleCategory ::Medium:
                        current = BundleCategory ::Medium;
                        break;
                    case BundleCategory ::Large:
                        current = BundleCategory ::Large;
                        break;
                    case BundleCategory ::Last:
                        break;
                }

                if(offerCallback(tempBundle.first, vec)){
                    freeResourcesFromBundlesNotUsed(current, bundles[tempBundle.first]);
                    writeBundlesToFile(current, tempBundle.first, vec, false);
                    break;
                }
            }
        }
    }
}
*/

void tempTest(){

    //Assemble alle kategorier av bundles
    //Legge de til et framework
    //
    cluster = apCluster();

    std::map<int, std::map<BundleCategory, std::vector<std::pair<int, Resource>>>> bundles;
    std::map<BundleCategory, std::vector<std::pair<int, Resource>>> bundle;

    int counter = 0;

    while(true){

    }

}
void Allocator::mappedAlgorithm(){
    cluster = apCluster();
    std::map<int, std::map<BundleCategory, std::vector<std::pair<int, Resource>>>> bundles;
    std::map<BundleCategory, std::vector<std::pair<int, Resource>>> bundle;
    std::unordered_map<int, Framework> shuffledFramworks = shuffle(registeredFrameworks);
    int counter = 0;
    //while()
}
void Allocator::run(bool pickRandomly){
    cluster = apCluster();
    std::map<int, std::map<BundleCategory, std::vector<std::pair<int, Resource>>>> bundles;
    std::map<BundleCategory, std::vector<std::pair<int, Resource>>> bundle;

    bool done = false;

    //Shuffle
    std::unordered_map<int, Framework> shuffledFramworks = shuffle(registeredFrameworks);
    for(std::pair<int, Framework> tempFramework : shuffledFramworks){
        Framework framework = tempFramework.second;
        if(framework.finished){
            continue;
        }
        done = false;
        BundleCategory currentCategory = BundleCategory::Small;

        std::vector<int> checkedDatacenters;
        std::vector<int> checkedClusters;

        int currentClusterId;
        int currentDatacenterId;
        //hente fra cluster

        if(mode == MappedAlg){
            currentDatacenterId = framework.datacenterMappedTo;
            currentClusterId = getExemplar(currentDatacenterId);
        }else{
            currentClusterId = getRandomClusterId(checkedClusters); //Sørge for at vi ikke velger noe som alt er sjekket
            currentDatacenterId = getRandomDatacenterId(currentClusterId, checkedDatacenters);
        }

        std::vector<int> slavesToCheck = datacenterSlaves(currentDatacenterId);
        std::vector<std::pair<int, Resource>> resourcesFromSlaves;

        std::map<std::string, int> remaining = getResourcesOfBundle(framework, currentCategory);

        while(!done) {
            for (int slaveId : slavesToCheck) {
                Slave *slave = getSlave(slaveId);
                Resource resourcesOfSlave = getTotalOfferableResources(*slave);
                std::vector<std::pair<int, Resource>> test = resourcesFromSlaves;
                int bundleSize = test.size();

                bool resourcesForCurrentBundle = checkResourcesForBundle(currentCategory, resourcesOfSlave, remaining,
                                                                         bundleSize);

                if (!resourcesForCurrentBundle/* && !pickRandomly*/) {
                    BundleCategory betterOptionForPreviousBundle = checkPreviousBundles(framework, currentCategory,
                                                                                        resourcesOfSlave,
                                                                                        bundles[framework.id]);

                    if (betterOptionForPreviousBundle == BundleCategory::Last) {
                        continue;
                    }

                    for (std::pair<int, Resource> tempPair : bundles[framework.id][betterOptionForPreviousBundle]) {
                        Slave *tempSlave = getSlave(tempPair.first);
                        tempSlave->increaseAvailable(tempPair.second);
                    }

                    std::map<std::string, int> tempRem = getResourcesOfBundle(framework, betterOptionForPreviousBundle);
                    Resource tempTakeFromSlave = takeResourceFromSlave(resourcesOfSlave, tempRem);

                    std::vector<std::pair<int, Resource>> tempVec;
                    tempVec.push_back(std::make_pair(slaveId, tempTakeFromSlave));
                    slave->decreaseAvailable(tempTakeFromSlave);

                    bundle[betterOptionForPreviousBundle] = tempVec;
                    //bundles.insert(std::make_pair(framework.id, bundle));
                    bundles[framework.id] = bundle;
                    continue;
                }

                Resource resourcesTakenFromSlave = takeResourceFromSlave(resourcesOfSlave, remaining);

                resourcesFromSlaves.push_back(std::make_pair(slaveId, resourcesTakenFromSlave));
                slave->decreaseAvailable(resourcesTakenFromSlave);

                if (isRemainingEmpty(remaining)) {
                    bundle[currentCategory] = resourcesFromSlaves;
                    //bundles.insert(std::make_pair(framework.id, bundle));
                    bundles[framework.id] = bundle;
                    done = currentCategory == BundleCategory::Large;
                    resourcesFromSlaves.clear();
                    currentCategory = getNextCategory(currentCategory);
                    if (currentCategory == BundleCategory::Last)
                        break;
                    remaining = getResourcesOfBundle(framework, currentCategory);

                    test = resourcesFromSlaves;
                    bundleSize = test.size();

                    while (checkResourcesForBundle(currentCategory, getTotalOfferableResources(*slave),
                                                   getResourcesOfBundle(framework, currentCategory), bundleSize)) {

                        resourcesOfSlave = getTotalOfferableResources(*slave);
                        Resource resourcesTakenFromSlave = takeResourceFromSlave(resourcesOfSlave, remaining);
                        resourcesFromSlaves.push_back(std::make_pair(slaveId, resourcesTakenFromSlave));
                        slave->decreaseAvailable(resourcesTakenFromSlave);

                        if (isRemainingEmpty(remaining)) {
                            bundle[currentCategory] = resourcesFromSlaves;
                            bundles[framework.id] = bundle;

                            done = currentCategory == BundleCategory::Large;
                            resourcesFromSlaves.clear();
                            if (done)
                                break;
                            currentCategory = getNextCategory(currentCategory);
                            if (currentCategory == BundleCategory::Last) {
                                done = true;
                                break;
                            }
                            remaining = getResourcesOfBundle(framework, currentCategory);
                        }

                        test = resourcesFromSlaves;
                        bundleSize = test.size();
                    }

                }

                if (done)
                    break;
            }

            if (done)
                break;


            checkedDatacenters.push_back(currentDatacenterId);
            if (!pickRandomly) {
                int closestDatacenter = findClosestDatacenter(currentDatacenterId, checkedDatacenters, framework.applicationType);

                if (closestDatacenter == -1) {
                    //std::cout << "Sjekket alle datacentre i cluster " << currentClusterId << std::endl;
                    checkedClusters.push_back(currentClusterId);
                    int closestCluster = findClosestCluster(currentClusterId, checkedClusters, framework.applicationType);
                    if (closestCluster == -1) {
                        //std::cout << "SJEKKET ALLE CLUSTERS" << std::endl;
                        break;
                    }

                    for(std::pair<int, Resource> toClear : resourcesFromSlaves){
                        Slave *tempSlave = getSlave(toClear.first);
                        tempSlave->increaseAvailable(toClear.second);
                    }

                    resourcesFromSlaves.clear();
                    currentClusterId = closestCluster;
                    currentDatacenterId = getRandomDatacenterId(currentClusterId, checkedDatacenters);
                    slavesToCheck = datacenterSlaves(currentDatacenterId);
                } else {
                    currentDatacenterId = closestDatacenter;
                    slavesToCheck = datacenterSlaves(currentDatacenterId);
                }
            }else{
                //std::cout << "FITTER FORSATT EHR DA" << std::endl;
                int nextDatacenter = randomlyPickDatacenter(currentDatacenterId, checkedDatacenters);
                std::cout << "Fra " << currentDatacenterId << " til " << nextDatacenter <<  "curent category " << currentCategory <<  std::endl;

                if(nextDatacenter == -1){
                    std::cout << "SJEKKET ALT INGENTING FUNKA" << std::endl;
                    break;
                }else{
                    currentClusterId = getExemplar(nextDatacenter);
                    currentDatacenterId = nextDatacenter;
                    slavesToCheck = datacenterSlaves(currentDatacenterId);
                }
            }
        }

        done = false;
        bundle.clear();
    }
    std::cout << "SJEKKET ALLE FRAMEWORKS " << std::endl;
    if(bundles.empty()){
        std::cout << "NO BUNDLES" << std::endl;
    }else{
        for(auto const& tempBundle : bundles){
            for(auto const& bc : bundles[tempBundle.first]){
                BundleCategory current;
                std::vector<std::pair<int, Resource>> vec = bundles[tempBundle.first][bc.first];
                switch(bc.first){
                    case BundleCategory::Small:
                        current = BundleCategory::Small;
                        break;
                    case BundleCategory ::Medium:
                        current = BundleCategory ::Medium;
                        break;
                    case BundleCategory ::Large:
                        current = BundleCategory ::Large;
                        break;
                    case BundleCategory ::Last:
                        break;
                }

                if(offerCallback(tempBundle.first, vec)){
                    freeResourcesFromBundlesNotUsed(current, bundles[tempBundle.first]);
                    writeBundlesToFile(current, tempBundle.first, vec, pickRandomly);
                    break;
                }
            }
        }
    }
}
