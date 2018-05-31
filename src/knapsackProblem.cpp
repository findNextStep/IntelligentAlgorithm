#include <iostream>
#include <math.h> // max
#include <algorithm> // std::sort
#include "test_tool.hpp"
#include "genetic/DNA/DNABool.hpp"
#include "genetic/unitVectorMulti.hpp"
#include <utility> // std::pair
#include <vector>
using ::std::cin;
using ::std::cout;
using ::std::endl;
using ::std::vector;
using ::std::pair;

vector<bool> test(const vector<pair<int, int> > &things, int max_weight) {
    vector<std::shared_ptr<::theNext::unitBase> > firstGroup;
    srand((unsigned)time(NULL));
    for(int i = 0; i < things.size(); ++i) {
        // vector<bool> dna(things.size());
        // dna.at(i % things.size());
        // firstGroup.push_back(::std::make_shared<::theNext::DNA::boolDNA>(dna));
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
        if(sum == 0) {
            un->getDNA()[rand() % things.size()] = true;
        } else {
            std::sort(list.begin(), list.end(), [](std::pair<int, std::pair<int, int> >a, std::pair<int, std::pair<int, int> >b) {
                return (a.second.second * 1.0 / a.second.first) < (b.second.second * 1.0 / b.second.first);
            });
            for (int i=0;i<list.size();++i){
                sum -= list[i].second.first;
                un->getDNA()[list[i].first] = false;
                if (sum <= max_weight){
                    return true;
                }
            }
            return false;
        }
        return true;
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
        if(sum) {
            return true;
        } else {
            return false;
        }
    });
    auto adapt = [&things, &max_weight](const ::theNext::DNA::boolDNA & dna) {
        int value = 0, weight = 0;
        for(int i = 0; i < dna.getDNA().size(); ++i) {
            if(dna.getDNA().at(i)) {
                weight += things[i].first;
                value += things[i].second;
            }
        }
        if(weight <= max_weight) {
            // cout << value << endl;
            return value * 1.0 + 1.0;
        } else {
            // cout << 1.0 << endl;
            return 1.0;
        }
    };
    // 设置适应值计算函数，以背包内总价值为适应值
    init.setAdaptFun(adapt);
    for(int i = 0; i < 6; ++i) {
        init = init.increase();
        init.eliminate();
    }
    auto group = init.getGroup();
    double max_adapt = 0.0;
    ::std::shared_ptr<::theNext::unitBase> best = nullptr;
    for(int i = 0; i < group.size(); ++i) {
        double ada = adapt(*((::theNext::DNA::boolDNA *)group[i].get()));
        if(ada > max_adapt) {
            max_adapt = ada;
            best = group[i];
        }
    }
    return ((::theNext::DNA::boolDNA *)best.get())->getDNA();

}
int dp(int w, int i, vector<vector<int> > &max_value, const vector<pair<int, int> > &things) {
    if(w < 0 || i < 0) {
        return 0;
    }
    if(max_value[w][i] == 0) {
        return max_value[w][i] = ::std::max(dp(w - things[i].first, i - 1, max_value, things) + things[i].second,
                                            dp(w, i - 1, max_value, things));
    }
    return max_value[w][i];
}
vector<bool> dp(const vector<pair<int, int> > &things, int max_weight) {
    // max_value[w][i] 在总重量为w物品i个的时候的最大价值
    vector<vector<int> > max_value(max_weight + 1);
    for(auto it = max_value.begin(); it != max_value.end(); ++it) {
        it->resize(things.size());
        // for (auto i = it->begin();i != it->end();++i){
        //     *i = 0;
        // }
    }
    dp(max_weight, things.size() - 1, max_value, things);
    int w = max_weight, i = things.size() - 1;
    vector<bool> result(things.size());
    // for(auto list : max_value) {
    //     for(auto i : list) {
    //         cout << i << "\t";
    //     }
    //     cout << endl;
    // }
    while(w > 0 && i > 0) {
        if(max_value[w][i] == max_value[w][i - 1]) {
            // 没有捡起这个
            result[i] = false;
            --i;
        } else {
            result[i] = true;
            w -= things[i].first;
            --i;
        }
    }
    return result;
}
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
    // auto result = dp(things, max_weight);
    // int sum = 0, wei = 0;
    // for(int i = 0; i < result.size(); ++i) {
    //     if(result[i]) {
    //         cout <<i<< "\tyes" << endl;
    //         sum += things[i].second;
    //         wei += things[i].first;
    //     } else {
    //         cout <<i<< "\tno" << endl;
    //     }
    // }
    // cout << wei << ',' << sum << endl;
    auto a = theNext::main([&max_weight, &things]() {
        auto result = dp(things, max_weight);
        int sum = 0, wei = 0;
        for(int i = 0; i < result.size(); ++i) {
            if(result[i]) {
                // cout <<i<< "\tyes" << endl;
                sum += things[i].second;
                wei += things[i].first;
            } else {
                // cout <<i<< "\tno" << endl;
            }
        }
        cout << wei << ',' << sum << endl;
    });
    auto b = theNext::main([&max_weight, &things]() {
        auto result = test(things, max_weight);
        int sum = 0, wei = 0;
        for(int i = 0; i < result.size(); ++i) {
            if(result[i]) {
                // cout <<i<< "\tyes" << endl;
                sum += things[i].second;
                wei += things[i].first;
            } else {
                // cout <<i<< "\tno" << endl;
            }
        }
        cout << wei << ',' << sum << endl;
    });
    cout << a << ":" << b << endl;
    return 0;
}
