# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.8

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/wrk/Documents/C++_environment/clion0

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/wrk/Documents/C++_environment/clion0/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/clion0.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/clion0.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/clion0.dir/flags.make

CMakeFiles/clion0.dir/main.cpp.o: CMakeFiles/clion0.dir/flags.make
CMakeFiles/clion0.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/wrk/Documents/C++_environment/clion0/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/clion0.dir/main.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/clion0.dir/main.cpp.o -c /Users/wrk/Documents/C++_environment/clion0/main.cpp

CMakeFiles/clion0.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/clion0.dir/main.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/wrk/Documents/C++_environment/clion0/main.cpp > CMakeFiles/clion0.dir/main.cpp.i

CMakeFiles/clion0.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/clion0.dir/main.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/wrk/Documents/C++_environment/clion0/main.cpp -o CMakeFiles/clion0.dir/main.cpp.s

CMakeFiles/clion0.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/clion0.dir/main.cpp.o.requires

CMakeFiles/clion0.dir/main.cpp.o.provides: CMakeFiles/clion0.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/clion0.dir/build.make CMakeFiles/clion0.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/clion0.dir/main.cpp.o.provides

CMakeFiles/clion0.dir/main.cpp.o.provides.build: CMakeFiles/clion0.dir/main.cpp.o


# Object files for target clion0
clion0_OBJECTS = \
"CMakeFiles/clion0.dir/main.cpp.o"

# External object files for target clion0
clion0_EXTERNAL_OBJECTS =

clion0: CMakeFiles/clion0.dir/main.cpp.o
clion0: CMakeFiles/clion0.dir/build.make
clion0: CMakeFiles/clion0.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/wrk/Documents/C++_environment/clion0/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable clion0"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/clion0.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/clion0.dir/build: clion0

.PHONY : CMakeFiles/clion0.dir/build

CMakeFiles/clion0.dir/requires: CMakeFiles/clion0.dir/main.cpp.o.requires

.PHONY : CMakeFiles/clion0.dir/requires

CMakeFiles/clion0.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/clion0.dir/cmake_clean.cmake
.PHONY : CMakeFiles/clion0.dir/clean

CMakeFiles/clion0.dir/depend:
	cd /Users/wrk/Documents/C++_environment/clion0/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/wrk/Documents/C++_environment/clion0 /Users/wrk/Documents/C++_environment/clion0 /Users/wrk/Documents/C++_environment/clion0/cmake-build-debug /Users/wrk/Documents/C++_environment/clion0/cmake-build-debug /Users/wrk/Documents/C++_environment/clion0/cmake-build-debug/CMakeFiles/clion0.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/clion0.dir/depend
