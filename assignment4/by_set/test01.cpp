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
#include <string>
#include <utility>
#include <stdlib.h>

#include "btree.h"

using std::copy;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::set;
using std::string;

namespace {

const long kMinInteger = 1000000;
const long kMaxInteger = 100000000;

void initRandom(unsigned long);
long getRandom(long low, long high);
void insertRandomNumbers(btree<long>&, set<long>&, size_t);
bool confirmEverythingMatches(const btree<long>&, const set<long>&);

/**
 * Initialises random number generator.
 * The default argument uses a 'random' seed, but a specific 
 * seed can be supplied to reproduce the random sequence for testing.
 **/
void initRandom(unsigned long seed = 0) {
  if (seed == 0) {
    srand(time(NULL));
  } else {
    srand(seed);
  }
}

/**
 * Produces a random number in the given range.
 * The random() function provides better quality random numbers
 * than the older rand() function.
 **/
long getRandom(long low, long high) {
  return (low + (rand() % ((high - low) + 1)));
}

/**
 * Tries to insert numbers into the specified test container, and every time 
 * the insertion succeeds (because the number wasn't previously in the btree), 
 * the same number is inserted into the off-the-shelf set 
 * class instance so we can later track the correctness of our btree.
 **/
void insertRandomNumbers(btree<long>& testContainer, set<long>& stableContainer, size_t size) {
  cout << "Let's insert up to " << size << " numbers." << endl;
  for (size_t i = 0; i < size; i++) {
    long rndNum = getRandom(kMinInteger, kMaxInteger);
    std::pair<btree<long>::iterator, bool> result = testContainer.insert(rndNum);
    if (result.second) stableContainer.insert(rndNum);
    if ((i + 1) % 100000 == 0) 
      cout << "Inserted some " << (i + 1) << " numbers thus far." << endl;
  }
  cout << endl;
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
  cout << "Confirms the btree and the set " 
          "contain exactly the same values..." << endl;
  for (long i = kMinInteger; i <= kMaxInteger; i++) {
    bool foundInTree = (testContainer.find(i) != testContainer.end());
    bool foundInSet = (stableContainer.find(i) != stableContainer.end());
    if (foundInTree != foundInSet) {
      cout << "- btree and set don't contain the same data!" << endl; 
      cout << "Mismatch at element: " << i << endl;
      return false;
    }
  }
  cout << "- btree checks out just fine." << endl;

  return true;
}

}  // namespace close


/**
 * Codes for testing various bits and pieces. Most of the code is commented out
 * you should uncomment it as appropriate.
 **/
int main(void) {
  // initialise random number generator with 'random' seed
  initRandom();

  // insert lots of random numbers and compare with a known correct container
  btree<long> testContainer(99);
  set<long> stableContainer;
    
  insertRandomNumbers(testContainer, stableContainer, 1000000);
  btree<long> btcpy = testContainer;
  confirmEverythingMatches(btcpy, stableContainer);

/***
  
  // this next portion was something I used to sort a bunch of chars
  // this was what I used to debug my iterator and made it work
 	btree<char> astring;
	
	cout << "\nInserting these random chars into the tree...\n";
	for(int i = 0; i < 10; i++) {
	pair<btree<char>::iterator, bool> result = 
    astring.insert(static_cast<char>(getRandom('A', 'z')));
    cout << *result.first;
	}
	cout << endl << endl;
		
	for(btree<char>::iterator iter = astring.begin(); iter != astring.end(); ++iter)
	cout << *iter;
	cout << endl;
  
  // const iterator test
  const btree<char>& refstring = astring;
    btree<char>::const_iterator iter;
	cout << "Voila!  Sorted!" << endl;
	for(iter = refstring.begin(); iter != refstring.end(); ++iter) {
		astring.insert(static_cast<char>(getRandom('A', 'z')));
	
		cout << *(iter) << " ";
	}
	
	for(btree<char>::const_iterator iter = refstring.begin(); !(iter == refstring.end()); ++iter)
		cout << *iter;
	cout << endl;

  // a full-scale string test of the tree using iterators
  btree<string> *strTable = new btree<string>(40);
		
  ifstream wordFile("twl.txt");
  if (!wordFile)
    return 1;  // file couldn't be opened for some reason, abort... 
  
  while (wordFile.good()) {
    string word;
    getline(wordFile, word);
    strTable->insert(word);
  }
  wordFile.close();

  cout << "twl.txt sorted by our wonderful tree..." << endl;
  // Such beautiful code with iterators...
  for(btree<string>::const_iterator iter = strTable->begin(); iter != strTable->end(); ++iter)
    cout << *iter << endl;

  // reverse iterator
  btree<string>::reverse_iterator riter = strTable->rbegin();
  btree<string>::const_iterator citer = strTable->begin();

  if (*citer != *riter) {
    cout << "success!" << endl;
  }

  // try to create a copy
  btree<string> btcpy2;
  
  btcpy2 = *strTable;
  
  ofstream ofs1("out1");
  ofstream ofs2("out2");
  
  copy(strTable->begin(), strTable->end(), ostream_iterator<string>(ofs1, " "));
  ofs1 << endl;
  ofs1 << *strTable << endl;
  
  delete strTable;

  copy(btcpy2.begin(), btcpy2.end(), ostream_iterator<string>(ofs2, " "));
  ofs2 << endl;
  ofs2 << btcpy2 << endl;
  
  ofs1.close();
  ofs2.close();
  
***/

  return 0;	
}

