#pragma once
#include <vector>
#include <algorithm>  // 全排列函数 next_permutation
#include <math.h>

namespace theNext {
using ::std::vector;

/**
 * @brief 地图链接的最大开销
 */
const int maxCost = 256;

/**
 * @brief 获取一个最大值为max的随机数
 *
 * @param max 获取的随机数的最大值
 * @return int 获取到的随机数
 */
int getRandom(int max) {
    return (rand() % max) + 1;
}

/**
 * @brief 通过给定的地图和路径计算路径总开销
 *
 * @param cost_map 给定估值地图
 * @param path 路径
 * @return int 路径开销
 */
int count(const vector<vector<int> > &cost_map, const vector<int> &path) {
    int sum = 0;
    for(int i = 1; i < path.size(); ++i) {
        sum += cost_map[path[i - 1]][path[i]];
    }
    return sum + cost_map[path[path.size() - 1]][path[0]];
}

/**
 * @brief 通过贪心算法计算tsp最小值的路径
 * 算法是在网上找到的
 * @param d 给定估值地图
 * @param big 是否求取最大值
 * @return vector<int> 最优路径
 */
vector<int> tx(const vector<vector<int>> &d, bool big = false) {
    vector<int>S(d.size());
    int i = 0, k, l, j, sum = 0;
    do {
        int k = 1;
        int Dtemp = d.size() * maxCost * 2;
        if(big) {
            Dtemp = 0;
        }
        do {
            int l = 0;
            int flag = 0;
            do {
                if(S[l] == k) { //判断该城市是否已被访问过，若被访问过，
                    flag = 1;//则flag为1
                    break;//跳出循环，不参与距离的比较
                } else {
                    l++;
                }
            } while(l < i);
            if(big) {
                if(flag == 0 && d[k][S[i - 1]] > Dtemp) { /*D[k][S[i - 1]]表示当前未被访问的城市k与上一个已访问过的城市i-1之间的距离*/
                    j = k;//j用于存储已访问过的城市k
                    Dtemp = d[k][S[i - 1]];//Dtemp用于暂时存储当前最小路径的值
                }
            } else {
                if(flag == 0 && d[k][S[i - 1]] < Dtemp) { /*D[k][S[i - 1]]表示当前未被访问的城市k与上一个已访问过的城市i-1之间的距离*/
                    j = k;//j用于存储已访问过的城市k
                    Dtemp = d[k][S[i - 1]];//Dtemp用于暂时存储当前最小路径的值
                }
            }
            k++;
        } while(k < d.size());
        S[i] = j;//将已访问过的城市j存入到S[i]中
        i++;
        sum += Dtemp;//求出各城市之间的最短距离，注意：在结束循环时，该旅行商尚未回到原出发的城市
    } while(i < d.size());
    return S;
}

/**
 * @brief 基于全排列遍历的最优解查找
 *
 * @param map 查找的估值地图
 * @param big 是否查找最大值
 * @return vector<int> 最优路径
 */
vector<int> baoli(vector<vector<int> > map, bool big = false) {
    vector<int> pl(map.size());
    for(int i = 0; i < map.size(); ++i) {
        pl[i] = i;
    }
    vector<int> best(map.size()), worse(map.size());
    int bestcount = -1, worsecount = maxCost * map.size();
    do {
        int now = count(map, pl);
        if(big) {
            if(now > bestcount) {
                bestcount = now;
                best = pl;
            }
        } else {
            if(now < worsecount) {
                worsecount = now;
                worse = pl;
            }
        }
        if(pl[0] != 0) {
            break;
        }
    } while(::std::next_permutation(pl.begin(), pl.end()));
    if(big) {
        return best;
    } else {
        return worse;
    }
}

/**
 * @brief 获取一个随机地图
 *
 * @param problemSize 地图大小
 * @return vector<vector<int> > 地图
 */
vector<vector<int> > make_map_random(int problemSize) {
    vector<vector<int>> cost_map(problemSize);
    for(auto it = cost_map.begin(); it != cost_map.end(); ++it) {
        it->resize(problemSize);
        for(auto i = it->begin(); i != it->end(); ++i) {
            auto base = getRandom(maxCost);
            *i = base;// * base * base;
        }
    }
    for(int i = 0; i < cost_map.size(); ++i) {
        cost_map[i][i] = 0;
    }
    return cost_map;
}
/**
 * @brief 通过贪心方式获取一个最大/最小的初始化值
 *
 * @param map 地图
 * @param big 是否求取最大值
 * @return vector<int> 初始值
 */
vector<int> make_init_small(const vector<vector<int> > &map, bool big = false) {
    vector<int> result(map.size());
    result[0] = 0;
    for(int i = 1; i < map.size(); ++i) {
        int n = 0;
        for(int j = 1; j < map.size(); ++j) {
            if(i != j) {
                if(big) {
                    if(map[i][j] > map[i][n]) {
                        n = j;
                    }
                } else {
                    if(map[i][j] < map[i][n]) {
                        n = j;
                    }
                }
            }
        }
        result[i] = n;
    }
    return result;
}

vector<int> make_randowm_path(int size) {
    vector<int> path(size);
    for(int i = 0; i < size; ++i) {
        path[i] = i;
    }
    for(int i = 2; i < path.size(); ++i) {
        // 使i与之前某个量交换
        int k = i - rand() % i;
        std::swap(path.at(i), path.at(k));
    }
    return path;
}
}