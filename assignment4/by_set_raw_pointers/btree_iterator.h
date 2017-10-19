#ifndef BTREE_ITERATOR_H
#define BTREE_ITERATOR_H

#include <iterator>
#include <cassert>

/**
 * You MUST implement the btree iterators as (an) external class(es) in this file.
 * Failure to do so will result in a total mark of 0 for this deliverable.
 **/

template <typename Type> class btree;

// iterator related interface stuff here; would be nice if you called your
// iterator class btree_iterator (and possibly const_btree_iterator)
/************************
*
*    This is an iterator base class
*
*************************/

template <typename Type>
class base_btree_iterator
{
protected:
    typename btree<Type>::tree_ele_ptr m_ele;
    typename btree<Type>::tree_ele_ptr m_rend;
    typename btree<Type>::tree_ele_ptr m_end;

public:
    typedef Type* pointer;
    typedef Type& reference;
    /***********************
    * Constructors
    ************************/
public:
    base_btree_iterator(
        const typename btree<Type>::tree_ele_ptr & ele,
        const typename btree<Type>::tree_ele_ptr & rend,
        const typename btree<Type>::tree_ele_ptr & end);

    base_btree_iterator(const base_btree_iterator & other);

    base_btree_iterator(base_btree_iterator && other);

    base_btree_iterator();

    // The destructor
public:
    ~base_btree_iterator();

    /***********************
    * Operator overloads
    ************************/
public:
    base_btree_iterator & operator = (const base_btree_iterator & other);

    base_btree_iterator & operator = (base_btree_iterator && other);

    virtual base_btree_iterator & operator ++ ();

    virtual base_btree_iterator operator ++ (int);

    virtual base_btree_iterator & operator -- ();

    virtual base_btree_iterator operator -- (int);

    virtual base_btree_iterator clone();

    bool operator == (const base_btree_iterator & right) const;

    bool operator != (const base_btree_iterator & right) const;
};

template<typename Type>
inline base_btree_iterator<Type>::base_btree_iterator(
    const typename btree<Type>::tree_ele_ptr & ele,
    const typename btree<Type>::tree_ele_ptr & rend,
    const typename btree<Type>::tree_ele_ptr & end)
    : m_ele{ ele }, m_rend{ rend }, m_end{ end }
{
}

template<typename Type>
inline base_btree_iterator<Type>::base_btree_iterator(const base_btree_iterator & other)
    : m_ele{ other.m_ele }, m_rend{ other.m_rend }, m_end{ other.m_end }
{
}

template<typename Type>
inline base_btree_iterator<Type>::base_btree_iterator(base_btree_iterator && other)
    : m_ele{ other.m_ele }, m_rend{ other.m_rend }, m_end{ other.m_end }
{
    other.m_ele = nullptr;
    other.m_rend = nullptr;
    other.m_end = nullptr;
}

template<typename Type>
inline base_btree_iterator<Type>::base_btree_iterator()
    : m_ele{ nullptr },
    m_rend{ nullptr },
    m_end{ nullptr }
{
}

template<typename Type>
inline base_btree_iterator<Type>::~base_btree_iterator()
{
}

template<typename Type>
inline base_btree_iterator<Type> & base_btree_iterator<Type>::operator=(const base_btree_iterator & other)
{
    this->m_ele = other.m_ele;
    this->m_rend = other.m_rend;
    this->m_end = other.m_end;
}

template<typename Type>
inline base_btree_iterator<Type> & base_btree_iterator<Type>::operator=(base_btree_iterator && other)
{
    this->m_ele = other.m_ele;
    this->m_rend = other.m_rend;
    this->m_end = other.m_end;
    other.m_ele = std::weak_ptr<typename btree<Type>::tree_element>();
    other.m_rend = std::weak_ptr<typename btree<Type>::tree_element>();
    other.m_end = std::weak_ptr<typename btree<Type>::tree_element>();
}

template<typename Type>
inline base_btree_iterator<Type> & base_btree_iterator<Type>::operator++()
{
    assert(this->m_end != this->m_ele);
    this->m_ele = this->m_ele->m_next;

    return *this;
}

template<typename Type>
inline base_btree_iterator<Type> base_btree_iterator<Type>::operator++(int)
{
    assert(this->m_end != this->m_ele);
    this->m_ele = this->m_ele->m_next;

    return this->clone();
}

template<typename Type>
inline base_btree_iterator<Type> & base_btree_iterator<Type>::operator--()
{
    assert(this->m_rend != this->m_ele->m_prev);
    this->m_ele = this->m_ele->m_prev;

    return *this;
}

