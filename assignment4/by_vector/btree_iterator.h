/**********************************************************
*
*               COMP 6771 Assignment 4
*                   Chunnan Sheng
*               Student Code: 5100764
*
***********************************************************/
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

/*=================================================

     Hierachy of all iterators of btree:

                 btree_iterator
                /             \
const_btree_iterator         btree_reverse_iterator
             /
const_btree_reverse_iterator

==================================================*/
template <typename Type>
class btree_iterator;

template <typename Type>
class const_btree_iterator;

template <typename Type>
class btree_reverse_iterator;

template <typename Type>
class const_btree_reverse_iterator;
/************************************
*
*    This is an iterator base class
*
*************************************/
template <typename Type>
class btree_iterator : public std::iterator<std::bidirectional_iterator_tag, Type>
{
protected:
    // Reference of the current position the iterator is pointing to
    typename btree<Type>::tree_ele_w_ptr m_ele;
    // This is the reference of the position before the first element of the btree
    typename btree<Type>::tree_ele_w_ptr m_rend;
    // This is the reference of the position after the last element of the btree 
    typename btree<Type>::tree_ele_w_ptr m_end;

    void copy_assign(const btree_iterator & other);
    void move_assign(btree_iterator & other);

public:
    typedef Type* pointer;
    typedef Type& reference;
    /***********************
    * Constructors
    ************************/
public:
    btree_iterator(
        const typename btree<Type>::tree_ele_w_ptr & ele,
        const typename btree<Type>::tree_ele_w_ptr & rend,
        const typename btree<Type>::tree_ele_w_ptr & end);

    btree_iterator(const btree_iterator & other);

    btree_iterator(btree_iterator && other);

    btree_iterator();

    // The destructor
public:
    ~btree_iterator();

    /***********************
    * Operator overloads
    ************************/
public:
    btree_iterator & operator = (const btree_iterator & other);

    btree_iterator & operator = (btree_iterator && other);

    virtual btree_iterator & operator ++ ();

    virtual btree_iterator operator ++ (int);

    virtual btree_iterator & operator -- ();

    virtual btree_iterator operator -- (int);

    typename btree_iterator::reference operator * () const;

    typename btree_iterator::pointer operator -> () const;

    operator const_btree_iterator<Type>();

    virtual btree_iterator clone();

    bool operator == (const btree_iterator & right) const;

    bool operator != (const btree_iterator & right) const;
};

template<typename Type>
inline void btree_iterator<Type>::copy_assign(const btree_iterator & other)
{
    this->m_ele = other.m_ele;
    this->m_rend = other.m_rend;
    this->m_end = other.m_end;
}

template<typename Type>
inline void btree_iterator<Type>::move_assign(btree_iterator & other)
{
    this->m_ele = other.m_ele;
    this->m_rend = other.m_rend;
    this->m_end = other.m_end;
    other.m_ele = std::weak_ptr<typename btree<Type>::tree_element>();
    other.m_rend = std::weak_ptr<typename btree<Type>::tree_element>();
    other.m_end = std::weak_ptr<typename btree<Type>::tree_element>();
}

template<typename Type>
inline btree_iterator<Type>::btree_iterator(
    const typename btree<Type>::tree_ele_w_ptr & ele,
    const typename btree<Type>::tree_ele_w_ptr & rend,
    const typename btree<Type>::tree_ele_w_ptr & end)
    : m_ele{ ele }, m_rend{ rend }, m_end{ end }
{
}

template<typename Type>
inline btree_iterator<Type>::btree_iterator(const btree_iterator & other)
    : m_ele{ other.m_ele }, m_rend{ other.m_rend }, m_end{ other.m_end }
{
}

template<typename Type>
inline btree_iterator<Type>::btree_iterator(btree_iterator && other)
    : m_ele{ other.m_ele }, m_rend{ other.m_rend }, m_end{ other.m_end }
{
    other.m_ele = std::weak_ptr<typename btree<Type>::tree_element>();
    other.m_rend = std::weak_ptr<typename btree<Type>::tree_element>();
    other.m_end = std::weak_ptr<typename btree<Type>::tree_element>();
}

template<typename Type>
inline btree_iterator<Type>::btree_iterator()
    : m_ele{ std::weak_ptr<typename btree<Type>::tree_element>() },
    m_rend{ std::weak_ptr<typename btree<Type>::tree_element>() },
    m_end{ std::weak_ptr<typename btree<Type>::tree_element>() }
{
}

