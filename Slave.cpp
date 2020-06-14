//
// Created by Hans Henrik Sande on 09/04/2020.
//
#include <iostream>
#include "Slave.h"
Resource Slave::getAvailable() {
    return this->available;
}

void Slave::increaseAvailable(Resource r) {
    int newCpu = r.getCpus() + this->available.getCpus();
    int newMem = r.getMem() + this->available.getMem();

    this->available.setCpus(newCpu);
    this->available.setMem(newMem);
    //std::cout << "New avaiable " << this->available.getCpus() << " " << this->available.getMem() << std::endl;
    //std::cout << datacenterId << " recovered resources; cpus: " << r.getCpus() << " mem: " << r.getMem() << std::endl;
}

void Slave::decreaseAvailable(Resource r) {
    int newCpu = this->available.getCpus() - r.getCpus();
    int newMem = this->available.getMem() - r.getMem();

    this->available.setCpus(newCpu);
    this->available.setMem(newMem);
    //std::cout << "New avaiable " << this->available.getCpus() << " " << this->available.getMem() << std::endl;
    //std::cout << datacenterId << " offered resources; cpus: " << r.getCpus() << " mem: " << r.getMem() << std::endl;

}

Slave::~Slave() {

}
Slave::Slave(int s, Resource r, int d){
    this->slaveId = s;
    this->available = r;
    this->datacenterId = d;
}

Slave::Slave(){
    
}

