#include <cassert>
#include <list>
#include <sstream>
#include <vector>

#include "EuclideanVector.h"

void provided_tests() {
    std::stringstream stream;

    evec::EuclideanVector a(2);

    std::list<double> l {1,2,3};
    evec::EuclideanVector b{l.begin(),l.end()};

    std::vector<double> v2 {4,5,6,7};
    evec::EuclideanVector c{v2.begin(),v2.end()};

    std::vector<double> a1 {5,4,3,2,1};
    evec::EuclideanVector d{a1.begin(),a1.end()};

    std::list<double> a2 {9,0,8,6,7};
    evec::EuclideanVector e{a2.begin(),a2.end()};

    // use the copy constructor
    evec::EuclideanVector f{e};

    stream << a.getNumDimensions() << ": " << a << std::endl;
    stream << "D1:" << b.get(1) << " " << b << std::endl;
    stream << c << " Euclidean Norm = " << c.getEuclideanNorm() << std::endl;
    stream << d << " Unit Vector: " << d.createUnitVector() << " L = " << d.createUnitVector().getEuclideanNorm() << std::endl;
    stream << e << std::endl;
    stream << f << std::endl;

    // test the move constructor
    evec::EuclideanVector g = std::move(f);
    stream << g << std::endl;
    stream << f << std::endl;

    // try operator overloading
    e += d;
    stream << e << std::endl;

    evec::EuclideanVector h = e - g;
    stream << h << std::endl;

    // test scalar multiplication
    h *= 2;
    stream << h << std::endl;

    evec::EuclideanVector j = b / 2;
    stream << j << std::endl;

    stream << "dot product = " << j * b << std::endl;

    if (g == (e - d)) stream << "true" << std::endl;
    if (j != b ) stream << "false" << std::endl;

    j[0] = 1;
    stream << j << std::endl;

    // type cast from EuclideanVector to a std::vector
    std::vector<double> vj = j;

    // type cast from EuclideanVector to a std::vector
    std::list<double> lj = j;

    for (auto d : lj) {
        stream << d << std::endl;
    }

    // list initialisation
    evec::EuclideanVector k {1, 2, 3};
    stream << k << std::endl;

    std::string output_result = "2: [0 0]\n"
                                "D1:2 [1 2 3]\n"
                                "[4 5 6 7] Euclidean Norm = 11.225\n"
                                "[5 4 3 2 1] Unit Vector: [0.6742 0.53936 0.40452 0.26968 0.13484] L = 1\n"
                                "[9 0 8 6 7]\n"
                                "[9 0 8 6 7]\n"
                                "[9 0 8 6 7]\n"
                                "[]\n"
                                "[14 4 11 8 8]\n"
                                "[5 4 3 2 1]\n"
                                "[10 8 6 4 2]\n"
                                "[0.5 1 1.5]\n"
                                "dot product = 7\n"
                                "true\n"
                                "false\n"
                                "[1 1 1.5]\n"
                                "1\n"
                                "1\n"
                                "1.5\n"
                                "[1 2 3]\n";
    assert(stream.str() == output_result);
}
