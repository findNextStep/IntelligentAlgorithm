#include <iostream>
#include "TSP/TSPtool.hpp"
#include "test_tool.hpp"
#include "antGroup/antGroup.hpp"

using namespace std;

vector<int> antTSP(const vector<vector<int> > &map, const bool big = false) {
    ::theNext::antGroup init(map, 10, big);
    const int n = 10;
    vector<int> best;
    int best_count = 0;
    if(!big) {
        best_count = map.size() * ::theNext::maxCost;
    }
    for(int i = 0; i < n; ++i) {
        init.setMessagePower(4.0 * i / n);
        init.setPathPower(4.0 * (n - i) / n);
        // cout << a << endl;
        // char d;
        // cin >> d;
        auto path = init.loopOnce();
        int count = ::theNext::count(map, path);
        if((big && best_count < count) || (!big && best_count > count)) {
            best_count = count;
            best = path;
        }
        // for(auto i : path) {
        //     cout << i << '\t';
        // }
        // cout << theNext::count(map, path) << endl;
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
            ans = antTSP(map, true);
        }) << "\t";
        // for(auto i : ans) {
        //     cout << i << '\t';
        // }
        // cout << endl;
        cout << ::theNext::count(map, ans) << "\t" << ::std::flush;

        cout << ::theNext::main([&ans, &map]() {
            ans = ::theNext::tx(map, true);
        }) << '\t';
        // for(auto i : ans) {
        //     cout << i << '\t';
        // }
        // cout << endl;
        cout << ::theNext::count(map, ans) << endl;
    }
    return 0;
}