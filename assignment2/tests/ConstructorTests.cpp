#include <cassert>
#include <iostream>
#include <list>
#include <vector>

#include "EuclideanVector.h"
#include "util.h"

void constructor_tests() {
    {
        // Default constructor.
        evec::EuclideanVector vec;
        assertEquals(vec, {0});
    }

    {
        // One argument.
        evec::EuclideanVector vec(5U);
        assertEquals(vec, {0, 0, 0, 0, 0});
    }

    {
        // One argument that gets converted.
        evec::EuclideanVector vec(25);
        assertEquals(vec, std::vector<double>(25));
    }

    {
        // Two arguments.
        evec::EuclideanVector vec(10U, -1.0);
        assertEquals(vec, std::vector<double>(10, -1));
    }

    {
        // Two arguments that get coverted.
        evec::EuclideanVector vec(12, 32);
        assertEquals(vec, std::vector<double>(12, 32));
    }

    {
        // A vector of dimension 0.
        evec::EuclideanVector vec(0, 10.0);
        assertEquals(vec, {});
    }

    {
        // A list iterator.
        std::list<double> l{-2.2, -3.5, 1.0, 0.0};
        evec::EuclideanVector vec{l.begin(), l.end()};
        assertEquals(vec, std::vector<double>(l.begin(), l.end()));
    }

    {
        // A const list iterator.
        const std::list<double> l{-1000.0, 0.001, 0.0};
        evec::EuclideanVector vec{l.cbegin(), l.cend()};
        assertEquals(vec, std::vector<double>(l.begin(), l.end()));
    }

    {
        // A vector iterator.
        std::vector<double> l{-2.2, -3.5, 1.0, 0.0};
        evec::EuclideanVector vec{l.begin(), l.end()};
        assertEquals(vec, l);
    }

    {
        // A const vector iterator.
        const std::vector<double> l{-1000.0, 0.001, 0.0};
        evec::EuclideanVector vec{l.cbegin(), l.cend()};
        assertEquals(vec, l);
    }

    {
        // Generic initializer list.
        evec::EuclideanVector vec{23421.32, 1000.0, -0.0001, 1000.0};
        assertEquals(vec, {23421.32, 1000.0, -0.0001, 1000.0});
    }

    {
        // Initializer list with one element that gets converted.
        evec::EuclideanVector vec{5U};
        assertEquals(vec, {5});
    }

    {
        // Initializer list with two elements that get converted.
        evec::EuclideanVector vec{5, 10};
        assertEquals(vec, {5, 10});
    }

    {
        // Copy constructor.
        evec::EuclideanVector vec1(1000, 12.3);
        evec::EuclideanVector vec2{vec1};
        assertEquals(vec2, std::vector<double>(1000, 12.3));
    }

    {
        // Const copy constructor.
        const evec::EuclideanVector vec1(1000, 12.3);
        evec::EuclideanVector vec2{vec1};
        assertEquals(vec2, std::vector<double>(1000, 12.3));
    }

    {
        // Move constructor.
        evec::EuclideanVector vec1(10000, -1000.1);
        evec::EuclideanVector vec2{std::move(vec1)};
        assertEquals(vec1, {});
        assertEquals(vec2, std::vector<double>(10000, -1000.1));
    }
}
