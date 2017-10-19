#include "suite.h"
#include "btree.h"

#include <vector>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cassert>

template <typename T>
void assertEqual(const btree<T>& tree, std::vector<T> expectedPrint) {
    auto expected_non_const = expectedPrint;

    // Testing with const btree
    {
        std::stringstream actual, expected;
        actual << tree;

        // Test printing is the same
        {
            for (auto it = expectedPrint.begin(); it != expectedPrint.end(); it++) {
                expected << *it;
                if (it != expectedPrint.end() - 1) {
                    expected << " ";
                }
            }

            if (expected.str() != actual.str()) {
                std::cout << "Expected:\n" << expected.str() << "\nGot:\n" << actual.str();
                assert(expected.str() == actual.str());
            }
        }

        std::sort(expectedPrint.begin(), expectedPrint.end());

        // Testing constant iterator same
        {
            auto t = tree.cbegin();
            for (auto v = expectedPrint.cbegin(); v != expectedPrint.cend(); ++v) {
                assert(*t == *v);
                ++t;
            }
        }

        // Testing const iterator with postfix increment
        {
            auto t = tree.cbegin();
            for (auto v = expectedPrint.cbegin(); v != expectedPrint.cend(); v++) {
                assert(*t == *v);
                t++;
            }
        }

        // Testing non const iterator (should be const override of begin)
        {
            auto t = tree.begin();
            for (auto v = expectedPrint.cbegin(); v != expectedPrint.cend(); ++v) {
                assert(*t == *v);
                ++t;
            }
        }

        // Testing reverse const iterator
        {
            auto iter = tree.crbegin();
            for (auto v = expectedPrint.crbegin(); v != expectedPrint.crend(); ++v) {
                assert(*iter == *v);
                ++iter;
            }
        }

        // Testing reverse iterator non const (should be rbgein const override)
        {
            auto iter = tree.rbegin();
            for (auto v = expectedPrint.crbegin(); v != expectedPrint.crend(); ++v) {
                assert(*iter == *v);
                ++iter;
            }
        }
    }

    // Testing with non_const btree
    {
        auto tree_nonconst = const_cast<btree<T>&>(tree);
        std::stringstream actual, expected;
        actual << tree_nonconst;

        // Test printing is the same
        {
            for (auto it = expected_non_const.begin(); it != expected_non_const.end(); it++) {
                expected << *it;
                if (it != expected_non_const.end() - 1) {
                    expected << " ";
                }
            }

            if (expected.str() != actual.str()) {
                std::cout << "Expected:\n" << expected.str() << "\nGot:\n" << actual.str();
                assert(expected.str() == actual.str());
            }
        }

        std::sort(expected_non_const.begin(), expected_non_const.end());

        // Testing constant iterator same
        {
            auto t = tree_nonconst.cbegin();
            for (auto v = expected_non_const.cbegin(); v != expected_non_const.cend(); ++v) {
                assert(*t == *v);
                ++t;
            }
        }

        // Testing const iterator with postfix increment
        {
            auto t = tree_nonconst.cbegin();
            for (auto v = expected_non_const.cbegin(); v != expected_non_const.cend(); v++) {
                assert(*t == *v);
                t++;
            }
        }

        // Testing non const iterator
        {
            auto t = tree_nonconst.begin();
            for (auto v = expected_non_const.cbegin(); v != expected_non_const.cend(); ++v) {
                assert(*t == *v);
                ++t;
            }
        }

        // Testing reverse const iterator
        {
            auto iter = tree_nonconst.crbegin();
            for (auto v = expected_non_const.crbegin(); v != expected_non_const.crend(); ++v) {
                assert(*iter == *v);
                ++iter;
            }
        }

        // Testing reverse iterator non const
        {
            auto iter = tree_nonconst.rbegin();
            for (auto v = expected_non_const.crbegin(); v != expected_non_const.crend(); ++v) {
                assert(*iter == *v);
                ++iter;
            }
        }
    }
}

