#pragma once
#include "unit.hpp"
#include <vector>
namespace theNext {
namespace DNA {
class tspDNA: public unitBase {
public:
    typedef ::std::vector<int> DNA_t;
    typedef tspDNA unit_t;
    tspDNA(int size): path(size) {
        for(int i = 0; i < size; ++i) {
            path[i] = i;
        }
    }
    void variation(rate_t rate) {
        int l = rand() % path.size(),
            r = rand() % path.size();
        while(l == r) {
            r = rand() % path.size();
        }
        auto tmp = path[l];
        path[l] = path[r];
        path[r] = tmp;
    }

    void overlap(const ::std::shared_ptr<unitBase> &base) {
        DNA_t others_dna(((unit_t *)base.get())->getDNA());
        int i = rand() % path.size(), v;
        do {
            v = path[i];
            path[i] = others_dna[i];
            if(findI(path, v) == -1) {
                i = findI(others_dna, v);
            } else {
                break;
            }
        } while(true);
    }
    ::std::shared_ptr<unitBase> copy() {
        ::std::shared_ptr<unitBase> ptr(new tspDNA(this->path.size()));
        return ptr;
    }
    std::vector<int> &getDNA() {
        return this->path;
    }
    const std::vector<int> &getDNA()const {
        return this->path;
    }
    static ::std::shared_ptr<unitBase> makeRandom(int size) {
        auto t = new tspDNA(size);
        for(int i = 2; i < t->path.size(); ++i) {
            // 使i与之前某个量交换
            int k = i - rand() % i;
            std::swap(t->path.at(i), t->path.at(k));
        }
        ::std::shared_ptr<unitBase> ans(t);
        return ans;
    }
protected:
    static int findI(::std::vector<int> &contain, int value) {
        for(int i = 0; i < contain.size(); ++i) {
            if(contain.at(i) == value) {
                return i;
            }
        }
        return -1;
    }
private:
    std::vector<int> path;
};

}
}