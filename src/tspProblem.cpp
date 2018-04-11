#include "DNA/DNAtsp.hpp"
#include "test_tool.hpp"
#include <iostream>
#include <vector>
using namespace std;
using namespace theNext;
vector<int> test(vector<vector<int>>cost_map) {
    srand((unsigned)time(NULL));
    vector<std::shared_ptr<::theNext::unitBase> > firstGroup;
    for(int i = 0; i < cost_map.size(); ++i) {
        firstGroup.push_back(::theNext::DNA::tspDNA::makeRandom(cost_map.size()));
    }
    unitVector<DNA::tspDNA> init(firstGroup);

    auto adapt = [&cost_map](const ::theNext::DNA::tspDNA & dna) {
        int sum = 0;
        for(int i = 0; i < cost_map.size(); ++i) {
            sum += cost_map[i][dna.getDNA()[i]];
        }
        return sum;
    };

    init.setAdaptFun(adapt);
    for(int i = 0; i < 6; ++i) {
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
int getRandom(int max) {
    return rand() % max + 1;
}
int main() {
    const int problemSize = 10;
    vector<vector<int>> cost_map(problemSize);
    for(auto it = cost_map.begin(); it != cost_map.end(); ++it) {
        it->resize(problemSize);
        for(auto i = it->begin(); i != it->end(); ++i) {
            *i = getRandom(255);
        }
    }
    for(int i = 0; i < cost_map.size(); ++i) {
        cost_map[i][i] = 0;
    }
    for(auto line : cost_map) {
        for(auto i : line) {
            cout << i << "\t";
        }
        cout << endl;
    }
    auto time = ::theNext::main([&cost_map]() {
        auto result = test(cost_map);
        for(auto i : result) {
            cout << i << "\t";
        }
        cout << endl;
    });
    cout << time << endl;
    return 0;
}