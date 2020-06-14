cmake_minimum_required(VERSION 3.15)
project(MasterSimulator)

set(CMAKE_CXX_STANDARD 11)

add_executable(MasterSimulator main.cpp Master.cpp Slave.cpp Allocator.cpp Resource.cpp Master.h Allocator.h Slave.h Resource.h affinity_propagation.cpp affinity_propagation.h Framework.cpp Framework.h Task.cpp Task.h)