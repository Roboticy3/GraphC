# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.30

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/pablo/lang-experiments/GraphC

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pablo/lang-experiments/GraphC/build-unix

# Include any dependencies generated for this target.
include CMakeFiles/GraphC.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/GraphC.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/GraphC.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/GraphC.dir/flags.make

CMakeFiles/GraphC.dir/src/combinations/choose.c.o: CMakeFiles/GraphC.dir/flags.make
CMakeFiles/GraphC.dir/src/combinations/choose.c.o: /home/pablo/lang-experiments/GraphC/src/combinations/choose.c
CMakeFiles/GraphC.dir/src/combinations/choose.c.o: CMakeFiles/GraphC.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/pablo/lang-experiments/GraphC/build-unix/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/GraphC.dir/src/combinations/choose.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/GraphC.dir/src/combinations/choose.c.o -MF CMakeFiles/GraphC.dir/src/combinations/choose.c.o.d -o CMakeFiles/GraphC.dir/src/combinations/choose.c.o -c /home/pablo/lang-experiments/GraphC/src/combinations/choose.c

CMakeFiles/GraphC.dir/src/combinations/choose.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/GraphC.dir/src/combinations/choose.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/pablo/lang-experiments/GraphC/src/combinations/choose.c > CMakeFiles/GraphC.dir/src/combinations/choose.c.i

CMakeFiles/GraphC.dir/src/combinations/choose.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/GraphC.dir/src/combinations/choose.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/pablo/lang-experiments/GraphC/src/combinations/choose.c -o CMakeFiles/GraphC.dir/src/combinations/choose.c.s

CMakeFiles/GraphC.dir/src/graph/graph.c.o: CMakeFiles/GraphC.dir/flags.make
CMakeFiles/GraphC.dir/src/graph/graph.c.o: /home/pablo/lang-experiments/GraphC/src/graph/graph.c
CMakeFiles/GraphC.dir/src/graph/graph.c.o: CMakeFiles/GraphC.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/pablo/lang-experiments/GraphC/build-unix/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/GraphC.dir/src/graph/graph.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/GraphC.dir/src/graph/graph.c.o -MF CMakeFiles/GraphC.dir/src/graph/graph.c.o.d -o CMakeFiles/GraphC.dir/src/graph/graph.c.o -c /home/pablo/lang-experiments/GraphC/src/graph/graph.c

CMakeFiles/GraphC.dir/src/graph/graph.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/GraphC.dir/src/graph/graph.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/pablo/lang-experiments/GraphC/src/graph/graph.c > CMakeFiles/GraphC.dir/src/graph/graph.c.i

CMakeFiles/GraphC.dir/src/graph/graph.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/GraphC.dir/src/graph/graph.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/pablo/lang-experiments/GraphC/src/graph/graph.c -o CMakeFiles/GraphC.dir/src/graph/graph.c.s

CMakeFiles/GraphC.dir/src/main.c.o: CMakeFiles/GraphC.dir/flags.make
CMakeFiles/GraphC.dir/src/main.c.o: /home/pablo/lang-experiments/GraphC/src/main.c
CMakeFiles/GraphC.dir/src/main.c.o: CMakeFiles/GraphC.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/pablo/lang-experiments/GraphC/build-unix/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/GraphC.dir/src/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/GraphC.dir/src/main.c.o -MF CMakeFiles/GraphC.dir/src/main.c.o.d -o CMakeFiles/GraphC.dir/src/main.c.o -c /home/pablo/lang-experiments/GraphC/src/main.c

CMakeFiles/GraphC.dir/src/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/GraphC.dir/src/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/pablo/lang-experiments/GraphC/src/main.c > CMakeFiles/GraphC.dir/src/main.c.i

CMakeFiles/GraphC.dir/src/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/GraphC.dir/src/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/pablo/lang-experiments/GraphC/src/main.c -o CMakeFiles/GraphC.dir/src/main.c.s

