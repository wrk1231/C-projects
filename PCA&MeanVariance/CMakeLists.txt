cmake_minimum_required(VERSION 3.9)
project(hw5)

set(CMAKE_CXX_STANDARD 14)

cmake_minimum_required(VERSION 3.8)
project(gurobitest)

set(CMAKE_CXX_STANDARD 14)

#set(SOURCE_FILES mip1_c++.cpp)
#add_executable(gurobitest ${SOURCE_FILES})
### Taken from http://www.openflipper.org/svnrepo/CoMISo/trunk/CoMISo/cmake/FindGUROBI.cmake
# - Try to find GUROBI
# Once done this will define
#  GUROBI_FOUND - System has Gurobi
#  GUROBI_INCLUDE_DIRS - The Gurobi include directories
#  GUROBI_LIBRARIES - The libraries needed to use Gurobi

if (GUROBI_INCLUDE_DIR)
    # in cache already
    set(GUROBI_FOUND TRUE)
    set(GUROBI_INCLUDE_DIRS "${GUROBI_INCLUDE_DIR}" )
    set(GUROBI_LIBRARIES "${GUROBI_LIBRARY};${GUROBI_CXX_LIBRARY}" )
else (GUROBI_INCLUDE_DIR)
    find_path(GUROBI_INCLUDE_DIR
            NAMES gurobi_c++.h
            PATHS "$ENV{GUROBI_HOME}/include"
            "/Library/gurobi751/mac64/include"
            "C:\\libs\\gurobi751\\include"
            )
    find_library( GUROBI_LIBRARY
            NAMES gurobi
            gurobi75
            PATHS "$ENV{GUROBI_HOME}/lib"
            "/Library/gurobi751/mac64/lib"
            "C:\\libs\\gurobi751\\lib"
            )
    find_library( GUROBI_CXX_LIBRARY
            NAMES gurobi_c++
            PATHS "$ENV{GUROBI_HOME}/lib"
            "/Library/gurobi751/mac64/lib"
            "C:\\libs\\gurobi751\\lib"
            )
    set(GUROBI_INCLUDE_DIRS "${GUROBI_INCLUDE_DIR}" )
    set(GUROBI_LIBRARIES "${GUROBI_LIBRARY};${GUROBI_CXX_LIBRARY}" )
    # use c++ headers as default
    # set(GUROBI_COMPILER_FLAGS "-DIL_STD" CACHE STRING "Gurobi Compiler Flags")
    include(FindPackageHandleStandardArgs)
    # handle the QUIETLY and REQUIRED arguments and set LIBCPLEX_FOUND to TRUE
    # if all listed variables are TRUE
    find_package_handle_standard_args(GUROBI  DEFAULT_MSG
            GUROBI_LIBRARY GUROBI_CXX_LIBRARY GUROBI_INCLUDE_DIR)
    mark_as_advanced(GUROBI_INCLUDE_DIR GUROBI_LIBRARY GUROBI_CXX_LIBRARY)
endif(GUROBI_INCLUDE_DIR)

include_directories(${GUROBI_INCLUDE_DIRS})
set(SOURCE_FILES ${GUROBI_INCLUDE_DIRS}/gurobi_c++.h ${GUROBI_INCLUDE_DIRS}/gurobi_c.h extra.cpp)
add_executable(hw5 ${SOURCE_FILES})
target_link_libraries(hw5 ${GUROBI_LIBRARIES})