template<typename Type>
inline base_btree_iterator<Type> base_btree_iterator<Type>::operator--(int)
{
    assert(this->m_rend != this->m_ele->m_prev);
    this->m_ele = this->m_ele->m_prev;

    return this->clone();
}

template<typename Type>
inline base_btree_iterator<Type> base_btree_iterator<Type>::clone()
{
    return base_btree_iterator(*this);
}

template <typename Type>
bool base_btree_iterator<Type>::operator == (const base_btree_iterator & right) const
{
    return (this->m_ele == right.m_ele) &&
        (this->m_rend == right.m_rend) &&
        (this->m_end == right.m_end);
}

template <typename Type>
bool base_btree_iterator<Type>::operator != (const base_btree_iterator & right) const
{
    bool equal = !(*(this) == right);
    return equal;
}

/****************************************************************************************/

template <typename Type>
class btree_iterator : public base_btree_iterator <Type>
{
public:
    btree_iterator(
        const typename btree<Type>::tree_ele_ptr & ele,
        const typename btree<Type>::tree_ele_ptr & rend,
        const typename btree<Type>::tree_ele_ptr & end);

    btree_iterator(const btree_iterator & other);

    btree_iterator(btree_iterator && other);

    typename base_btree_iterator<Type>::reference operator * () const;

    typename base_btree_iterator<Type>::pointer operator -> () const;

    virtual base_btree_iterator<Type> clone();
};

template<typename Type>
inline btree_iterator<Type>::btree_iterator(
    const typename btree<Type>::tree_ele_ptr & ele,
    const typename btree<Type>::tree_ele_ptr & rend,
    const typename btree<Type>::tree_ele_ptr & end)
    : base_btree_iterator<Type>::base_btree_iterator(ele, rend, end)
{
}

template<typename Type>
inline btree_iterator<Type>::btree_iterator(const btree_iterator & other)
    : base_btree_iterator<Type>::base_btree_iterator(other)
{
}

template<typename Type>
inline btree_iterator<Type>::btree_iterator(btree_iterator && other)
    : base_btree_iterator<Type>::base_btree_iterator(other)
{
}

template<typename Type>
inline typename base_btree_iterator<Type>::reference btree_iterator<Type>::operator*() const
{
    return *(this->m_ele->m_value);
}

template<typename Type>
inline typename base_btree_iterator<Type>::pointer btree_iterator<Type>::operator->() const
{
    return this->m_ele->m_value.get();
}

template<typename Type>
inline base_btree_iterator<Type> btree_iterator<Type>::clone()
{
    return btree_iterator(*this);
}


/********************************************************************************/

template <typename Type>
class btree_reverse_iterator : public btree_iterator <Type>
{
public:
    btree_reverse_iterator(
        const typename btree<Type>::tree_ele_ptr & ele,
        const typename btree<Type>::tree_ele_ptr & rend,
        const typename btree<Type>::tree_ele_ptr & end);

    btree_reverse_iterator(const btree_reverse_iterator & other);

    btree_reverse_iterator(btree_reverse_iterator && other);

    virtual base_btree_iterator<Type> & operator ++ ();

    virtual base_btree_iterator<Type> operator ++ (int);

    virtual base_btree_iterator<Type> & operator -- ();

    virtual base_btree_iterator<Type> operator -- (int);

    virtual base_btree_iterator<Type> clone();
};

template<typename Type>
inline btree_reverse_iterator<Type>::btree_reverse_iterator(
    const typename btree<Type>::tree_ele_ptr & ele,
    const typename btree<Type>::tree_ele_ptr & rend,
    const typename btree<Type>::tree_ele_ptr & end)
    : btree_iterator<Type>::btree_iterator(ele, rend, end)
{
}

template<typename Type>
inline btree_reverse_iterator<Type>::btree_reverse_iterator(const btree_reverse_iterator & other)
    : btree_iterator<Type>::btree_iterator(other)
{
}

template<typename Type>
inline btree_reverse_iterator<Type>::btree_reverse_iterator(btree_reverse_iterator && other)
    : btree_iterator<Type>::btree_iterator(other)
{
}

template<typename Type>
inline base_btree_iterator<Type> & btree_reverse_iterator<Type>::operator++()
{
    assert(this->m_rend != this->m_ele);
    this->m_ele = this->m_ele->m_prev;

    return *this;
}

template<typename Type>
inline base_btree_iterator<Type> btree_reverse_iterator<Type>::operator++(int)
{
    assert(this->m_rend != this->m_ele);
    this->m_ele = this->m_ele->m_prev;

    return this->clone();
}

