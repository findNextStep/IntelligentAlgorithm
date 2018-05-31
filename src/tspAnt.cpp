#include <iostream>
#include "TSP/TSPtool.hpp"
#include "test_tool.hpp"
#include "antGroup/antGroup.hpp"
#include "tabu/tubu.hpp"
#include "annealing/annealing.hpp"
#include "genetic/unitVectorMulti.hpp"
#include "genetic/DNA/DNAtsp.hpp"
using namespace std;

/**
 * @brief 通过模拟退火算法求解TSP问题
 *
 * 计算步数为地图宽度
 * @param map 估值地图
 * @param big 是否求取最大值
 * @param complex 是否采用精细求解
 * @return vector<int> 求取的最优解
 */
vector<int> annealing(const vector<vector<int> > &map, bool big = false, bool complex = false) {
    ::theNext::annealing<vector<int> > init;
    for(int i = 0; i < map.size() * 10; ++i) {
        init.addunit(::theNext::make_randowm_path(map.size()));
    }
    // 设置适应值函数
    init.setAdaptFun([&map, &big](auto path) {
        if(big) {
            return 1.0 * ::theNext::count(map, path);
        } else {
            return 1.0 / ::theNext::count(map, path);
        }
    });

    // 添加操作参数
    if(complex) {
        // 如果选择精细求解,输入全部可以执行的操作
        for(int i = 1; i < map.size(); ++i) {
            for(int j = 0; j < i; ++j)
                init.addOperator([i, j, &map](auto start) {
                std::swap(start[j], start[i]);
                return start;
            });
        }
    } else {
        for(int i = 0; i < map.size(); ++i) {
            init.addOperator([i, &map](auto path) {
                if(i) {
                    std::swap(path[i - 1], path[i]);
                } else {
                    std::swap(path[0], path[map.size() - 1]);
                }
                return path;
            });
        }
    }

    // 逐代迭代
    double temperature = 50;
    while(temperature > 0.5) {
        // 每次降温到原来的95%
        temperature = temperature * 0.95;
        init.setTemperature(temperature);
        init.SAoperation();
    }

    int best = 0;
    if(!big) {
        best = map.size() * ::theNext::maxCost;
    }
    vector<int > best_unit(map.size());
    // 在最终的所有个体中选择最优个体
    for(auto unit : init.getunits()) {
        int ada = ::theNext::count(map, unit);
        if(((best > ada) && !big) || ((best < ada) && big)) {
            best = ada;
            best_unit = unit;
        }
    }
    return best_unit;
}

/**
 * @brief 通过禁忌搜索算法求取tsp问题
 * 禁忌表长度为地图宽度的$\frac14$
 * 计算步数为地图宽度
 * @param map 估值地图
 * @param big 是否求取最大值
 * @param complex 是否采用精细求解
 * @return vector<int> 求取的最优解
 */
vector<int> testtubu(const vector<vector<int>> &map, bool big = false, bool complex = false) {
    ::theNext::tubeHelper<vector<int>> tube(map.size() / 4);
    tube.setAdaptFun([&big, &map](const vector<int> &path) {
        if(big) {
            return ::theNext::count(map, path);
        } else {
            return (int)map.size() * theNext::maxCost - ::theNext::count(map, path);
        }
    }).setCmp([](auto a, auto b) {
        for(int i = 0; i < a.size(); ++i) {
            if(a[i] != b[i]) {
                return false;
            }
        }
        return true;
    });
    if(complex) {
        for(int i = 1; i < map.size(); ++i) {
            for(int j = 0; j < i; ++j)
                tube.addOperator([i, j, &map](auto start) {
                std::swap(start[j], start[i]);
                return start;
            });
        }
    } else {
        for(int i = 0; i < map.size(); ++i) {
            tube.addOperator([i, &map](auto path) {
                if(i) {
                    std::swap(path[i - 1], path[i]);
                } else {
                    std::swap(path[0], path[map.size() - 1]);
                }
                return path;
            });
        }
    }
    // vector<int> init = ::theNext::make_init_small(map,big);
    vector<int> init(map.size());
    for(int i = 0; i < init.size(); ++i) {
        init[i] = i;
    }
    vector<int> best = init;
    tube.setNow(init);
    int c = 0;
    for(int i = 0; i < map.size(); ++i) {
        if(!tube.nextStep()) {
            cout << "???" << i << endl;
            break;
        }
        if(big) {
            if(::theNext::count(map, best) < ::theNext::count(map, tube.getNow())) {
                c = i;
                best = tube.getNow();
            }
        } else {
            if(::theNext::count(map, best) > ::theNext::count(map, tube.getNow())) {
                c = i;
                best = tube.getNow();
            }
        }
    }
    return best;
}

