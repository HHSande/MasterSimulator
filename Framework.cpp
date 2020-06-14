//
// Created by Hans Henrik Sande on 11/04/2020.
//

#include <iostream>
#include "Framework.h"

Resource sumOfTasks(std::vector<Task> req){
    Resource r;
    int tempDisk = 0;
    int tempCpu = 0;
    int tempMem = 0;

    for(Task t : req){
        tempDisk += t.required.getDisk();
        tempCpu += t.required.getCpus();
        tempMem += t.required.getMem();
    }

    r.setCpus(tempCpu);
    r.setMem(tempMem);
    r.setDisk(tempDisk);

    return r;

}

bool Framework::enoughResourceForTask(Resource res, Task task) {
    return res.getCpus() >= task.required.getCpus() && res.getMem() >= task.required.getMem();
}
Framework::Framework(int id, std::vector<Task> req, ApplicationType applicationType) {
    this->id = id;
    this->required = sumOfTasks(req);
    this->numberOfTasks = req;
    this->applicationType = applicationType;
    this->finished = false;
}

Framework::Framework(){

}

bool Framework::sendOffer(Resource res, std::vector<std::pair<int, Resource>> bundles) {
    bool temp = false;
    for(int i = 0; i < this->numberOfTasks.size(); i++){
        if(!numberOfTasks[i].finished){
            if(enoughResourceForTask(res, numberOfTasks[i])){
                //std::cout << "TASK RUNNING" << std::endl;
                temp = true;
            }
        }
    }

    return temp;
}

int Framework::getTasksInQueue() {
    int temp = 0;
    for(int i = 0; i < numberOfTasks.size(); i++){
        if(!numberOfTasks[i].finished){
            temp++;
        }
    }

    return temp;
}
void Framework::setSlaves(std::vector<std::pair<int, Resource>> slaves) {
    //std::cout << "SLAVES ADDED TO FRAMEWORK " << id << " " << std::endl;
    this->slavesServingFramework = slaves;
}

void Framework::run() {
    for(int i = 0; i < numberOfTasks.size(); i++){
        if(this->numberOfTasks[i].finished == true){
            this->numberOfTasks.erase(numberOfTasks.begin()+i);
        }
    }

    if(numberOfTasks.size() == 0){
        //std::cout << "Framework finished" << std::endl;
        this->finished = true;
    }
}

