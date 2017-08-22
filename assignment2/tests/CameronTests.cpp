#include "CameronTests.h"

#include <iostream>
#include <vector>
#include <list>
#include <cassert>
#include <cmath>
#include <sstream>
#include <numeric>
#include <limits>

#include "EuclideanVector.h"
#include "EuclideanVector.h" // testing double include
#include "util.h"

void cameron_tests() {

    // Testing default constructor
    {
        evec::EuclideanVector v;
        assert(v.getNumDimensions() == 1);
        assert(v.get(0) == 0.0);

        const evec::EuclideanVector v1;
        assert(v1.getNumDimensions() == 1);
        assert(v1.get(0) == 0.0);

        assert(almost_eq(v.getEuclideanNorm(), 0));
        assert(almost_eq(v1.getEuclideanNorm(), 0));
    }

    // Testing constructor w/ dimensions
    {
        evec::EuclideanVector v(1);
        assert(v.getNumDimensions() == 1);
        for (unsigned int i = 0; i < v.getNumDimensions(); ++i) {
            assert(v.get(i) == 0.0);
        }

        evec::EuclideanVector v1(5);
        assert(v1.getNumDimensions() == 5);
        for (unsigned int i = 0; i < v1.getNumDimensions(); ++i) {
            assert(v1.get(i) == 0.0);
        }

        evec::EuclideanVector v2(5000);
        assert(v2.getNumDimensions() == 5000);
        for (unsigned int i = 0; i < v2.getNumDimensions(); ++i) {
            assert(v2.get(i) == 0.0);
        }

        const evec::EuclideanVector v3(5);
        assert(v3.getNumDimensions() == 5);
        for (unsigned int i = 0; i < v3.getNumDimensions(); ++i) {
            assert(v3.get(i) == 0.0);
        }

        assert(almost_eq(v.getEuclideanNorm(), 0));
        assert(almost_eq(v1.getEuclideanNorm(), 0));
        assert(almost_eq(v2.getEuclideanNorm(), 0));
        assert(almost_eq(v3.getEuclideanNorm(), 0));
    }

    // Testing constructor w/ dimensions and magnitude
    {
        evec::EuclideanVector v(1, 5.2);
        assert(v.getNumDimensions() == 1);
        assert(v.get(0) == 5.2);

        evec::EuclideanVector v1(1, -5.2);
        assert(v1.getNumDimensions() == 1);
        assert(v1.get(0) == -5.2);

        const evec::EuclideanVector v2(2, 5.2);
        assert(v2.getNumDimensions() == 2);
        assert(v2.get(0) == 5.2);
        assert(v2.get(1) == 5.2);

        assert(almost_eq(v.getEuclideanNorm(), sqrt(5.2*5.2)));
        assert(almost_eq(v1.getEuclideanNorm(), sqrt(5.2*5.2)));
        assert(almost_eq(v2.getEuclideanNorm(), sqrt(5.2*5.2 + 5.2*5.2)));
    }

    // Testing constructor w/ Iterator
    {
        std::list<double> l {1,2,3};
        evec::EuclideanVector v1{l.begin(),l.end()};
        assert(v1.getNumDimensions() == 3);
        unsigned int index{0};
        for (auto it = l.begin(); it != l.end(); ++it) {
            assert(v1.get(index) == (*it));
            ++index;
        }
        assert(almost_eq(v1.getEuclideanNorm(), sqrt(1 + 4 + 9)));

        const std::list<double> l1 {1,2,3};
        const evec::EuclideanVector v2{l1.cbegin(),l1.cend()};
        assert(v2.getNumDimensions() == 3);
        assert(v2.get(0) == 1);
        assert(v2.get(1) == 2);
        assert(v2.get(2) == 3);
        assert(almost_eq(v2.getEuclideanNorm(), sqrt(1 + 4 + 9)));

        const evec::EuclideanVector v3{l.begin(),l.end()};
        assert(v3.getNumDimensions() == 3);
        assert(v3.get(0) == 1);
        assert(v3.get(1) == 2);
        assert(v3.get(2) == 3);
        assert(almost_eq(v3.getEuclideanNorm(), sqrt(1 + 4 + 9)));

        std::vector<double> v {4,5,6,7};
        evec::EuclideanVector v4{v.begin(),v.end()};
        assert(v4.getNumDimensions() == 4);
        for (unsigned int i = 0; i < v4.getNumDimensions(); ++i) {
            assert(v4.get(i) == v.at(i));
        }
        assert(almost_eq(v4.getEuclideanNorm(), sqrt(16 + 25 + 36 + 49)));

        const std::vector<double> v_c {4,5,6,7};
        const evec::EuclideanVector v5{v_c.begin(),v_c.end()};
        assert(v5.getNumDimensions() == 4);
        for (unsigned int i = 0; i < v5.getNumDimensions(); ++i) {
            assert(v5.get(i) == v_c.at(i));
        }
        assert(almost_eq(v5.getEuclideanNorm(), sqrt(16 + 25 + 36 + 49)));

        const evec::EuclideanVector v6{v.begin(),v.end()};
        assert(v6.getNumDimensions() == 4);
        for (unsigned int i = 0; i < v6.getNumDimensions(); ++i) {
            assert(v6.get(i) == v.at(i));
        }
        assert(almost_eq(v6.getEuclideanNorm(), sqrt(16 + 25 + 36 + 49)));
    }

    // Testing constructor w/ Initialiser list
    {
        std::vector<double> l {1,2,3,4};
        evec::EuclideanVector v{1, 2, 3, 4};
        assert(v.getNumDimensions() == 4);
        for (unsigned int i = 0; i < v.getNumDimensions(); ++i) {
            assert(v.get(i) == l.at(i));
        }
        assert(almost_eq(v.getEuclideanNorm(), sqrt(1 + 4 + 9 + 16)));

        std::vector<double> l1 {-1.0,2.1,5.2,9.0,-2.1};
        const evec::EuclideanVector v1{-1.0,2.1,5.2,9.0,-2.1};
        assert(v1.getNumDimensions() == 5);
        for (unsigned int i = 0; i < v1.getNumDimensions(); ++i) {
            assert(v1.get(i) == l1.at(i));
        }
        assert(almost_eq(v1.getEuclideanNorm(), sqrt(1 + 2.1*2.1 + 5.2*5.2 + 9.0*9.0 + 2.1*2.1)));
    }

    // Testing copy constructor
    {
        std::vector<double> l {1,2,3,4};
        evec::EuclideanVector v{1, 2, 3, 4};
        evec::EuclideanVector v2(v);
        const evec::EuclideanVector v3(v2);

        assert(v.getNumDimensions() == 4);
        for (unsigned int i = 0; i < v.getNumDimensions(); ++i) {
            assert(v.get(i) == l.at(i));
        }

        assert(v2.getNumDimensions() == 4);
        for (unsigned int i = 0; i < v2.getNumDimensions(); ++i) {
            assert(v2.get(i) == l.at(i));
        }

        assert(v3.getNumDimensions() == 4);
        for (unsigned int i = 0; i < v3.getNumDimensions(); ++i) {
            assert(v3.get(i) == l.at(i));
        }

        assert(almost_eq(v.getEuclideanNorm(), sqrt(1 + 4 + 9 + 16)));
        assert(almost_eq(v2.getEuclideanNorm(), sqrt(1 + 4 + 9 + 16)));
        assert(almost_eq(v3.getEuclideanNorm(), sqrt(1 + 4 + 9 + 16)));
    }

    // Testing move constructor
    {
        std::vector<double> l {1,2,3,4};
        evec::EuclideanVector v{1, 2, 3, 4};
        evec::EuclideanVector v2 = std::move(v);

        assert(v2.getNumDimensions() == 4);
        for (unsigned int i = 0; i < v2.getNumDimensions(); ++i) {
            assert(v2.get(i) == l.at(i));
        }

        assert(v.getNumDimensions() == 0);
        //assert(fabs(v2.getEuclideanNorm() - sqrt(1 + 4 + 9 + 16)) < std::numeric_limits<double>::epsilon());

        const evec::EuclideanVector v3 = std::move(v2);
        assert(v2.getNumDimensions() == 0);
        assert(v3.getNumDimensions() == 4);
        for (unsigned int i = 0; i < v3.getNumDimensions(); ++i) {
            assert(v3.get(i) == l.at(i));
        }

        assert(almost_eq(v.getEuclideanNorm(), 0));
        assert(almost_eq(v2.getEuclideanNorm(), 0));
        assert(almost_eq(v3.getEuclideanNorm(), sqrt(1 + 4 + 9 + 16)));
    }


    // Testing copy assignment;
    {
        std::vector<double> l {1,2,3,4};
        evec::EuclideanVector v{1, 2, 3, 4};
        evec::EuclideanVector v2;
        v2 = v;

        assert(v2.getNumDimensions() == 4);
        for (unsigned int i = 0; i < v2.getNumDimensions(); ++i) {
            assert(v2.get(i) == l.at(i));
        }

        assert(v.getNumDimensions() == 4);
        for (unsigned int i = 0; i < v.getNumDimensions(); ++i) {
            assert(v.get(i) == l.at(i));
        }

        assert(almost_eq(v.getEuclideanNorm(), sqrt(1 + 4 + 9 + 16)));
        assert(almost_eq(v2.getEuclideanNorm(), sqrt(1 + 4 + 9 + 16)));

        const evec::EuclideanVector const_v{1,2,3,4};
        evec::EuclideanVector copy;
        copy = const_v;

        assert(const_v.getNumDimensions() == 4);
        for (unsigned int i = 0; i < const_v.getNumDimensions(); ++i) {
            assert(const_v.get(i) == l.at(i));
        }

        assert(copy.getNumDimensions() == 4);
        for (unsigned int i = 0; i < copy.getNumDimensions(); ++i) {
            assert(copy.get(i) == l.at(i));
        }

        assert(almost_eq(const_v.getEuclideanNorm(), sqrt(1 + 4 + 9 + 16)));
        assert(almost_eq(copy.getEuclideanNorm(), sqrt(1 + 4 + 9 + 16)));
    }

    // Testing move assignment"
    {
        std::vector<double> l {1,2,3,4};
        evec::EuclideanVector v{1, 2, 3, 4};
        evec::EuclideanVector v2;
        v2 = std::move(v);

        assert(v2.getNumDimensions() == 4);
        for (unsigned int i = 0; i < v2.getNumDimensions(); ++i) {
            assert(v2.get(i) == l.at(i));
        }

        assert(v.getNumDimensions() == 0);

        assert(almost_eq(v.getEuclideanNorm(), 0));
        assert(almost_eq(v2.getEuclideanNorm(), sqrt(1 + 4 + 9 + 16)));
    }

    // Testing getEuclideanNorm
    {
        evec::EuclideanVector v{1, 2, 3};
        assert(almost_eq(v.getEuclideanNorm(), sqrt(14)));

        const evec::EuclideanVector v1{1, 1, 1, 1};
        assert(v1.getEuclideanNorm() == 2);
    }

    // Testing createUnitVector
    {
        evec::EuclideanVector v{1, 2, 3};
        evec::EuclideanVector u = v.createUnitVector();

        assert(u.getNumDimensions() == 3);
        assert(almost_eq(u.getEuclideanNorm(), 1));
        for (unsigned int i = 0; i < u.getNumDimensions(); ++i) {
            assert(almost_eq(u.get(i), v.get(i) / sqrt(14)));
        }

        const evec::EuclideanVector v1{1, 1, 1, 1};
        const evec::EuclideanVector u1 = v1.createUnitVector();
        assert(u1.getNumDimensions() == 4);
        assert(almost_eq(u1.getEuclideanNorm(), 1));
        for (unsigned int i = 0; i < u1.getNumDimensions(); ++i) {
            assert(almost_eq(u1.get(i), v1.get(i) / 2.0));
        }
    }


    // Testing [] operator overload
    {
        evec::EuclideanVector v{1, 2, 3};
        assert(almost_eq(v.getEuclideanNorm(), sqrt(1 + 4 + 9)));
        assert(v[0] == 1);
        assert(v[1] == 2);
        assert(v[2] == 3);
        v[0] = 5;
        assert(v[0] == 5);
        assert(almost_eq(v.getEuclideanNorm(), sqrt(25 + 4 + 9)));

        const evec::EuclideanVector v1{1, 1, 1, 1};
        assert(v1[0] == 1);
        assert(v1[1] == 1);
        assert(v1[2] == 1);
        assert(v1[3] == 1);
        assert(almost_eq(v1.getEuclideanNorm(), 2.0));
    }

    // Testing += operator overload
    {
        evec::EuclideanVector v{1, 2, 3};
        evec::EuclideanVector v1{1, 1, 1};
        v += v1;
        assert(v[0] == 2);
        assert(v[1] == 3);
        assert(v[2] == 4);
        assert(almost_eq(v.getEuclideanNorm(), sqrt(4 + 9 + 16)));

        evec::EuclideanVector v2{1, 2, 3};
        const evec::EuclideanVector v3{1, 1, 1};
        v2 += v3;
        assert(v2[0] == 2);
        assert(v2[1] == 3);
        assert(v2[2] == 4);
        assert(almost_eq(v2.getEuclideanNorm(), sqrt(4 + 9 + 16)));
    }

    // Testing -= operator overload
    {
        evec::EuclideanVector v{1, 2, 3};
        evec::EuclideanVector v1{1, 1, 1};
        v -= v1;
        assert(v[0] == 0);
        assert(v[1] == 1);
        assert(v[2] == 2);
        assert(almost_eq(v.getEuclideanNorm(), sqrt(5)));

        evec::EuclideanVector v2{1, 2, 3};
        const evec::EuclideanVector v3{1, 1, 1};
        v2 -= v3;
        assert(v2[0] == 0);
        assert(v2[1] == 1);
        assert(v2[2] == 2);
        assert(almost_eq(v2.getEuclideanNorm(), sqrt(5)));
    }

    // Testing *= operator overload
    {
        evec::EuclideanVector v{1, 2, 3};
        v *= 5;
        assert(v[0] == 5);
        assert(v[1] == 10);
        assert(v[2] == 15);
        assert(almost_eq(v.getEuclideanNorm(), sqrt(25 + 100 + 15*15)));

        evec::EuclideanVector v1{1, 1, 1};
        v1 *= -1.1;
        assert(v1[0] == -1.1);
        assert(v1[1] == -1.1);
        assert(v1[2] == -1.1);
        assert(almost_eq(v1.getEuclideanNorm(), sqrt(1.1*1.1 + 1.1*1.1 + 1.1*1.1)));
    }

    // Testing /= operator overload
    {
        evec::EuclideanVector v{1, 2, 3};
        v /= 2;
        assert(v[0] == 1.0/2);
        assert(v[1] == 1);
        assert(v[2] == 3.0/2);
        assert(almost_eq(v.getEuclideanNorm(), sqrt(1.0/4 + 1 + 9.0/4)));

        evec::EuclideanVector v1{1, 1, 1};
        v1 /= -1;
        assert(v1[0] == -1);
        assert(v1[1] == -1);
        assert(v1[2] == -1);
        assert(almost_eq(v1.getEuclideanNorm(), sqrt(3)));
    }

    // Testing implicit type conversion
    {
        evec::EuclideanVector v{1, 2, 3};
        const std::vector<double> vf = v;
        std::list<double> lf = v;

        assert(vf.size() == 3);
        assert(vf[0] == 1);
        assert(vf[1] == 2);
        assert(vf[2] == 3);

        assert(lf.size() == 3);
        assert(lf.front() == 1);
        assert(lf.back() == 3);

        const evec::EuclideanVector v2{1, 2, 3};
        const std::vector<double> vf2 = v;
        const std::list<double> lf2 = v;

        assert(vf2.size() == 3);
        assert(vf2[0] == 1);
        assert(vf2[1] == 2);
        assert(vf2[2] == 3);

        assert(lf2.size() == 3);
        assert(lf2.front() == 1);
        assert(lf2.back() == 3);
    }

    // Testing == friend function
    {
        evec::EuclideanVector a{1, 2, 3};
        evec::EuclideanVector b{1, 2, 3};
        const evec::EuclideanVector c{1, 2};
        assert((a == b) == true);
        assert((b == a) == true);
        assert((c == a) == false);
        assert((a == c) == false);
    }

    // Testing != friend function
    {
        evec::EuclideanVector a{1, 2, 3};
        const evec::EuclideanVector b{1, 2, 3};
        evec::EuclideanVector c{1, 2};
        assert((a != b) == false);
        assert((a != c) == true);
    }

    // Testing + friend function
    {
        evec::EuclideanVector a;
        evec::EuclideanVector b{1, 1, 1};
        const evec::EuclideanVector c{1, 1, 1};
        a = b + c;
        assert(a[0] == 2);
        assert(a[1] == 2);
        assert(a[2] == 2);

        assert(b[0] == 1);
        assert(b[1] == 1);
        assert(b[2] == 1);

        assert(c[0] == 1);
        assert(c[1] == 1);
        assert(c[2] == 1);
    }

    // Testing - friend function
    {
        evec::EuclideanVector a;
        const evec::EuclideanVector b{1, 1, 1};
        evec::EuclideanVector c{1, 1, 1};
        a = b - c;
        assert(a[0] == 0);
        assert(a[1] == 0);
        assert(a[2] == 0);

        assert(b[0] == 1);
        assert(b[1] == 1);
        assert(b[2] == 1);

        assert(c[0] == 1);
        assert(c[1] == 1);
        assert(c[2] == 1);
    }

    // Testing * dot product friend function
    {
        const evec::EuclideanVector b{1, 2};
        const evec::EuclideanVector c{3, 4};
        assert((b * c) == 11);
    }

    // Testing * scalar product friend function
    {
        const evec::EuclideanVector b{1, 2};
        evec::EuclideanVector a;
        a = 3 * b;
        assert(a[0] == 3);
        assert(a[1] == 6);

        a = b * 3;
        assert(a[0] == 3);
        assert(a[1] == 6);
    }

    // Testing / scalar division friend function
    {
        const evec::EuclideanVector b{1, 2};
        evec::EuclideanVector a;
        a = b / 2;
        assert(a[0] == 1.0/2);
        assert(a[1] == 1);
    }

    // Testing << stream append friend function
    {
        evec::EuclideanVector b{1, 2, 3};
        std::ostringstream os;
        os << b;
        assert(os.str() == "[1 2 3]");

        evec::EuclideanVector c = std::move(b);
        std::ostringstream os2;
        os2 << b;
        assert(os2.str() == "[]");

        const evec::EuclideanVector d{2, 3, 4};
        std::ostringstream os3;
        os3 << d;
        assert(os3.str() == "[2 3 4]");
    }
}