template<typename Type>
inline btree_iterator<Type>::~btree_iterator()
{
}

template<typename Type>
inline btree_iterator<Type> & btree_iterator<Type>::operator=(const btree_iterator & other)
{
    this->copy_assign(other);
    return *this;
}

template<typename Type>
inline btree_iterator<Type> & btree_iterator<Type>::operator=(btree_iterator && other)
{
    this->move_assign(other);
    return *this;
}

template<typename Type>
inline btree_iterator<Type> & btree_iterator<Type>::operator++()
{
    assert(this->m_end.lock() != this->m_ele.lock());
    this->m_ele = this->m_ele.lock()->m_next;

    return *this;
}

template<typename Type>
inline btree_iterator<Type> btree_iterator<Type>::operator++(int)
{
    assert(this->m_end.lock() != this->m_ele.lock());

    btree_iterator<Type> to_return = this->clone();
    this->m_ele = this->m_ele.lock()->m_next;

    return to_return;
}

template<typename Type>
inline btree_iterator<Type> & btree_iterator<Type>::operator--()
{
    assert(this->m_rend.lock() != this->m_ele.lock()->m_prev.lock());
    this->m_ele = this->m_ele.lock()->m_prev;

    return *this;
}

template<typename Type>
inline btree_iterator<Type> btree_iterator<Type>::operator--(int)
{
    assert(this->m_rend.lock() != this->m_ele.lock()->m_prev.lock());

    btree_iterator<Type> to_return = this->clone();
    this->m_ele = this->m_ele.lock()->m_prev;

    return to_return;
}

template<typename Type>
inline typename btree_iterator<Type>::reference btree_iterator<Type>::operator*() const
{
    return *(this->m_ele.lock()->m_value);
}

template<typename Type>
inline typename btree_iterator<Type>::pointer btree_iterator<Type>::operator->() const
{
    return this->m_ele.lock()->m_value.get();
}

template<typename Type>
inline btree_iterator<Type>::operator const_btree_iterator<Type>()
{
    return const_btree_iterator<Type>(this->m_ele, this->m_rend, this->m_end);
}

template<typename Type>
inline btree_iterator<Type> btree_iterator<Type>::clone()
{
    return btree_iterator(*this);
}

template <typename Type>
bool btree_iterator<Type>::operator == (const btree_iterator & right) const
{
    return (this->m_ele.lock() == right.m_ele.lock()) &&
        (this->m_rend.lock() == right.m_rend.lock()) &&
        (this->m_end.lock() == right.m_end.lock());
}

template <typename Type>
bool btree_iterator<Type>::operator != (const btree_iterator & right) const
{
    bool equal = !(*(this) == right);
    return equal;
}

/**************************************

    btree_reverse_iterator

******************************************/

template <typename Type>
class btree_reverse_iterator : public btree_iterator <Type>
{
public:
    btree_reverse_iterator(
        const typename btree<Type>::tree_ele_w_ptr & ele,
        const typename btree<Type>::tree_ele_w_ptr & rend,
        const typename btree<Type>::tree_ele_w_ptr & end);

    btree_reverse_iterator(const btree_reverse_iterator & other);

    btree_reverse_iterator(btree_reverse_iterator && other);

    btree_reverse_iterator & operator = (const btree_reverse_iterator & other);

    btree_reverse_iterator & operator = (btree_reverse_iterator && other);

    btree_reverse_iterator();

    virtual btree_iterator<Type> & operator ++ ();

    virtual btree_iterator<Type> operator ++ (int);

    virtual btree_iterator<Type> & operator -- ();

    virtual btree_iterator<Type> operator -- (int);

    virtual btree_iterator<Type> clone();

    operator const_btree_reverse_iterator<Type>();
};

template<typename Type>
inline btree_reverse_iterator<Type>::btree_reverse_iterator(
    const typename btree<Type>::tree_ele_w_ptr & ele,
    const typename btree<Type>::tree_ele_w_ptr & rend,
    const typename btree<Type>::tree_ele_w_ptr & end)
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
inline btree_reverse_iterator<Type>::btree_reverse_iterator()
    : btree_iterator<Type>::btree_iterator()
{
}

template<typename Type>
inline btree_reverse_iterator<Type> & btree_reverse_iterator<Type>::operator=(const btree_reverse_iterator & other)
{
    this->copy_assign(other);
    return *this;
}

