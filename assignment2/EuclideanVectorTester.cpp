#include <iostream>
#include <vector>
#include <list>

#include "EuclideanVector.h"

int main()
{
    evec::EuclideanVector a(2);

    std::list<double> l{ 1,2,3 };
    evec::EuclideanVector b(l.begin(),l.end());

    std::vector<double> v2{ 4,5,6,7 };
    evec::EuclideanVector c{ v2.begin(),v2.end() };

    std::vector<double> a1{ 5,4,3,2,1 };
    evec::EuclideanVector d{ a1.begin(),a1.end() };

    std::list<double> a2{ 9,0,8,6,7 };
    evec::EuclideanVector e{ a2.begin(),a2.end() };

    // use the copy constructor
    evec::EuclideanVector f{ e };

    std::cout << a.getNumDimensions() << ": " << a << "\n";
    std::cout << "D1:" << b.get(1) << " " << b << "\n";
    std::cout << c << " Euclidean Norm = " << c.getEuclideanNorm() << "\n";
    std::cout << d << " Unit Vector: " << d.createUnitVector() << " L = " << d.createUnitVector().getEuclideanNorm() << "\n";
    std::cout << e << "\n";
    std::cout << f << "\n";

    // test the move constructor
    evec::EuclideanVector g = std::move(f);
    std::cout << g << "\n";
    std::cout << f << "\n";

    // try operator overloading
    e += d;
    std::cout << e << "\n";

    evec::EuclideanVector h = e - g;
    std::cout << h << "\n";

    // test scalar multiplication
    h *= 2;
    std::cout << h << "\n";

    evec::EuclideanVector j = b / 2;
    std::cout << j << "\n";

    std::cout << "dot product = " << j * b << "\n";

    if (g == (e - d)) std::cout << "true" << "\n";
    if (j != b) std::cout << "false" << "\n";

    j[0] = 1;
    std::cout << j << "\n";

    // type cast from EuclideanVector to a std::vector
    std::vector<double> vj = j;

    // type cast from EuclideanVector to a std::vector
    std::list<double> lj = j;

    for (auto d : lj)
    {
        std::cout << d << "\n";
    }

    // list initialisation
    evec::EuclideanVector k{ 1, 2, 3 };
    std::cout << k << "\n";

    std::cout << "========================= my own test cases =========================" << "\n";

    evec::EuclideanVector scn;
    evec::EuclideanVector scn0{ 3, 3 };
    evec::EuclideanVector scn1{ 9, 8, 7, 6, 5 };
    evec::EuclideanVector scn2{ 1, 2, 3, 4, 5, 6 };
    evec::EuclideanVector scn3(scn1);
    evec::EuclideanVector scn4;

    std::cout << scn << "\n";
    scn0 = std::move(scn1);
    std::cout << scn0 << "\n";
    std::cout << scn1 << "\n";

    scn0 = -1 * scn0;
    std::cout << scn0 << "\n";
    scn0 = scn2 * (scn3 * scn0);
    std::cout << scn0 << "\n";
    std::cout << scn3 << "\n";

    scn4 = scn2;

    std::cout << scn4 << "\n";

    scn4 = (scn2 + (scn0 - scn2) * 3.33) / 12.33;

    std::cout << scn4 << "\n";

    scn4 += scn2;

    std::cout << scn4 << "\n";

    scn4 *= 2;

    std::cout << scn4 << "\n";

    scn4 /= 100;

    std::cout << scn4 << "\n";

    scn4 -= scn0;

    std::cout << scn4 << "\n";

    scn4 = scn4.createUnitVector();

    std::cout << scn4 << "\n";

    try
    {
        evec::EuclideanVector scn5 = scn + scn2;
    }
    catch (std::exception &ex)
    {
        std::cout << ex.what() << "\n";
    }

    try
    {
        scn += scn2;
    }
    catch (std::exception &ex)
    {
        std::cout << ex.what() << "\n";
    }

    try
    {
        evec::EuclideanVector scn5 = scn - scn2;
    }
    catch (std::exception &ex)
    {
        std::cout << ex.what() << "\n";
    }

    try
    {
        scn -= scn2;
    }
    catch (std::exception &ex)
    {
        std::cout << ex.what() << "\n";
    }

    try
    {
        double dot = scn * scn2;
        std::cout << dot << "\n";
    }
    catch (std::exception &ex)
    {
        std::cout << ex.what() << "\n";
    }

    try
    {
        scn1 = scn1;
    }
    catch (std::exception &ex)
    {
        std::cout << ex.what() << "\n";
    }

    try
    {
        scn1 = std::move(scn1);
    }
    catch (std::exception &ex)
    {
        std::cout << ex.what() << "\n";
    }

    try
    {
        std::cout << scn0[100] << "\n";
    }
    catch (std::exception &ex)
    {
        std::cout << ex.what() << "\n";
    }

    try
    {
        std::cout << scn0.get(-2) << "\n";
    }
    catch (std::exception &ex)
    {
        std::cout << ex.what() << "\n";
    }

    try
    {
        std::cout << (scn0 /= 0) << "\n";
    }
    catch (std::exception &ex)
    {
        std::cout << ex.what() << "\n";
    }

    try
    {
        std::cout << (scn0 / 0) << "\n";
    }
    catch (std::exception &ex)
    {
        std::cout << ex.what() << "\n";
    }

    try
    {
        evec::EuclideanVector zero_v{ 0, 0, 0, 0, 0, 0 };
        std::cout << (zero_v.createUnitVector()) << "\n";
    }
    catch (std::exception &ex)
    {
        std::cout << ex.what() << "\n";
    }

    std::cout << "=================== end of the program =================" << "\n";
}