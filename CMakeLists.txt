cmake_minimum_required(VERSION 3.0.0)
project(IntelligentCalculation)
# 采用c++14的auto关键字和新的标准库
add_compile_options(-std=c++14)

include_directories(
    include
)
# 多线程库需要
link_libraries(pthread)

# 背包问题的遗传算法求解
add_executable(knapsackProblem src/knapsackProblem.cpp)

# tsp问题的遗传算法求解
add_executable(tspProblem src/tspProblem.cpp)

# tsp问题的禁忌搜索算法求解
add_executable(tspTube src/tspTubu.cpp)

# tsp问题的模拟退火算法求解
add_executable(tspAnnealing src/tspAnnealing.cpp)

# tsp问题的蚁群算法求解
add_executable(tspAnt src/tspAnt.cpp)

# 粒子群算法样例
add_executable(testParticleSwarm src/testParticleSwarm.cpp)