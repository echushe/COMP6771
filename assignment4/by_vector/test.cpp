#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <random>
#include <set>
#include "btree.h"

using namespace std;

template <typename T>
void test_iterator(const btree<T> & tree)
{
    for (btree<int>::iterator itr = tree.begin(); itr != tree.end(); ++itr)
    {
        cout << *itr << ' ';
    }
    cout << endl;

    btree<int>::iterator itr1 = tree.end();
    while (itr1 != tree.begin())
    {
        --itr1;
        cout << *itr1 << ' ';
    }
    cout << endl;
}

template <typename T>
void test_reverse_iterator(const btree<T> & tree)
{
    for (btree<int>::reverse_iterator itr = tree.rbegin(); itr != tree.rend(); ++itr)
    {
        cout << *itr << ' ';
    }
    cout << endl;

    btree<int>::reverse_iterator itr1 = tree.rend();
    while (itr1 != tree.rbegin())
    {
        --itr1;
        cout << *itr1 << ' ';
    }
    cout << endl;
}

template <typename T>
void test_const_iterator(const btree<T> & tree)
{
    for (btree<int>::const_iterator itr = tree.cbegin(); itr != tree.cend(); ++itr)
    {
        cout << *itr << ' ';
    }
    cout << endl;

    btree<int>::const_iterator itr1 = tree.cend();
    while (itr1 != tree.cbegin())
    {
        --itr1;
        cout << *itr1 << ' ';
    }
    cout << endl;
}

template <typename T>
void test_const_reverse_iterator(const btree<T> & tree)
{
    for (btree<int>::const_reverse_iterator itr = tree.crbegin(); itr != tree.crend(); ++itr)
    {
        cout << *itr << ' ';
    }
    cout << endl;

    btree<int>::const_reverse_iterator itr1 = tree.crend();
    while (itr1 != tree.crbegin())
    {
        --itr1;
        cout << *itr1 << ' ';
    }
    cout << endl;
}

template <typename T>
void test_iterator_out_of_bound(const btree<T> & tree)
{
/*    try
    {
        for (auto itr = tree.begin(); ; ++itr);
    }
    catch (...)
    {
    }
*/

/*    try
    {
        for (auto itr = tree.end(); ; --itr);
    }
    catch (...)
    {
    }
*/
/*    try
    {
        for (auto itr = tree.cbegin(); ; ++itr);
    }
    catch (...)
    {
    }
*/
/*    try
    {
        for (auto itr = tree.cend(); ; --itr);
    }
    catch (...)
    {
    }
*/
/*    try
    {
        for (auto itr = tree.rbegin(); ; ++itr);
    }
    catch (...)
    {
    }
*/
/*    try
    {
        for (auto itr = tree.rend(); ; --itr);
    }
    catch (...)
    {
    }
*/
/*    try
    {
        for (auto itr = tree.crbegin(); ; ++itr);
    }
    catch (...)
    {
    }
*/
/*    try
    {
        for (auto itr = tree.crend(); ; --itr);
    }
    catch (...)
    {
    }
*/
}

long long test_time_complexity(int elements)
{
    btree<int> tree6{ 50 };
    cout << "---- insert " << elements << " random numbers ----" << endl;
    for (int i = 0; i < elements; i++)
    {
        tree6.insert(rand() % elements);
    }
    cout << "---- go through all numbers ----" << endl;
    for (auto itr = tree6.begin(); itr != tree6.end(); ++itr)
    {
    }

    chrono::milliseconds ms_begin =
        chrono::duration_cast <chrono::milliseconds>(chrono::system_clock::now().time_since_epoch());

    cout << "---- test find ----" << endl;
    for (int i = 0; i < 1000000; ++i)
    {
        tree6.find(rand() % elements);
    }

    chrono::milliseconds ms_end =
        chrono::duration_cast <chrono::milliseconds>(chrono::system_clock::now().time_since_epoch());

    return ms_end.count() - ms_begin.count();
}

