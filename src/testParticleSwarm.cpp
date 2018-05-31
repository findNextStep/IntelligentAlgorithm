#include "ParticleSwarm/particleSwarm.hpp"
#include <iostream>
using namespace std;
using ::std::cout;
int main() {
    const int num = 5;
    auto test = [](const array<double, 5> &a) {
        double result = 0.0;
        for(int i = 0; i < a.size() - 1; ++i) {
            result += 100 * (a[i + 1] - a[i] * a[i]) * (a[i + 1] - a[i] * a[i]) + (a[i] - 1) * (a[i] - 1);
        }
        return 1.0 / result;
    };
    const int n = 500;
    theNext::paricleSwarm<5> a(test);
    a.setNumberRange(30, -30);
    a.setSpeedRange(5, -5);
    a.setGlobalBestPower(4);
    a.setItemBestPower(4);
    a.addNewUnit(5);
    vector<vector<double> > ans;
    ans.resize(500);
    for(int i = 0; i < n; ++i) {
        // a.setItemBestPower(4.0 * i / n);
        // a.setGlobalBestPower(4.0 * (n - i) / n);

        // cout << i << endl;
        for(auto item : a.getUnits()) {
            auto ps = item.best.first;
            //     for(auto i : ps) {
            //         printf("%03.3lf\t",i);
            //     }
            ans[i].push_back(test(ps));
            //     cout << test(ps);
            //     cout << endl;
        }
        a.oneIteration();
    }
    cout << 1.0 /  a.getBest().second << endl;
    cout << endl;
    for(auto line : ans) {
        auto best = 9999999999999999999999999999999.9;
        for(auto i : line) {
            if(1 / i < best) {
                best = 1 / i;
            }
            // cout << 1 / i << "\t";
        }
        cout << best;
        cout << endl;
    }
    cout << endl;
    for(auto i : a.getBest().first) {
        cout << i << endl;
    }
    cout << "endl" << endl;
    return 0;
}