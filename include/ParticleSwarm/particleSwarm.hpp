#pragma once
#include <array>
#include <stdlib.h> // rand
#include <functional>
#include <utility>
#include <vector>

namespace theNext {
template <size_t size> class paricleSwarm {
public:
    typedef ::std::array<double, size> path_t;
    typedef ::std::pair<path_t, double> record_t;
    typedef ::std::pair<double, double> range_t;
    typedef ::std::function<double(path_t)> fun_t;
    typedef paricleSwarm<size> this_t;
    struct item_t {
        path_t position;
        record_t best;
        path_t speed;
    };
public:
    /**
     * @brief 粒子群算法解的构造函数,
     * 参数为一个输入为array返回值的评价函数
     * 算法类求取评价函数最小值
     * @param fun 评价函数
     */
    paricleSwarm(const fun_t &fun) : count_fun(fun) {
        this->group_best.second = -999999999999999999999.9;
        this->setItemBestPower(0.25);
        this->setGlobalBestPower(0.25);
    }
    /**
     * @brief 设置所有参数范围
     *
     * @param max 参数最大值
     * @param min 参数最小值
     * @return this_t
     */
    this_t setNumberRange(double max, double min) {
        this->NumberRange = std::make_pair(max, min);
        return *this;
    }
    /**
     * @brief 设置速度范围
     *
     * @param max 速度最大值
     * @param min 速度最小值
     * @return this_t
     */
    this_t setSpeedRange(double max, double min) {
        this->speedRange = std::make_pair(max, min);
        return *this;
    }
    /**
     * @brief 添加一个粒子
     *
     * @param position 粒子位置
     * @return this_t
     */
    this_t addNewUnit(path_t position) {
        return addNewUnit(position, make_random_speed());
    }
    /**
     * @brief 添加一个粒子
     *
     * @param position 粒子位置
     * @param speed 粒子速度
     * @return this_t
     */
    this_t addNewUnit(path_t position, path_t speed) {
        this->units.push_back(make_init_item(position, speed));
        if((*(this->units.end())).best.second > this->group_best.second) {
            this->group_best.first = (*(this->units.end())).best.first;
            this->group_best.second = (*(this->units.end())).best.second;
        }
        return *this;
    }
    /**
     * @brief 添加一些粒子
     *
     * @param number 需要添加的粒子数量
     * @return this_t
     */
    this_t addNewUnit(int number) {
        while(number--) {
            this->addNewUnit(make_random_position());
        }
        return *this;
    }
    /**
     * @brief 设置个体最优解的权重
     *
     * @param c
     * @return this_t
     */
    this_t setItemBestPower(double c) {
        this->item_best_power = c;
        return *this;
    }
    /**
     * @brief 设置全局最优值的权重
     *
     * @param c
     * @return this_t
     */
    this_t setGlobalBestPower(double c) {
        this->global_best_power = c;
        return *this;
    }
    /**
     * @brief 进行一次迭代
     */
    void oneIteration() {
        // 重新计算所有点的速度
        for(int i = 0; i < size; ++i) {
            this->count_speed(this->units[i]);
        }
        for(int i = 0; i < size; ++i) {
            this->fly(this->units[i]);
        }
        find_best();
    }
    /**
     * @brief 获取当前全局最优解的记录
     *
     * @return record_t
     */
    record_t getBest() {
        return group_best;
    }
    /**
     * @brief 获取当前所有粒子的状态
     *
     * @return auto
     */
    auto getUnits() {
        return this->units;
    }
protected:
    /**
     * @brief 进行一次全局最优解的查找
     */
    void find_best() {
        double best = this->group_best.second;
        for(auto item : this->units) {
            if(item.best.second > this->group_best.second) {
                this->group_best.first = item.best.first;
                this->group_best.second = item.best.second;
            }
        }
    }
    /**
     * @brief 根据现状计算一个item的速度
     *
     * @param item
     */
    void count_speed(item_t &item) {
        for(int i = 0; i < size; ++i) {
            double item_best = item_best_power * get_random();
            double global_best = global_best_power * get_random();
            item.speed[i] = 0.5 * item.speed[i] +
                            item_best * (item.best.first[i] - item.position[i]) +
                            global_best * (this->group_best.first[i] - item.position[i]);
            // if(item.speed[i] > this->speedRange.first) {
            //     item.speed[i] = this->speedRange.first;
            // }
            // if(item.speed[i] < this->speedRange.second) {
            //     item.speed[i] = this->speedRange.second;
            // }
        }
    }
    /**
     * @brief 让一个粒子沿当前速度飞行
     *
     * @param item 待飞行的粒子
     */
    void fly(item_t &item) {
        for(int i = 0; i < size; ++i) {
            item.position[i] += item.speed[i];
            // if(item.position[i] > this->NumberRange.first) {
            //     item.position[i] = this->NumberRange.first;
            // }
            // if(item.position[i] < this->NumberRange.second) {
            //     item.position[i] = this->NumberRange.second;
            // }
        }
        auto current = this->count_fun(item.position);
        if(current > item.best.second) {
            item.best.first = item.position;
            item.best.second = current;
        }
    }
    /**
     * @brief 根据给出的范围返回一个随机生成的路径对象
     *
     * @param range 数字范围
     * @return path_t 随机生成的路径
     */
    path_t make_random_path(range_t range) {
        path_t result;
        for(auto it = result.begin(); it != result.end(); ++it) {
            *it = rand() / double(RAND_MAX) * (range.first - range.second) + range.second;
        }
        return result;
    }
    /**
     * @brief 产生一个随机位置
     *
     * @return path_t
     */
    path_t make_random_position() {
        return make_random_path(this->NumberRange);
    }
    /**
     * @brief 产生一个随机速度
     *
     * @return path_t
     */
    path_t make_random_speed() {
        return make_random_path(this->speedRange);
    }
    /**
     * @brief 建立一个初始化的位置信息
     *
     * @param position 初始位置
     * @return ::std::pair<path_t, record_t> 初始记录对
     */
    item_t make_init_item(path_t position, path_t speed) {
        item_t result;
        result.speed = speed;
        result.position = position;
        result.best.first = position;
        result.best.second = this->count_fun(position);
        return result;
    }
    static double get_random() {
        return rand() / (double)RAND_MAX;
    }
private:
    /**
     * @brief 记录当前粒子群体
     */
    ::std::vector<item_t> units;
    /**
     * @brief 评价函数
     */
    fun_t count_fun;
    /**
     * @brief 记录当前粒子群最优解
     */
    record_t group_best;
    /**
     * @brief 数字范围和速度范围
     */
    range_t speedRange, NumberRange;
    /**
     * @brief 个体最优和全局最优的权值
     */
    double item_best_power, global_best_power;
};// paricleSwarm
} // namespace theNext