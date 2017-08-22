#include "MattTests.h"

#include <iostream>
#include <vector>
#include <list>
#include <cassert>
#include <cmath>
#include <sstream>
#include <numeric>
#include <limits>

#include "EuclideanVector.h"
#include "util.h"

using vec=evec::EuclideanVector;

void matt_tests() {
    assertEquals(vec(), {0});
    assertEquals(vec(2), {0, 0});
    assertEquals(vec(3, 1.5), {1.5, 1.5, 1.5});
    assertEquals(vec(3, 1), {1, 1, 1});
    assertEquals(vec{1}, {1});

    auto v1 = vec{1, 2, 3}; // move constructor?
    assertEquals(v1, {1, 2, 3});
    auto v2 = vec{v1}; // copy constructor
    assertEquals(v1, {1, 2, 3});
    assertEquals(v2, {1, 2, 3});
    v1 = v1;
    assertEquals(v1, {1, 2, 3});
    auto stdvec = static_cast<std::vector<double>>(v1);
    assertEquals(v1, stdvec);
    auto stdlist = static_cast<std::list<double>>(v1);
    assert(stdlist.size() == 3 && stdlist.front() == 1 && stdlist.back() == 3);

    assertEquals(vec(stdvec.begin(), stdvec.end()), {1, 2, 3});
    assertEquals(vec(stdvec.cbegin(), stdvec.cend()), {1, 2, 3});
    assertEquals(vec(stdlist.begin(), stdlist.end()), {1, 2, 3});
    assertEquals(vec(stdlist.cbegin(), stdlist.cend()), {1, 2, 3});

    assert(v1 == v2);
    assert((v1 == vec{1, 2, 3}));
    assert((v1 != vec{1, 2, 4}));
    assert((v1 != vec{1, 2}));
    assert((v1 * 10 == vec{10, 20, 30}));
    assert((v1 * -1 == vec{-1, -2, -3}));
    assert((v1 / 2 == vec{0.5, 1.0, 1.5}));
    assert((v1 + vec{5, 7, 9} == vec{6, 9, 12}));
    assert((v1 - vec{5, 7, 9} == vec{-4, -5, -6}));
    assert(almost_eq(v1.getEuclideanNorm(), sqrt(14)));
    v1[0] = 3;
    assert(v1[0] == 3 && v1.get(0) == 3);
    assert(almost_eq(v1.getEuclideanNorm(), sqrt(22)));
    v1 = vec{3, 4}; // move assignment
    assert((v1 == vec{3, 4}));
    assert(almost_eq(v1.getEuclideanNorm(), 5));
    v1 *= 2;
    assert((v1 == vec{6, 8}));
    assert(almost_eq(v1.getEuclideanNorm(), 10));
    v1 /= 2;
    assert((v1 == vec{3, 4}));
    assert(almost_eq(v1.getEuclideanNorm(), 5));
    assert((vec(4, 2.0).createUnitVector() == vec(4, 0.5)));
    v1 += vec{1, 3};
    assert((v1 == vec{4, 7}));
    assert(almost_eq(v1.getEuclideanNorm(), sqrt(16 + 49)));
    v1 -= vec{3, 1};
    assert((v1 == vec{1, 6}));
    assert(almost_eq(v1.getEuclideanNorm(), sqrt(37)));
    assert((v1 * vec{3, 5} == 33));
    assert((v1 * 2 == vec{2, 12}));
    assert((2 * v1 == vec{2, 12}));

    std::ostringstream os;
    os << v1;
    assert(os.str() == "[1 6]");
    v1 = v2; // copy assignment
    assert((v1 == vec{1, 2, 3}));

    auto v3 = v2 * 2;
    assert((v3 == vec{2, 4, 6}));

    vec v4{std::move(v2)}; // move constructor
    assert((v2 == vec(0)));
    assert((v4 == vec{1, 2, 3}));

    std::list<double> values = {12, 3, 4, 5, 6};
    assertEquals(vec(values.begin(), values.end()), {12, 3, 4, 5, 6});

    const auto constVec = vec{2, 2, 2, 2};
    assert(constVec.getNumDimensions() == 4);
    assert(constVec.get(2) == 2);
    assert(almost_eq(constVec.getEuclideanNorm(), 4));
    assert((constVec.createUnitVector() == vec{0.5, 0.5, 0.5, 0.5}));
    assert(constVec[0] == 2);
    assert((static_cast<std::vector<double>>(constVec) == std::vector<double>{2, 2, 2, 2}));
    assert((static_cast<std::list<double>>(constVec) == std::list<double>{2, 2, 2, 2}));
    assert(constVec == constVec);
    assert(constVec != v3);
    assert(constVec + constVec == constVec * 2);
    assert(constVec - vec(4) == constVec / 1);
    os << constVec;
}
