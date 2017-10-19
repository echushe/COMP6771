#ifndef BTREE_SIMULATOR_H
#define BTREE_SIMULATOR_H

#include <set>
#include <cassert>

#include "btree.h"

template <typename T>
struct simulator {
    std::set<T> s;
    btree<T> b;

    simulator(size_t max=40): b{max} {}

    void insert(const T& val) {
        auto bres = b.insert(val);
        auto sres = s.insert(val);
        if (bres.second != sres.second)
            std::cerr << "Inserting " << val << " failed. btree produced" << bres.second <<
                      ", set produced " << sres.second << std::endl;
        assert(*bres.first == val);
    }

    void find(const T& val) {
        typename btree<T>::const_iterator bres = b.find(val);
        if (s.find(val) == s.end())
            assert(bres == b.end());
        else
            assert(*bres == val);
        typename btree<T>::iterator bresnonconst = b.find(val);
        if (s.find(val) == s.end())
            assert(bresnonconst == b.end());
        else {
            assert(*bresnonconst == val);
            *bresnonconst = val;
        }
    }

    void assertEmpty() {
        assert(s.empty()); // if this fails my tests are fucked
        assert(b.begin() == b.end());
    }

    void checkEqual() {
        typename btree<T>::const_iterator biter = b.cbegin();
        typename btree<T>::const_iterator bpostfix = b.cbegin();
        auto siter = s.cbegin();
        while (siter != s.cend()) {
            assert(*siter == *biter);
            assert(*siter == *bpostfix++);
            ++biter;
            ++siter;
        }
        assert(biter == b.cend());
        assert(bpostfix == b.cend());
        assert(biter == bpostfix);
    }

    void checkIterators() {
        checkEqual(); // const forward
        { // normal forward
            typename btree<T>::iterator biter = b.begin();
            typename btree<T>::iterator bpostfix = b.begin();
            auto siter = s.begin();
            while (siter != s.end()) {
                assert(*siter == *biter);
                assert(*siter == *bpostfix++);
                ++biter;
                ++siter;
            }
            assert(biter == b.end());
            assert(bpostfix == b.end());
            assert(biter == bpostfix);
            if (siter != s.begin())
                --bpostfix;
            while (siter != s.begin()) {
                --siter;
                --biter;
                assert(bpostfix == b.begin() || *bpostfix-- == *siter);
                assert(*siter == *biter);
            }
        }

        { // const reverse
            auto biter = b.crbegin();
            auto bpostfix = b.crbegin();
            auto siter = s.crbegin();
            while (siter != s.crend()) {
                assert(*siter == *biter);
                assert(*siter == *bpostfix++);
                ++biter;
                ++siter;
            }
            assert(biter == b.crend());
            assert(bpostfix == b.crend());
            assert(biter == bpostfix);
            if (siter != s.crbegin())
                --bpostfix;
            while (siter != s.crbegin()) {
                --siter;
                --biter;
                assert(bpostfix == b.crbegin() || *bpostfix-- == *siter);
                assert(*siter == *biter);
            }
        }

        { // normal reverse
            auto biter = b.rbegin();
            auto bpostfix = b.rbegin();
            auto siter = s.rbegin();
            while (siter != s.rend()) {
                assert(*siter == *biter);
                assert(*siter == *bpostfix++);
                ++biter;
                ++siter;
            }
            assert(biter == b.rend());
            assert(bpostfix == b.rend());
            assert(biter == bpostfix);
            if (siter != s.crbegin())
                --bpostfix;
            while (siter != s.crbegin()) {
                --siter;
                --biter;
                assert(bpostfix == b.rbegin() || *bpostfix-- == *siter);
                assert(*siter == *biter);
            }
        }
    }
};

#endif //BTREE_SIMULATOR_H
