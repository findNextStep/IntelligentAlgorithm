#include <chrono>
#include <functional>

namespace theNext {

double main(const std::function<void()> &func) {
    auto start = ::std::chrono::system_clock::now();
    func();
    auto end   = ::std::chrono::system_clock::now();
    auto duration = ::std::chrono::duration_cast<::std::chrono::microseconds>(end - start);
    return double(duration.count()) * ::std::chrono::microseconds::period::num / ::std::chrono::microseconds::period::den;
}
}