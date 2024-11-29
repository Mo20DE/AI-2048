cmake_minimum_required(VERSION 3.14)

project(thesisProject) # project name

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_RELEASE "-O3")
find_package(Eigen3 3.3 REQUIRED NO_MODULE)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/algorithms/expectimax.cpp
    src/algorithms/mctse.cpp
    src/algorithms/mcts.cpp
    src/library/globals.cpp
    src/library/board.cpp
    src/library/node.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE 
    src/algorithms
    src/library
)

target_link_libraries(${PROJECT_NAME} Eigen3::Eigen)
