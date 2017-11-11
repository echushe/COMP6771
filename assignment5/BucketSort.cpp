/*********************************************

    COMP6771 Assignment 5
    Programmed by Chunnan Sheng
            z5100764

**********************************************/
#include <vector>
#include <iostream>
#include "BucketSort.h"

std::mutex BucketSort::the_mutex;

Bucket::Bucket(unsigned int prepared_size)
    : to_sort{ prepared_size }, size{ 0 }, vector_size{ prepared_size }
{
    for (int i = 0; i < 11; ++i)
    {
        this->sizes[i] = 0;
        this->vector_sizes[i] = 0;
    }
}

Bucket::Bucket()
    : size{ 0 }, vector_size{ 0 }
{
    for (int i = 0; i < 11; ++i)
    {
        this->sizes[i] = 0;
        this->vector_sizes[i] = 0;
    }
}

// This function implements radix sort from lower digits to higher digits
void Bucket::sort()
{
    this->to_sort.resize(this->size);

    for (int j = 0; j < 11; ++j)
    {
        this->sub_buckets[j].resize(this->size / 5 + 1);
        this->vector_sizes[j] = this->size / 5 + 1;
    }

    // Each cycle of this loop executes bucket sort for each digit in the number
    for (int i = max_digit_len - 1; i >= 0; --i)
    {
        for (int j = 0; j < 11; ++j)
        {
            this->sizes[j] = 0;
        }

        // Distribute numbers into buckets via digit number
        uint32 b_size;
        for (Digits n : this->to_sort)
        {
            int b_index = n.digits[i] + 1;
            b_size = this->sizes[b_index];

            if (b_size == this->vector_sizes[b_index])
            {
                this->sub_buckets[b_index].resize(b_size * 2);
                this->vector_sizes[b_index] = b_size * 2;
            }

            this->sub_buckets[b_index][b_size] = n;
            ++this->sizes[b_index];
        }

        uint32 index = 0;

        // Digit -1 means this digit is None (does not exist)
        // None digit indicates earlier position in sorting.
        // 
        // -1 < 0 < 1 < 2 < 3 < 4 < 5 < 6 < 7 < 8 < 9
        //
        // None digit is to solve the problem that some numbers like 15, 1500 and 1500000 should also be sorted correctly.
        std::vector<Digits> & sub_bucket = this->sub_buckets[0];
        b_size = this->sizes[0];
        for (uint32 k = 0; k < b_size; ++k)
        {
            this->to_sort[index] = sub_bucket[k];
            ++index;
        }

        // Digits which are not None will be sequenced normally
        for (int j = 1; j < 11; ++j)
        {
            std::vector<Digits> & l_sub_bucket = this->sub_buckets[j];
            b_size = this->sizes[j];
            for (uint32 k = 0; k < b_size; ++k)
            {
                this->to_sort[index] = l_sub_bucket[k];
                ++index;
            }
        }
    }

    this->print();
}

void Bucket::sort_recursive()
{
    this->to_sort.resize(this->size);
    this->sort_recursive(0, this->to_sort);
    this->print();
}

void Bucket::sort_recursive(int digit_index, std::vector<Digits> & to_sort)
{
    if (digit_index == max_digit_len)
    {
        return;
    }

    if (to_sort.size() < 2)
    {
        return;
    }

    std::vector<Digits> buckets[11];

    // Distribute numbers into buckets via digit number
    for (Digits n : to_sort)
    {
        int b_index = n.digits[digit_index] + 1;

        buckets[b_index].push_back(n);
        ++this->sizes[b_index];
    }

    for (int i = 0; i < 11; ++i)
    {
        sort_recursive(digit_index + 1, buckets[i]);
    }

    uint32 index = 0;
    // Digit -1 means this digit is None (does not exist)
    // None digit indicates earlier position in sorting.
    // 
    // -1 < 0 < 1 < 2 < 3 < 4 < 5 < 6 < 7 < 8 < 9
    //
    // None digit is to solve the problem that some numbers like 15, 1500 and 1500000 should also be sorted correctly.
    for (Digits n : buckets[0])
    {
        to_sort[index] = n;
        ++index;
    }

    // Digits which are not None will be sequenced normally
    for (int j = 1; j < 11; ++j)
    {
        for (Digits n : buckets[j])
        {
            to_sort[index] = n;
            ++index;
        }
    }
}

