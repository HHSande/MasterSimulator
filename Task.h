//
// Created by Hans Henrik Sande on 18/04/2020.
//

#include "Resource.h"

#ifndef MASTERSIMULATOR_TASK_H
#define MASTERSIMULATOR_TASK_H

#endif //MASTERSIMULATOR_TASK_H


class Task{
public:
    Resource required;
    bool finished = false;

    void run();
};