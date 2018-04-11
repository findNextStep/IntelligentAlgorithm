cmake_minimum_required(VERSION 3.0.0)
project(geneticAlgorithm VERSION 0.0.0)
add_compile_options(-std=c++11)

include_directories(
    include
)
add_executable(knapsackProblem src/knapsackProblem.cpp)