void Bucket::print() const
{
    if (BucketSort::debug && this->to_sort.size() < 100)
    {
        BucketSort::the_mutex.lock();
        for (auto n : this->to_sort)
        {
            std::cout << n.val << '\t';
        }
        std::cout << std::endl;
        BucketSort::the_mutex.unlock();
    }
}

void Bucket::sort_in_new_thread()
{
    this->my_thread = std::thread(
        [&]
    {
        this->sort();
    });
}

void Bucket::join()
{
    this->my_thread.join();
}


// Get the system time for debug
uint64 BucketSort::get_time()
{
    return std::chrono::duration_cast
        <std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

int BucketSort::digits(const uint32 & val_in)
{
    int sum = 0;
    uint32 l_n = val_in;
    while (l_n > 0)
    {
        l_n /= 10;
        ++sum;
    }
    return sum;
}

// extend integer value with zeros
uint64 BucketSort::extend(const uint32 & val_in)
{
    int digits = this->digits(val_in);
    uint64 to_extend = val_in;

    for (int i = 0; i < max_digit_len - digits; ++i)
    {
        to_extend *= 10;
    }
    return to_extend;
}

// Convert integer into digit arrays
Digits BucketSort::to_digits(const uint32 & val_in, uint64 & extended)
{
    int digits = 0;
    char l_digits[max_digit_len];

    // Fill in non empty digits
    uint32 l_val_in = val_in;
    while (l_val_in > 0)
    {
        l_digits[digits] = l_val_in % 10;
        l_val_in /= 10;
        ++digits;
    }

    extended = val_in;
    Digits n;
    n.val = val_in;

    // Fill in digits
    for (int i = 0; i < digits; ++i)
    {
        n.digits[i] = l_digits[digits - i - 1];
    }

    // Fill in empty digits with -1
    for (int i = max_digit_len - 1; i >= digits; --i)
    {
        n.digits[i] = -1;
        extended *= 10;
    }

    return n;
}


void BucketSort::sort(unsigned int numCores)
{
    this->prepare_for_bucket_sort(numCores);

    if (debug)
        std::cout << "sorting threads start ..." << std::endl;

    uint64 time = get_time();

    // Execute sort job of all the buckets
    for (uint32 i = 0; i < numCores - 1; ++i)
    {
        this->buckets[i].sort_in_new_thread();
    }

    // The last bucket will be executed by the main thread
    this->buckets[numCores - 1].sort();

    for (uint32 i = 0; i < numCores - 1; ++i)
    {
        // Do not join the last bucket because it is executed by the main thread
        this->buckets[i].join();
    }

    // Collect sorted data from all buckets
    if (debug)
    {
        std::cout << "Time used: " << get_time() - time << std::endl;
        time = get_time();
        std::cout << "Collect sorted data from all buckets..." << std::endl;
    }

    uint32 global_index = 0;
    for (uint32 i = 0; i < numCores; ++i)
    {
        std::vector<Digits> & to_sort = this->buckets[i].to_sort;
        for (Digits n : to_sort)
        {
            this->numbersToSort[global_index] = n.val;
            ++global_index;
        }
    }

    if (debug)
    {
        std::cout << "Time used: " << get_time() - time << std::endl;
        time = get_time();
        std::cout << "Sort done..." << std::endl;
    }

    this->buckets.clear();
}

void BucketSort::prepare_for_bucket_sort(uint32 numCores)
{
    if (debug)
        std::cout << "prepare_for_bucket_sort ..." << std::endl;

    if (numCores < 1)
        return;

    uint64 num_size = this->numbersToSort.size();

    // Insert the buckets into the bucket container
    for (uint32 i = 0; i < numCores; ++i)
    {
        this->buckets.push_back(Bucket(num_size * 2 / numCores + 1));
    }

    uint64 approx_min = std::numeric_limits<uint64>::max();
    uint64 approx_max = 0;

    if (numbersToSort.empty())
    {
        return;
    }

    // Figure out the maximun and minimum of the numbers
    if (debug)
        std::cout << "Figure out the maximun and minimum of the numbers ..." << std::endl;

    uint64 time = get_time();

    for (unsigned int n : this->numbersToSort)
    {
        // Explanation:
        // For example
        // 12   < 1306 < 14   < 15   < 2    < 337   can be converted to
        // 1200 < 1306 < 1400 < 1500 < 2000 < 3370
        // There may be some clashes between some numbers like 12, 120, 1200, 12000
        // because they share the same extended value like 12000.
        // However, these clashes don't matter since they usually do not affect
        // much of bucket distributions.
        uint64 l_n = this->extend(n);
        if (l_n < approx_min)
        {
            approx_min = l_n;
        }

        if (l_n > approx_max)
        {
            approx_max = l_n;
        }
    }

    // We introduce the concept of value_equilibrium here to
    // evenly distribute numbers into buckets.

    // The idea of value quilibrium is adapted from Histogram Equalization algorithm used in image processing.
    // An image that is too bright, too dark or too plain in color can apply 
    // Histogram Equalization to evenly distribute pixcels in colors range from 0 to 255 (RGB).
    // In this scenario the range of values is from 0 to 2^32-1 that is too large for us, so
    // we need to use eq_size_ratio to make the range smaller (from 0 to about 1000)
    uint64 eq_size = approx_max - approx_min + 1;
    uint64 eq_size_ratio = eq_size / 1000;

    if (eq_size_ratio < 1)
    {
        eq_size_ratio = 1;
    }

    eq_size /= eq_size_ratio;

    if (debug)
    {
        std::cout << "Time used: " << get_time() - time << std::endl;
        time = get_time();
        std::cout << "Create value_equilibrium vector ..." << std::endl;
        std::cout << "value_equilibrium vector size: " << eq_size << std::endl;
    }

    uint32 value_equilibrium[2000];
    for (uint32 i = 0; i < eq_size; ++i)
    {
        value_equilibrium[i] = 0;
    }

    if (debug)
    {
        std::cout << "Time used: " << get_time() - time << std::endl;
        time = get_time();
        std::cout << "Fill in value_equilibrium vector ..." << std::endl;
    }

    // Add countings of numbers into value_equilibrium

    for (unsigned int n : this->numbersToSort)
    {
        uint64 l_n = this->extend(n);

        uint32 eq_index = (l_n - approx_min) / eq_size_ratio;
        if (eq_index == eq_size)
        {
            --eq_index;
        }

        ++value_equilibrium[eq_index];
    }

    if (debug)
    {
        std::cout << "Time used: " << get_time() - time << std::endl;
        time = get_time();
        std::cout << "Change value_equilibrium vector into incremental mode..." << std::endl;
    }

    // re-arrange countings value_equilibrium into incremental sum order

    uint32 prev_sum = value_equilibrium[0];
    value_equilibrium[0] = 0;

    for (uint32 i = 1; i < eq_size; ++i)
    {
        uint32 bak = value_equilibrium[i];
        value_equilibrium[i] = prev_sum;
        prev_sum += bak;
    }

    // Distribute all the numbers to the buckets via countings in value_equilibrium

    if (debug)
    {
        std::cout << "Time used: " << get_time() - time << std::endl;
        time = get_time();
        std::cout << "Distribute all the numbers to the buckets..." << std::endl;
    }

    for (uint32 n : this->numbersToSort)
    {
        uint64 l_n;
        Digits digits = this->to_digits(n, l_n);

        uint32 eq_index = (l_n - approx_min) / eq_size_ratio;
        if (eq_index == eq_size)
        {
            --eq_index;
        }

        Bucket &bucket = this->buckets[numCores * value_equilibrium[eq_index] / num_size];

        if (bucket.size == bucket.vector_size)
        {
            bucket.to_sort.resize(bucket.size * 2);
            bucket.vector_size = bucket.size * 2;
        }

        bucket.to_sort[bucket.size] = digits;
        ++bucket.size;
    }

    if (debug)
    {
        std::cout << "Time used: " << get_time() - time << std::endl;
        time = get_time();
    }
}

std::ostream & operator << (std::ostream & os, const BucketSort & sort)
{
    if (sort.numbersToSort.size() >= 1000)
        return os;

    uint32 i = 0;
    for (unsigned int n : sort.numbersToSort)
    {
        if (0 == i % 20)
        {
            os << '\n';
        }
        os << n << '\t';
        ++i;
    }

    return os;
}

uint32 Digits::to_integer()
{
    uint32 val = 0;
    for (int j = 0; j < max_digit_len; ++j)
    {
        if (digits[j] == -1)
        {
            break;
        }

        val *= 10;
        val += digits[j];
    }

    return val;
}
