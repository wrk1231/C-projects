# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.9

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
CMAKE_SOURCE_DIR = /Users/wrk/Documents/C++_environment/hw5_a

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/wrk/Documents/C++_environment/hw5_a/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/hw5_a.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/hw5_a.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/hw5_a.dir/flags.make

CMakeFiles/hw5_a.dir/main.cpp.o: CMakeFiles/hw5_a.dir/flags.make
CMakeFiles/hw5_a.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/wrk/Documents/C++_environment/hw5_a/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/hw5_a.dir/main.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/hw5_a.dir/main.cpp.o -c /Users/wrk/Documents/C++_environment/hw5_a/main.cpp

CMakeFiles/hw5_a.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/hw5_a.dir/main.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/wrk/Documents/C++_environment/hw5_a/main.cpp > CMakeFiles/hw5_a.dir/main.cpp.i

CMakeFiles/hw5_a.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/hw5_a.dir/main.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/wrk/Documents/C++_environment/hw5_a/main.cpp -o CMakeFiles/hw5_a.dir/main.cpp.s

CMakeFiles/hw5_a.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/hw5_a.dir/main.cpp.o.requires

CMakeFiles/hw5_a.dir/main.cpp.o.provides: CMakeFiles/hw5_a.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/hw5_a.dir/build.make CMakeFiles/hw5_a.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/hw5_a.dir/main.cpp.o.provides

CMakeFiles/hw5_a.dir/main.cpp.o.provides.build: CMakeFiles/hw5_a.dir/main.cpp.o


# Object files for target hw5_a
hw5_a_OBJECTS = \
"CMakeFiles/hw5_a.dir/main.cpp.o"

# External object files for target hw5_a
hw5_a_EXTERNAL_OBJECTS =

hw5_a: CMakeFiles/hw5_a.dir/main.cpp.o
hw5_a: CMakeFiles/hw5_a.dir/build.make
hw5_a: /Library/gurobi752/mac64/lib/libgurobi75.so
hw5_a: /Library/gurobi752/mac64/lib/libgurobi_c++.a
hw5_a: CMakeFiles/hw5_a.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/wrk/Documents/C++_environment/hw5_a/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable hw5_a"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/hw5_a.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/hw5_a.dir/build: hw5_a

.PHONY : CMakeFiles/hw5_a.dir/build

CMakeFiles/hw5_a.dir/requires: CMakeFiles/hw5_a.dir/main.cpp.o.requires

.PHONY : CMakeFiles/hw5_a.dir/requires

CMakeFiles/hw5_a.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/hw5_a.dir/cmake_clean.cmake
.PHONY : CMakeFiles/hw5_a.dir/clean

CMakeFiles/hw5_a.dir/depend:
	cd /Users/wrk/Documents/C++_environment/hw5_a/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/wrk/Documents/C++_environment/hw5_a /Users/wrk/Documents/C++_environment/hw5_a /Users/wrk/Documents/C++_environment/hw5_a/cmake-build-debug /Users/wrk/Documents/C++_environment/hw5_a/cmake-build-debug /Users/wrk/Documents/C++_environment/hw5_a/cmake-build-debug/CMakeFiles/hw5_a.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/hw5_a.dir/depend

