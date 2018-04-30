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
    ::theNext::tubeHelper<vector<int>, 10> tube;
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
        for(int j = 0; j < i; ++j)
            tube.addOperator([i, j, &map](auto start) {
            std::swap(start[j], start[i]);
            return start;
        });
    }
    vector<int> init(map.size());
    for(int i = 0; i < init.size(); ++i) {
        init[i] = i;
    }
    vector<int> best = init;
    tube.setNow(init);
    for(int i = 0; i < map.size() * map.size(); ++i) {
        // for(auto i : tube.getNow()) {
        //     cout << i << '\t' ;
        // }
        // cout << endl;
        if(!tube.nextStep()) {
            // cout << "???" << i << endl;
            break;
        }
        if(::theNext::count(map, best) < ::theNext::count(map, tube.getNow())) {
            best = tube.getNow();
        }
        // cout << i << "\t" << ::theNext::count(map, best) << endl;
    }
    return best;
}
int main() {
    auto map = ::theNext::make_map_random(5);
    vector<int> ans;
    cout << ::theNext::main([&ans, &map]() {
        ans = tx(map);
    }) << "\t";
    // for(auto i : ans) {
    //     cout << i << '\t';
    // }
    // cout << endl;
    cout << ::theNext::count(map, ans) << endl;
    cout << ::theNext::main([&ans, &map]() {
        ans = ::theNext::baoli(map, true);
    }) << '\t';
    // for(auto i : ans) {
    //     cout << i << '\t';
    // }
    // cout << endl;
    cout << ::theNext::count(map, ans) << endl;
    return 0;
}