template<typename Type>
inline btree_reverse_iterator<Type> & btree_reverse_iterator<Type>::operator=(btree_reverse_iterator && other)
{
    this->move_assign(other);
    return *this;
}

template<typename Type>
inline btree_iterator<Type> & btree_reverse_iterator<Type>::operator++()
{
    assert(this->m_rend.lock() != this->m_ele.lock());
    this->m_ele = this->m_ele.lock()->m_prev;

    return *this;
}

template<typename Type>
inline btree_iterator<Type> btree_reverse_iterator<Type>::operator++(int)
{
    assert(this->m_rend.lock() != this->m_ele.lock());

    btree_iterator<Type> to_return = this->clone();
    this->m_ele = this->m_ele.lock()->m_prev;

    return to_return;
}

template<typename Type>
inline btree_iterator<Type> & btree_reverse_iterator<Type>::operator--()
{
    assert(this->m_end.lock() != this->m_ele.lock()->m_next.lock());
    this->m_ele = this->m_ele.lock()->m_next;

    return *this;
}

template<typename Type>
inline btree_iterator<Type> btree_reverse_iterator<Type>::operator--(int)
{
    assert(this->m_end.lock() != this->m_ele.lock()->m_next.lock());

    btree_iterator<Type> to_return = this->clone();
    this->m_ele = this->m_ele.lock()->m_next;

    return to_return;
}

template<typename Type>
inline btree_iterator<Type> btree_reverse_iterator<Type>::clone()
{
    return btree_reverse_iterator(*this);
}

template<typename Type>
inline btree_reverse_iterator<Type>::operator const_btree_reverse_iterator<Type>()
{
    return const_btree_reverse_iterator<Type>(this->m_ele, this->m_rend, this->m_end);
}

/******************************************

    const_btree_iterator

***********************************************/

template <typename Type>
class const_btree_iterator : public btree_iterator <Type>
{
public:
    const_btree_iterator(const typename btree<Type>::tree_ele_w_ptr & ele,
        const typename btree<Type>::tree_ele_w_ptr & rend,
        const typename btree<Type>::tree_ele_w_ptr & end);

    const_btree_iterator(const const_btree_iterator & other);

    const_btree_iterator(const_btree_iterator && other);

    const_btree_iterator();

    const_btree_iterator & operator = (const const_btree_iterator & other);

    const_btree_iterator & operator = (const_btree_iterator && other);

    const typename btree_iterator<Type>::reference operator * () const;

    const typename btree_iterator<Type>::pointer operator -> () const;

    virtual btree_iterator<Type> clone();

    operator btree_iterator<Type>();
};

template<typename Type>
inline const_btree_iterator<Type>::const_btree_iterator(
    const typename btree<Type>::tree_ele_w_ptr & ele,
    const typename btree<Type>::tree_ele_w_ptr & rend,
    const typename btree<Type>::tree_ele_w_ptr & end)
    : btree_iterator<Type>::btree_iterator(ele, rend, end)
{
}

template<typename Type>
inline const_btree_iterator<Type>::const_btree_iterator(const const_btree_iterator & other)
    : btree_iterator<Type>::btree_iterator(other)
{
}

template<typename Type>
inline const_btree_iterator<Type>::const_btree_iterator(const_btree_iterator && other)
    : btree_iterator<Type>::btree_iterator(other)
{
}

template<typename Type>
inline const_btree_iterator<Type>::const_btree_iterator()
    : btree_iterator<Type>::btree_iterator()
{
}

template<typename Type>
inline const_btree_iterator<Type> & const_btree_iterator<Type>::operator=(const const_btree_iterator & other)
{
    this->copy_assign(other);
    return *this;
}

template<typename Type>
inline const_btree_iterator<Type> & const_btree_iterator<Type>::operator=(const_btree_iterator && other)
{
    this->move_assign(other);
    return *this;
}

template<typename Type>
inline const typename btree_iterator<Type>::reference const_btree_iterator<Type>::operator*() const
{
    return *(this->m_ele.lock()->m_value);
}

template<typename Type>
inline const typename btree_iterator<Type>::pointer const_btree_iterator<Type>::operator->() const
{
    return this->m_ele.lock()->m_value.get();
}

template<typename Type>
inline btree_iterator<Type> const_btree_iterator<Type>::clone()
{
    return const_btree_iterator(*this);
}

