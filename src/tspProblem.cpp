#include "DNA/DNAtsp.hpp"
#include "test_tool.hpp"
#include <iostream>
#include <vector>
using namespace std;
using namespace theNext;
vector<int> test(vector<vector<int>>cost_map) {
    srand((unsigned)time(NULL));
    vector<std::shared_ptr<::theNext::unitBase> > firstGroup;
    for(int i = 0; i < 12; ++i) {
        firstGroup.push_back(::theNext::DNA::tspDNA::makeRandom(cost_map.size()));
    }
    unitVector<DNA::tspDNA> init(firstGroup);

    auto adapt = [&cost_map](const ::theNext::DNA::tspDNA & dna) {
        int sum = 0;
        for(int i = 1; i < cost_map.size(); ++i) {
            sum += cost_map[dna.getDNA()[i - 1]][dna.getDNA()[i]];
        }
        sum += cost_map[dna.getDNA()[cost_map.size() - 1]][0];
        return 1.0 / sum;
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
int getRandom(int max) {
    return rand() % max + 1;
}
int count (vector<vector<int> >&cost_map,vector<int>path){
    int sum =0;
    for (int i=1;i<path.size();++i){
        sum += cost_map[path[i-1]][path[i]];
    }
    return sum + cost_map[path.size()-1][0];
}
int main() {
    const int problemSize = 200;
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
    vector<int> ans_test, ans_tx;
    auto time_test = ::theNext::main([&cost_map, &ans_test]() {
        ans_test = test(cost_map);
    });
    auto time_tx = ::theNext::main([&cost_map, &ans_tx](){
        ans_tx = tx(cost_map);
    });
    cout << count(cost_map,ans_test) << " : " << count(cost_map,ans_tx) << endl; 
    cout << time_test <<" : " << time_tx << endl;
    return 0;
}