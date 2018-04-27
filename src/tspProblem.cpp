#include "DNA/DNAtsp.hpp"
#include "unitVectorMulti.hpp"
#include "test_tool.hpp"
#include <iostream>
#include <algorithm>  // 全排列函数
#include <vector>
using namespace std;
using namespace theNext;
const int maxCost = 256;
int getRandom(int max) {
    return (rand() % max) + 1;
}
int count(vector<vector<int> > &cost_map, vector<int>path) {
    int sum = 0;
    for(int i = 1; i < path.size(); ++i) {
        sum += cost_map[path[i - 1]][path[i]];
    }
    return sum + cost_map[path[path.size() - 1]][path[0]];
}
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
vector<int> tx(vector<vector<int>>d) {
    vector<int>S(d.size());
    int i = 0, k, l, j, sum = 0;
    do {
        int k = 1;
        int Dtemp = 10000;
        do {
            int l = 0;
            int flag = 0;
            do {
                if(S[l] == k) { //判断该城市是否已被访问过，若被访问过，
                    flag = 1;//则flag为1
                    break;//跳出循环，不参与距离的比较
                } else {
                    l++;
                }
            } while(l < i);
            if(flag == 0 && d[k][S[i - 1]] < Dtemp) { /*D[k][S[i - 1]]表示当前未被访问的城市k与上一个已访问过的城市i-1之间的距离*/
                j = k;//j用于存储已访问过的城市k
                Dtemp = d[k][S[i - 1]];//Dtemp用于暂时存储当前最小路径的值
            }
            k++;
        } while(k < d.size());
        S[i] = j;//将已访问过的城市j存入到S[i]中
        i++;
        sum += Dtemp;//求出各城市之间的最短距离，注意：在结束循环时，该旅行商尚未回到原出发的城市
    } while(i < d.size());
    return S;
}

vector<int> baoli(vector<vector<int> > map, bool big = false) {
    vector<int> pl(map.size());
    for(int i = 0; i < map.size(); ++i) {
        pl[i] = i;
    }
    vector<int> best(map.size()), worse(map.size());
    int bestcount = -1, worsecount = maxCost * map.size();
    do {
        int now = count(map, pl);
        if(big) {
            if(now > bestcount) {
                bestcount = now;
                std::copy(pl.begin(), pl.end(), best.begin());
            }
        } else {
            if(now < worsecount) {
                worsecount = now;
                std::copy(pl.begin(), pl.end(), worse.begin());
            }
        }
        if(pl[0] != 0) {
            break;
        }
    } while(::std::next_permutation(pl.begin(), pl.end()));
    if(big) {
        return best;
    } else {
        return worse;
    }
}
int main() {
    int problemSize = 100;
    while(cin >> problemSize) {
        vector<vector<int>> cost_map(problemSize);
        for(auto it = cost_map.begin(); it != cost_map.end(); ++it) {
            it->resize(problemSize);
            for(auto i = it->begin(); i != it->end(); ++i) {
                auto base = getRandom(maxCost);
                *i = base;// * base * base;
            }
        }
        for(int i = 0; i < cost_map.size(); ++i) {
            cost_map[i][i] = 0;
        }
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