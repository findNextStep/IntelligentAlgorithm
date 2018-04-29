#pragma once
#include <vector>
#include <functional>
namespace theNext {

template<class T>
class tubeHelper {
public:
typedef ::std::function<T(T)> operation_t;
typedef ::std::vector<operation_t> operator_list_t;
public:
    /**
     * @brief 添加一个操作
     * 输入是一个出发点
     * 返回值是经过一个操作的一个到达点
     * @param fun 操作函数
     */
    void addOperator(const operation_t& fun){
        this->operation_list.push_back(fun);
    }
private:
    /**
     * @brief 保存操作的数组
     */
    operator_list_t operation_list;
};
}