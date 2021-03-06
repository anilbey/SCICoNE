cmake_minimum_required(VERSION 3.5.1)
project(sc_dna)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS "-fsanitize=undefined")
set(CMAKE_CXX_FLAGS "-fsanitize=address")

# set(CMAKE_CXX_FLAGS "--coverage")
# set(CMAKE_CXX_FLAGS "-fprofile-arcs -ftest-coverage")
set(CMAKE_CXX_FLAGS_DEBUG  "-g2") # icc needs -O3 after -g
set(CMAKE_CXX_FLAGS_DEBUG "-Wuninitialized")
set(CMAKE_CXX_FLAGS_DEBUG "-Wpedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-Wall")
set(CMAKE_CXX_FLAGS_DEBUG "-Wextra")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -gdwarf-3")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -gdwarf-3")

# ----------------------------------------------------------------------------
# Dependencies
# ----------------------------------------------------------------------------

# Search for OpenMP for parallelization
find_package(OpenMP)

# Search for boost and nlopt
find_package(Boost 1.47 COMPONENTS random REQUIRED)
find_package(NLopt REQUIRED)
include_directories(${NLOPT_INCLUDE_DIRS})

include_directories(src)
include_directories(include)
include_directories(simulations)

add_executable(inference
        src/MathOp.cpp
        src/infer_trees.cpp
        src/MathOp.h
        src/Tree.h src/Node.h  src/Inference.h src/SingletonRandomGenerator.h include/xxhash.h include/xxhash.c src/Utils.cpp src/Utils.h src/globals.cpp src/Lgamma.cpp src/Lgamma.h src/CustomExceptions.h)
target_link_libraries(inference ${NLOPT_LIBRARY_DIRS}/libnlopt.so OpenMP::OpenMP_CXX)

add_executable(simulation simulations/run_simulations.cpp src/MathOp.cpp
        src/MathOp.h
        src/Tree.h src/Node.h  src/Inference.h src/SingletonRandomGenerator.h include/xxhash.h include/xxhash.c src/Utils.cpp src/Utils.h simulations/run_simulations.cpp simulations/Simulation.h src/globals.cpp src/Lgamma.cpp src/Lgamma.h src/CustomExceptions.h)
target_link_libraries(simulation ${NLOPT_LIBRARY_DIRS}/libnlopt.so OpenMP::OpenMP_CXX)

add_executable(score src/score_tree.cpp src/MathOp.cpp
        src/MathOp.h
        src/Tree.h src/Node.h  src/Inference.h src/SingletonRandomGenerator.h include/xxhash.h include/xxhash.c src/Utils.cpp src/Utils.h src/globals.cpp src/Lgamma.cpp src/Lgamma.h src/CustomExceptions.h)
target_link_libraries(score ${NLOPT_LIBRARY_DIRS}/libnlopt.so OpenMP::OpenMP_CXX)

add_executable(breakpoint_detection
        src/breakpoint_detection.cpp src/MathOp.cpp src/MathOp.h  src/Utils.cpp src/Utils.h  src/SignalProcessing.cpp src/SignalProcessing.h src/Lgamma.cpp src/Lgamma.h include/xxhash.h include/xxhash.c src/CustomExceptions.h)
target_link_libraries(breakpoint_detection ${NLOPT_LIBRARY_DIRS}/libnlopt.so OpenMP::OpenMP_CXX)

# tests
enable_testing()

add_executable(tests tests/run_tests.cpp src/MathOp.cpp
        src/MathOp.h
        src/Tree.h src/Node.h  src/Inference.h  src/SignalProcessing.cpp src/SignalProcessing.h  src/SingletonRandomGenerator.h tests/validation.h include/xxhash.h include/xxhash.c src/Utils.cpp src/Utils.h src/globals.cpp src/Lgamma.cpp src/Lgamma.h src/CustomExceptions.h)
target_link_libraries(tests ${NLOPT_LIBRARY_DIRS}/libnlopt.so OpenMP::OpenMP_CXX)

add_executable(save_root_per_nu tests/save_root_score_per_nu.cpp src/MathOp.cpp
        src/MathOp.h
        src/Tree.h src/Node.h  src/Inference.h src/SingletonRandomGenerator.h tests/validation.h include/xxhash.h include/xxhash.c src/Utils.cpp src/Utils.h src/globals.cpp src/Lgamma.cpp src/Lgamma.h src/CustomExceptions.h)
target_link_libraries(save_root_per_nu ${NLOPT_LIBRARY_DIRS}/libnlopt.so OpenMP::OpenMP_CXX)

# unit tests
add_test(unit_tests tests)

#src/Inference.h
