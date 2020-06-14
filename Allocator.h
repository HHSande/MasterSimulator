//
// Created by Hans Henrik Sande on 09/04/2020.
//

#ifndef MASTERSIMULATOR_ALLOCATOR_H
#define MASTERSIMULATOR_ALLOCATOR_H


#include <map>
#include "Slave.h"
#include <string>
#include <unordered_map>
#include "Framework.h"

enum BundleCategory {
    Small = 0,
    Medium,
    Large,
    Last
};
enum Mode {
    Alg = 0,
    RandomAlg,
    MappedAlg
};

enum BundleMode {
    FineGrained = 0,
    Aggresive,
    Normal
};

class Allocator{
public:
    Mode mode;
    BundleMode bundleMode;
    int cpuThresh;
    int memThresh;
    int bundleSize;
    int numTasks;
    int numFrameworks;
    int numSlaves;
    double avgLatency = 0.0;
    double avgBw = 0.0;
    double execTime;
    double lines = 0.0;
    std::string algParamters;
    std::string randomAlgParamters;
    std::string mappedAlgParameters;
    double wasted;
    bool tasksRandom;
    bool randomSlavesPerDatacenter;
    bool randomTasksPerFramework;
    int datacenters;
    int taskCpu;
    int taskMem;
    int slaveCpu;
    int slaveMem;

    Resource getTotalResourcesOfSlaves();
    int randomNumberFromMap(std::map<int, Framework> running);
    void mappedAlgorithm();
    int getTotalTasksInQueue();
    int getTotalFinishedFrameworks();
    void freeResourcesFromBundlesNotUsed(BundleCategory current, std::map<BundleCategory, std::vector<std::pair<int, Resource>>> bundle);
    int randomlyPickDatacenter(int curretnCluster, std::vector<int> checked);
    int randomlyPickCluster(int currentDatacenter, std::vector<int> checked);
    void finishFramework(int frameworkId);
    std::unordered_map<int, Framework> registeredFrameworks;
    std::map<int, Slave> slaves;
    std::map<int, Framework> runningFrameworks;
    void run(bool pickRandomly);
    Slave* getSlave(int slaveId);
    Framework* getFramework(int frameworkId);
    int getRandomClusterId(std::vector<int> checkedClusters);
    void addFramework(Framework framework);
    void registerSlave(Slave slave);
    Resource getTotalOfferableResources(Slave slave);
    std::map<std::string, int> getResourcesOfBundle(Framework framework, BundleCategory category);
    int getRandomDatacenterId(int currentClusterId, std::vector<int> checkedDatacenters);
    BundleCategory checkPreviousBundles(Framework framework, BundleCategory category, Resource resourcesOfSlave,
                                        std::map<BundleCategory, std::vector<std::pair<int, Resource>>> bundles);
    Resource takeResourceFromSlave(Resource resourcesOfSlave, std::map<std::string, int>& tempMap);
    bool isRemainingEmpty(std::map<std::string, int> remaining);
    BundleCategory getNextCategory(BundleCategory category);
    int findClosestDatacenter(int currentDatacenterId, std::vector<int> checkedDatacenters, ApplicationType applicationType);
    std::vector<int> datacenterSlaves(int currentDatacenterId);
    void writeBundlesToFile(BundleCategory current, int frameworkId, std::vector<std::pair<int, Resource>> resources, bool pickRandomly);
    bool offerCallback(int frameworkId, std::vector<std::pair<int, Resource>> res);
    int findClosestCluster(int currentClusterId, std::vector<int> checkedClusters, ApplicationType applicationType);
    bool checkResourcesForBundle(BundleCategory currentCategory, Resource resourcesOfSlave, std::map<std::string, int> remaining, int num);
    double getSpecificResourceType(Resource toOffer, std::string resourceType);



};
#endif //MASTERSIMULATOR_ALLOCATOR_H