template<typename Type>
inline const_btree_iterator<Type>::operator btree_iterator<Type>()
{
    return btree_iterator<Type>(this->m_ele, this->m_rend, this->m_end);
}

/*********************************************

    const_btree_reverse_iterator

*************************************************/

template <typename Type>
class const_btree_reverse_iterator : public const_btree_iterator <Type>
{
public:
    const_btree_reverse_iterator(
        const typename btree<Type>::tree_ele_w_ptr & ele,
        const typename btree<Type>::tree_ele_w_ptr & rend,
        const typename btree<Type>::tree_ele_w_ptr & end);

    const_btree_reverse_iterator(const const_btree_reverse_iterator & other);

    const_btree_reverse_iterator(const_btree_reverse_iterator && other);

    const_btree_reverse_iterator();

    const_btree_reverse_iterator & operator = (const const_btree_reverse_iterator & other);

    const_btree_reverse_iterator & operator = (const_btree_reverse_iterator && other);

    virtual btree_iterator<Type> & operator ++ ();

    virtual btree_iterator<Type> operator ++ (int);

    virtual btree_iterator<Type> & operator -- ();

    virtual btree_iterator<Type> operator -- (int);

    virtual btree_iterator<Type> clone();

    operator btree_reverse_iterator<Type>();
};

template<typename Type>
inline const_btree_reverse_iterator<Type>::const_btree_reverse_iterator(
    const typename btree<Type>::tree_ele_w_ptr & ele,
    const typename btree<Type>::tree_ele_w_ptr & rend,
    const typename btree<Type>::tree_ele_w_ptr & end)
    : const_btree_iterator<Type>::const_btree_iterator(ele, rend, end)
{
}

template<typename Type>
inline const_btree_reverse_iterator<Type>::const_btree_reverse_iterator(const const_btree_reverse_iterator & other)
    : const_btree_iterator<Type>::const_btree_iterator(other)
{
}

template<typename Type>
inline const_btree_reverse_iterator<Type>::const_btree_reverse_iterator(const_btree_reverse_iterator && other)
    : const_btree_iterator<Type>::const_btree_iterator(other)
{
}

template<typename Type>
inline const_btree_reverse_iterator<Type>::const_btree_reverse_iterator()
    : const_btree_iterator<Type>::const_btree_iterator()
{
}

template<typename Type>
inline const_btree_reverse_iterator<Type> & const_btree_reverse_iterator<Type>::operator=(const const_btree_reverse_iterator & other)
{
    this->copy_assign(other);
    return *this;
}

template<typename Type>
inline const_btree_reverse_iterator<Type> & const_btree_reverse_iterator<Type>::operator=(const_btree_reverse_iterator && other)
{
    this->move_assign(other);
    return *this;
}

template<typename Type>
inline btree_iterator<Type> & const_btree_reverse_iterator<Type>::operator++()
{
    assert(this->m_rend.lock() != this->m_ele.lock());
    this->m_ele = this->m_ele.lock()->m_prev;

    return *this;
}

template<typename Type>
inline btree_iterator<Type> const_btree_reverse_iterator<Type>::operator++(int)
{
    assert(this->m_rend.lock() != this->m_ele.lock());

    btree_iterator<Type> to_return = this->clone();
    this->m_ele = this->m_ele.lock()->m_prev;

    return to_return;
}

template<typename Type>
inline btree_iterator<Type> & const_btree_reverse_iterator<Type>::operator--()
{
    assert(this->m_end.lock() != this->m_ele.lock()->m_next.lock());
    this->m_ele = this->m_ele.lock()->m_next;

    return *this;
}

template<typename Type>
inline btree_iterator<Type> const_btree_reverse_iterator<Type>::operator--(int)
{
    assert(this->m_end.lock() != this->m_ele.lock()->m_next.lock());

    btree_iterator<Type> to_return = this->clone();
    this->m_ele = this->m_ele.lock()->m_next;

    return to_return;
}

template<typename Type>
inline btree_iterator<Type> const_btree_reverse_iterator<Type>::clone()
{
    return const_btree_reverse_iterator(*this);
}

template<typename Type>
inline const_btree_reverse_iterator<Type>::operator btree_reverse_iterator<Type>()
{
    return btree_reverse_iterator<Type>(this->m_ele, this->m_rend, this->m_end);
}

/***************************************************************************/

#endif

