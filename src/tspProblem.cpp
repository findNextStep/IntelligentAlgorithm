#include "DNA/DNAtsp.hpp"
#include "unitVectorMulti.hpp"
#include "test_tool.hpp"
#include "TSP/TSPtool.hpp"
#include <iostream>
#include <algorithm>  // 全排列函数
#include <vector>
using namespace std;
using namespace theNext;
const int maxCost = 256;
vector<int> test(vector<vector<int>>cost_map) {
    srand((unsigned)time(NULL));
    vector<std::shared_ptr<::theNext::unitBase> > firstGroup;
    for(int i = 0; i < 50; ++i) {
        firstGroup.push_back(::theNext::DNA::tspDNA::makeRandom(cost_map.size()));
    }
    ::theNext::DNA::tspDNA::setMap(cost_map);
    unitVectorMulti<DNA::tspDNA> init(firstGroup);
    init.setOverlapRate(1.0);
    init.setVariationRate(0.2);
    auto adapt = [&cost_map](const ::theNext::DNA::tspDNA & dna) {
        auto diff = count(cost_map, dna.getDNA());
        return  1.0 / diff;
    };
    init.setAdaptFun(adapt);
    for(int i = 0; i < 20; ++i) {

        init = init.increase();
        init.eliminate();
    }
    auto group = init.getGroup();
    double max_adapt = 0.0;
    ::std::shared_ptr<::theNext::unitBase> best = nullptr;
    for(int i = 0; i < group.size(); ++i) {
        double ada = adapt(*((::theNext::DNA::tspDNA *)group[i].get()));
        if(ada > max_adapt) {
            max_adapt = ada;
            best = group[i];
        }
    }
    return ((::theNext::DNA::tspDNA *)best.get())->getDNA();
}

int main() {
    int problemSize = 100;
    while(cin >> problemSize) {
        vector<vector<int>> cost_map = ::theNext::make_map_random(problemSize);
        // for(auto line : cost_map) {
        //     for(auto i : line) {
        //         cout << i << "\t";
        //     }
        //     cout << endl;
        // }
        vector<int> ans_test, ans_tx, ans_bl;
        auto time_test = ::theNext::main([&cost_map, &ans_test]() {
            ans_test = test(cost_map);
        });
        cout << count(cost_map, ans_test) << "\t";//: " << time_test << endl;
        // auto time_tx = ::theNext::main([&cost_map, &ans_tx]() {
        //     ans_tx = tx(cost_map);
        // });
        // cout  << count(cost_map, ans_tx) << "\t";//: " << time_tx << endl;
        auto time_bl = ::theNext::main([&cost_map, &ans_bl]() {
            ans_bl = baoli(cost_map);
        });
        cout << count(cost_map, ans_bl) << "\t";//: " << time_bl << endl;
        time_bl = ::theNext::main([&cost_map, &ans_bl]() {
            ans_bl = baoli(cost_map, true);
        });
        cout << count(cost_map, ans_bl) << "\t";//: " << time_bl << endl;
        cout << endl;
        // for(auto i : ans_test) {
        //     cout << i << "->";
        // }
        // cout << endl;
        // for(auto i : ans_tx) {
        //     cout << i << "->";
        // }
        // cout << endl;
        // cout << time_test << "\t" << time_tx << endl;
    }
    // cout << endl;
    return 0;
}