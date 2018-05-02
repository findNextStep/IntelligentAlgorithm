#include <iostream>
#include <vector>
#include <functional>
#include <math.h>
#include "test_tool.hpp"

namespace theNext {
template <class T>
class annealing {
public:
    typedef annealing<T> this_t;
    typedef ::std::function<T(T)> operation_t;
public:
    annealing();

public:
// 主要循环操作
    void SAoperation(){
        multiFor(this->units,[&this](auto unit,int i){
            int op_i = rand() / this->operation_list.size();
            auto son = this->operation_list.at(op_i);
            if (this->canLeave(this->adapt(unit),this->adapt(son))){
                this->units.at(i) = son;
            }
        })
    }
public:
// 一些设置函数
    /**
     * @brief 添加一个操作
     * 输入是一个出发点
     * 返回值是经过一个操作的一个到达点
     * @param fun 操作函数
     */
    this_t &addOperator(const operation_t &fun) {
        this->operation_list.push_back(fun);
    }
    this_t &setAdaptFun(const std::function<double(const T &)> &fun) {
        this->adapt = fun;
        return *this;
    }
    this_t &addunit(T unit) {
        this->units.push_back(unit);
    }
    this_t &setTemperature(double temperature) {
        this->temperature = temperature;
    }
protected:
    /**
     * @brief 判断一个新的表现型是否可以存活
     * @param old_adapt 旧的表现型的适应值
     * @param new_adapt 新的表现型的适应型
     * @return true 新的表现型可以保留
     * @return false 新的表现型不可保留
     */
    bool canLeave(double old_adapt, double new_adapt) {
        if(new_adapt > old_adapt) {
            return true;
        } else {
            return needToDo(exp((new_adapt - old_adapt) / this->temperature));
        }
    }
    /**
     * @brief 以rate为概率返回true
     * @param rate 返回true的概率
     * @return true 以rate的概率返回
     * @return false 以(1-rate)的概率返回
     */
    static bool needToDo(rate_t rate) {
        rate_t luck = rand() / rate_t(RAND_MAX);
        return luck <= rate;
    }
private:
    std::vector<T> units;
    /**
     * @brief 保存操作的数组
     */
    operator_list_t operation_list;
    /**
     * @brief 适应值函数
     */
    ::std::function<double(T)> adapt;
    /**
     * @brief 设置温度
     */
    double temperature;
};
}