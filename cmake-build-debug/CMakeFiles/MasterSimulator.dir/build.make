# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/HansHenrikSande/CLionProjects/MasterSimulator

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/HansHenrikSande/CLionProjects/MasterSimulator/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/MasterSimulator.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/MasterSimulator.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/MasterSimulator.dir/flags.make

CMakeFiles/MasterSimulator.dir/main.cpp.o: CMakeFiles/MasterSimulator.dir/flags.make
CMakeFiles/MasterSimulator.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/HansHenrikSande/CLionProjects/MasterSimulator/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/MasterSimulator.dir/main.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MasterSimulator.dir/main.cpp.o -c /Users/HansHenrikSande/CLionProjects/MasterSimulator/main.cpp

CMakeFiles/MasterSimulator.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MasterSimulator.dir/main.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/HansHenrikSande/CLionProjects/MasterSimulator/main.cpp > CMakeFiles/MasterSimulator.dir/main.cpp.i

CMakeFiles/MasterSimulator.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MasterSimulator.dir/main.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/HansHenrikSande/CLionProjects/MasterSimulator/main.cpp -o CMakeFiles/MasterSimulator.dir/main.cpp.s

CMakeFiles/MasterSimulator.dir/Master.cpp.o: CMakeFiles/MasterSimulator.dir/flags.make
CMakeFiles/MasterSimulator.dir/Master.cpp.o: ../Master.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/HansHenrikSande/CLionProjects/MasterSimulator/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/MasterSimulator.dir/Master.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MasterSimulator.dir/Master.cpp.o -c /Users/HansHenrikSande/CLionProjects/MasterSimulator/Master.cpp

CMakeFiles/MasterSimulator.dir/Master.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MasterSimulator.dir/Master.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/HansHenrikSande/CLionProjects/MasterSimulator/Master.cpp > CMakeFiles/MasterSimulator.dir/Master.cpp.i

CMakeFiles/MasterSimulator.dir/Master.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MasterSimulator.dir/Master.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/HansHenrikSande/CLionProjects/MasterSimulator/Master.cpp -o CMakeFiles/MasterSimulator.dir/Master.cpp.s

CMakeFiles/MasterSimulator.dir/Slave.cpp.o: CMakeFiles/MasterSimulator.dir/flags.make
CMakeFiles/MasterSimulator.dir/Slave.cpp.o: ../Slave.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/HansHenrikSande/CLionProjects/MasterSimulator/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/MasterSimulator.dir/Slave.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MasterSimulator.dir/Slave.cpp.o -c /Users/HansHenrikSande/CLionProjects/MasterSimulator/Slave.cpp

CMakeFiles/MasterSimulator.dir/Slave.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MasterSimulator.dir/Slave.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/HansHenrikSande/CLionProjects/MasterSimulator/Slave.cpp > CMakeFiles/MasterSimulator.dir/Slave.cpp.i

CMakeFiles/MasterSimulator.dir/Slave.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MasterSimulator.dir/Slave.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/HansHenrikSande/CLionProjects/MasterSimulator/Slave.cpp -o CMakeFiles/MasterSimulator.dir/Slave.cpp.s

CMakeFiles/MasterSimulator.dir/Allocator.cpp.o: CMakeFiles/MasterSimulator.dir/flags.make
CMakeFiles/MasterSimulator.dir/Allocator.cpp.o: ../Allocator.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/HansHenrikSande/CLionProjects/MasterSimulator/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/MasterSimulator.dir/Allocator.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MasterSimulator.dir/Allocator.cpp.o -c /Users/HansHenrikSande/CLionProjects/MasterSimulator/Allocator.cpp

CMakeFiles/MasterSimulator.dir/Allocator.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MasterSimulator.dir/Allocator.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/HansHenrikSande/CLionProjects/MasterSimulator/Allocator.cpp > CMakeFiles/MasterSimulator.dir/Allocator.cpp.i

CMakeFiles/MasterSimulator.dir/Allocator.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MasterSimulator.dir/Allocator.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/HansHenrikSande/CLionProjects/MasterSimulator/Allocator.cpp -o CMakeFiles/MasterSimulator.dir/Allocator.cpp.s

CMakeFiles/MasterSimulator.dir/Resource.cpp.o: CMakeFiles/MasterSimulator.dir/flags.make
CMakeFiles/MasterSimulator.dir/Resource.cpp.o: ../Resource.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/HansHenrikSande/CLionProjects/MasterSimulator/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/MasterSimulator.dir/Resource.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MasterSimulator.dir/Resource.cpp.o -c /Users/HansHenrikSande/CLionProjects/MasterSimulator/Resource.cpp

CMakeFiles/MasterSimulator.dir/Resource.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MasterSimulator.dir/Resource.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/HansHenrikSande/CLionProjects/MasterSimulator/Resource.cpp > CMakeFiles/MasterSimulator.dir/Resource.cpp.i

CMakeFiles/MasterSimulator.dir/Resource.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MasterSimulator.dir/Resource.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/HansHenrikSande/CLionProjects/MasterSimulator/Resource.cpp -o CMakeFiles/MasterSimulator.dir/Resource.cpp.s

CMakeFiles/MasterSimulator.dir/affinity_propagation.cpp.o: CMakeFiles/MasterSimulator.dir/flags.make
CMakeFiles/MasterSimulator.dir/affinity_propagation.cpp.o: ../affinity_propagation.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/HansHenrikSande/CLionProjects/MasterSimulator/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/MasterSimulator.dir/affinity_propagation.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MasterSimulator.dir/affinity_propagation.cpp.o -c /Users/HansHenrikSande/CLionProjects/MasterSimulator/affinity_propagation.cpp

