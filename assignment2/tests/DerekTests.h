#ifndef DEREK_TESTS_H
#define DEREK_TESTS_H

#include <vector>

#include "EuclideanVector.h"

void derek_tests();

void assertEqualsSimple(const evec::EuclideanVector& myvec, std::vector<double> values);

#endif
