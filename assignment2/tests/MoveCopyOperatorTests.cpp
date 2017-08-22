#include <cassert>
#include <sstream>

#include "EuclideanVector.h"

void movecopyoperator_tests() {
    std::stringstream stream;

    //Copy ASSIGNMENT
	evec::EuclideanVector a1 {1,2,3,4,5};
	evec::EuclideanVector a2 {6,7,8,9,10};

	a2 = a1;
	stream << a1 << std::endl;
	stream << a2 << std::endl;
    stream << (a1 == a2) << std::endl;

	//Move ASSIGNMENT
	evec::EuclideanVector b1 {1,2,3,4,5};
	evec::EuclideanVector b2 {6,7,8,9,10};
	b1 = std::move(b2);
	stream << b1 << std::endl;
	stream << b2 << std::endl;
    stream << (b1 != b2) << std::endl;

    //Multiple Move ASSIGNMENT
	evec::EuclideanVector c1 {1,2,3,4,5};
	evec::EuclideanVector c2 {6,7,8,9,10};
	c1 = std::move(c2);
    c1 = std::move(c2);
	stream << c1 << std::endl;
	stream << c2 << std::endl;
    stream << (c1 == c2) << std::endl;

    //Simple Swap
    evec::EuclideanVector d1 {1,2,3,4,5};
	evec::EuclideanVector d2 {6,7,8,9,10};
    evec::EuclideanVector tmp(std::move(d1));
    d1 = std::move(d2);
    d2 = std::move(tmp);
    stream << d1 << std::endl;
	stream << d2 << std::endl;
    stream << tmp << std::endl;


    std::string output_result = "[1 2 3 4 5]\n"
                                "[1 2 3 4 5]\n"
                                "1\n"
                                "[6 7 8 9 10]\n"
                                "[]\n"
                                "1\n"
                                "[]\n"
                                "[]\n"
                                "1\n"
                                "[6 7 8 9 10]\n"
                                "[1 2 3 4 5]\n"
                                "[]\n";
    assert(stream.str() == output_result);
}
