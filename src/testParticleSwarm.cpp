#include "ParticleSwarm/particleSwarm.hpp"
#include <iostream>
using namespace std;
int main() {
    theNext::paricleSwarm<5> a([](const auto a) {
        double result = 0.0;

        for(int i = 0; i < a.size() - 1; ++i) {
            result += 100 *
                      (a[i + 1] - a[i] * a[i]) *
                      (a[i + 1] - a[i] * a[i]) +

                      (a[i] - 1) *
                      (a[i] - 1);
        }
        return 1.0 / result;
    });
    a.setNumberRange(30, -30);
    a.setSpeedRange(5, -5);
    a.setGlobalBestPower(2);
    a.setItemBestPower(2);
    a.addNewUnit(5);
    for(int i = 0; i < 1000; ++i) {
        cout << i << endl;
        for(auto item : a.getUnits()) {
            for(auto i : item.position) {
                cout << i << "\t";
            }
            cout << endl;
        }
        a.oneIteration();
    }
    cout << 1.0 /  a.getBest().second << endl;
    cout << "endl" << endl;
}