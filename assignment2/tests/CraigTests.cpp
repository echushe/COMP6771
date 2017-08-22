#include "CraigTests.h"

#include "EuclideanVector.h"
#include "util.h"

#include <algorithm>
#include <cassert>
#include <vector>
#include <list>
#include <array>

void craig_tests() {
    // Test EuclideanVector with STL containers
    
    // Try sorting a vector of EuclideanVectors
    {
        std::vector<evec::EuclideanVector> vectors;
        vectors.emplace_back(4, 3.0);
        vectors.emplace_back(2, 5.0);
        vectors.emplace_back(3, 4.0);
        vectors.emplace_back(1, 6.0);

        std::sort(vectors.begin(), vectors.end(), 
                  [](const evec::EuclideanVector& lhs, 
                     const evec::EuclideanVector& rhs) {
                        return lhs.get(0) < rhs.get(0);
                     });

        assert((vectors[0] == evec::EuclideanVector{3.0, 3.0, 3.0, 3.0}));
        assert((vectors[1] == evec::EuclideanVector{4.0, 4.0, 4.0}));
        assert((vectors[2] == evec::EuclideanVector{5.0, 5.0}));
        assert((vectors[3] == evec::EuclideanVector{6.0}));
    }

    // Initialilse a vector with empty EuclideanVectors
    {
        std::vector<evec::EuclideanVector> matrix;
        matrix.resize(3);
        std::fill(matrix.begin(), matrix.end(), evec::EuclideanVector(3));
        
        assert((matrix[0] == evec::EuclideanVector{0.0, 0.0, 0.0}));
        assert((matrix[1] == evec::EuclideanVector{0.0, 0.0, 0.0}));
        assert((matrix[2] == evec::EuclideanVector{0.0, 0.0, 0.0}));
    }

    // Equality tests. Spec only requires that number of dimensions and values
    // are equal
    {
        evec::EuclideanVector ev1{ 1.0, 1.5, 3.0 }; 
        evec::EuclideanVector ev2{ 1.0, 1.5, 3.0 }; 
        evec::EuclideanVector ev3{ 2.0, 3.0, 6.0 }; 

        assert((ev1 == ev2));
        assert((ev2 == ev1));
        assert((ev1 != ev3));
        assert((ev1 == (ev3 / 2.0)));

        auto ev4 = std::move(ev2);
        assert((ev1 == ev4));

        ev2 = ev4;
        assert((ev1 == ev2) && (ev2 == ev4));

        evec::EuclideanVector ev5{ 1.0, 1.5, 3.0, 0.0 };

        assert((ev1 != ev5));
        assert((ev2 != ev5));
        assert((ev4 != ev5));

        ev4 = std::move(ev2);
        assert(ev2 != ev4);
        assert(!(ev2 == ev4));
        assert(ev2 == ev2);

        evec::EuclideanVector ev6(0);
        assert(ev6 != ev5);
        assert(ev6 == ev6);
    }

    // Casting tests
    {
        evec::EuclideanVector ev1{ 5.0, 2.0, 0.0 };
        auto vec1 = static_cast<std::vector<double>>(ev1);

        assert(almost_eq(vec1[0], ev1[0]));
        assert(almost_eq(vec1[1], ev1[1]));
        assert(almost_eq(vec1[2], ev1[2]));

        auto lst1 = static_cast<std::list<double>>(ev1);

        assert(almost_eq(lst1.front(), ev1[0]));
        lst1.pop_front();
        assert(almost_eq(lst1.front(), ev1[1]));
        lst1.pop_front();
        assert(almost_eq(lst1.front(), ev1[2]));
        lst1.pop_front();

        std::vector<double> vec2{ 1.0, 2.0, 3.0, 4.0 };
        evec::EuclideanVector ev2{ vec2.begin(), vec2.end() };

        assert((vec2 == static_cast<std::vector<double>>(ev2)));

        std::list<double> lst2{ 1.0, 2.0, 3.0, 4.0 };
        evec::EuclideanVector ev3{ lst2.begin(), lst2.end() };

        assert((lst2 == static_cast<std::list<double>>(ev2)));
    }

    // Const vector tests. Spec is completely clear but I think you should be
    // able to get the normal of a const evec
    {
        const evec::EuclideanVector e1{ 3.0, 4.0 };

        assert(almost_eq(e1.getEuclideanNorm(), 5.0));
        assert((e1.getNumDimensions() == 2));
        assert(almost_eq(e1[0], 3.0));
        assert(almost_eq(e1[1], 4.0));

        const auto unitE1 = e1.createUnitVector();

        assert(almost_eq(unitE1.getEuclideanNorm(), 1.0));
        assert((unitE1.getNumDimensions() == 2));
        assert(e1 == e1);
        assert(!(e1 != e1));
        assert(static_cast<std::list<double>>(e1) == static_cast<std::list<double>>(e1));
        assert(static_cast<std::vector<double>>(e1) == static_cast<std::vector<double>>(e1));
    }
}