CMakeFiles/GraphC.dir/src/programs/petersen.c.o: CMakeFiles/GraphC.dir/flags.make
CMakeFiles/GraphC.dir/src/programs/petersen.c.o: /home/pablo/lang-experiments/GraphC/src/programs/petersen.c
CMakeFiles/GraphC.dir/src/programs/petersen.c.o: CMakeFiles/GraphC.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/pablo/lang-experiments/GraphC/build-unix/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/GraphC.dir/src/programs/petersen.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/GraphC.dir/src/programs/petersen.c.o -MF CMakeFiles/GraphC.dir/src/programs/petersen.c.o.d -o CMakeFiles/GraphC.dir/src/programs/petersen.c.o -c /home/pablo/lang-experiments/GraphC/src/programs/petersen.c

CMakeFiles/GraphC.dir/src/programs/petersen.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/GraphC.dir/src/programs/petersen.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/pablo/lang-experiments/GraphC/src/programs/petersen.c > CMakeFiles/GraphC.dir/src/programs/petersen.c.i

CMakeFiles/GraphC.dir/src/programs/petersen.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/GraphC.dir/src/programs/petersen.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/pablo/lang-experiments/GraphC/src/programs/petersen.c -o CMakeFiles/GraphC.dir/src/programs/petersen.c.s

CMakeFiles/GraphC.dir/src/programs/randomgraph.c.o: CMakeFiles/GraphC.dir/flags.make
CMakeFiles/GraphC.dir/src/programs/randomgraph.c.o: /home/pablo/lang-experiments/GraphC/src/programs/randomgraph.c
CMakeFiles/GraphC.dir/src/programs/randomgraph.c.o: CMakeFiles/GraphC.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/pablo/lang-experiments/GraphC/build-unix/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/GraphC.dir/src/programs/randomgraph.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/GraphC.dir/src/programs/randomgraph.c.o -MF CMakeFiles/GraphC.dir/src/programs/randomgraph.c.o.d -o CMakeFiles/GraphC.dir/src/programs/randomgraph.c.o -c /home/pablo/lang-experiments/GraphC/src/programs/randomgraph.c

CMakeFiles/GraphC.dir/src/programs/randomgraph.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/GraphC.dir/src/programs/randomgraph.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/pablo/lang-experiments/GraphC/src/programs/randomgraph.c > CMakeFiles/GraphC.dir/src/programs/randomgraph.c.i

CMakeFiles/GraphC.dir/src/programs/randomgraph.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/GraphC.dir/src/programs/randomgraph.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/pablo/lang-experiments/GraphC/src/programs/randomgraph.c -o CMakeFiles/GraphC.dir/src/programs/randomgraph.c.s

# Object files for target GraphC
GraphC_OBJECTS = \
"CMakeFiles/GraphC.dir/src/combinations/choose.c.o" \
"CMakeFiles/GraphC.dir/src/graph/graph.c.o" \
"CMakeFiles/GraphC.dir/src/main.c.o" \
"CMakeFiles/GraphC.dir/src/programs/petersen.c.o" \
"CMakeFiles/GraphC.dir/src/programs/randomgraph.c.o"

# External object files for target GraphC
GraphC_EXTERNAL_OBJECTS =

GraphC: CMakeFiles/GraphC.dir/src/combinations/choose.c.o
GraphC: CMakeFiles/GraphC.dir/src/graph/graph.c.o
GraphC: CMakeFiles/GraphC.dir/src/main.c.o
GraphC: CMakeFiles/GraphC.dir/src/programs/petersen.c.o
GraphC: CMakeFiles/GraphC.dir/src/programs/randomgraph.c.o
GraphC: CMakeFiles/GraphC.dir/build.make
GraphC: CMakeFiles/GraphC.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/pablo/lang-experiments/GraphC/build-unix/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking C executable GraphC"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/GraphC.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/GraphC.dir/build: GraphC
.PHONY : CMakeFiles/GraphC.dir/build

CMakeFiles/GraphC.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/GraphC.dir/cmake_clean.cmake
.PHONY : CMakeFiles/GraphC.dir/clean

CMakeFiles/GraphC.dir/depend:
	cd /home/pablo/lang-experiments/GraphC/build-unix && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pablo/lang-experiments/GraphC /home/pablo/lang-experiments/GraphC /home/pablo/lang-experiments/GraphC/build-unix /home/pablo/lang-experiments/GraphC/build-unix /home/pablo/lang-experiments/GraphC/build-unix/CMakeFiles/GraphC.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/GraphC.dir/depend

