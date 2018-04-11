cmake_minimum_required(VERSION 3.0.0)
project(geneticAlgorithm)
add_compile_options(-std=c++11)

include_directories(
    include
)
add_executable(knapsackProblem src/knapsackProblem.cpp)
add_executable(tspProblem src/tspProblem.cpp)
