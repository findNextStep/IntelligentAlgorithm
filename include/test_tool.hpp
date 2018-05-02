#pragma once
#include <chrono>
#include <functional>
#include <thread>

namespace theNext {

double main(const std::function<void()> &func) {
    auto start = ::std::chrono::system_clock::now();
    func();
    auto end   = ::std::chrono::system_clock::now();
    auto duration = ::std::chrono::duration_cast<::std::chrono::microseconds>(end - start);
    return double(duration.count()) * ::std::chrono::microseconds::period::num / ::std::chrono::microseconds::period::den;
}

template <class X>
void multiFor(std::vector<X> &list, const std::function<void(X, int)> &func) {
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
}