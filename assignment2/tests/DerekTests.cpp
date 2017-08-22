#include "DerekTests.h"

#include <cassert>
#include <limits>
#include <cmath>

#include "EuclideanVector.h"
#include "util.h"

void derek_tests() {
   // --- more norm cache tests ---
   evec::EuclideanVector v1{5, 3, 5, 1, 7};
   assert(almost_eq(v1.getEuclideanNorm(), sqrt(109)));

   // check that cached norm is not copied to newly created unit vector
   evec::EuclideanVector v2 = v1.createUnitVector();
   assert(almost_eq(v2.getEuclideanNorm(), 1));

   // --- division by zero ---
   auto inf = std::numeric_limits<double>::infinity();
   assertEqualsSimple(v1/0, std::vector<double>(5, inf));              // +inf
   assertEqualsSimple((v1 * -1)/0, std::vector<double>(5, inf * -1));  // -inf
}

void assertEqualsSimple(const evec::EuclideanVector& myvec, std::vector<double> values) {
   assert(myvec.getNumDimensions() == values.size());
   for (auto i = 0U; i < values.size(); i++) {
      assert(myvec.get(i) == values.at(i));
   }
}

