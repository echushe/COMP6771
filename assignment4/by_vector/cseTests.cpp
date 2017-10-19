/**
 * THE CODE HEREIN IS PROVIDED AS IS, WITH NO EXPRESS, OR IMPLIED, WARRANTIES
 * OR SUITABILITY FOR PURPOSE! This means it is up to you to figure out how
 * it works, fix it if something is broken, and adapt it as appropriate for
 * your needs.
 *
 * It is supplied as a starting point for your testing. Of course, it is 
 * inadequate as your only test and you will be in trouble if you
 * do not write lots and lots of additional tests of your own!
 **/

#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <set>
#include <memory>
#include <cassert>
#include <string>

#include "btree.h"
#include "suite.h"

using std::copy;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::set;
using std::string;

namespace {

std::mt19937* rng;

const long kMinInteger = 0;
const long kMaxInteger = 100000;

void insertRandomNumbers(btree<long>&, set<long>&, size_t);
bool confirmEverythingMatches(const btree<long>&, const set<long>&);

unsigned int getRandom(unsigned int min, unsigned int max) {
    static auto dist = std::uniform_int_distribution<unsigned int>(min, max);
    return dist(*rng);
}

/**
 * Tries to insert numbers into the specified test container, and every time
 * the insertion succeeds (because the number wasn't previously in the btree),
 * the same number is inserted into the off-the-shelf set
 * class instance so we can later track the correctness of our btree.
 **/
void insertRandomNumbers(btree<long>& testContainer, set<long>& stableContainer, size_t size) {
    for (size_t i = 0; i < size; i++) {
        long rndNum = getRandom(kMinInteger, kMaxInteger);
        std::pair<btree<long>::iterator, bool> result = testContainer.insert(rndNum);
        assert(result.second == stableContainer.insert(rndNum).second);
    }
}

/**
 * Confirms that the specified btree and the specified
 * set contain exactly the same numbers.  This does so by
 * considering all numbers that could have been added, and
 * asserting that each of those numbers is either present
 * in or absent from both containers.  If all checks out,
 * we return 0 to signal success; if along the way we see
 * an integer in one container and not the other, then
 * we bail immediately and return one to express failure.
 **/
bool confirmEverythingMatches(const btree<long>& testContainer, const set<long>& stableContainer) {
    for (long i = kMinInteger; i <= kMaxInteger; i++) {
        bool foundInTree = (testContainer.find(i) != testContainer.end());
        bool foundInSet = (stableContainer.find(i) != stableContainer.end());
        if (foundInTree != foundInSet) {
            cout << "- btree and set don't contain the same data!" << endl;
            cout << "Mismatch at element: " << i << endl;
            cout << "Found in tree: " << foundInTree << ", found in set: " << foundInSet;
            return false;
        }
    }

    return true;
}

}  // namespace close


/**
 * Codes for testing various bits and pieces. Most of the code is commented out
 * you should uncomment it as appropriate.
 **/
void cseTests(std::mt19937 rng) {
    ::rng = &rng;
    btree<long> tmp(1);
    tmp.insert(2);
    tmp.insert(1);
    tmp.insert(3);

    assert(tmp.find(1) != tmp.end());
    assert(tmp.find(2) != tmp.end());
    assert(tmp.find(3) != tmp.end());

    // insert lots of random numbers and compare with a known correct container
    btree<long> testContainer(99);
    set<long> stableContainer;

    insertRandomNumbers(testContainer, stableContainer, 10000);
    btree<long> btcpy = testContainer;
    assert(confirmEverythingMatches(btcpy, stableContainer));


    // this next portion was something I used to sort a bunch of chars
    // this was what I used to debug my iterator and made it work
    btree<char> astring;

    std::set<char> chars;
    for(int i = 0; i < 10; i++) {
        char val = getRandom('A', 'z');
        std::pair<btree<char>::iterator, bool> result = astring.insert(val);
        assert(result.second == chars.insert(val).second);
        assert(*result.first == val);
    }

    auto vec = chars.cbegin();
    for(btree<char>::iterator iter = astring.begin(); iter != astring.end(); ++iter) {
        assert(*iter == *vec);
        ++vec;
    }
    assert(vec == chars.end());

    // const iterator test
//    const btree<char>& refstring = astring;
//    btree<char>::const_iterator iter;
//    cout << "Voila!  Sorted!" << endl;
//    for(iter = refstring.begin(); iter != refstring.end(); ++iter) {
//        astring.insert(static_cast<char>(getRandom('A', 'z')));
//
//        cout << *(iter) << " ";
//    }
//
//    for(btree<char>::const_iterator iter = refstring.begin(); !(iter == refstring.end()); ++iter)
//        cout << *iter;
//    cout << endl;

    // a full-scale string test of the tree using iterators
    std::unique_ptr<btree<string>> strTable = std::make_unique<btree<string>>(40);

    ifstream wordFile("twl.txt");
    if (!wordFile) {
        std::cerr << "Failed to open twl.txt. Skipping some tests";
        return;
    }

    while (wordFile.good()) {
        string word;
        getline(wordFile, word);
        strTable->insert(word);
    }
    wordFile.close();

    for(btree<string>::const_iterator iter = strTable->begin(); iter != strTable->end();) {
        const auto &old = *iter;
        if(++iter != strTable->end())
            assert(old < *iter);
    }
    // reverse iterator
    btree<string>::reverse_iterator riter = strTable->rbegin();
    btree<string>::const_iterator citer = strTable->begin();

    assert(*citer != *riter);

    // try to create a copy
    btree<string> btcpy2;

    btcpy2 = *strTable;

//    ofstream ofs1("out1");
//    ofstream ofs2("out2");
//
//    copy(strTable->begin(), strTable->end(), std::ostream_iterator<string>(ofs1, " "));
//    ofs1 << endl;
//    ofs1 << *strTable << endl;

    strTable = nullptr; // delete it

//    copy(btcpy2.begin(), btcpy2.end(), std::ostream_iterator<string>(ofs2, " "));
//    ofs2 << endl;
//    ofs2 << btcpy2 << endl;
//
//    ofs1.close();
//    ofs2.close();
}