template<typename Type>
inline base_btree_iterator<Type> & btree_reverse_iterator<Type>::operator--()
{
    assert(this->m_end != this->m_ele->m_next);
    this->m_ele = this->m_ele->m_next;

    return *this;
}

template<typename Type>
inline base_btree_iterator<Type> btree_reverse_iterator<Type>::operator--(int)
{
    assert(this->m_end != this->m_ele->m_next);
    this->m_ele = this->m_ele->m_next;

    return this->clone();
}

template<typename Type>
inline base_btree_iterator<Type> btree_reverse_iterator<Type>::clone()
{
    return btree_reverse_iterator(*this);
}

/*****************************************************************************************/

template <typename Type>
class const_btree_iterator : public base_btree_iterator <Type>
{
public:
    const_btree_iterator(const typename btree<Type>::tree_ele_ptr & ele,
        const typename btree<Type>::tree_ele_ptr & rend,
        const typename btree<Type>::tree_ele_ptr & end);

    const_btree_iterator(const const_btree_iterator & other);

    const_btree_iterator(const_btree_iterator && other);

    const typename base_btree_iterator<Type>::reference operator * () const;

    const typename base_btree_iterator<Type>::pointer operator -> () const;

    virtual base_btree_iterator<Type> clone();
};

template<typename Type>
inline const_btree_iterator<Type>::const_btree_iterator(
    const typename btree<Type>::tree_ele_ptr & ele,
    const typename btree<Type>::tree_ele_ptr & rend,
    const typename btree<Type>::tree_ele_ptr & end)
    : base_btree_iterator<Type>::base_btree_iterator(ele, rend, end)
{
}

template<typename Type>
inline const_btree_iterator<Type>::const_btree_iterator(const const_btree_iterator & other)
    : base_btree_iterator<Type>::base_btree_iterator(other)
{
}

template<typename Type>
inline const_btree_iterator<Type>::const_btree_iterator(const_btree_iterator && other)
    : base_btree_iterator<Type>::base_btree_iterator(other)
{
}

template<typename Type>
inline const typename base_btree_iterator<Type>::reference const_btree_iterator<Type>::operator*() const
{
    return *(this->m_ele->m_value);
}

template<typename Type>
inline const typename base_btree_iterator<Type>::pointer const_btree_iterator<Type>::operator->() const
{
    return this->m_ele->m_value.get();
}

template<typename Type>
inline base_btree_iterator<Type> const_btree_iterator<Type>::clone()
{
    return const_btree_iterator(*this);
}

/**********************************************************************************************/

template <typename Type>
class const_btree_reverse_iterator : public btree_reverse_iterator <Type>
{
public:
    const_btree_reverse_iterator(
        const typename btree<Type>::tree_ele_ptr & ele,
        const typename btree<Type>::tree_ele_ptr & rend,
        const typename btree<Type>::tree_ele_ptr & end);

    const_btree_reverse_iterator(const const_btree_reverse_iterator & other);

    const_btree_reverse_iterator(const_btree_reverse_iterator && other);

    const typename base_btree_iterator<Type>::reference operator * () const;

    const typename base_btree_iterator<Type>::pointer operator -> () const;

    virtual base_btree_iterator<Type> clone();
};

template<typename Type>
inline const_btree_reverse_iterator<Type>::const_btree_reverse_iterator(
    const typename btree<Type>::tree_ele_ptr & ele,
    const typename btree<Type>::tree_ele_ptr & rend,
    const typename btree<Type>::tree_ele_ptr & end)
    : btree_reverse_iterator<Type>::btree_reverse_iterator(ele, rend, end)
{
}

template<typename Type>
inline const_btree_reverse_iterator<Type>::const_btree_reverse_iterator(const const_btree_reverse_iterator & other)
    : btree_reverse_iterator<Type>::btree_reverse_iterator(other)
{
}

template<typename Type>
inline const_btree_reverse_iterator<Type>::const_btree_reverse_iterator(const_btree_reverse_iterator && other)
    : btree_reverse_iterator<Type>::btree_reverse_iterator(other)
{
}

template<typename Type>
inline const typename base_btree_iterator<Type>::reference const_btree_reverse_iterator<Type>::operator*() const
{
    return *(this->m_ele->m_value);
}

template<typename Type>
inline const typename base_btree_iterator<Type>::pointer const_btree_reverse_iterator<Type>::operator->() const
{
    return this->m_ele->m_value.get();
}

template<typename Type>
inline base_btree_iterator<Type> const_btree_reverse_iterator<Type>::clone()
{
    return const_btree_reverse_iterator(*this);
}

/***************************************************************************/

#endif

