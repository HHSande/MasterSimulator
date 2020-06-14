#include <iostream>
#include <fstream>
#include "Master.h"
#include "Allocator.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "affinity_propagation.h"

float normalize(float input)
{
    float min = 1;
    float max = 500;
    return 1 - ((input - min) / (max - min));
}

void test(int numDatacenters, const char* bwFile, const char* ltFile, const char* bwltFile){
    double bandwidth[numDatacenters][numDatacenters];
    srand(time(NULL));
    for(int i = 0; i < numDatacenters; i++){
        for(int j = 0; j < numDatacenters; j++){
            int randomNumber = (int)((double)rand() / ((double)RAND_MAX + 1) * 10000);
            //int randomNumber = rand() % 7000 + 1000;
            double divided = randomNumber/10000.0;
            if(i == j){
                bandwidth[i][j] = 0.0;
            }else{
                bandwidth[i][j] = divided;
            }
        }
    }

    std::ofstream tempFile;
    tempFile.open(bwFile);
    for(int i = 0; i < numDatacenters; i++){
        for(int j = 0; j < numDatacenters; j++){
            tempFile << bandwidth[i][j] << "\t";
        }
        tempFile << "\n";
    }

    tempFile.close();

    double latency[numDatacenters][numDatacenters];

    for(int i = 0; i < numDatacenters; i++){
        for(int j = 0; j < numDatacenters; j++){
            int randomNumber = (int)((double)rand() / ((double)RAND_MAX + 1) * 10000);
            //int randomNumber = rand() % 6000 + 4000;
            double divided = randomNumber/10000.0;
            if(i == j){
                latency[i][j] = 0.0;
            }else{
                //latency[i][j] = divided;
            }
        }
    }

    for(int i = 0; i < numDatacenters; i++){
        for(int j = 0; j < numDatacenters; j++) {
            int randomNumber = (int)((double)rand() / ((double)RAND_MAX + 1) * 10000);
            //int randomNumber = rand() % 6000 + 4000;
            double divided = randomNumber/10000.0;
            if(i != j){
                latency[i][j] = divided;
                latency[j][i] = divided;
            }
        }
    }

    tempFile.open(ltFile);
    for(int i = 0; i < numDatacenters; i++){
        for(int j = 0; j < numDatacenters; j++){
            tempFile << latency[i][j] << "\t";
        }
        tempFile << "\n";
    }
    tempFile.close();

    tempFile.open(bwltFile);
    for(int i = 0; i < numDatacenters; i++){
        for(int j = 0; j < numDatacenters; j++){
            tempFile << ((latency[i][j] + bandwidth[i][j])/2) << "\t";
        }
        tempFile << "\n";
    }

    tempFile.close();
    /*
    std::map<int, std::vector<int>> cluster = apCluster();
    std::vector<int> v;
    for(std::map<int,std::vector<int>>::iterator it = cluster.begin(); it != cluster.end(); ++it) {
        v.push_back(it->first);
    }

    for(int i = 0; i < v.size(); i++){
        std::cout << "Cluster " << v[i] << std::endl;
        std::vector<int> datacenters = cluster[v[i]];
        for(int j = 0; j < datacenters.size(); j++){
            std::cout << getDatacenterDistance(v[i], datacenters[j], ApplicationType::Temp1) << " ";
        }
        std::cout << "" << std::endl;
    }
     */

}

void runSimulation(std::string baseUrl, Allocator &a){
    std::ofstream tempFile;

    std::string parametersOfSimulation = "/";
    parametersOfSimulation += std::to_string(a.mode) + "_";
    parametersOfSimulation += std::to_string(a.numSlaves) + "_";
    parametersOfSimulation += std::to_string(a.slaveCpu) + "_" + std::to_string(a.slaveMem) + "_";
    parametersOfSimulation += std::to_string(a.numFrameworks) + "_";
    parametersOfSimulation += std::to_string(a.numTasks) + "_";
    parametersOfSimulation += std::to_string(a.taskCpu) + "_" + std::to_string(a.taskMem) + "_";
    parametersOfSimulation += std::to_string(a.cpuThresh) + "_";
    parametersOfSimulation += std::to_string(a.datacenters) + "_";
    parametersOfSimulation += std::to_string(a.bundleMode) + "_";
    parametersOfSimulation += std::to_string(a.bundleSize) + ".txt";

    a.algParamters = parametersOfSimulation;


    std::clock_t start;
    double duration;
    start = std::clock();

    switch(a.mode){
        case Alg:
            while(a.getTotalTasksInQueue() != 0){
                a.run(false);

                for(int z = 0; z < a.registeredFrameworks.size(); z++){
                    Framework* temp = &a.registeredFrameworks.at(z);
                    a.finishFramework(temp->id);
                }
            }
            break;
        case RandomAlg:
            while(a.getTotalTasksInQueue() != 0){
                a.run(true);

                for(int z = 0; z < a.registeredFrameworks.size(); z++){
                    Framework* temp = &a.registeredFrameworks.at(z);
                    a.finishFramework(temp->id);
                }
            }
            break;
        case MappedAlg:
            int datacenterToMapTo = 0;
            //ERROR HER
            int rest = a.registeredFrameworks.size() & a.datacenters;
            int per = a.registeredFrameworks.size() - rest;
            int frameworksPerDatecenter = per / a.datacenters;

            int counter = frameworksPerDatecenter;
            for(int z = 0; z < per; z++){
                if(z >= counter){
                    counter += frameworksPerDatecenter;
                    datacenterToMapTo++;

                }
                a.registeredFrameworks.at(z).datacenterMappedTo = datacenterToMapTo;
            }

            datacenterToMapTo = 0;
            for(int j = per; j < a.registeredFrameworks.size(); j++){
                a.registeredFrameworks.at(j).datacenterMappedTo = datacenterToMapTo;
                datacenterToMapTo++;
            }

            while(a.getTotalTasksInQueue() != 0){
                a.run(true);
                for(int z = 0; z < a.registeredFrameworks.size(); z++){
                    Framework* temp = &a.registeredFrameworks.at(z);
                    a.finishFramework(temp->id);
                }
            }
            break;
    }

    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    std::cout<<"printf: "<< duration << '\n';
    tempFile.open(baseUrl, std::ios::app);
    tempFile << duration;
    tempFile.close();
}

