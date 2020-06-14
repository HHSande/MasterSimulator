//
// Created by Hans Henrik Sande on 09/04/2020.
//

#ifndef MASTERSIMULATOR_SLAVE_H
#define MASTERSIMULATOR_SLAVE_H

#include <vector>
#include "Resource.h"

class Slave{
public:
    Slave(int s, Resource a, int d);
    Slave();
    ~Slave();
    int slaveId;
    int datacenterId;
    Resource available;

    Resource getAvailable();

    void increaseAvailable(Resource r);
    void decreaseAvailable(Resource r);

    void setResources();
    void setDatacenterId(int datacenterId);

};


#endif //MASTERSIMULATOR_SLAVE_H
