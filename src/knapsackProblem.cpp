#include <iostream>
#include "DNA/DNABool.hpp"
#include <utility> // std::pair
#include <vector>
using ::std::cin;
using ::std::cout;
using ::std::endl;
using ::std::vector;
using ::std::pair;
int main() {
    int n, max_weight;
    cin >> n >> max_weight;
    vector<pair<int, int> > things;
    things.reserve(n);
    while(n--) {
        int weight, value, num;
        cin >> weight >> value ;
        /**
         * 数据存储为
         * {
         *      数量
         *      {
         *           重量，价值
         *      }
         * }
         */
        things.push_back(std::make_pair(weight, value));
    }
    vector<std::shared_ptr<::theNext::unitBase> > firstGroup;
    srand((unsigned)time(NULL));
    for(int i = 0; i < things.size() * 8; ++i) {
        firstGroup.push_back(::theNext::DNA::boolDNA::makeRandomDNA(things.size()));
    }
    ::theNext::unitVector<::theNext::DNA::boolDNA> init(firstGroup);
    auto adapt = [&things, &max_weight](const ::theNext::DNA::boolDNA & a) {
        int value = 0, weight = 0;
        for(int i = 0; i < things.size(); ++i) {
            if(a.getDNA()[i]) {
                weight += things[i].first;
                value += things[i].second;
            }
        }
        if(weight <= max_weight) {
            return value;
        } else {
            return 1;
        }
    };
    // 设置适应值计算函数，以背包内总价值为适应值
    init.setAdaptFun(adapt);
    for(int i = 0; i < 6; ++i) {
        // cout << init.getGroup().size() << endl;
        init = init.increase();
        // cout << init.getGroup().size() << endl;
        init.eliminate();
        // cout << init.getGroup().size() << endl;
        // cout << "endl"<<endl;
        // cin >> n;
    }
    auto group = init.getGroup();
    double max_adapt = 0.0;
    ::std::shared_ptr<::theNext::unitBase> best = nullptr;
    // cout << "?>??" << group.size() << endl;
    for(int i = 0; i < group.size(); ++i) {
        double ada = adapt(*((::theNext::DNA::boolDNA *)group[i].get()));
        if(ada > max_adapt) {
            max_adapt = ada;
            best = group[i];
        }
    }
    for(int i = 0; i < ((::theNext::DNA::boolDNA *)best.get())->getDNA().size(); ++i) {
        if(((::theNext::DNA::boolDNA *)best.get())->getDNA()[i]) {
            cout << "yes" << " ";
        } else {
            cout << "no" << " ";
        }
    }
    cout << max_adapt <<endl;
}