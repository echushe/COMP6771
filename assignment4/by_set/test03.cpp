#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>

#include "btree.h"

template <typename T>
void find_in_tree(const btree<T> &b, T val) {
  auto iter = std::find(b.begin(), b.end(), val);
  if (iter == b.end()) 
    std::cout << val << " not found" << std::endl;
  else
    std::cout << val << " found" << std::endl;
}

int main(void) {
  btree<std::string> bts;

  bts.insert("comp3000");
  bts.insert("comp6771");
  bts.insert("comp2000");
  bts.insert("comp1000");

  find_in_tree(bts, std::string("comp6771"));

  btree<int> bti;

  bti.insert(1);
  bti.insert(10);
  bti.insert(3);
  bti.insert(4);

  find_in_tree(bti, 100);

  return 0;
}
