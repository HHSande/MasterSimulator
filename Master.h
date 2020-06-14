//
// Created by Hans Henrik Sande on 09/04/2020.
//

#ifndef MASTERSIMULATOR_MASTER_H
#define MASTERSIMULATOR_MASTER_H

#include <vector>


class Master{
public:
    void generateOffer();
    void registerSlave(int slaveId);

    std::vector<int> slaves;
};


#endif //MASTERSIMULATOR_MASTER_H
