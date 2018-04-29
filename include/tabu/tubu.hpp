#pragma once
#include <vector>
#include <list>
#include <functional>
namespace theNext {
/**
 * @brief 建立一个禁忌算法求解的抽象
 *
 * @tparam T 求解对象
 * @tparam size 禁忌表大小
 */
template<class T, int size>
class tubeHelper {
public:
    typedef ::std::function<T(T)> operation_t;
    typedef ::std::vector<operation_t> operator_list_t;
    typedef tubeHelper<T, size> this_t;
public:
    tubeHelper() {}
protected:
    bool tubeIt(T son) {
        for(auto item : son) {
            if(cmp(item, son)) {
                return false;
            }
        }
        return true;
    }
    /**
     * @brief 返回当前位置在当前所有从操作中所能达到的最优点
     *
     * @return T 最优点
     */
    virtual T next() {
        T best;
        double best_adapt = -1;
        for(auto fun : this->operation_list) {
            auto son = fun(this->now);
            if(!tubeIt(son)) {
                double adapt = this->adapt(son);
                if(adapt < best_adapt) {
                    best_adapt = adapt;
                    best = son;
                }
            }
        }
        if(best_adapt == -1) {
            return now;
        } else {
            return best;
        }
    }
public:
    /**
     * @brief 返回下一步
     *
     * @return true 已经走出新一步
     * @return false 无法走出新一步
     */
    bool nextStep() {
        this->tube.push_back(this->now);
        auto n = next();
        if(this->cmp(n, now)) {
            return false;
        }
        if(this->tube.size() > size) {
            this->tube.pop_front();
        }
        return true;
    }
public:
// 一些获取函数
    T getNow() {
        return now;
    }
public:
// 一些设定函数
    /**
     * @brief 设置当前位置
     *
     * @param thing 带设定的当前位置
     */
    this_t &setNow(T &thing) {
        this->now = thing;
        return *this;
    }
    /**
     * @brief 添加一个操作
     * 输入是一个出发点
     * 返回值是经过一个操作的一个到达点
     * @param fun 操作函数
     */
    this_t &addOperator(const operation_t &fun) {
        this->operation_list.push_back(fun);
    }
    /**
     * @brief 设置适应值函数
     * 返回值应当是一个非负的数值，表示个体的适应值，彼此之间差距应当较小
     * @param fun 适应值函数
     * @return unitVector<const unit&>& this
     */
    this_t &setAdaptFun(const std::function<T(const unit &)> &fun) {
        this->adaptFun = [fun](const unit & un) {
            return (double)fun(un);
        };
        return *this;
    }
    this_t &setAdaptFun(const std::function<double(const unit &)> &fun) {
        this->adaptFun = fun;
        return *this;
    }
    /**
     * @brief 设置比较函数
     *
     * @param fun 比较两个对象是否是同样的
     * @return this_t&
     */
    this_t &setCmp(const ::std::function<bool(T, T)> &fun) {
        this->cmp = fun;
    }
private:
    /**
     * @brief 保存操作的数组
     */
    operator_list_t operation_list;
    /**
     * @brief 当前位置
     */
    T now;
    /**
     * @brief 适应值函数
     */
    ::std::function<double(T)> adapt;
    /**
     * @brief 禁忌表
     * 禁忌表采用单向链表便于统计数量和头插入尾删除
     * 采用迭代器遍历,减少遍历开销
     */
    ::std::list<T> tube;
    /**
     * @brief 设置禁忌比较函数
     */
    ::std::function<bool(T, T)> cmp;
};
}