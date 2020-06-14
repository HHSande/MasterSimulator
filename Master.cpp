//
// Created by Hans Henrik Sande on 09/04/2020.
//
#include <vector>
#include <iostream>
#include "Master.h"


void Master::generateOffer(){
    for(int i = 0; i < slaves.size(); i++){
        std::cout << "Sender offer til slave " << slaves[i] << std::endl;
    }
}

void Master::registerSlave(int slaveId){
    slaves.push_back(slaveId);
}
