#include <algorithm>
#include <chrono>
#include <iostream>
#include <future>
#include <random>
#include <string>

#include "BucketSort.h"

int fuzzer()
{
    static constexpr const size_t numbers = 500000;
    static constexpr const size_t runs = 500;

    std::mt19937 mt(std::random_device{}());
    std::uniform_int_distribution<unsigned int> numberDist;
    std::uniform_int_distribution<unsigned int> coreDist(1, std::thread::hardware_concurrency() * 10);

    BucketSort bs;

    for (size_t r = 0; r < runs; ++r)
        bs.sort(coreDist(mt));

    bs.numbersToSort.resize(numbers);

    for (size_t r = 0; r < runs; ++r) {
        for (size_t n = 0; n < numbers; ++n)
            bs.numbersToSort[n] = numberDist(mt);

        std::vector<std::string> ref(numbers);
        std::transform(bs.numbersToSort.cbegin(), bs.numbersToSort.cend(), ref.begin(), static_cast<std::string(*)(unsigned int)>(std::to_string));
        auto refFuture = std::async([&]() {std::sort(ref.begin(), ref.end()); });

        bs.sort(coreDist(mt));

        std::vector<std::string> result(numbers);
        std::transform(bs.numbersToSort.cbegin(), bs.numbersToSort.cend(), result.begin(), static_cast<std::string(*)(unsigned int)>(std::to_string));

        refFuture.get();
        for (size_t n = 0; n < numbers; ++n) {
            if (ref[n] != result[n]) {
                std::cerr << "Mismatch at index " << n << ": " << ref[n] << " != " << result[n] << "\n";
                return 1;
            }
        }

        std::cerr << ".";
    }

    return 0;
}

int provided_test()
{
    unsigned int totalNumbers = 500000;
    unsigned int printIndex = 259000;

    // use totalNumbers required as the seed for the random
    // number generator. 
    std::mt19937 mt(totalNumbers);
    std::uniform_int_distribution<unsigned int> dist(1, std::numeric_limits<unsigned int>::max());

    // create a sort object
    BucketSort pbs;

    // insert random numbers into the sort object
    for (unsigned int i = 0; i < totalNumbers; ++i)
    {
        pbs.numbersToSort.push_back(dist(mt));
    }

    // call sort giving the number of cores available.
    const unsigned int numCores = 8; //std::thread::hardware_concurrency();
    pbs.sort(numCores);

    std::cout << "number of cores used: " << numCores << std::endl;

    // print certain values from the buckets
    std::cout << "Demonstrating that all the numbers that start with 1 come first" << std::endl;
    std::cout << pbs.numbersToSort[0] << " " << pbs.numbersToSort[printIndex - 10000]
        << " " << pbs.numbersToSort[printIndex] << " " << pbs.numbersToSort[pbs.numbersToSort.size() - 1]
        << std::endl;

    return 0;
}

int my_test()
{
    // unsigned int a = 123004;
    // std::string a_str = std::to_string(a);
    // std::cout << a_str << std::endl;
    BucketSort pbs;
    for (unsigned int i = 0; i < 500; ++i)
    {
        pbs.numbersToSort.push_back(100 + rand() % 200);
    }

   /* for (unsigned int i = 0; i < 100; ++i)
    {
        pbs.numbersToSort.push_back(200 + rand() % 2000);
    }

    for (unsigned int i = 0; i < 10; ++i)
    {
        pbs.numbersToSort.push_back(2000 + rand() % 20000);
    }
    */
    pbs.sort(8);

    std::cout << pbs << std::endl;

    return 0;
}

int main()
{
    my_test();
    provided_test();
    //fuzzer();

    return 0;
}