enum ChangingParameter{
    Slaves = 0,
    SlaveResources,
    Frameworks,
    Tasks,
    TaskResources,
    Threshhold,
    Datacenter,
    Bundles,
    BundleType,
    DefaultMode
};


std::string variableParameter(ChangingParameter param){
    switch(param){
        case Slaves:
            return "Variable_slave_number";
            break;
        case SlaveResources:
            return "Variable_slave_resources";
            break;
        case Frameworks:
            return "Variable_framework_number";
            break;
        case Tasks:
            return "Variable_task_number";
            break;
        case TaskResources:
            return "Variable_task_resources";
            break;
        case Threshhold:
            return "Variable_threshhold";
            break;
        case Datacenter:
            return "Variable_datacenter_number";
            break;
        case Bundles:
            return "Variable_bundle_sizes";
            break;
        case BundleType:
            return "Variable_bundle_type";
            break;
        case DefaultMode:
            return "Default_mode";
            break;

    }
}
std::string fileNameBuilder(ChangingParameter param, Allocator a){

    int flaggedValue = -1;
    int resourceFlagged = -1;

    std::vector<std::string> params;

    params.push_back(std::to_string(a.numSlaves));
    params.push_back(std::to_string(a.slaveCpu));
    params.push_back(std::to_string(a.slaveMem));
    params.push_back(std::to_string(a.numFrameworks));
    params.push_back(std::to_string(a.numTasks));
    params.push_back(std::to_string(a.taskCpu));
    params.push_back(std::to_string(a.taskMem));
    params.push_back(std::to_string(a.cpuThresh));
    params.push_back(std::to_string(a.datacenters));
    params.push_back(std::to_string(a.bundleSize));
    params.push_back(std::to_string(a.bundleMode));

    switch(param){
        case Slaves:
            flaggedValue = 0;
            break;
        case SlaveResources:
            flaggedValue = 1;
            resourceFlagged = 2;
            break;
        case Frameworks:
            flaggedValue = 3;
            break;
        case Tasks:
            flaggedValue = 4;
            break;
        case TaskResources:
            flaggedValue = 5;
            flaggedValue = 6;
            break;
        case Threshhold:
            flaggedValue = 7;
            break;
        case Datacenter:
            flaggedValue = 8;
            break;
        case Bundles:
            flaggedValue = 9;
            break;
        case BundleType:
            flaggedValue = 10;
            break;

    }

    if(param != DefaultMode) {
        if (resourceFlagged > -1) {
            //Mem og CPU skal flagges
            params[flaggedValue] = params[flaggedValue] + "V";
            params[resourceFlagged] = params[resourceFlagged] + "V";
        } else {
            params[flaggedValue] = params[flaggedValue] + "V";
        }
    }

    std::string toReturn;

    for(int i = 0; i < params.size(); i++){
        if(i == params.size()-1){
            toReturn += params[i];
        }else{
            toReturn += params[i] + "_";
        }
    }

    return toReturn;
}

bool dirExists(const char* pathname){
    struct stat info;

    if( stat( pathname, &info ) != 0 ){
        printf( "cannot access %s\n", pathname );
    }
    else if( info.st_mode & S_IFDIR ){
        printf( "%s is a directory\n", pathname );
        return true;
    }
    else{
        printf( "%s is no directory\n", pathname );
        return false;
    }

    return false;
}