CMakeFiles/MasterSimulator.dir/affinity_propagation.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MasterSimulator.dir/affinity_propagation.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/HansHenrikSande/CLionProjects/MasterSimulator/affinity_propagation.cpp > CMakeFiles/MasterSimulator.dir/affinity_propagation.cpp.i

CMakeFiles/MasterSimulator.dir/affinity_propagation.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MasterSimulator.dir/affinity_propagation.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/HansHenrikSande/CLionProjects/MasterSimulator/affinity_propagation.cpp -o CMakeFiles/MasterSimulator.dir/affinity_propagation.cpp.s

CMakeFiles/MasterSimulator.dir/Framework.cpp.o: CMakeFiles/MasterSimulator.dir/flags.make
CMakeFiles/MasterSimulator.dir/Framework.cpp.o: ../Framework.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/HansHenrikSande/CLionProjects/MasterSimulator/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/MasterSimulator.dir/Framework.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MasterSimulator.dir/Framework.cpp.o -c /Users/HansHenrikSande/CLionProjects/MasterSimulator/Framework.cpp

CMakeFiles/MasterSimulator.dir/Framework.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MasterSimulator.dir/Framework.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/HansHenrikSande/CLionProjects/MasterSimulator/Framework.cpp > CMakeFiles/MasterSimulator.dir/Framework.cpp.i

CMakeFiles/MasterSimulator.dir/Framework.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MasterSimulator.dir/Framework.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/HansHenrikSande/CLionProjects/MasterSimulator/Framework.cpp -o CMakeFiles/MasterSimulator.dir/Framework.cpp.s

CMakeFiles/MasterSimulator.dir/Task.cpp.o: CMakeFiles/MasterSimulator.dir/flags.make
CMakeFiles/MasterSimulator.dir/Task.cpp.o: ../Task.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/HansHenrikSande/CLionProjects/MasterSimulator/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/MasterSimulator.dir/Task.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MasterSimulator.dir/Task.cpp.o -c /Users/HansHenrikSande/CLionProjects/MasterSimulator/Task.cpp

CMakeFiles/MasterSimulator.dir/Task.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MasterSimulator.dir/Task.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/HansHenrikSande/CLionProjects/MasterSimulator/Task.cpp > CMakeFiles/MasterSimulator.dir/Task.cpp.i

CMakeFiles/MasterSimulator.dir/Task.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MasterSimulator.dir/Task.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/HansHenrikSande/CLionProjects/MasterSimulator/Task.cpp -o CMakeFiles/MasterSimulator.dir/Task.cpp.s

# Object files for target MasterSimulator
MasterSimulator_OBJECTS = \
"CMakeFiles/MasterSimulator.dir/main.cpp.o" \
"CMakeFiles/MasterSimulator.dir/Master.cpp.o" \
"CMakeFiles/MasterSimulator.dir/Slave.cpp.o" \
"CMakeFiles/MasterSimulator.dir/Allocator.cpp.o" \
"CMakeFiles/MasterSimulator.dir/Resource.cpp.o" \
"CMakeFiles/MasterSimulator.dir/affinity_propagation.cpp.o" \
"CMakeFiles/MasterSimulator.dir/Framework.cpp.o" \
"CMakeFiles/MasterSimulator.dir/Task.cpp.o"

# External object files for target MasterSimulator
MasterSimulator_EXTERNAL_OBJECTS =

MasterSimulator: CMakeFiles/MasterSimulator.dir/main.cpp.o
MasterSimulator: CMakeFiles/MasterSimulator.dir/Master.cpp.o
MasterSimulator: CMakeFiles/MasterSimulator.dir/Slave.cpp.o
MasterSimulator: CMakeFiles/MasterSimulator.dir/Allocator.cpp.o
MasterSimulator: CMakeFiles/MasterSimulator.dir/Resource.cpp.o
MasterSimulator: CMakeFiles/MasterSimulator.dir/affinity_propagation.cpp.o
MasterSimulator: CMakeFiles/MasterSimulator.dir/Framework.cpp.o
MasterSimulator: CMakeFiles/MasterSimulator.dir/Task.cpp.o
MasterSimulator: CMakeFiles/MasterSimulator.dir/build.make
MasterSimulator: CMakeFiles/MasterSimulator.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/HansHenrikSande/CLionProjects/MasterSimulator/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Linking CXX executable MasterSimulator"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/MasterSimulator.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/MasterSimulator.dir/build: MasterSimulator

.PHONY : CMakeFiles/MasterSimulator.dir/build

CMakeFiles/MasterSimulator.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/MasterSimulator.dir/cmake_clean.cmake
.PHONY : CMakeFiles/MasterSimulator.dir/clean

CMakeFiles/MasterSimulator.dir/depend:
	cd /Users/HansHenrikSande/CLionProjects/MasterSimulator/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/HansHenrikSande/CLionProjects/MasterSimulator /Users/HansHenrikSande/CLionProjects/MasterSimulator /Users/HansHenrikSande/CLionProjects/MasterSimulator/cmake-build-debug /Users/HansHenrikSande/CLionProjects/MasterSimulator/cmake-build-debug /Users/HansHenrikSande/CLionProjects/MasterSimulator/cmake-build-debug/CMakeFiles/MasterSimulator.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/MasterSimulator.dir/depend
