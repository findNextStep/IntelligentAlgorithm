#pragma once
#include <vector>
#include <functional>
#include <memory>
#include <math.h>
#include <ctime>

namespace theNext {

// 定义通用概率类型
// 此类型的数据应当小于等于1
// 并且大于等于0
typedef float rate_t;
class unitBase {
public:
    unitBase() {}
    virtual ~unitBase() {}
    /**
     * @brief 基因型的一次变异
     *
     * @param rate 变异概率
     */
    virtual void variation(rate_t rate) = 0;
    /**
     * @brief 两个基因的一次交换
     *
     * @param base 交换的另一个基因
     */
    virtual void overlap(const ::std::shared_ptr<unitBase>& base) = 0;
    /**
     * @brief 获取本体的一个拷贝
     *
     * @return ::std::shared_ptr<unitBase> 自身的拷贝
     */
    virtual ::std::shared_ptr<unitBase> copy() = 0;
};
template <class unit>
class unitVector {
protected:
    typedef ::std::vector<::std::shared_ptr<unitBase> > bv;
// 类型定义
public:
// 定义构造函数
    /**
     * @brief 通过填充一个对应基因型的数组构造
     *
     * @param init 种群包含的基因型
     */
    unitVector(const bv &init):group(init) {
        this->defalutParam();
    }
    /**
     * @brief 通过填充一个对应基因型的数组构造
     *
     * @param begin 种群的迭代器头
     * @param end 种群迭代器尾
     */
    unitVector(bv::iterator begin,bv::iterator end):group(begin,end) {
        this->defalutParam();
    }

// 种群操作函数
public:
    /**
     * @brief 在种群中进行一次繁殖
     * 每一次运行的结果都可能不同
     * 双亲选择采用随机选择的方式
     * @return unitVector<unit> 繁殖产生的种群
     */
    unitVector<unit>&  increase()const {
        // 随机数种子重设
        srand((unsigned)time(NULL));
        // 保存下一代的数组
        bv nextGeneration;

        // 对于所有父本，产生子代
        for(int i=0; i<this->group.size(); ++i) {
            auto son = this->group.at(i)->copy();

            // 判断是否需要交叉
            if(needToDo(this->overlapRate)) {
                int mother = rand() % this->group.size();
                son->overlap(this->group.at(mother));
            }

            // 判断是否需要变异
            if(needToDo(this->variationRate)) {
                son->variation(this->variationRate);
            }

            if(son) {
                nextGeneration.push_back(son);
            }
        }

        return unitVector<unit>(nextGeneration);
    }
// 参数设置函数
public:
    /**
     * @brief 设置变异概率
     *
     * @param newVariationRate 设置的变异概率
     * @return unitVector<unit>& this
     */
    unitVector<unit>& setVariationRate(rate_t newVariationRate) {
        this->variationRate = newVariationRate;
        return *this;
    }
    /**
     * @brief 设置交叉的概率
     *
     * @param newOverlapRate 设置的交换的概率
     * @return unitVector<unit>& this
     */
    unitVector<unit>& setOverlapRate(rate_t newOverlapRate) {
        this->overlapRate = newOverlapRate;
        return *this;
    }
    /**
     * @brief 设置适应值函数
     * 返回值应当是一个非负的数值，表示个体的适应值，彼此之间差距应当较小
     * @param fun 适应值函数
     * @return unitVector<const unit&>& this
     */
    unitVector<unit>& setAdaptFun(const std::function<double(const unit&)> & fun) {
        this->adaptFun = fun;
        return *this;
    }

protected:
    /**
     * @brief 设置默认参数
     *
     */
    void defalutParam() {
        this->setVariationRate(0.05).setOverlapRate(0.95)
        .setAdaptFun([](const unit& a) {
            return 1.0;
        })
    }
    /**
     * @brief 以rate为概率返回true
     *
     * @param rate 返回true的概率
     * @return true 以rate的概率返回
     * @return false 以(1-rate)的概率返回
     */
    static bool needToDo(rate_t rate) {
        rate_t luck = rand()/rate_t(RAND_MAX);
        return luck <= rate;
    }
private:
    // 种群成员
    bv group;
    // 种群突变概率
    rate_t variationRate;
    // 种群交叉概率
    rate_t overlapRate;
    // 个体适应值计算函数
    std::function<double (const unit&)> adaptFun;
};
}