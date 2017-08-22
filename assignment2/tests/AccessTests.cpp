#include <cassert>
#include <cmath>

#include "EuclideanVector.h"
#include "util.h"

void access_tests() {
    {
        // Simple get.
        evec::EuclideanVector vec;
        assert(vec.get(0) == 0.0);
    }

    {
        // Const get.
        const evec::EuclideanVector vec;
        assert(vec.get(0) == 0.0);
    }

    {
        // Zero vector Euclidean norm.
        evec::EuclideanVector vec;
        assert(vec.getEuclideanNorm() == 0.0);
    }

    {
        // Euclidean norm with negatives.
        evec::EuclideanVector vec{-10.0, 5.0, -5.0, 10.0, 0.0};
        assert(almost_eq(vec.getEuclideanNorm(), ::sqrt(250)));
    }

    {
        // Euclidean norm for 0 dim-vector. Arguably not defined in the spec.
        evec::EuclideanVector vec(0);
        assert(vec.getEuclideanNorm() == 0.0);
    }

    {
        // Norm of const vector.
        const evec::EuclideanVector vec{3.0, 4.0};
        assert(vec.getEuclideanNorm() == 5.0);
    }

    {
        // Euclidean norm for move constructed vector.
        evec::EuclideanVector vec1(4, 1.0);
        evec::EuclideanVector vec2{std::move(vec1)};
        assert(vec1.getEuclideanNorm() == 0.0);
        assert(vec2.getEuclideanNorm() == 2.0);
    }

    {
        // Euclidean norm for move assigned vector.
        evec::EuclideanVector vec1(9, 1.0);
        evec::EuclideanVector vec2;
        vec2 = std::move(vec1);
        assert(vec1.getEuclideanNorm() == 0.0);
        assert(vec2.getEuclideanNorm() == 3.0);
    }

    {
        // Simple unit vector.
        auto vec = evec::EuclideanVector(4, 1.0).createUnitVector();
        assertEquals(vec, {0.5, 0.5, 0.5, 0.5});
    }

    {
        // Unit vector from const.
        const evec::EuclideanVector vec{4.0, 9.0};
        assertEquals(vec.createUnitVector(), {4.0 / sqrt(97), 9.0 / sqrt(97)});
    }

    {
        // Standard index read.
        evec::EuclideanVector vec{1.0, 2.0, 3.0};
        assert(vec[0] == 1.0);
    }

    {
        // Index read from const vector.
        const evec::EuclideanVector vec{5.0, 6.0};
        assert(vec[1] == 6.0);
    }

    {
        // Standard index write.
        evec::EuclideanVector vec{-1.0, -2.0};
        vec[0] = 1.0;
        assert(vec[0] == 1.0 && vec[1] == -2.0);
    }

    {
        // Writing to a reference taken by indexing.
        evec::EuclideanVector vec;
        auto& ref = vec[0];
        ref = 10.0;
        assert(vec[0] == 10.0);
    }
}
