//
// Created by Hans Henrik Sande on 09/04/2020.
//

#include "Resource.h"


Resource::Resource(int c, int m, int d) {
    cpus = c;
    mem = m;
    disk = d;
}

Resource::Resource() {

}

void Resource::setCpus(int c) {
    cpus = c;
}

void Resource::setDisk(int d) {
    disk = d;
}

void Resource::setMem(int m) {
    mem = m;
}
int Resource::getDisk() {
    return disk;
}

int Resource::getCpus() {
    return cpus;
}

int Resource::getMem() {
    return mem;
}

