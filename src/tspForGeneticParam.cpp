#include "genetic/unit.hpp"
#include "genetic/DNA/DNAtsp.hpp"
#include "TSP/TSPtool.hpp"
#include "test_tool.hpp"
#include <vector>
#include <iostream>
using namespace std;
vector<int> test(vector<vector<int>>cost_map, int groupSize, int increaseTimes) {
    vector<std::shared_ptr<::theNext::unitBase> > firstGroup;
    for(int i = 0; i < groupSize; ++i) {
        firstGroup.push_back(::theNext::DNA::tspDNA::makeRandom(cost_map.size()));
    }
    ::theNext::DNA::tspDNA::setMap(cost_map);
    ::theNext::unitVector<::theNext::DNA::tspDNA> init(firstGroup);
    init.setOverlapRate(1.0);
    init.setVariationRate(0.2);
    auto adapt = [&cost_map](const ::theNext::DNA::tspDNA & dna) {
        auto diff = ::theNext::count(cost_map, dna.getDNA());
        return  1.0 / diff;
    };
    init.setAdaptFun(adapt);
    for(int i = 0; i < increaseTimes; ++i) {
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
    int n;
    while(cin >> n) {
        auto map = ::theNext::make_map_random(n);
        vector<vector<double> > ansTable(4 * n);
        for(int i = 1; i < 4 * n; ++i) {
            for(int j = 4; j < 4 * n; ++j) {
                cout << ::theNext::main([i, j, &ansTable, &map]() {
                    ansTable[i].push_back(1.0 *::theNext::count(map, test(map, i, j)));
                }) << '\t';
            }
            cout << endl;
        }
        for(auto line : ansTable) {
            for(auto i : line) {
                cout << i << '\t';
            }
            cout << endl;
        }
    }
}