#include <iostream>
#include <algorithm>
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
    cin >> max_weight >> n;
    vector<pair<int, int> > things;
    things.reserve(n);
    while(n--) {
        int weight, value, num;
        cin >> weight >> value ;
        /**
         * 数据存储为
         * {
         *      位置
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
    init.setFixAndCheckFun(
    [&things, &max_weight](::std::shared_ptr<::theNext::unitBase> &unit) {
        int sum = 0;
        ::theNext::DNA::boolDNA *un = (::theNext::DNA::boolDNA *)unit.get();
        vector < std::pair<int, std::pair<int, int> > > list;
        for(int i = 0; i < un->getDNA().size(); ++i) {
            if(un->getDNA()[i]) {
                sum += things[i].first;
                list.push_back(std::make_pair(i, things[i]));
            }
        }
        std::sort(list.begin(), list.end(), [](std::pair<int, std::pair<int, int> >a, std::pair<int, std::pair<int, int> >b) {
            return (a.second.second * 1.0 / a.second.first) < (b.second.second * 1.0 / b.second.first);
        });
        for(int i = 0; i < list.size(); ++i) {
            un->getDNA()[list[i].first] = false;
            sum -= list[i].second.first;
            if(sum <= max_weight) {
                return true;
            }
        }
        return false;
    }, [&things, &max_weight](::std::shared_ptr<::theNext::unitBase> &unit) {
        int sum = 0;
        ::theNext::DNA::boolDNA *un = (::theNext::DNA::boolDNA *)unit.get();
        for(int i = 0; i < un->getDNA().size(); ++i) {
            if(un->getDNA()[i]) {
                sum += things[i].first;
                if(sum > max_weight) {
                    return false;
                }
            }
        }
        return true;
    });
    auto adapt = [&things, &max_weight](const ::theNext::DNA::boolDNA & dna) {
        int value = 0, weight = 0;
        for(int i = 0; i < things.size(); ++i) {
            if(dna.getDNA().at(i)) {
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
            cout << "yes" << endl;
        } else {
            cout << "no" << endl;
        }
    }
    cout << max_adapt << endl;
}