template <typename T>
void test_find(const btree<T> & tree, const T & to_find)
{
    auto found = tree.find(to_find);
    if (found != tree.end())
    {
        cout << *found << endl;
    }
    else
    {
        cout << to_find << " is not found!" << endl;
    }
}

void check_equal(std::set<int> & s, btree<int> & b)
{
    int number_of_ele = rand() % 3000 + 1;

    for (int i = 0; i < number_of_ele; i++)
    {
        int item = rand() % 8000;
        //cout << "inserting " << item << endl;
        s.insert(item);
        b.insert(item);
    }

    cout << "Compare btree with set of " << s.size() << " random elements .... " << endl;
    {
        auto s_itr = s.begin();
        auto b_itr = b.begin();

        for (; s_itr != s.end(); ++s_itr)
        {
            //cout << "checking " << *s_itr << " " << *b_itr << endl;
            if (*s_itr != *b_itr++)
            {
                cout << "****************************** unequal ******************************" << endl;
                break;
            }
        }
    }
    {
        auto s_itr = s.cbegin();
        auto b_itr = b.cbegin();

        for (; s_itr != s.cend(); ++s_itr)
        {
            //cout << "checking " << *s_itr << " " << *b_itr << endl;
            if (*s_itr != *b_itr++)
            {
                cout << "****************************** unequal ******************************" << endl;
                break;
            }
        }
    }
    {
        auto rs_itr = s.rbegin();
        auto rb_itr = b.rbegin();

        for (; rs_itr != s.rend(); ++rs_itr)
        {
            //cout << "checking " << *s_itr << " " << *b_itr << endl;
            if (*rs_itr != *rb_itr++)
            {
                cout << "****************************** unequal ******************************" << endl;
                break;
            }
        }
    }
    {
        auto rs_itr = s.crbegin();
        auto rb_itr = b.crbegin();

        for (; rs_itr != s.crend(); ++rs_itr)
        {
            //cout << "checking " << *s_itr << " " << *b_itr << endl;
            if (*rs_itr != *rb_itr++)
            {
                cout << "****************************** unequal ******************************" << endl;
                break;
            }
        }
    }

    {
        auto s_itr = s.end();
        auto b_itr = b.end();

        --s_itr;
        --b_itr;
        for (; s_itr != s.begin(); --s_itr)
        {
            //cout << "checking " << *s_itr << " " << *b_itr << endl;
            if (*s_itr != *b_itr--)
            {
                cout << "****************************** unequal ******************************" << endl;
                break;
            }
        }

        if (*s_itr != *b_itr)
        {
            cout << "****************************** unequal ******************************" << endl;
        }

    }
    {
        auto s_itr = s.rend();
        auto b_itr = b.rend();

        --s_itr;
        --b_itr;
        for (; s_itr != s.rbegin(); --s_itr)
        {
            //cout << "checking " << *s_itr << " " << *b_itr << endl;
            if (*s_itr != *b_itr--)
            {
                cout << "****************************** unequal ******************************" << endl;
                break;
            }
        }

        if (*s_itr != *b_itr)
        {
            cout << "****************************** unequal ******************************" << endl;
        }

    }
    {
        auto s_itr = s.cend();
        auto b_itr = b.cend();

        --s_itr;
        --b_itr;
        for (; s_itr != s.cbegin(); --s_itr)
        {
            //cout << "checking " << *s_itr << " " << *b_itr << endl;
            if (*s_itr != *b_itr--)
            {
                cout << "****************************** unequal ******************************" << endl;
                break;
            }
        }

        if (*s_itr != *b_itr)
        {
            cout << "****************************** unequal ******************************" << endl;
        }

    }
    {
        auto s_itr = s.crend();
        auto b_itr = b.crend();

        --s_itr;
        --b_itr;
        for (; s_itr != s.crbegin(); --s_itr)
        {
            //cout << "checking " << *s_itr << " " << *b_itr << endl;
            if (*s_itr != *b_itr--)
            {
                cout << "****************************** unequal ******************************" << endl;
                break;
            }
        }

        if (*s_itr != *b_itr)
        {
            cout << "****************************** unequal ******************************" << endl;
        }

    }
}