vector<int> test(vector<vector<int>>cost_map) {
    srand((unsigned)time(NULL));
    vector<std::shared_ptr<::theNext::unitBase> > firstGroup;
    for(int i = 0; i < 50; ++i) {
        firstGroup.push_back(::theNext::DNA::tspDNA::makeRandom(cost_map.size()));
    }
    ::theNext::DNA::tspDNA::setMap(cost_map);
    ::theNext::unitVectorMulti<::theNext::DNA::tspDNA> init(firstGroup);
    init.setOverlapRate(1.0);
    init.setVariationRate(0.2);
    auto adapt = [&cost_map](const ::theNext::DNA::tspDNA & dna) {
        auto diff = ::theNext::count(cost_map, dna.getDNA());
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

vector<int> antTSP(const vector<vector<int> > &map, const bool big = false, int group = 10, int times = 10) {
    ::theNext::antGroup init(map, group, big);
    const int n = times;
    vector<int> best;
    int best_count = 0;
    if(!big) {
        best_count = map.size() * ::theNext::maxCost;
    }
    for(int i = 0; i < n; ++i) {
        init.setMessagePower(4.0 * i / n);
        init.setPathPower(4.0 * (n - i) / n);
        auto path = init.loopOnce();
        int count = ::theNext::count(map, path);
        if((big && best_count < count) || (!big && best_count > count)) {
            best_count = count;
            best = path;
        }
    }
    return best;
}


int smain() {
    auto map = ::theNext::make_map_random(25);
    vector<vector<double > > times;
    for(int i = 3; i < 50; ++i) {
        times.resize(times.size() + 1);
        for(int j = 3; j < 30; ++j) {
            vector<int> ans;
            times[times.size() - 1].push_back(::theNext::main([i, j, &ans, &map]() {
                ans = antTSP(map, true, i, j);
            }));
            cout << ::theNext::count(map, ans) << "\t" << ::std::flush;
        }
        cout << endl;
    }
    cout << endl;
    for(auto line : times) {
        for(auto t : line) {
            cout << t << "\t";
        }
        cout << endl;
    }
    return 0;
}


int main() {
    int n;
    while(cin >> n) {
        cout << n << "\t";
        auto map = ::theNext::make_map_random(n);
        vector<int> ans;

        cout << ::theNext::main([&ans, &map]() {
            ans = test(map);
        }) << "\t";
        // for(auto i : ans) {
        //     cout << i << '\t';
        // }
        // cout << endl;
        cout << ::theNext::count(map, ans) << "\t" << ::std::flush;

        cout << ::theNext::main([&ans, &map]() {
            ans = testtubu(map, false);
        }) << "\t";
        // for(auto i : ans) {
        //     cout << i << '\t';
        // }
        // cout << endl;
        cout << ::theNext::count(map, ans) << "\t" << ::std::flush;

        cout << ::theNext::main([&ans, &map]() {
            ans = annealing(map, false);
        }) << "\t";
        // for(auto i : ans) {
        //     cout << i << '\t';
        // }
        // cout << endl;
        cout << ::theNext::count(map, ans) << "\t" << ::std::flush;

        cout << ::theNext::main([&ans, &map]() {
            ans = antTSP(map, false);
        }) << "\t";
        // for(auto i : ans) {
        //     cout << i << '\t';
        // }
        // cout << endl;
        cout << ::theNext::count(map, ans) << "\t" << ::std::flush;

        cout << ::theNext::main([&ans, &map]() {
            ans = ::theNext::tx(map, false);
        }) << '\t';
        // for(auto i : ans) {
        //     cout << i << '\t';
        // }
        // cout << endl;
        cout << ::theNext::count(map, ans) << endl;
    }
    return 0;
}
int mains() {
    int n;
    cin >> n;
    vector<int> x(n), y(n);
    for(int i = 0; i < n; ++i) {
        cin >> x[i];
    }
    for(int i = 0; i < n; ++i) {
        cin >> y[i];
    }
    vector<vector<int> > map(n);
    for(int i = 0; i < n; ++i) {
        map[i].resize(n);
        for(int j = 0; j < n; ++j) {
            map[i][j] = sqrt(pow(x[i] - x[j], 2) + pow(y[i] - y[j], 2));
        }
    }
    cout << theNext::count(map,test(map)) << endl;
}