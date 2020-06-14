//
// Created by Hans Henrik Sande on 11/04/2020.
//

#ifndef MASTERSIMULATOR_FRAMEWORK_H
#define MASTERSIMULATOR_FRAMEWORK_H


#include <vector>
#include "Resource.h"
#include "Task.h"

enum ApplicationType {
    Temp1 = 0,
    Temp2,
    Temp3,
    Temp4
};
class Framework{
public:
    int id;
    Resource required;
    ApplicationType applicationType;
    bool offerAccepted;
    std::vector<std::pair<int, Resource>> slavesServingFramework;
    std::vector<Task> numberOfTasks;
    bool finished;
    int datacenterMappedTo;

    Framework();
    Framework(int id, std::vector<Task> required, ApplicationType applicationType);

    bool sendOffer(Resource res, std::vector<std::pair<int, Resource>> bundles);
    void setSlaves(std::vector<std::pair<int, Resource>> bundles);
    void run();
    int getTasksInQueue();
    bool enoughResourceForTask(Resource res, Task task);

};
#endif //MASTERSIMULATOR_FRAMEWORK_H
