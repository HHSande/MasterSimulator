//
// Created by Hans Henrik Sande on 09/04/2020.
//

#ifndef MASTERSIMULATOR_RESOURCE_H
#define MASTERSIMULATOR_RESOURCE_H


class Resource{
    public:
    int cpus;
    int mem;
    int disk;
    Resource(int c, int m, int d);
    Resource();
    int getCpus();
    int getMem();
    int getDisk();

    void setCpus(int cpus);
    void setMem(int mem);
    void setDisk(int disk);

};
#endif //MASTERSIMULATOR_RESOURCE_H