int main(void) 
{
    cout << "============================== test a tree node =============================" << endl;
    btree<int> tree1{ 4 };
    tree1.insert(100);
    tree1.insert(0);
    tree1.insert(300);
    tree1.insert(200);
    cout << "---------- recursive way of print -----------" << endl;
    cout << tree1 << endl;

    cout << "============================== test a tree node and its children =============================" << endl;

    tree1.insert(-90);
    tree1.insert(-100);
    tree1.insert(-70);
    tree1.insert(-80);

    tree1.insert(400);
    tree1.insert(380);
    tree1.insert(390);

    tree1.insert(110);
    tree1.insert(140);
    tree1.insert(130);
    tree1.insert(120);

    tree1.insert(381);
    tree1.insert(382);
    tree1.insert(385);
    tree1.insert(383);
    tree1.insert(384);

    tree1.insert(-101);
    tree1.insert(-102);
    tree1.insert(-103);
    tree1.insert(-105);
    tree1.insert(-104);

    cout << "---------- recursive way of print -----------" << endl;
    cout << tree1 << endl;

    cout << "============================== test copy constructor =============================" << endl;

    btree<int> tree2{ tree1 };
    cout << "---------- recursive way of print -----------" << endl;
    cout << tree2 << endl;

    cout << "============================== test copy assignment =============================" << endl;

    btree<int> tree3{ 3 };
    tree3.insert(10);
    tree3.insert(777);
    tree3.insert(888);
    tree3.insert(666);

    tree3 = tree2;
    cout << "---------- recursive way of print -----------" << endl;
    cout << tree3 << endl;

    cout << "============================== test move constructor =============================" << endl;

    btree<int> tree4{ std::move(tree3) };

    cout << "---------- recursive way of print of moved -----------" << endl;
    cout << tree3 << endl;

    cout << "---------- recursive way of print constructed -----------" << endl;
    cout << tree4 << endl;

    tree3.insert(1000);
    tree3.insert(2000);
    tree3.insert(-1000);
    tree3.insert(3000);
    tree3.insert(3900);
    tree3.insert(3400);
    tree3.insert(3500);
    tree3.insert(3600);
    tree3.insert(200);
    tree3.insert(400);
    tree3.insert(100);
    tree3.insert(300);

    cout << "---------- can the moved be rebuilt -----------" << endl;
    cout << tree3 << endl;

    cout << "============================== test move assignment =============================" << endl;

    btree<int> tree5{ 2 };

    tree5.insert(999);
    tree5.insert(123);
    tree5.insert(321);
    tree5.insert(-999);
    tree5.insert(-1200);
    tree5.insert(-333);

    tree5 = std::move(tree4);

    cout << "---------- recursive way of print of moved -----------" << endl;
    cout << tree4 << endl;

    cout << "---------- recursive way of print constructed -----------" << endl;
    cout << tree5 << endl;

    tree4.insert(1000);
    tree4.insert(2000);
    tree4.insert(-1000);
    tree4.insert(3000);
    tree4.insert(3900);
    tree4.insert(3400);
    tree4.insert(3500);
    tree4.insert(3600);
    tree4.insert(200);
    tree4.insert(400);
    tree4.insert(100);
    tree4.insert(300);

    cout << "---------- can the moved be rebuilt -----------" << endl;
    cout << tree4 << endl;

    cout << "============================== test iterators =============================" << endl;

    cout << "---------- iterator of tree1 -----------" << endl;
    test_iterator<int>(tree1);

    cout << "---------- iterator of tree2 -----------" << endl;
    test_iterator<int>(tree2);

    cout << "---------- iterator of tree3 -----------" << endl;
    test_iterator<int>(tree3);

    cout << "---------- iterator of tree4 -----------" << endl;
    test_iterator<int>(tree4);

    cout << "---------- iterator of tree5 -----------" << endl;
    test_iterator<int>(tree5);

    cout << "============================== test reverse iterators =============================" << endl;

    cout << "---------- reverse iterator of tree1 -----------" << endl;
    test_reverse_iterator<int>(tree1);

    cout << "---------- reverse iterator of tree2 -----------" << endl;
    test_reverse_iterator<int>(tree2);

    cout << "---------- reverse iterator of tree3 -----------" << endl;
    test_reverse_iterator<int>(tree3);

    cout << "---------- reverse iterator of tree4 -----------" << endl;
    test_reverse_iterator<int>(tree4);

    cout << "---------- reverse iterator of tree5 -----------" << endl;
    test_reverse_iterator<int>(tree5);

    cout << "============================== test const iterators =============================" << endl;

    cout << "---------- const iterator of tree1 -----------" << endl;
    test_const_iterator<int>(tree1);

    cout << "---------- const iterator of tree2 -----------" << endl;
    test_const_iterator<int>(tree2);

    cout << "---------- const iterator of tree3 -----------" << endl;
    test_const_iterator<int>(tree3);

    cout << "---------- const iterator of tree4 -----------" << endl;
    test_const_iterator<int>(tree4);

    cout << "---------- const iterator of tree5 -----------" << endl;
    test_const_iterator<int>(tree5);

    cout << "============================== test const reverse iterators =============================" << endl;

    cout << "---------- const reverse iterator of tree1 -----------" << endl;
    test_const_reverse_iterator<int>(tree1);

    cout << "---------- const reverse iterator of tree2 -----------" << endl;
    test_const_reverse_iterator<int>(tree2);

    cout << "---------- const reverse iterator of tree3 -----------" << endl;
    test_const_reverse_iterator<int>(tree3);

    cout << "---------- const reverse iterator of tree4 -----------" << endl;
    test_const_reverse_iterator<int>(tree4);

    cout << "---------- const reverse iterator of tree5 -----------" << endl;
    test_const_reverse_iterator<int>(tree5);

    cout << "============================== test find =============================" << endl;

    test_find<int>(tree1, -105);
    test_find<int>(tree1, -104);
    test_find<int>(tree1, -103);
    test_find<int>(tree1, -102);
    test_find<int>(tree1, -101);
    test_find<int>(tree1, -80);
    test_find<int>(tree1, -60);
    test_find<int>(tree1, 0);
    test_find<int>(tree1, 10);
    test_find<int>(tree1, 101);
    test_find<int>(tree1, 110);
    test_find<int>(tree1, 300);
    test_find<int>(tree1, 382);
    test_find<int>(tree1, 384);
    test_find<int>(tree1, 392);
    test_find<int>(tree1, 400);
    test_find<int>(tree1, 500);


    cout << "============================== test iterators out of bound =========================" << endl;
    test_iterator_out_of_bound(tree1);

    cout << "============================== test time complexity =============================" << endl;
    
    cout << test_time_complexity(10) << endl;
    cout << test_time_complexity(100) << endl;
    cout << test_time_complexity(1000) << endl;
    cout << test_time_complexity(10000) << endl;
    cout << test_time_complexity(100000) << endl;
	cout << test_time_complexity(1000000) << endl;

    cout << "============================== test accuracy =============================" << endl;

    std::set<int> s1, s2;
    btree<int> b1, b2;
    for (int i = 0; i < 100; i++)
    {
        if (0 == rand() % 3)
        {
            s1.clear();
            b1.clear();
            check_equal(s1, b1);
        }
        else if (1 == rand() % 3)
        {
            s2 = s1;
            b2 = b1;
            check_equal(s2, b2);
            s1 = s2;
            b1 = b2;
        }
        else
        {
            s2 = std::move(s1);
            b2 = std::move(b1);
            check_equal(s1, b1);
            check_equal(s2, b2);
            s1 = s2;
            b1 = b2;
        }
    }
}