bool dirMaker(const char* dirName){

    int status;
    status = mkdir(dirName, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

    if(status != 0){
        std::cout << "Error creating directory";
        return false;
    }else{
        std::cout << "Successfully created directory";
        return true;
    }


}

int main() {

    /*test(
            10,
            "/Users/HansHenrikSande/CLionProjects/MasterSimulator/bw.txt",
            "/Users/HansHenrikSande/CLionProjects/MasterSimulator/lt.txt",
            "/Users/HansHenrikSande/CLionProjects/MasterSimulator/test.txt");


    return 0;
    */
    int numSlavesRequested;
    int slaveCpuRequested;
    int slaveMemRequested;
    int numDatacentersRequested;
    int bundleSizeRequested;
    int numFrameworksRequested;
    int numTasksRequested;
    int taskCpuRequested;
    int taskMemRequested;
    int bundleTypeRequested;

    std::vector<int> variancesToObserve;

    std::cout << "Enter number of slaves" << std::endl;
    std::cin >> numSlavesRequested;

    std::cout << "Enter cpus of each slave" << std::endl;
    std::cin >> slaveCpuRequested;

    std::cout << "Enter memory of each slave" << std::endl;
    std::cin >> slaveMemRequested;

    std::cout << "Enter number of datacenters" << std::endl;
    std::cin >> numDatacentersRequested;

    std::cout << "Enter maximum bundle size for each offer" << std::endl;
    std::cin >> bundleSizeRequested;

    std::cout << "Enter number of frameworks" << std::endl;
    std::cin >> numFrameworksRequested;

    std::cout << "Enter number of tasks" << std::endl;
    std::cin >> numTasksRequested;

    std::cout << "Enter cpus of each task" << std::endl;
    std::cin >> taskCpuRequested;

    std::cout << "Enter memory of each task" << std::endl;
    std::cin >> taskMemRequested;

    std::cout << "Enter bundling strategy" << std::endl;
    std::cin >> bundleTypeRequested;

    bool observeBundleSize = false;
    bool observeFrameworks = false;
    bool observeTasks = false;
    bool observeTaskResources = false;
    bool observeSlaves = false;
    bool observeSlaveResource = false;
    bool observeDatacenter = false;
    bool observeBundleType = false;
    bool defaultMode = false;

    int input = -1;
    while(input != 0) {
        std::cout << "Which variances to you want to observe with the given paramters?" << std::endl;
        std::cout << "Variable bundle sizes (1)" << std::endl;
        std::cout << "Variable task number (2)" << std::endl;
        std::cout << "Variable task resources (3)" << std::endl;
        std::cout << "Variable number of datacenters (4)" << std::endl;
        std::cout << "Variable number of frameworks (5)" << std::endl;
        std::cout << "Variable slave resources (6)" << std::endl;
        std::cout << "Variable bundling strategies (7)" << std::endl;
        std::cout << "Variable number of slaves (8)" << std::endl;
        std::cout << "Default mode (9)" << std::endl;
        std::cout << "Enter 0 to finish input and start simulation" << std::endl;
        std::cin >> input;
        if(input != 0){
            switch(input){
                case 1:
                    observeBundleSize = true;
                    break;
                case 2:
                    observeTasks = true;
                    break;
                case 3:
                    observeTaskResources = true;
                    break;
                case 4:
                    observeDatacenter = true;
                    break;
                case 5:
                    observeFrameworks = true;
                    break;
                case 6:
                    observeSlaveResource = true;
                    break;
                case 7:
                    observeBundleType = true;
                    break;
                case 8:
                    observeSlaves = true;
                    break;
                case 9:
                    defaultMode = true;
                    break;
            }
        }
    }

    Allocator a;

    //Slots

    std::vector<int> datacenterSlots = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 150, 200};
    std::vector<int> slavesSlots = {100, 200, 300, 400, 500, 600, 800, 900, 1000};
    std::vector<int> frameworksSlots = {50, 100, 150, 200, 250, 300, 350, 400, 450, 500};
    std::vector<int> bundleModeSlots = {Normal, Aggresive, FineGrained};
    std::vector<int> modeSlots = {Alg, RandomAlg, MappedAlg};
    std::vector<int> taskSlots = {5, 10, 15, 20, 25, 30, 35, 40, 45, 50};
    std::vector<int> bundleSizeSlots = {1, 2, 3, 4, 5, 6};
    std::vector<int> threshSlots = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    std::vector<int> slaveCpuSlots = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    std::vector<int> slaveMemSlots = {2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000, 11000, 12000};
    std::vector<int> taskCpuSlots = {1, 2, 3, 4, 5, 6, 7, 8};
    std::vector<int> taskMemClots = {1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000};


    //Teste slots
    int tempSlaveCpu = slaveCpuRequested;
    int tempSlaveMem = slaveMemRequested;
    int tempBundleSize = bundleSizeRequested;
    int tempFrameworkSlots = numFrameworksRequested;
    int tempSlaves = numSlavesRequested;
    int tempDatacenters = numDatacentersRequested;
    int tempTaskMem = taskMemRequested;
    int tempTaskCpu = taskCpuRequested;
    int tempTasks = numTasksRequested;
    int tempBundleType = bundleTypeRequested;

    //Preppe allocatoren
    std::unordered_map<int, Framework> frameworks;
    std::map<int, Slave> simulationSlaves;

    int numSlaves = tempSlaves;
    int datacenters = tempDatacenters;
    int slaveMem = tempSlaveMem;
    int slaveCpu = tempSlaveCpu;
    int rest = numSlaves % datacenters;
    int slavesToDatacenter = (numSlaves-rest)/datacenters;
    int counter = slavesToDatacenter;
    int datacenterId = 0;
    int numTasks = tempTasks;
    int numFrameworks = tempFrameworkSlots;
    int bundleType = tempBundleType;

    for(int u = 0; u < (numSlaves-rest); u++){
        Resource slaveRes(slaveCpu, slaveMem, 4000); //PICK RANDOMLY
        if(u >= counter){
            datacenterId++;
            counter += slavesToDatacenter;
        }
        Slave slave(u, slaveRes, datacenterId);
        simulationSlaves.insert(std::make_pair(slave.slaveId, slave));
    }

    datacenterId = 0;

    for(int x = (numSlaves-rest); x < numSlaves; x++){
        Resource slaveRes(slaveCpu, slaveMem, 4000); //PICK RANDOMLY
        Slave slave(x, slaveRes, datacenterId);
        simulationSlaves.insert(std::make_pair(slave.slaveId, slave));
        datacenterId++;
    }

    a.slaves = simulationSlaves;
    a.numSlaves = numSlaves;

    int taskMem = tempTaskMem;
    int taskCpu = tempTaskCpu;

    for(int i = 0; i < numFrameworks; i++){
        std::vector<Task> tasks;
        int tasksPerFramework = numTasks == 0 || numFrameworks == 0 ? 0 : numTasks;
        for(int x = 0; x < tasksPerFramework; x++){
            Task task;
            Resource k(taskCpu, taskMem, 4000);
            task.required = k;
            tasks.push_back(task);
        }
        Framework framework(i, tasks, ApplicationType::Temp1);
        frameworks.insert(std::make_pair(framework.id, framework));
    }

    a.bundleMode = (BundleMode) bundleType;
    a.registeredFrameworks = frameworks;
    a.numFrameworks = numFrameworks;
    a.numTasks = numTasks;
    a.slaves = simulationSlaves;
    a.cpuThresh = 100/tempBundleSize;
    a.memThresh = 100/tempBundleSize;
    a.bundleSize = tempBundleSize;
    a.slaveMem = slaveMem;
    a.slaveCpu = slaveCpu;
    a.datacenters = datacenters;
    a.taskCpu = taskCpu;
    a.taskMem = taskMem;

    test(
            datacenters,
            "/Users/HansHenrikSande/CLionProjects/MasterSimulator/bw.txt",
            "/Users/HansHenrikSande/CLionProjects/MasterSimulator/lt.txt",
            "/Users/HansHenrikSande/CLionProjects/MasterSimulator/test.txt");


    std::string dirName;
    std::string parentDirName;
    std::string baseUrl;
    std::string toCheck;

    bool dirAlreadyExist;
    std::string specificFile;
    bool skip;

    if(defaultMode){
        dirName = fileNameBuilder(DefaultMode, a);
        parentDirName = variableParameter(DefaultMode);
        baseUrl = "/Users/HansHenrikSande/CLionProjects/MasterSimulator/GraphFiles/";
        toCheck = baseUrl + parentDirName;

        dirAlreadyExist = dirExists(toCheck.data());

        if (!dirAlreadyExist) {
            dirMaker(toCheck.data());
        } else {
            std::cout << "Dirname " << dirName << " already exists" << std::endl;
        }

        specificFile = toCheck + "/" + dirName;

        dirAlreadyExist = dirExists(specificFile.data());
        skip = false;
        if (!dirAlreadyExist) {
            dirMaker(specificFile.data());
        } else {
            std::cout << "Filename " << dirName << " already exists" << std::endl;
            std::cout << "Skipped this" << std::endl;
            skip = true;
        }

        if (!skip) {
            for (int i = 0; i < modeSlots.size(); i++) {
                a.mode = (Mode) modeSlots[i];

                std::ofstream tempFile;
                std::string fileN = specificFile + "/" + std::to_string(a.mode);
                tempFile.open(fileN, std::ios::app);
                tempFile.close();
                runSimulation(fileN, a);
                a.registeredFrameworks = frameworks;
                a.slaves = simulationSlaves;

                tempFile.open(fileN, std::ios::app);
                tempFile << ";" << a.avgBw / a.lines << ";" << a.avgLatency / a.lines << ";" << a.wasted / a.lines
                << "\n";
                tempFile.close();
                std::cout << "ALT SATT TIL 0 NÅ" << std::endl;
                a.avgBw = 0.0; //Check that these actually get nulled out after each run.
                a.avgLatency = 0.0;
                a.lines = 0;
                a.wasted = 0.0;

            }
        }

        //Reset allocator
        a.bundleMode = (BundleMode) bundleType;
        a.registeredFrameworks = frameworks;
        a.numFrameworks = numFrameworks;
        a.numTasks = numTasks;
        a.slaves = simulationSlaves;
        a.cpuThresh = 100/tempBundleSize;
        a.memThresh = 100/tempBundleSize;
        a.bundleSize = tempBundleSize;
        a.slaveMem = slaveMem;
        a.slaveCpu = slaveCpu;
        a.datacenters = datacenters;
        a.taskCpu = taskCpu;
        a.taskMem = taskMem;
    }


    //Teste bundle size

    if(observeBundleSize) {
        dirName = fileNameBuilder(Bundles, a);
        parentDirName = variableParameter(Bundles);
        baseUrl = "/Users/HansHenrikSande/CLionProjects/MasterSimulator/GraphFiles/";
        toCheck = baseUrl + parentDirName;

        dirAlreadyExist = dirExists(toCheck.data());

        if (!dirAlreadyExist) {
            dirMaker(toCheck.data());
        } else {
            std::cout << "Dirname " << dirName << " already exists" << std::endl;
        }

        specificFile = toCheck + "/" + dirName;

        dirAlreadyExist = dirExists(specificFile.data());
        skip = false;
        if (!dirAlreadyExist) {
            dirMaker(specificFile.data());
        } else {
            std::cout << "Filename " << dirName << " already exists" << std::endl;
            std::cout << "Skipped this" << std::endl;
            skip = true;
        }

        if (!skip) {
            for (int i = 0; i < modeSlots.size(); i++) {
                a.mode = (Mode) modeSlots[i];

                std::ofstream tempFile;
                std::string fileN = specificFile + "/" + std::to_string(a.mode);
                tempFile.open(fileN, std::ios::app);
                tempFile.close();
                for (int j = 0; j < bundleSizeSlots.size(); j++) {
                    a.bundleSize = bundleSizeSlots[j];
                    a.cpuThresh = 100/bundleSizeSlots[j];
                    a.memThresh = 100/bundleSizeSlots[j];
                    tempFile.open(fileN, std::ios::app);
                    tempFile << std::to_string(bundleSizeSlots[j]) << ";";
                    tempFile.close();
                    runSimulation(fileN, a);
                    a.registeredFrameworks = frameworks;
                    a.slaves = simulationSlaves;

                    tempFile.open(fileN, std::ios::app);
                    tempFile << ";" << a.avgBw / a.lines << ";" << a.avgLatency / a.lines << ";" << a.wasted / a.lines
                             << "\n";
                    tempFile.close();
                    a.avgBw = 0.0;
                    a.avgLatency = 0.0;
                    a.lines = 0;
                    a.wasted = 0.0;
                }

            }
        }

        //Reset allocator
        a.bundleMode = (BundleMode) bundleType;
        a.registeredFrameworks = frameworks;
        a.numFrameworks = numFrameworks;
        a.numTasks = numTasks;
        a.slaves = simulationSlaves;
        a.cpuThresh = 100/tempBundleSize;
        a.memThresh = 100/tempBundleSize;
        a.bundleSize = tempBundleSize;
        a.slaveMem = slaveMem;
        a.slaveCpu = slaveCpu;
        a.datacenters = datacenters;
        a.taskCpu = taskCpu;
        a.taskMem = taskMem;
    }



    //Teste slave resources

    if(observeSlaveResource) {
        dirName = fileNameBuilder(SlaveResources, a);
        parentDirName = variableParameter(SlaveResources);
        baseUrl = "/Users/HansHenrikSande/CLionProjects/MasterSimulator/GraphFiles/";
        toCheck = baseUrl + parentDirName;

        dirAlreadyExist = dirExists(toCheck.data());

        if (!dirAlreadyExist) {
            dirMaker(toCheck.data());
        } else {
            std::cout << "Dirname " << dirName << " already exists" << std::endl;
        }

        specificFile = toCheck + "/" + dirName;

        dirAlreadyExist = dirExists(specificFile.data());
        skip = false;
        if (!dirAlreadyExist) {
            dirMaker(specificFile.data());
        } else {
            std::cout << "Filename " << dirName << " already exists" << std::endl;
            std::cout << "Skipped this" << std::endl;
            skip = true;
        }
        if (!skip) {
            for (int i = 0; i < modeSlots.size(); i++) {
                a.mode = (Mode) modeSlots[i];
                std::ofstream tempFile;
                std::string fileN = specificFile + "/" + std::to_string(a.mode);
                tempFile.open(fileN, std::ios::app);
                tempFile.close();
                for (int j = 0; j < slaveCpuSlots.size(); j++) {
                    int cpu = slaveCpuSlots[j];
                    int mem = slaveMemSlots[j];
                    Resource slaveRes(cpu, mem, 4000);
                    //Update slaves with new resources
                    std::map<int, Slave> slaveCopy = simulationSlaves;
                    for (int x = 0; x < slaveCopy.size(); x++) {
                        Slave *temp = &slaveCopy[x];
                        temp->available = slaveRes;
                        std::cout << temp->datacenterId << std::endl;
                    }
                    a.slaves = slaveCopy;
                    tempFile.open(fileN, std::ios::app);
                    tempFile << "[" + std::to_string(cpu) + "," + std::to_string(mem) + "]" << ";";
                    tempFile.close();
                    runSimulation(fileN, a);
                    a.registeredFrameworks = frameworks;
                    tempFile.open(fileN, std::ios::app);
                    tempFile << ";" << a.avgBw / a.lines << ";" << a.avgLatency / a.lines << ";" << a.wasted / a.lines
                             << "\n";
                    tempFile.close();
                    a.avgBw = 0.0;
                    a.avgLatency = 0.0;
                    a.lines = 0;
                    a.wasted = 0.0;
                }
            }
        }

        //Reset allocator
        a.bundleMode = (BundleMode) bundleType;
        a.registeredFrameworks = frameworks;
        a.numFrameworks = numFrameworks;
        a.numTasks = numTasks;
        a.slaves = simulationSlaves;
        a.cpuThresh = 100/tempBundleSize;
        a.memThresh = 100/tempBundleSize;
        a.bundleSize = tempBundleSize;
        a.slaveMem = slaveMem;
        a.slaveCpu = slaveCpu;
        a.datacenters = datacenters;
        a.taskCpu = taskCpu;
        a.taskMem = taskMem;
    }



    //Teste antall slaver
    if(observeSlaves) {

        dirName = fileNameBuilder(Slaves, a);
        parentDirName = variableParameter(Slaves);
        baseUrl = "/Users/HansHenrikSande/CLionProjects/MasterSimulator/GraphFiles/";
        toCheck = baseUrl + parentDirName;

        dirAlreadyExist = dirExists(toCheck.data());

        if (!dirAlreadyExist) {
            dirMaker(toCheck.data());
        } else {
            std::cout << "Dirname " << dirName << " already exists" << std::endl;
        }

        specificFile = toCheck + "/" + dirName;

        dirAlreadyExist = dirExists(specificFile.data());
        skip = false;
        if (!dirAlreadyExist) {
            dirMaker(specificFile.data());
        } else {
            std::cout << "Filename " << dirName << " already exists" << std::endl;
            std::cout << "Skipped this" << std::endl;
            skip = true;
        }
        if (!skip) {
            for (int i = 0; i < modeSlots.size(); i++) {
                a.mode = (Mode) modeSlots[i];
                std::ofstream tempFile;
                std::string fileN = specificFile + "/" + std::to_string(a.mode);
                tempFile.open(fileN, std::ios::app);
                tempFile.close();

                for (int j = 0; j < slavesSlots.size(); j++) {
                    int currentNumSlaves = slavesSlots[j];
                    std::map<int, Slave> slaveCopy;

                    int rest = currentNumSlaves % datacenters;
                    int perDatacenter = (currentNumSlaves - rest) / datacenters;
                    int temp = (currentNumSlaves - rest);
                    int counter = perDatacenter;
                    int datacenterId = 0;
                    for (int u = 0; u < temp; u++) {
                        Resource slaveRes(slaveCpu, slaveMem, 4000); //PICK RANDOMLY
                        if (u >= counter) {
                            datacenterId++;
                            counter += perDatacenter;
                        }
                        std::cout << "DatacenterID " << datacenterId << std::endl;
                        Slave slave(u, slaveRes, datacenterId);
                        slaveCopy.insert(std::make_pair(slave.slaveId, slave));
                    }

                    datacenterId = 0;
                    for (int j = temp; j < currentNumSlaves; j++){
                        Resource slaveRes(slaveCpu, slaveMem, 4000);
                        Slave slave(j, slaveRes, datacenterId);
                        slaveCopy.insert(std::make_pair(slave.slaveId, slave));
                        datacenterId++;
                    }

                    a.slaves = slaveCopy;
                    tempFile.open(fileN, std::ios::app);
                    tempFile << currentNumSlaves << ";";
                    tempFile.close();
                    runSimulation(fileN, a);
                    a.registeredFrameworks = frameworks;
                    tempFile.open(fileN, std::ios::app);
                    tempFile << ";" << a.avgBw / a.lines << ";" << a.avgLatency / a.lines << ";" << a.wasted / a.lines
                             << "\n";
                    tempFile.close();
                    a.avgBw = 0.0;
                    a.avgLatency = 0.0;
                    a.lines = 0;
                    a.wasted = 0.0;
                }
            }
        }

        a.bundleMode = (BundleMode) bundleType;
        a.registeredFrameworks = frameworks;
        a.numFrameworks = numFrameworks;
        a.numTasks = numTasks;
        a.slaves = simulationSlaves;
        a.cpuThresh = 100/tempBundleSize;
        a.memThresh = 100/tempBundleSize;
        a.bundleSize = tempBundleSize;
        a.slaveMem = slaveMem;
        a.slaveCpu = slaveCpu;
        a.datacenters = datacenters;
        a.taskCpu = taskCpu;
        a.taskMem = taskMem;
    }

    //Teste antall datacentere
    if(observeDatacenter) {
        dirName = fileNameBuilder(Datacenter, a);
        parentDirName = variableParameter(Datacenter);
        baseUrl = "/Users/HansHenrikSande/CLionProjects/MasterSimulator/GraphFiles/";
        toCheck = baseUrl + parentDirName;

        dirAlreadyExist = dirExists(toCheck.data());

        if (!dirAlreadyExist) {
            dirMaker(toCheck.data());
        } else {
            std::cout << "Dirname " << dirName << " already exists" << std::endl;
        }

        specificFile = toCheck + "/" + dirName;

        dirAlreadyExist = dirExists(specificFile.data());
        skip = false;
        if (!dirAlreadyExist) {
            dirMaker(specificFile.data());
        } else {
            std::cout << "Filename " << dirName << " already exists" << std::endl;
            std::cout << "Skipped this" << std::endl;
            skip = true;
        }
        if (!skip) {
            for (int i = 0; i < modeSlots.size(); i++) {
                a.mode = (Mode) modeSlots[i];
                std::ofstream tempFile;
                std::string fileN = specificFile + "/" + std::to_string(a.mode);
                tempFile.open(fileN, std::ios::app);
                tempFile.close();

                for (int j = 0; j < datacenterSlots.size(); j++) {
                    int currentNumDatacenters = datacenterSlots[j];

                    test(
                            currentNumDatacenters,
                            "/Users/HansHenrikSande/CLionProjects/MasterSimulator/bw.txt",
                            "/Users/HansHenrikSande/CLionProjects/MasterSimulator/lt.txt",
                            "/Users/HansHenrikSande/CLionProjects/MasterSimulator/test.txt");

                    int rest = numSlaves % currentNumDatacenters;
                    int perDatacenter = (numSlaves-rest) / currentNumDatacenters;
                    int counter = perDatacenter;
                    int datacenterId = 0;
                    std::map<int, Slave> slaveCopy = simulationSlaves;
                    //Update datacenters of slave
                    for (int u = 0; u < (numSlaves-rest); u++) {
                        if (u >= counter) {
                            datacenterId++;
                            counter += perDatacenter;
                        }
                        Slave *temp = &slaveCopy[u];
                        temp->datacenterId = datacenterId;
                    }

                    datacenterId = 0;
                    for(int x = (numSlaves-rest); x < numSlaves; x++){
                        Slave *temp = &slaveCopy[x];
                        temp->datacenterId = datacenterId;
                        datacenterId++;
                    }
                    a.slaves = slaveCopy;
                    tempFile.open(fileN, std::ios::app);
                    tempFile << currentNumDatacenters << ";";
                    tempFile.close();
                    runSimulation(fileN, a);
                    a.registeredFrameworks = frameworks;
                    tempFile.open(fileN, std::ios::app);
                    tempFile << ";" << a.avgBw / a.lines << ";" << a.avgLatency / a.lines << ";" << a.wasted / a.lines
                             << "\n";
                    tempFile.close();
                    a.avgBw = 0.0;
                    a.avgLatency = 0.0;
                    a.lines = 0;
                    a.wasted = 0.0;
                }
            }
        }

        a.bundleMode = (BundleMode) bundleType;
        a.registeredFrameworks = frameworks;
        a.numFrameworks = numFrameworks;
        a.numTasks = numTasks;
        a.slaves = simulationSlaves;
        a.cpuThresh = 100/tempBundleSize;
        a.memThresh = 100/tempBundleSize;
        a.bundleSize = tempBundleSize;
        a.slaveMem = slaveMem;
        a.slaveCpu = slaveCpu;
        a.datacenters = datacenters;
        a.taskCpu = taskCpu;
        a.taskMem = taskMem;
    }

    //Teste frameworks
    if(observeFrameworks) {
        dirName = fileNameBuilder(Frameworks, a);
        parentDirName = variableParameter(Frameworks);
        baseUrl = "/Users/HansHenrikSande/CLionProjects/MasterSimulator/GraphFiles/";
        toCheck = baseUrl + parentDirName;

        dirAlreadyExist = dirExists(toCheck.data());

        if (!dirAlreadyExist) {
            dirMaker(toCheck.data());
        } else {
            std::cout << "Dirname " << dirName << " already exists" << std::endl;
        }

        specificFile = toCheck + "/" + dirName;

        dirAlreadyExist = dirExists(specificFile.data());
        skip = false;
        if (!dirAlreadyExist) {
            dirMaker(specificFile.data());
        } else {
            std::cout << "Filename " << dirName << " already exists" << std::endl;
            std::cout << "Skipped this" << std::endl;
            skip = true;
        }
        if (!skip) {
            for (int i = 0; i < modeSlots.size(); i++) {
                a.mode = (Mode) modeSlots[i];
                std::ofstream tempFile;
                std::string fileN = specificFile + "/" + std::to_string(a.mode);
                tempFile.open(fileN, std::ios::app);
                tempFile.close();
                for (int j = 0; j < frameworksSlots.size(); j++) {
                    std::unordered_map<int, Framework> frameworkCopy;

                    int currNumFrameworks = frameworksSlots[j];

                    for (int i = 0; i < currNumFrameworks; i++) {
                        std::vector<Task> tasks;
                        int tasksPerFramework = numTasks;
                        for (int x = 0; x < numTasks; x++) {
                            Task task;
                            Resource k(taskCpu, taskMem, 4000);
                            task.required = k;
                            tasks.push_back(task);
                        }
                        Framework framework(i, tasks, ApplicationType::Temp1);
                        frameworkCopy.insert(std::make_pair(framework.id, framework));
                    }

                    a.registeredFrameworks = frameworkCopy;
                    tempFile.open(fileN, std::ios::app);
                    tempFile << currNumFrameworks << ";";
                    tempFile.close();
                    runSimulation(fileN, a);
                    a.slaves = simulationSlaves;
                    tempFile.open(fileN, std::ios::app);
                    tempFile << ";" << a.avgBw / a.lines << ";" << a.avgLatency / a.lines << ";" << a.wasted / a.lines
                             << "\n";
                    tempFile.close();
                    a.avgBw = 0.0;
                    a.avgLatency = 0.0;
                    a.lines = 0;
                    a.wasted = 0.0;
                }
            }
        }
        a.bundleMode = (BundleMode) bundleType;
        a.registeredFrameworks = frameworks;
        a.numFrameworks = numFrameworks;
        a.numTasks = numTasks;
        a.slaves = simulationSlaves;
        a.cpuThresh = 100/tempBundleSize;
        a.memThresh = 100/tempBundleSize;
        a.bundleSize = tempBundleSize;
        a.slaveMem = slaveMem;
        a.slaveCpu = slaveCpu;
        a.datacenters = datacenters;
        a.taskCpu = taskCpu;
        a.taskMem = taskMem;
    }

    //Teste task size
    if(observeTasks) {
        dirName = fileNameBuilder(Tasks, a);
        parentDirName = variableParameter(Tasks);
        baseUrl = "/Users/HansHenrikSande/CLionProjects/MasterSimulator/GraphFiles/";
        toCheck = baseUrl + parentDirName;

        dirAlreadyExist = dirExists(toCheck.data());

        if (!dirAlreadyExist) {
            dirMaker(toCheck.data());
        } else {
            std::cout << "Dirname " << dirName << " already exists" << std::endl;
        }

        specificFile = toCheck + "/" + dirName;

        dirAlreadyExist = dirExists(specificFile.data());
        skip = false;
        if (!dirAlreadyExist) {
            dirMaker(specificFile.data());
        } else {
            std::cout << "Filename " << dirName << " already exists" << std::endl;
            std::cout << "Skipped this" << std::endl;
            skip = true;
        }
        if (!skip) {
            for (int i = 0; i < modeSlots.size(); i++) {
                a.mode = (Mode) modeSlots[i];
                std::ofstream tempFile;
                std::string fileN = specificFile + "/" + std::to_string(a.mode);
                tempFile.open(fileN, std::ios::app);
                tempFile.close();
                for (int j = 0; j < taskSlots.size(); j++) {
                    std::unordered_map<int, Framework> frameworkCopy;
                    int currNumTasks = taskSlots[j];
                    for (int i = 0; i < numFrameworks; i++) {
                        std::vector<Task> tasks;
                        int tasksPerFramework = currNumTasks;
                        for (int x = 0; x < tasksPerFramework; x++) {
                            Task task;
                            Resource k(taskCpu, taskMem, 4000);
                            task.required = k;
                            tasks.push_back(task);
                        }
                        Framework framework(i, tasks, ApplicationType::Temp1);
                        frameworkCopy.insert(std::make_pair(framework.id, framework));
                    }

                    a.registeredFrameworks = frameworkCopy;
                    tempFile.open(fileN, std::ios::app);
                    tempFile << currNumTasks << ";";
                    tempFile.close();
                    runSimulation(fileN, a);
                    a.slaves = simulationSlaves;
                    tempFile.open(fileN, std::ios::app);
                    tempFile << ";" << a.avgBw / a.lines << ";" << a.avgLatency / a.lines << ";" << a.wasted / a.lines
                             << "\n";
                    tempFile.close();
                    a.avgBw = 0.0;
                    a.avgLatency = 0.0;
                    a.lines = 0;
                    a.wasted = 0.0;
                }
            }
        }
        a.bundleMode = (BundleMode) bundleType;
        a.registeredFrameworks = frameworks;
        a.numFrameworks = numFrameworks;
        a.numTasks = numTasks;
        a.slaves = simulationSlaves;
        a.cpuThresh = 100/tempBundleSize;
        a.memThresh = 100/tempBundleSize;
        a.bundleSize = tempBundleSize;
        a.slaveMem = slaveMem;
        a.slaveCpu = slaveCpu;
        a.datacenters = datacenters;
        a.taskCpu = taskCpu;
        a.taskMem = taskMem;
    }

    //Teste task resources

    if(observeTaskResources) {
        dirName = fileNameBuilder(TaskResources, a);
        parentDirName = variableParameter(TaskResources);
        baseUrl = "/Users/HansHenrikSande/CLionProjects/MasterSimulator/GraphFiles/";
        toCheck = baseUrl + parentDirName;

        dirAlreadyExist = dirExists(toCheck.data());

        if (!dirAlreadyExist) {
            dirMaker(toCheck.data());
        } else {
            std::cout << "Dirname " << dirName << " already exists" << std::endl;
        }

        specificFile = toCheck + "/" + dirName;

        dirAlreadyExist = dirExists(specificFile.data());
        skip = false;
        if (!dirAlreadyExist) {
            dirMaker(specificFile.data());
        } else {
            std::cout << "Filename " << dirName << " already exists" << std::endl;
            std::cout << "Skipped this" << std::endl;
            skip = true;
        }
        if (!skip) {
            for (int i = 0; i < modeSlots.size(); i++) {
                a.mode = (Mode) modeSlots[i];
                //a.bundleMode = FineGrained;
                std::ofstream tempFile;
                std::string fileN = specificFile + "/" + std::to_string(a.mode);
                tempFile.open(fileN, std::ios::app);
                tempFile.close();

                for (int j = 0; j < taskCpuSlots.size(); j++) {
                    int cpu = taskCpuSlots[j];
                    int mem = taskMemClots[j];
                    Resource k(cpu, mem, 4000);

                    std::unordered_map<int, Framework> frameworkCopy = frameworks;
                    for (int x = 0; x < frameworkCopy.size(); x++) {
                        Framework *tempFramework = &frameworkCopy[x];

                        for (int w = 0; w < tempFramework->numberOfTasks.size(); w++) {
                            Task *task = &tempFramework->numberOfTasks[w];
                            task->required = k;
                        }

                    }

                    a.registeredFrameworks = frameworkCopy;
                    tempFile.open(fileN, std::ios::app);
                    tempFile << "[" + std::to_string(cpu) + "," + std::to_string(mem) + "]" << ";";
                    tempFile.close();
                    runSimulation(fileN, a);
                    a.slaves = simulationSlaves;
                    tempFile.open(fileN, std::ios::app);
                    tempFile << ";" << a.avgBw / a.lines << ";" << a.avgLatency / a.lines << ";" << a.wasted / a.lines
                             << "\n";
                    tempFile.close();
                    a.avgBw = 0.0;
                    a.avgLatency = 0.0;
                    a.lines = 0;
                    a.wasted = 0.0;
                }
            }
        }

        a.bundleMode = (BundleMode) bundleType;
        a.registeredFrameworks = frameworks;
        a.numFrameworks = numFrameworks;
        a.numTasks = numTasks;
        a.slaves = simulationSlaves;
        a.cpuThresh = 100/tempBundleSize;
        a.memThresh = 100/tempBundleSize;
        a.bundleSize = tempBundleSize;
        a.slaveMem = slaveMem;
        a.slaveCpu = slaveCpu;
        a.datacenters = datacenters;
        a.taskCpu = taskCpu;
        a.taskMem = taskMem;

    }

    //Teste bundling strategier

    if(observeBundleType) {
        dirName = fileNameBuilder(BundleType, a);
        parentDirName = variableParameter(BundleType);
        baseUrl = "/Users/HansHenrikSande/CLionProjects/MasterSimulator/GraphFiles/";
        toCheck = baseUrl + parentDirName;

        dirAlreadyExist = dirExists(toCheck.data());

        if (!dirAlreadyExist) {
            dirMaker(toCheck.data());
        } else {
            std::cout << "Dirname " << dirName << " already exists" << std::endl;
        }

        specificFile = toCheck + "/" + dirName;

        dirAlreadyExist = dirExists(specificFile.data());
        skip = false;
        if (!dirAlreadyExist) {
            dirMaker(specificFile.data());
        } else {
            std::cout << "Filename " << dirName << " already exists" << std::endl;
            std::cout << "Skipped this" << std::endl;
            skip = true;
        }
        if (!skip) {
            for (int i = 0; i < modeSlots.size(); i++) {
                a.mode = (Mode) modeSlots[i];
                std::ofstream tempFile;
                std::string fileN = specificFile + "/" + std::to_string(a.mode);
                tempFile.open(fileN, std::ios::app);
                tempFile.close();

                for (int j = 0; j < bundleModeSlots.size(); j++) {

                    a.bundleMode = (BundleMode) bundleModeSlots[j];


                    tempFile.open(fileN, std::ios::app);
                    tempFile << (BundleMode) bundleModeSlots[j] << ";";
                    tempFile.close();
                    runSimulation(fileN, a);
                    a.slaves = simulationSlaves;
                    a.registeredFrameworks = frameworks;
                    tempFile.open(fileN, std::ios::app);
                    tempFile << ";" << a.avgBw / a.lines << ";" << a.avgLatency / a.lines << ";" << a.wasted / a.lines
                             << "\n";
                    tempFile.close();
                    a.avgBw = 0.0;
                    a.avgLatency = 0.0;
                    a.lines = 0;
                    a.wasted = 0.0;
                }
            }
        }

    }

    return 0;
}


