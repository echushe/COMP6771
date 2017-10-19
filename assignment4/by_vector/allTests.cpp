#include <iostream>
#include <random>

#include "mattTests.h"
#include "suite.h"

int main() {
    auto seed = std::random_device()();
    std::cout << "Running tests with seed " << seed << std::endl;
    std::mt19937 rng(seed);

    std::cout << "Running CSE's tests" << std::endl;
    cseTests(rng);
    std::cout << "Passed CSE's tests" << std::endl;
    std::cout << "Running Cameron's tests" << std::endl;
    cameronTests();
    std::cout << "Passed Cameron's tests" << std::endl;
    std::cout << "Running Matt's tests" << std::endl;
    mattTests(rng);
    std::cout << "Passed Matt's tests" << std::endl;
}
