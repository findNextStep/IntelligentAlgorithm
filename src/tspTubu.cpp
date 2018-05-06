#include "tabu/tubu.hpp"
#include "test_tool.hpp"
#include "TSP/TSPtool.hpp"
#include "test_tool.hpp"
#include <vector>
#include <iostream>
using ::std::cin;
using ::std::cout;
using ::std::endl;
using ::std::vector;
/**
 * @brief 通过禁忌搜索算法求取tsp问题
 * 禁忌表长度为地图宽度的$\frac14$
 * 计算步数为地图宽度
 * @param map 估值地图
 * @param big 是否求取最大值
 * @param complex 是否采用精细求解
 * @return vector<int> 求取的最优解
 */
vector<int> test(const vector<vector<int>> &map, bool big = false, bool complex = false) {
    ::theNext::tubeHelper<vector<int>> tube(map.size() / 4);
    tube.setAdaptFun([&big, &map](const vector<int> &path) {
        // 设置适应值函数
        if(big) {
            return ::theNext::count(map, path);
        } else {
            return (int)map.size() * theNext::maxCost - ::theNext::count(map, path);
        }
    }).setCmp([](auto a, auto b) {
        //设置比较函数,判断两个个体是否相同
        for(int i = 0; i < a.size(); ++i) {
            if(a[i] != b[i]) {
                return false;
            }
        }
        return true;
    });
    if(complex) {
        // 如果选择更精细的结果
        for(int i = 1; i < map.size(); ++i) {
            for(int j = 0; j < i; ++j)
                tube.addOperator([i, j, &map](auto start) {
                std::swap(start[j], start[i]);
                return start;
            });
        }
    } else {
        // 如果选择简单一些的结果
        for(int i = 0; i < map.size(); ++i) {
            tube.addOperator([i, &map](auto path) {
                if(i) {
                    std::swap(path[i - 1], path[i]);
                } else {
                    std::swap(path[0], path[map.size() - 1]);
                }
                return path;
            });
        }
    }
    vector<int> init(map.size());
    for(int i = 0; i < init.size(); ++i) {
        init[i] = i;
    }
    vector<int> best = init;
    // 设置当前位置
    tube.setNow(init);
    int c = 0;
    for(int i = 0; i < map.size(); ++i) {
        // 寻找下一步
        if(!tube.nextStep()) {
            break;
        }
        // 更新最优解
        if(big) {
            if(::theNext::count(map, best) < ::theNext::count(map, tube.getNow())) {
                c = i;
                best = tube.getNow();
            }
        } else {
            if(::theNext::count(map, best) > ::theNext::count(map, tube.getNow())) {
                c = i;
                best = tube.getNow();
            }
        }
    }
    return best;
}
int main() {
    int n;
    while(cin >> n) {
        cout << n << "\t";
        auto map = ::theNext::make_map_random(n);
        vector<int> ans;
        cout << ::theNext::main([&ans, &map]() {
            ans = test(map, false, false);
        }) << "\t";
        // for(auto i : ans) {
        //     cout << i << '\t';
        // }
        // cout << endl;
        cout << ::theNext::count(map, ans) << "\t" << ::std::flush;

        cout << ::theNext::main([&ans, &map]() {
            ans = test(map, false, true);
        }) << "\t";
        // for(auto i : ans) {
        //     cout << i << '\t';
        // }
        // cout << endl;
        cout << ::theNext::count(map, ans) << "\t" << ::std::flush;

        cout << ::theNext::main([&ans, &map]() {
            ans = ::theNext::tx(map, false);
        }) << '\t';
        // for(auto i : ans) {
        //     cout << i << '\t';
        // }
        // cout << endl;
        cout << ::theNext::count(map, ans) << endl;
    }
    return 0;
}