void cameronTests() {
    std::cout << "Testing basic insertion" << std::endl;
    {
        btree<int> testContainer(4);
        auto pair = testContainer.insert(5);
        assert(*(pair.first) == 5);
        assert(pair.second == true);
    }

    std::cout << "Testing repeated insertion fails" << std::endl;
    {
        btree<int> testContainer(4);
        auto pair = testContainer.insert(5);
        auto second_pair = testContainer.insert(5);
        assert(pair.first == second_pair.first);
        assert(second_pair.second == false);
    }

    std::cout << "Testing insertion creates child" << std::endl;
    {
        btree<int> testContainer(4);
        testContainer.insert(1);
        testContainer.insert(3);
        testContainer.insert(5);
        testContainer.insert(7);
        auto pair = testContainer.insert(0);
        assert(*(pair.first) == 0);
        assert(pair.second == true);
    }

    std::cout << "Testing ++ on iterator returned by insert" << std::endl;
    {
        btree<int> testContainer(4);
        testContainer.insert(1);
        testContainer.insert(3);
        testContainer.insert(5);
        testContainer.insert(7);
        testContainer.insert(0);
        auto pair = testContainer.insert(0);
        auto it = pair.first;
        assert(*it == 0);
        ++it;
        assert(*it == 1);
    }

    std::cout << "Testing print" << std::endl;
    {
        btree<int> testContainer(4);
        testContainer.insert(1);
        testContainer.insert(3);
        testContainer.insert(5);
        testContainer.insert(7);
        assertEqual(testContainer, {1, 3, 5, 7});
    }

    std::cout << "Testing print on tree with children" << std::endl;
    {
        btree<int> testContainer(4);
        testContainer.insert(1);
        testContainer.insert(3);
        testContainer.insert(5);
        testContainer.insert(7);
        testContainer.insert(0);
        auto pair = testContainer.insert(2);
        assert(*(pair.first) == 2);
        assert(pair.second == true);
        auto second_pair = testContainer.insert(2);
        assert(pair.first == second_pair.first);
        assert(second_pair.second == false);

        assertEqual(testContainer, {1, 3, 5, 7, 0, 2});
    }

    std::cout << "Testing const print" << std::endl;
    {
        btree<int> testContainer(4);
        testContainer.insert(1);
        testContainer.insert(3);
        testContainer.insert(5);
        testContainer.insert(7);
        testContainer.insert(0);
        testContainer.insert(2);

        const auto copy = testContainer;
        assertEqual(testContainer, {1, 3, 5, 7, 0, 2});
        assertEqual(copy, {1, 3, 5, 7, 0, 2});
    }

    std::cout << "Testing print on empty tree" << std::endl;
    {
        btree<int> testContainer(4);
        assertEqual(testContainer, {});
    }

    std::cout << "Testing copy constructor" << std::endl;
    {
        btree<int> testContainer(4);
        testContainer.insert(1);
        testContainer.insert(3);
        testContainer.insert(5);
        testContainer.insert(7);
        testContainer.insert(0);
        testContainer.insert(2);

        auto copy = testContainer;
        assertEqual(testContainer, {1, 3, 5, 7, 0, 2});
        assertEqual(copy, {1, 3, 5, 7, 0, 2});

        testContainer.insert(4);
        assertEqual(testContainer, {1, 3, 5, 7, 0, 2, 4});
        assertEqual(copy, {1, 3, 5, 7, 0, 2});
    }

    std::cout << "Testing copy assignment" << std::endl;
    {
        btree<int> testContainer(4);
        testContainer.insert(1);
        testContainer.insert(3);
        testContainer.insert(5);
        testContainer.insert(7);
        testContainer.insert(0);
        testContainer.insert(2);

        btree<int> copy;
        copy = testContainer;
        assertEqual(testContainer, {1, 3, 5, 7, 0, 2});
        assertEqual(copy, {1, 3, 5, 7, 0, 2});

        testContainer.insert(4);
        assertEqual(testContainer, {1, 3, 5, 7, 0, 2, 4});
        assertEqual(copy, {1, 3, 5, 7, 0, 2});

        // Copying when already have data
        copy = testContainer;
        assertEqual(testContainer, {1, 3, 5, 7, 0, 2, 4});
        assertEqual(copy, {1, 3, 5, 7, 0, 2, 4});
    }

    std::cout << "Testing move constructor" << std::endl;
    {
        btree<int> testContainer(4);
        testContainer.insert(1);
        testContainer.insert(3);
        testContainer.insert(5);
        testContainer.insert(7);
        testContainer.insert(0);
        testContainer.insert(2);

        auto copy = std::move(testContainer);
        assertEqual(testContainer, {});
        assertEqual(copy, {1, 3, 5, 7, 0, 2});

        // Should still function
        testContainer.insert(100);
        assertEqual(copy, {1, 3, 5, 7, 0, 2});
        assertEqual(testContainer, {100});
    }

    std::cout << "Testing move assignment" << std::endl;
    {
        btree<int> testContainer(4);
        testContainer.insert(1);
        testContainer.insert(3);
        testContainer.insert(5);
        testContainer.insert(7);
        testContainer.insert(0);
        testContainer.insert(2);

        btree<int> copy;
        copy = std::move(testContainer);
        assertEqual(testContainer, {});
        assertEqual(copy, {1, 3, 5, 7, 0, 2});

        // Should still function
        testContainer.insert(100);
        assertEqual(copy, {1, 3, 5, 7, 0, 2});
        assertEqual(testContainer, {100});

        // Moving when already have data
        testContainer = copy;
        assertEqual(testContainer, {1, 3, 5, 7, 0, 2});
        assertEqual(copy, {1, 3, 5, 7, 0, 2});
        testContainer.insert(9);
        assertEqual(testContainer, {1, 3, 5, 7, 0, 2, 9});

        copy = std::move(testContainer);
        assertEqual(testContainer, {});
        assertEqual(copy, {1, 3, 5, 7, 0, 2, 9});

        // Should still function
        testContainer.insert(100);
        assertEqual(copy, {1, 3, 5, 7, 0, 2, 9});
        assertEqual(testContainer, {100});
    }

    std::cout << "Testing find non const" << std::endl;
    {
        btree<int> testContainer(4);
        testContainer.insert(1);
        testContainer.insert(3);
        testContainer.insert(5);
        testContainer.insert(7);
        testContainer.insert(0);
        testContainer.insert(2);

        auto iter = testContainer.find(1);
        assert(*iter == 1);
        ++iter;
        assert(*iter == 2);

        iter = testContainer.find(0);
        assert(*iter == 0);
        ++iter;
        assert(*iter == 1);

        iter = testContainer.find(100);
        assert(iter == testContainer.end());

        typename btree<int>::const_iterator const_iter = testContainer.find(100);
        assert(const_iter == testContainer.end());
        assert(const_iter == testContainer.cend());
    }

    std::cout << "Testing find const" << std::endl;
    {
        btree<int> testContainer(4);
        testContainer.insert(1);
        testContainer.insert(3);
        testContainer.insert(5);
        testContainer.insert(7);
        testContainer.insert(0);
        testContainer.insert(2);

        const auto const_tree = testContainer;
        auto iter = const_tree.find(1);
        assert(*iter == 1);
        ++iter;
        assert(*iter == 2);

        iter = const_tree.find(0);
        assert(*iter == 0);
        ++iter;
        assert(*iter == 1);

        iter = const_tree.find(100);
        assert(iter == const_tree.end());
        assert(iter == const_tree.cend());

        typename btree<int>::const_iterator const_iter = testContainer.find(100);
        assert(const_iter == testContainer.end());
        assert(const_iter == testContainer.cend());;
    }

    std::cout << "Testing for each loop compatability" << std::endl;
    {
        btree<int> testContainer(4);
        testContainer.insert(1);
        testContainer.insert(3);
        testContainer.insert(5);
        testContainer.insert(7);
        testContainer.insert(0);
        testContainer.insert(2);

        std::vector<int> v{0, 1, 2, 3, 5, 7};
        auto iter = v.cbegin();
        for (auto elem : testContainer) {
            assert(elem == *iter);
            ++iter;
        }
    }

    std::cout << "Testing increasing each element by 1" << std::endl;
    {
        btree<int> testContainer(4);
        testContainer.insert(1);
        testContainer.insert(3);
        testContainer.insert(5);
        testContainer.insert(7);
        testContainer.insert(0);
        testContainer.insert(2);

        assertEqual(testContainer, {1, 3, 5, 7, 0, 2});

        for (auto it = testContainer.begin(); it != testContainer.end(); ++it) {
            (*it)++;
        }

        assertEqual(testContainer, {2, 4, 6, 8, 1, 3});
    }

    std::cout << "All tests passed, you are awesome!" << std::endl;
}
