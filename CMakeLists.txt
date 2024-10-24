cmake_minimum_required(VERSION 3.14)

# project name
project(ThesisProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_RELEASE "-O3")

find_package(Eigen3 3.3 REQUIRED NO_MODULE)
add_executable(algoProject
    src/main.cpp
    src/algorithms/expectimax.cpp
    src/algorithms/mcts.cpp
    src/algorithms/heuristics.cpp
    src/library/globals.cpp
    src/library/board.cpp
    src/library/node.cpp
)

target_include_directories(algoProject PRIVATE 
    src/library
    src/algorithms
)

target_link_libraries(algoProject Eigen3::Eigen)
