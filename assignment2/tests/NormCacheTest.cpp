#include <iostream>
#include <list>
#include <vector>

#include <cmath>
#include <ctime>

#include <assert.h>

#include "EuclideanVector.h"

// Test to ensure using a cache for the norm does not break the
// getEuclideanNorm function.
// Also tests the speed of caching vs recalculating norm
// when calling the getEuclideanNorm function many times.
void norm_cache_tests() {

	const double eps = 10e-6;

	// If a cache is being used the norm needs to be updated if the
	// Contents of the vector has changed

	evec::EuclideanVector e_test{2, 2, 2, 2};

	assert(std::fabs(e_test.getEuclideanNorm() - 4) < eps);


	e_test[3] = 8;
	// Ensure the norm is recalculated correctly
	assert(std::fabs(e_test.getEuclideanNorm() - 8.71779788708) < eps);


	// Maka a new vector
	evec::EuclideanVector e_tmp{1, 1, 1, 1};


	// Add the vector to e_test
	e_test += e_tmp;

	// Test the norm is still correct
	assert(std::fabs(e_test.getEuclideanNorm() - 10.3923048454) < eps);


	e_test *= 2;
	// Test the norm is still correct
	assert(std::fabs(e_test.getEuclideanNorm() - 20.7846096908) < eps);


	e_test /= 2;
	// Test the norm is still correct
	assert(std::fabs(e_test.getEuclideanNorm() - 10.3923048454) < eps);


	// Copy assign
	e_test = e_tmp;

	// Test the norm is still correct
	assert(std::fabs(e_test.getEuclideanNorm() - 2) < eps);


	evec::EuclideanVector e_tmp2{2, 2, 2, 2};

	// Move assign
	e_test = std::move(e_tmp2);
	assert(std::fabs(e_test.getEuclideanNorm() - 4) < eps);


	// Now test the speed of the cache

	// Make a really big evec
	evec::EuclideanVector e1 (100000);


	std::time_t t = clock();


	std::vector<double> norm_list_2;

	for (unsigned int i = 0; i < e1.getNumDimensions(); ++i)
	{
		// Check the size using the cache
		norm_list_2.push_back(e1.getEuclideanNorm());
	}


	std::time_t t2 = clock();


	std::vector<double> norm_list;

	for (unsigned int i = 0; i < e1.getNumDimensions(); ++i)
	{
		e1[e1.getNumDimensions() - 1 - i] = i;
		// The norm needs to be re-calculated every time.
		norm_list.push_back(e1.getEuclideanNorm());

	}

	std::time_t t3 = clock();

	// Can print to see the time difference
	std::cout << "With cache:  " << float(t2 - t)/CLOCKS_PER_SEC << std::endl;
	std::cout << "Without cache: " << float(t3 - t2)/CLOCKS_PER_SEC << std::endl;
	// This may not say much about the speed of caching due to optimisations etc...
	assert((t2 - t) < (t3 - t2));

}
