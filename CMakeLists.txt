cmake_minimum_required(VERSION 3.0.0)
project(geneticAlgorithm)
add_compile_options(-std=c++11)

include_directories(
    include
)
add_executable(knapsackProblem src/knapsackProblem.cpp)
target_link_libraries(knapsackProblem pthread)
add_executable(tspProblem src/tspProblem.cpp)
target_link_libraries(tspProblem pthread)
