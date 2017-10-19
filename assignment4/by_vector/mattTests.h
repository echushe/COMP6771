#ifndef BTREE_INSERTTESTER_H
#define BTREE_INSERTTESTER_H

#include <functional>
#include <random>
#include <set>
#include <utility>
#include <limits>
#include <array>

#include "simulator.h"

namespace {

constexpr bool check_every_iteration = false;

std::mt19937 *rng;

template<unsigned int min = 0, unsigned int max = std::numeric_limits<unsigned int>::max()>
unsigned int randint() {
    static auto dist = std::uniform_int_distribution<unsigned int>(min, max);
    return dist(*rng);
}

std::vector<std::function<void()>> makeFns(std::vector<std::pair<unsigned int, std::function<void()>>> fns) {
    std::vector<std::function<void()>> out;
    for (auto p : fns) {
        for (auto i = 0U; i < p.first; ++i)
            out.push_back(p.second);
    }
    return out;
}

template<typename T>
void runTests(std::function<T()> generator, size_t nOps, size_t max) {
    std::vector<simulator<T>> s(20);
    size_t last;
    auto getSim = [&]() -> simulator<T> & {
        last = randint<>() % s.size();
        return s.at(last);
    };
    auto fns = makeFns({
        // construction and deletion
        {5, [&]() { s.emplace_back(randint<1, 100>()); }},
        {15, [&]() { s.erase(s.begin() + (randint<>() % s.size())); }},

        // move and copy constructors / assignment
        {5, [&]() { s.emplace_back(getSim()); }},
        {5, [&]() {
            s.emplace_back(std::move(getSim()));
            s[last].assertEmpty();
        }},
        {20, [&]() { getSim() = getSim(); }},
        {10, [&]() {
            auto base = randint<>() % s.size();
            auto add = randint<>() % (s.size() - 1) + 1;
            s[base] = std::move(s[(base + add) % s.size()]);
            s[(base + add) % s.size()].assertEmpty();
        }},

        // normal operations
        {1000, [&]() { getSim().insert(generator()); }},
        {1000, [&]() { getSim().find(generator()); }},

        // correctness / iterator checking
        {20, [&]() { getSim().checkEqual(); }},
        {1, [&]() { getSim().checkIterators(); }},
    });
    if (check_every_iteration)
        s[last % s.size()].checkEqual();
    auto dist = std::uniform_int_distribution<unsigned int>(0, fns.size() - 1);
    std::array<unsigned int, 1000> ops;
    for (auto i = 0U; i < nOps; ++i) {
        if (s.size() < 5)
            s.push_back(simulator<T>(randint<1, 100>()));
        ops[i % ops.size()] = (dist(*rng));
        fns[ops[i % ops.size()]]();
    }
}

void mattTests(std::mt19937 rng) {
    ::rng = &rng;

    auto uintgenerator = []() { return randint<0, 1000>(); };
    runTests<unsigned int>(uintgenerator, 1000000, 40);
}

}

#endif //BTREE_INSERTTESTER_H
