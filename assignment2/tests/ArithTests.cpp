#include <cassert>
#include <iostream>

#include "EuclideanVector.h"
#include "util.h"

void arith_tests() {
    {
        // Self increment
        evec::EuclideanVector vec1{3.0, 2.0};
        evec::EuclideanVector vec2{4.0, 1.0};
        vec1 += vec2;
        assertEquals(vec1, {7.0, 3.0});
    }

    {
        // Self increment with rvalue.
        evec::EuclideanVector vec1{3.0, 2.0};
        vec1 += evec::EuclideanVector{-2.0, -3.0};
        assertEquals(vec1, {1.0, -1.0});
    }

    {
        // Self increment with const.
        evec::EuclideanVector vec1{5.0, 5.0};
        const evec::EuclideanVector vec2{-5.0, -5.0};
        vec1 += vec2;
        assertEquals(vec1, {0.0, 0.0});
    }

    {
        // Empty increment.
        evec::EuclideanVector vec1(0);
        evec::EuclideanVector vec2(0);
        vec1 += vec2;
        assertEquals(vec1, {});
    }

    {
        // Moved increment.
        evec::EuclideanVector vec1(5, 3.0);
        evec::EuclideanVector temp_vec;
        temp_vec = std::move(vec1);
        evec::EuclideanVector vec2(0);
        vec1 += vec2;
        assertEquals(vec1, {});
    }

    {
        // Self decrement
        evec::EuclideanVector vec1{3.0, 2.0};
        evec::EuclideanVector vec2{4.0, 1.0};
        vec1 -= vec2;
        assertEquals(vec1, {-1.0, 1.0});
    }

    {
        // Self decrement with rvalue.
        evec::EuclideanVector vec1{3.0, 2.0};
        vec1 -= evec::EuclideanVector{-2.0, -3.0};
        assertEquals(vec1, {5.0, 5.0});
    }

    {
        // Self decrement with const.
        evec::EuclideanVector vec1{5.0, 5.0};
        const evec::EuclideanVector vec2{-5.0, -5.0};
        vec1 -= vec2;
        assertEquals(vec1, {10.0, 10.0});
    }

    {
        // Scalar multiplication.
        evec::EuclideanVector vec{5.0, 1.0};
        vec *= 2.2;
        assertEquals(vec, {11.0, 2.2});
    }

    {
        // Empty multiplication.
        evec::EuclideanVector vec(0);
        vec *= 2;
        assertEquals(vec, {});
    }

    {
        // Scalar division.
        evec::EuclideanVector vec{5.0, 1.0};
        vec /= 2.5;
        assertEquals(vec, {2.0, 0.4});
    }

    {
        // Empty division.
        evec::EuclideanVector vec(0);
        vec /= 2.1;
        assertEquals(vec, {});
    }

    {
        // Add two vectors that are rvalues.
        assertEquals(evec::EuclideanVector(2, 2.0) + evec::EuclideanVector(2, 3.0), {5.0, 5.0});
    }

    {
        // Add two const vectors.
        const evec::EuclideanVector vec1(3, 2.0);
        const evec::EuclideanVector vec2(3, 1.0);
        assertEquals(vec1 + vec2, {3.0, 3.0, 3.0});
    }

    {
        // Add two empty const vectors.
        const evec::EuclideanVector vec1(0);
        const evec::EuclideanVector vec2(0);
        assertEquals(vec1 + vec2, {});
    }

    {
        // Subtract two vectors that are rvalues.
        assertEquals(evec::EuclideanVector(2, 2.0) - evec::EuclideanVector(2, 3.0), {-1.0, -1.0});
    }

    {
        // Subtract two const vectors.
        const evec::EuclideanVector vec1(3, 2.0);
        const evec::EuclideanVector vec2(3, 1.0);
        assertEquals(vec1 - vec2, {1.0, 1.0, 1.0});
    }

    {
        // Subtract two empty const vectors.
        const evec::EuclideanVector vec1(0);
        const evec::EuclideanVector vec2(0);
        assertEquals(vec1 - vec2, {});
    }

    {
        // Dotprod two vectors that are rvalues.
        assert(almost_eq(evec::EuclideanVector(2, 2.0) * evec::EuclideanVector(2, 3.0), 12.0));
    }

    {
        // Dotprod two const vectors.
        const evec::EuclideanVector vec1(3, 2.0);
        const evec::EuclideanVector vec2(3, 1.0);
        assert(almost_eq(vec1 * vec2, 6.0));
    }

    {
        // Dotprod two empty const vectors. Implied legal by the spec.
        // (Sum of no elements should be 0).
        const evec::EuclideanVector vec1(0);
        const evec::EuclideanVector vec2(0);
        assert(almost_eq(vec1 * vec2, 0.0));
    }

    {
        // Scalar multiply an rvalue.
        assertEquals(evec::EuclideanVector(2, 2.0) * 2.0, {4.0, 4.0});
        assertEquals(0.0 * evec::EuclideanVector(2, 2.0), {0.0, 0.0});
    }

    {
        // Scalar multiply a const vectors.
        const evec::EuclideanVector vec(3, 2.0);
        assertEquals(vec * 3.0, {6.0, 6.0, 6.0});
        assertEquals(4.0 * vec, {8.0, 8.0, 8.0});
    }

    {
        // Scalar multiply a const empty const vectors.
        const evec::EuclideanVector vec(0);
        assertEquals(vec * 5.0, {});
        assertEquals(5.0 * vec, {});
    }

    {
        // Scalar divide an rvalue.
        assertEquals(evec::EuclideanVector(2, 2.0) / 2.0, {1.0, 1.0});
    }

    {
        // Scalar divide a const vectors.
        const evec::EuclideanVector vec(3, 2.0);
        assertEquals(vec / 3.0, {2.0 / 3.0, 2.0 / 3.0, 2.0 / 3.0});
    }

    {
        // Scalar divide a const empty const vectors.
        const evec::EuclideanVector vec(0);
        assertEquals(vec / 5.0, {});
    }
}
