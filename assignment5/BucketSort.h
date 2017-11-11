/*********************************************
    
    COMP6771 Assignment 5
    Programmed by Chunnan Sheng
            z5100764

**********************************************/

#pragma once
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <chrono>

typedef unsigned long long uint64;
typedef unsigned int uint32;

#define max_digit_len 10

struct Digits
{
    char digits[max_digit_len];
    uint32 val;
    uint32 to_integer();
};

struct Bucket
{
    std::thread my_thread;
    std::vector<Digits> to_sort;
    std::vector<Digits> sub_buckets[11];
    uint32 sizes[11];
    uint32 vector_sizes[11];

    uint32 size;
    uint32 vector_size;

    Bucket(unsigned int prepared_size);
    Bucket();

    void sort_in_new_thread();
    void sort();
    void sort_recursive();
    void sort_recursive(int digit_index, std::vector<Digits> & to_sort);
    void print() const;
    void join();
};

struct BucketSort
{
    static std::mutex the_mutex;
    static const bool debug = false;

    static uint64 get_time();

    // vector of numbers
    std::vector<unsigned int> numbersToSort;
    // Buckets
    std::vector<Bucket> buckets;

    void sort(unsigned int numCores);

private:
    int digits(const uint32 & val_in);
    uint64 extend(const uint32 & val_in);
    //uint32 get_bucket_id(const uint64 & extended, const uint32 & numCores);
    Digits to_digits(const uint32 & val_in, uint64 & extended);
    void prepare_for_bucket_sort(unsigned int numCores);
};

std::ostream & operator << (std::ostream & os, const BucketSort & sort);
