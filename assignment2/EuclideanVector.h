/*********************************************
*
*          COMP6771 Assignment 2
*             Chunnan Sheng
*               z5100764
*
*********************************************/

#pragma once
#include <vector>
#include <list>
#include <ostream>
#include <string>
#include <limits>

// All integers are 64bit wide here
typedef long long lint;

// Definition of namespace evec
namespace evec
{
    // Exception descriptions
    // const std::string zero_dim("Dimension of zero size is invalid for EuclideanVector!");
    const std::string out_range("EuclideanVector: Index out of range!");
    const std::string diff_dim("EuclideanVector: Numbers of dimensons should be the same!");
    // const std::string zero_div("EuclideanVector: Divisor should not be zero!");
    const std::string zero_norm("EuclideanVector: Unit vector of a vector whose norm is zero cannot be acquired!");
    // const std::string assn_self("EuclideanVector: Cannot copy, assign or move an instance to itself!");

    const double inf = std::numeric_limits<double>::infinity();
    const double minus_inf = std::numeric_limits<double>::infinity() * (-1);

    // Definition of class EuclideanVector
    class EuclideanVector
    {
    private:
        // long long (64bit integer) as amount of dimensions
        lint m_dim;
        // Pointer to the dynamic norm value of this vector
        // default value of this pointer is nullptr until there is once calculation
        // of norm of this vector.
        // The double instance of this pointer will be deleted and this pointer will 
        // be changed back to nullptr if there is a modification of this vector.
        mutable double *m_norm;
        // This pointer will point to to the dynamic memory of vector dimensions
        double *m_data;

    public:
        // A default constructor that takes number of dimensions
        EuclideanVector(lint dim = 1);
        
        // A constructor that takes number of dimensions and an arbitrary value for all dimensions
        EuclideanVector(lint dim, double m);

        // A constructor that takes an iterator of a vector<double>
        EuclideanVector(const std::vector<double>::iterator &begin, const std::vector<double>::iterator &end);

        // A constructor that takes a const iterator of a vector<double>
        EuclideanVector(const std::vector<double>::const_iterator &begin, const std::vector<double>::const_iterator &end);

        // A constructor that takes an iterator of a list<double>
        EuclideanVector(const std::list<double>::iterator &begin, const std::list<double>::iterator &end);

        // A constructor that takes a const iterator of a list<double>
        EuclideanVector(const std::list<double>::const_iterator &begin, const std::list<double>::const_iterator &end);

        // A construtor that takes a list of double values
        EuclideanVector(std::initializer_list<double> list);

        //Copy constructor
        EuclideanVector(const EuclideanVector &other);

        //Move constructor
        EuclideanVector(EuclideanVector &&other);

        //Destructor
        ~EuclideanVector();

    public:
        //Copy assignment
        EuclideanVector & operator = (const EuclideanVector &other);

        //Move assignment
        EuclideanVector & operator = (EuclideanVector &&other);

        // A [] operator that would change the value of a dimension
        double & operator [] (lint index);

        // A [] operator accessing value of a dimension
        double operator [] (lint index) const;

        // Example: b = -a
        EuclideanVector operator - () const;

        // Adding right vectors to itself
        EuclideanVector & operator += (const EuclideanVector &right);

        // Subtracting right vectors from itself
        EuclideanVector & operator -= (const EuclideanVector &right);

        // Scalar multiplication
        EuclideanVector & operator *= (double scalar);

        // Scalar division
        EuclideanVector & operator /= (double scalar);

        // Implicitly convert into a list<double> instance
        operator std::list<double>() const;

        // Implicitly convert into a vector<double> instance
        operator std::vector<double>() const;

    public:

        lint getNumDimensions() const;

        double get(lint index) const;

        // This function caches calculated norm value.
        // The cache will be removed if the EuclideanVector is updated.
        double getEuclideanNorm() const;

        // Create and return a unit vector
        EuclideanVector createUnitVector() const;

    private:
        void destroyMe();
        void destroyMeWithoutMemoryRelease();
    };

    /************************************************************
          I have tried my best not to use friend functions
    *************************************************************/

    // Overloading of a == b
    bool operator == (const EuclideanVector &left, const EuclideanVector &right);

    // Overloading of a != b
    bool operator != (const EuclideanVector &left, const EuclideanVector &right);

    // Overloading of a + b
    EuclideanVector operator + (const EuclideanVector &left, const EuclideanVector &right);

    // Overloading of a - b
    EuclideanVector operator - (const EuclideanVector &left, const EuclideanVector &right);

    // Overloading of a * b (dot product)
    double operator * (const EuclideanVector &left, const EuclideanVector &right);

    // Overloading of a * b, b is a double
    EuclideanVector operator * (const EuclideanVector &left, double scalar);

    // Overloading of a * b, a is a double
    EuclideanVector operator * (double scalar, const EuclideanVector &right);

    // Overloading of a / b, b is a double
    EuclideanVector operator / (const EuclideanVector &left, double scalar);

    // Overloading of output stream << operator
    std::ostream& operator<<(std::ostream& os, const EuclideanVector &v);
}

