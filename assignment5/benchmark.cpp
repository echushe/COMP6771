#include <iostream>
#include <random>
#include <thread>
#include <cassert>
#include <algorithm>
#include <chrono>
#include <string>
#include <fstream>

#include "BucketSort.h"

constexpr auto numreps = 1U;
constexpr auto totalNumbers = 1000000U;

int main() {

    std::mt19937 mt(std::random_device{}());
    std::uniform_int_distribution<unsigned int> dist(1, std::numeric_limits<unsigned int>::max());
    std::uniform_real_distribution<float> floatdist(std::numeric_limits<float>::min(), std::numeric_limits<float>::max());

    std::vector<std::pair<std::function<unsigned int()>, std::string>> dataSets = {
        {[&] { return dist(mt); }, "Uniform random distribution"},
        {[&] { return (dist(mt) / 1000) * 1000; }, "Common value distribution"},
//        {[&] { return reinterpret_cast<unsigned int>(floatdist(mt)); }, "Extreme distribution"},
//        {[] { return 0; }, "All zeros"},
//        {[] { return std::numeric_limits<unsigned int>::max(); }, "All max int"},
//        {[] {
//            static unsigned int upto = 0U;
//            return ++upto;
//        }, "Monotomically increasing"},
    };

    const unsigned int numCores = std::thread::hardware_concurrency();

    std::ofstream results("results.csv");
    results << totalNumbers << '\n';
    for (const auto &dataset : dataSets) {
        const auto &generator = dataset.first;
        const auto &desc = dataset.second;

        BucketSort b;
        std::vector<unsigned int> data;
        // insert random numbers into the sort object
        for (unsigned int i=0; i < totalNumbers; ++i) {
            data.push_back(generator());
        }
        b.numbersToSort = data;
        b.sort(numCores); // ensure the cache is fair for each test

        // potentially could do i *= 2, not ++i
        for (auto currentCores = 1U; currentCores <= numCores; ++currentCores) {
            std::cout << "Testing " << desc << " with " << currentCores << " core(s)" << std::endl;
            results << desc << ',' << currentCores;
            for (auto i = 0U; i < numreps; ++i) {
                BucketSort b;
                b.numbersToSort = data;

                auto start = std::chrono::high_resolution_clock::now();
                b.sort(currentCores);
                auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::high_resolution_clock::now() - start
                );
                results << ',' << ms.count();
            }
            results << std::endl;
        }
    }
}
