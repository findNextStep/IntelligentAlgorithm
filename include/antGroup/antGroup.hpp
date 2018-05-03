#pragma once
#include <vector>
#include <functional>
#include <math.h>
#include "TSP/TSPtool.hpp"
#include <iostream>
using namespace std;

namespace theNext {
class antGroup {
public:
    typedef antGroup this_t;
    typedef ::std::vector<::std::vector<int> > map_t;
    typedef ::std::vector<int> path_t;
public:
    /**
     * @brief 蚁群tsp算法抽象的构造函数
     *
     * @param map 路径中点到点的路程
     * @param ant_size 蚁群规模
     * @param big 是否求取最大值.如果为否,求取最小值
     */
    antGroup(const map_t &map, int ant_size, bool big):
        map(map),
        messages(map.size(), ::std::vector<double>(map.size(), 1.0)),
        big(big),
        ant_size(ant_size) {}
    /**
     * @brief 设置信息素的权重
     *
     * @param power 新的权重
     * @return this_t
     */
    this_t setMessagePower(double power) {
        this->message_power = power;
        return *this;
    }
    /**
     * @brief 设置路径参数的权重
     *
     * @param power 新的权重
     * @return this_t
     */
    this_t setPathPower(double power) {
        this->path_power = power;
        return *this;
    }
public:
    path_t loopOnce() {
        ::std::vector<path_t> ants(this->ant_size);
        int best = 0;
        path_t result;
        if(!big) {
            best = ::theNext::maxCost * map.size();
        }
        for(auto it = ants.begin(); it != ants.end(); ++it) {
            *it = antFindPath();
            int count = ::theNext::count(this->map, *it);

            if(count > best && big || count < best && !big) {
                best = count;
                result = *it;
            }
        }

        for(auto path : ants) {
            addMessage(path);
        }
        for (auto p = this->messages.begin();p != this->messages.end();++p){
            for (auto i = p->begin();i!=p->end();++i){
                // *i =(*i+1)/2;
                *i *= 0.95;
            }
        }
        // for(auto line : this->messages) {
        //     for(auto i : line) {
        //         cout << i - 1 << '\t';
        //     }
        //     cout << endl;
        // }cout << endl;
        return ants[0];
    }
protected:
    /**
     * @brief 向信息素地图中添加信息素
     *
     * @param path 添加信息素依赖的路径
     */
    void addMessage(path_t path) {
        const double Q = 12;
        // 蚁周模型
        {
            auto sum = ::theNext::count(map, path);
            for(int i = 1; i < path.size(); ++i) {
                if(big) {
                    this->messages[path[i - 1]][path[i]] += Q * sum;
                } else {
                    this->messages[path[i - 1]][path[i]] += Q / sum;
                }
            }
            if(big) {
                this->messages[*path.rbegin()][*path.begin()] += Q * sum;
            } else {
                this->messages[*path.rbegin()][*path.begin()] += Q / sum;
            }
        }
        // 蚁密模型
        // {
        //     for(int i = 1; i < path.size(); ++i) {
        //         this->messages[path[i - 1]][path[i]] += Q;
        //     }
        //     this->messages[*path.rbegin()][*path.begin()] += Q;
        // }
        // 蚁量模型
        // {
        //     for(int i = 1; i < path.size(); ++i) {
        //         if(big) {
        //             this->messages[path[i - 1]][path[i]] += Q * map[path[i - 1]][path[i]];
        //         } else {
        //             this->messages[path[i - 1]][path[i]] += Q / map[path[i - 1]][path[i]];
        //         }
        //     }
        //     if(big) {
        //         this->messages[*path.rbegin()][*path.begin()] += Q * map[*path.rbegin()][*path.begin()];
        //     } else {
        //         this->messages[*path.rbegin()][*path.begin()] += Q / map[*path.rbegin()][*path.begin()];
        //     }
        // }
    }
    /**
     * @brief 一只蚂蚁通过轮盘赌方式获取一条路径
     *
     * @return path_t 蚂蚁获取的路径
     */
    path_t antFindPath() {
        int size = this->map.size();
        vector<int> result;
        // cout << "test" << size << endl;
        result.reserve(map.size() + 1);
        // cout << result.capacity() << endl;
        result.resize(map.size());
        // cout << "test" << endl;
        result[0] = 0;
        int p = 0;
        while(p < result.size()) {
            auto worth = this->makeWorth(result[p]);
            for(int i = 0; i <= p; ++i) {
                worth[result[i]] = 0.0;
            }
            result[++p] = this->select(worth);
        }
        return result;
    }
    /**
     * @brief 构建一个点向其他点移动的估值列表
     *
     * @param path 当前点
     * @return ::std::vector<double> 估值列表
     */
    ::std::vector<double> makeWorth(int path) {
        ::std::vector<double> worth(map.size());
        for(int i = 0; i < worth.size(); ++i) {
            worth[i] = pow(this->messages[path][i], message_power);
            if(this->big) {
                worth[i] *= pow(this->map[path][i], path_power);
            } else {
                worth[i] *= pow(this->map[path][i], -path_power);
            }
        }
        return worth;
    }
    /**
     * @brief 进行一次轮盘赌运算
     * 根据rates中的数据选择,rate数据越大,可能越高
     * @param rates 选择参数
     * @return int 被选择的个体
     */
    static int select(const ::std::vector<double> &rates) {
        double sum = 0.0;
        for(double rate : rates) {
            sum += rate;
        }
        double chose = rand() / double(RAND_MAX) * sum;
        for(int i = 0; i < rates.size(); ++i) {
            chose -= rates[i];
            if(chose <= 0.0) {
                return i;
            }
        }
    }
private:
    double message_power;
    double path_power;
    int ant_size;
    bool big;
    // 信息素量
    ::std::vector<::std::vector<double> > messages;
    const map_t map;
};
}