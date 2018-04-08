#pragma once
#include "unit.hpp"
#include <array>
namespace theNext {
namespace DNA {
class boolDNA : public unitBase {
public:
    typedef ::std::vector<bool> DNA_t;
    typedef boolDNA unit_t;
    boolDNA(const DNA_t &DNA) : DNA(DNA) {}
    static ::std::shared_ptr<boolDNA> makeRandomDNA(int length) {
        DNA_t dna(length);
        for(int i = 0; i < length; ++i) {
            dna[i] = rand() % 2;
        }
        return ::std::make_shared<boolDNA>(dna);
    }

    void variation(rate_t rate) {
        srand((unsigned)time(NULL));
        for(int i = 0; i < this->DNA.size(); ++i) {
            rate_t luck = rand() / rate_t(RAND_MAX);
            if(luck <= rate) {
                this->DNA[i] = !this->DNA[i];
            }
        }
    }

    void overlap(const ::std::shared_ptr<unitBase> &base) {
        DNA_t others_dna(((unit_t *)base.get())->getDNA());
        if(rand() % 2) {
            for(int i = 0; i < this->DNA.size() / 2; ++i) {
                this->DNA[i] = others_dna[i];
            }
        } else {
            for(int i = this->DNA.size() / 2 + 1; i < this->DNA.size(); ++i) {
                this->DNA[i] = others_dna[i];
            }
        }
    }

    ::std::shared_ptr<unitBase> copy() {
        ::std::shared_ptr<unitBase> ptr(new boolDNA(this->DNA));
        return ptr;
    }
    DNA_t &getDNA() {
        return DNA;
    } 
    const DNA_t &getDNA() const {
        return DNA;
    }
private:
    DNA_t DNA;
};
}
}