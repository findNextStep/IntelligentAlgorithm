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
vector<int> tx(const vector<vector<int>> &map) {
    ::theNext::tubeHelper<vector<int>, 100> tube;
    tube.setAdaptFun([&map](const vector<int> &path) {
        return ::theNext::count(map, path);
    }).setCmp([](auto a, auto b) {
        for(int i = 0; i < a.size(); ++i) {
            if(a[i] != b[i]) {
                return false;
            }
        }
        return true;
    });
    for(int i = 0; i < map.size(); ++i) {
        tube.addOperator([i, &map](auto start) {
            if(i == map.size() - 1) {
                std::swap(start[0], start[i]);
            } else {
                std::swap(start[i], start[i + 1]);
            }
            return start;
        });
    }
    vector<int> init(map.size());
    for(int i = 0; i < init.size(); ++i) {
        init[i] = i;
    }
    vector<int> best = init;
    tube.setNow(init);
    for(int i = 0; i < 100; ++i) {
        for(auto i : tube.getNow()) {
            cout << i << '\t' ;
        }
        cout << endl;
        if(!tube.nextStep()) {
            cout << "???" << i << endl;
            break;
        }
        if (::theNext::count(map,best) < ::theNext::count(map,tube.getNow())){
            best = tube.getNow();
        }
    }
    return best;
}
int main() {
    auto map = ::theNext::make_map_random(10);
    vector<int> ans;
    ::theNext::main([&ans, &map]() {
        ans = tx(map);
    });
    for(auto i : ans) {
        cout << i << '\t';
    }cout << endl;
    cout << ::theNext::count(map, ans) << endl;
    ans = ::theNext::baoli(map,true);
    for(auto i : ans) {
        cout << i << '\t';
    }cout << endl;
    cout << ::theNext::count(map, ans) << endl;
    return 0;
}
