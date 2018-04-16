#include "unit.hpp"
#include <thread>
#include <mutex>
namespace theNext {
using ::std::mutex;
using ::std::thread;
// 进行多线程加速的遗传算法种群
template<typename T>
class unitVectorMulti : public unitVector<T> {
protected:
    typedef unitVector<T> base;
    typedef ::std::vector<::std::shared_ptr<unitBase> > bv;
    const int static threadSize = 7;
public:
    // 定义构造函数
    /**
     * @brief 通过填充一个对应基因型的数组构造
     *
     * @param init 种群包含的基因型
     */
    unitVectorMulti(const ::std::vector<::std::shared_ptr<unitBase> > &init): base(init) {
    }

    unitVectorMulti(const unitVector<T> &copy): base(copy) {
    }
public:
    /**
     * @brief 对当前种群进行一次淘汰
     *
     * @param leave 最终剩余个体数量, 默认淘汰一半
     */
    void eliminate(unsigned int leave = 0) {
        if(leave == 0) {
            leave = this->group.size() / 2;
        } else if(leave >= this->group.size()) {
            return;
        }
        std::vector<double> rate(this->group.size());
        // for(int i = 0; i < this->group.size(); ++i) {
        //     rate[i] = this->adaptFun(*((T *)this->group[i].get()));
        // }
        multiFor<::std::shared_ptr<unitBase> >(this->group, [&rate, this](::std::shared_ptr<unitBase> unit, int i) {
            rate[i] = this->adaptFun(*((T *)unit.get()));
        });
        typename unitVector<T>::bv remain;
        int count = 0;
        std::vector<bool> select(unitVector<T>::selectUnit(rate, leave));
        for(int i = 0; i < this->group.size(); ++i) {
            if(select[i]) {
                remain.push_back(this->group[i]);
                ++count;
            }
        }
        this->group.resize(0);
        this->group.reserve(remain.size());
        for(auto man : remain) {
            this-> group.push_back(man);
        }
    }
protected:
    template<typename X>
    void static multiFor(std::vector<X> &list, const std::function<void(X, int)> &func) {
        mutex count_mutex;
        int count = 0;
        ::std::vector<thread> threadPort(threadSize);
        for(auto it = threadPort.begin(); it != threadPort.end(); ++it) {
            *it = ::std::thread([&count, &count_mutex, &list, &func, &threadPort, &it]() {
                while(true) {
                    count_mutex.lock();
                    int i = 0;
                    if(count == list.size()) {
                        count_mutex.unlock();
                        break;
                    } else {
                        i = count++;
                    }
                    count_mutex.unlock();
                    func(list[i], i);
                }
            });
        }
        for(auto it = threadPort.begin(); it != threadPort.end(); ++it) {
            if(it->joinable()) {
                it->join();
            }
        }

    }
};
}