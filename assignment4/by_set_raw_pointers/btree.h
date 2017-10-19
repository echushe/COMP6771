/**********************************************************
*
*               COMP 6771 Assignment 3
*                   Chunnan Sheng
*               Student Code: 5100764
*
***********************************************************/

/**
 * The btree is a linked structure which operates much like
 * a binary search tree, save the fact that multiple client
 * elements are stored in a single node.  Whereas a single element
 * would partition the tree into two ordered subtrees, a node 
 * that stores m client elements partition the tree 
 * into m + 1 sorted subtrees.
 */

#ifndef BTREE_H
#define BTREE_H

#include <iostream>
#include <cstddef>
#include <utility>
#include <memory>
#include <set>
#include <map>

// we better include the iterator
#include "btree_iterator.h"

// we do this to avoid compiler errors about non-template friends
// what do we do, remember? :)


template <typename Type> 
class btree
{
private:
    struct tree_node;
    struct tree_element;
    class tree_ele_less;
    class tree_node_less;
    
public:

    friend class base_btree_iterator<Type>;
    friend class btree_iterator<Type>;
    friend class const_btree_iterator<Type>;
    friend class btree_reverse_iterator<Type>;
    friend class const_btree_reverse_iterator<Type>;

    typedef btree_iterator<Type> iterator;
    typedef btree_reverse_iterator<Type> reverse_iterator;
    typedef const_btree_iterator<Type> const_iterator;
    typedef const_btree_reverse_iterator<Type> const_reverse_iterator;

public:
    typedef Type * type_ptr;

    typedef tree_node * tree_node_ptr;
    typedef tree_element * tree_ele_ptr;

    typedef std::set<tree_ele_ptr, tree_ele_less> tree_ele_container;
    typedef std::set<tree_node_ptr, tree_node_less> tree_node_container;

    /********************************************

    *       Definitions Nested classes          *

    *********************************************/
private:
    struct tree_node
    {
        //
        tree_node_ptr m_parent_node;

        // Reference to the key of this tree node
        tree_ele_ptr m_left_parent;

        // All elements of this node
        // Values of elements as keys for this container
        tree_ele_container m_elements;

        // All children of this node
        // Values of left parent as keys for this container
        tree_node_container m_children;

        // Constructor 1
        tree_node(const tree_node_ptr & parent, const tree_ele_ptr & left_p);

        // Constructor 2
        tree_node(const tree_node_ptr & parent);

        // Constructor 3
        tree_node();

        ~tree_node();

        // Copy Constructor
        // Copy constructor only copy elements of this node
        // It does not do anything to the children nodes
        tree_node(const tree_node & other);

        // Move constructor
        tree_node(tree_node && other) = delete;

        // Copy assignment
        tree_node & operator = (const tree_node & other) = delete;

        // Move assignment
        tree_node & operator = (tree_node && other) = delete;
    };

    struct tree_element
    {
        tree_ele_ptr m_prev;

        type_ptr m_value;

        tree_ele_ptr m_next;

        tree_element(const Type &val);

        tree_element();

        tree_element(const tree_element & other) = delete;

        tree_element(tree_element && other) = delete;

        // Copy assignment
        tree_element & operator = (const tree_element & other) = delete;

        // Move assignment
        tree_element & operator = (tree_element && other) = delete;

        ~tree_element();
    };

    class tree_ele_less
    {
    public:
        bool operator() (const tree_ele_ptr & left, const tree_ele_ptr & right) const
        {
            // We assume that none of left or right are nullptrs
            // In case at least one of left or right is null
            if (left->m_value == nullptr)
            {
                if (right->m_value != nullptr)
                {
                    return true;
                }

                return false;
            }
            else if (right->m_value == nullptr)
            {
                return false;
            }

            // Compare value
            return *(left->m_value) < *(right->m_value);
        }
    };

    class tree_node_less
    {
    public:
        bool operator() (const tree_node_ptr & left, const tree_node_ptr & right) const
        {
            tree_ele_ptr ll_p = left->m_left_parent;            
            tree_ele_ptr rl_p = right->m_left_parent;
            
            tree_ele_less less;
            // Only need to compare left parent of each node
            return less(ll_p, rl_p);
        }
    };

public:
    /** Hmm, need some iterator typedefs here... friends? **/
 
    /**
    * Constructs an empty btree.  Note that
    * the elements stored in your btree must
    * have a well-defined zero-arg constructor,
    * copy constructor, operator=, and destructor.
    * The elements must also know how to order themselves
    * relative to each other by implementing operator<
    * and operator==. (These are already implemented on
    * behalf of all built-ins: ints, doubles, strings, etc.)
    * 
    * @param maxNodeElems the maximum number of elements
    *        that can be stored in each B-Tree node
    */
    btree(size_t maxNodeElems = 40);

    /**
    * The copy constructor and  assignment operator.
    * They allow us to pass around B-Trees by value.
    * Although these operations are likely to be expensive
    * they make for an interesting programming exercise.
    * Implement these operations using value semantics and 
    * make sure they do not leak memory.
    */

    /** 
    * Copy constructor
    * Creates a new B-Tree as a copy of original.
    *
    * @param original a const lvalue reference to a B-Tree object
    */
    btree(const btree<Type>& original);

    /** 
    * Move constructor
    * Creates a new B-Tree by "stealing" from original.
    *
    * @param original an rvalue reference to a B-Tree object
    */
    btree(btree<Type>&& original);
  
  
    /** 
    * Copy assignment
    * Replaces the contents of this object with a copy of rhs.
    *
    * @param rhs a const lvalue reference to a B-Tree object
    */
    btree<Type>& operator=(const btree<Type>& rhs);

    /** 
    * Move assignment
    * Replaces the contents of this object with the "stolen"
    * contents of original.
    *
    * @param rhs a const reference to a B-Tree object
    */
    btree<Type>& operator=(btree<Type>&& rhs);

    /**
    * Puts a breadth-first traversal of the B-Tree onto the output
    * stream os. Elements must, in turn, support the output operator.
    * Elements are separated by space. Should not output any newlines.
    *
    * @param os a reference to a C++ output stream
    * @param tree a const reference to a B-Tree object
    * @return a reference to os
    */
    // friend std::ostream& operator<< <Type> (std::ostream& os, const btree<T>& tree);

    /**
    * The following can go here
    * -- begin() 
    * -- end() 
    * -- rbegin() 
    * -- rend() 
    * -- cbegin() 
    * -- cend() 
    * -- crbegin() 
    * -- crend() 
    */
    iterator begin() const;

    iterator end() const;

    reverse_iterator rbegin() const;

    reverse_iterator rend() const;

    const_iterator cbegin() const;

    const_iterator cend() const;

    const_reverse_iterator crbegin() const;

    const_reverse_iterator crend() const;
  
    /**
    * Returns an iterator to the matching element, or whatever 
    * the non-const end() returns if the element could 
    * not be found.  
    *
    * @param elem the client element we are trying to match.  The elem,
    *        if an instance of a true class, relies on the operator< and
    *        and operator== methods to compare elem to elements already 
    *        in the btree.  You must ensure that your class implements
    *        these things, else code making use of btree<T>::find will
    *        not compile.
    * @return an iterator to the matching element, or whatever the
    *         non-const end() returns if no such match was ever found.
    */
    iterator find(const Type& elem);
    
    /**
    * Identical in functionality to the non-const version of find, 
    * save the fact that what's pointed to by the returned iterator
    * is deemed as const and immutable.
    *
    * @param elem the client element we are trying to match.
    * @return an iterator to the matching element, or whatever the
    *         const end() returns if no such match was ever found.
    */
    const_iterator find(const Type& elem) const;
      
    /**
    * Operation which inserts the specified element
    * into the btree if a matching element isn't already
    * present.  In the event where the element truly needs
    * to be inserted, the size of the btree is effectively
    * increases by one, and the pair that gets returned contains
    * an iterator to the inserted element and true in its first and
    * second fields.  
    *
    * If a matching element already exists in the btree, nothing
    * is added at all, and the size of the btree stays the same.  The 
    * returned pair still returns an iterator to the matching element, but
    * the second field of the returned pair will store false.  This
    * second value can be checked to after an insertion to decide whether
    * or not the btree got bigger.
    *
    * The insert method makes use of T's zero-arg constructor and 
    * operator= method, and if these things aren't available, 
    * then the call to btree<T>::insert will not compile.  The implementation
    * also makes use of the class's operator== and operator< as well.
    *
    * @param elem the element to be inserted.
    * @return a pair whose first field is an iterator positioned at
    *         the matching element in the btree, and whose second field 
    *         stores true if and only if the element needed to be added 
    *         because no matching element was there prior to the insert call.
    */
    std::pair<iterator, bool> insert(const Type& elem);

    /**
    * Disposes of all internal resources, which includes
    * the disposal of any client objects previously
    * inserted using the insert operation. 
    * Check that your implementation does not leak memory!
    */
    ~btree();

    void print(std::ostream & os) const;
  
private:
    // Size limit of each B-tree node
    size_t m_size_limit;

    // The root node of this B-tree
    tree_node_ptr m_root;

    // rend (abstract node before begin)
    tree_ele_ptr m_rend;

    // end (abstract node after rbegin)
    tree_ele_ptr m_end;

    // Recursive function of copy
    void copy_children(tree_node_ptr & copy_to, const tree_node_ptr & copy_from);

    // Recursive function to connect all elements of this tree preparing for iteration
    void connect_elements(tree_ele_ptr & prev_ele, tree_node_ptr & current_node);

    // Recursive function of find
    bool find(const tree_node_ptr & node, const Type & elem, tree_ele_ptr & found) const;

    // Recursive function to insert a new element knowing that
    // this element does not exist in this b-tree
    std::pair<iterator, bool> insert(const tree_node_ptr & node, const tree_ele_ptr & ele_key);

    void print(const tree_node_ptr & current_node, std::ostream & os, size_t depth) const;

    void clear_without_destroy();

    void destroy();
};

template<typename Type>
inline btree<Type>::tree_element::tree_element(const Type & val)
    : m_prev{ nullptr }, m_value{ new Type(val) }, m_next{ nullptr }
{
    //++m_ele_count;
}

template<typename Type>
inline btree<Type>::tree_element::tree_element()
    : m_prev{ nullptr }, m_value{ nullptr }, m_next{ nullptr }
{
    //++m_ele_count;
}

template<typename Type>
inline btree<Type>::tree_element::~tree_element()
{
    delete this->m_value;
    //--m_ele_count;

    //std::cout << "Ele count: " << m_ele_count << std::endl;
}

/*************************************************
*
*           All function definitions of tree_node
*
**************************************************/

template<typename Type>
inline btree<Type>::tree_node::tree_node(const tree_node_ptr & parent, const tree_ele_ptr & left_p)
    : m_parent_node{ parent }, m_left_parent {left_p}
{
    // The left most element is an element of null value
    this->m_elements.insert(new tree_element());
    //++m_node_count;
}

template<typename Type>
inline btree<Type>::tree_node::tree_node()
    : m_parent_node{ nullptr }, m_left_parent{ nullptr }
{
    // The left most element is an element of null value
    this->m_elements.insert(new tree_element());
    //++m_node_count;
}

template<typename Type>
inline btree<Type>::tree_node::tree_node(const tree_node_ptr & parent)
    : m_parent_node{ parent }, m_left_parent{ nullptr }
{
    // The left most element is an element of null value
    this->m_elements.insert(new tree_element());
    ++m_node_count;
}

template<typename Type>
inline btree<Type>::tree_node::tree_node(const tree_node & other)
{
    for (tree_ele_ptr ele : other.m_elements)
    {
        if (nullptr == ele->m_value)
        {
            tree_ele_ptr new_ele = new tree_element();
            this->m_elements.insert(new_ele);
        }
        else
        {
            tree_ele_ptr new_ele = new tree_element(*(ele->m_value));
            this->m_elements.insert(new_ele);
        }
    }

    //++m_node_count;
}

template<typename Type>
inline btree<Type>::tree_node::~tree_node()
{
    for (auto itr = this->m_children.begin(); itr != this->m_children.end(); ++itr)
    {
        delete *(itr);
    }

    for (auto itr = this->m_elements.begin(); itr != this->m_elements.end(); ++itr)
    {
        delete *(itr);
    }
    //--m_node_count;

    //std::cout << "Node count: "<< m_node_count << std::endl;
}

/*************************************************
*
*           All function definitions of btree
*
**************************************************/
template<typename Type>
inline btree<Type>::btree(size_t maxNodeElems)
    : m_size_limit{ maxNodeElems },
    // Root should be an existing empty node
    m_root{ new tree_node() },
    m_rend{ new tree_element() },
    m_end{ new tree_element() }

{
    this->m_rend->m_next = this->m_end;
    this->m_end->m_prev = this->m_rend;
    this->m_root->m_left_parent = m_rend;
}


template<typename Type>
inline btree<Type>::btree(const btree<Type>& original)
    : m_size_limit{ original.m_size_limit },
    m_root{ new tree_node(*(original.m_root)) },
    m_rend{ new tree_element() },
    m_end{ new tree_element() }
{
    // Copy all nodes and elements
    this->copy_children(this->m_root, original.m_root);

    // Connect all elements
    // Illustration: (m_rend as beginning and m_end as ending)
    // m_rend <-> ele_1 <-> ... <-> ele_n <-> m_end
    tree_ele_ptr prev_ele = this->m_rend;
    this->connect_elements(prev_ele, this->m_root);   
    prev_ele->m_next = this->m_end;
    this->m_end->m_prev = prev_ele;
}


template<typename Type>
inline btree<Type>::btree(btree<Type>&& original)
    : m_size_limit{ original.m_size_limit },
    m_root{ original.m_root },
    m_rend{ original.m_rend },
    m_end{ original.m_end }
{
    // Do not forget to reset orignial btree to an empty tree
    original.clear_without_destroy();
}

template<typename Type>
inline btree<Type>& btree<Type>::operator=(const btree<Type>& original)
{
    this->destroy();
    // clear up all original data and copy the root node
    this->m_size_limit = original.m_size_limit;
    this->m_root = new tree_node(*(original.m_root));
    this->m_rend = new tree_element();
    this->m_end = new tree_element();

    // Copy all nodes and elements
    this->copy_children(this->m_root, original.m_root);

    // Connect all elements
    // Illustration: (m_rend as beginning and m_end as ending)
    // m_rend <-> ele_1 <-> ... <-> ele_n <-> m_end
    tree_ele_ptr prev_ele = this->m_rend;
    this->connect_elements(prev_ele, this->m_root);
    prev_ele->m_next = this->m_end;
    this->m_end->m_prev = prev_ele;

    return *this;
}

template<typename Type>
inline btree<Type>& btree<Type>::operator=(btree<Type>&& other)
{
    this->destroy();
    // Smart ptrs can clear up original data, so do not worry
    // Assign all smart ptrs of the other b-tree to this b-tree
    this->m_size_limit = other.m_size_limit;
    this->m_root = other.m_root;
    this->m_rend = other.m_rend;
    this->m_end = other.m_end;

    // Do not forget to reset orignial btree to an empty tree
    other.clear_without_destroy();

    return *this;
}


template<typename Type>
inline typename btree<Type>::iterator btree<Type>::begin() const
{
    return iterator(this->m_rend->m_next, this->m_rend, this->m_end);
}

template<typename Type>
inline typename btree<Type>::iterator btree<Type>::end() const
{
    return iterator(this->m_end, this->m_rend, this->m_end);
}

template<typename Type>
inline typename btree<Type>::reverse_iterator btree<Type>::rbegin() const
{
    return reverse_iterator(this->m_end->m_prev, this->m_rend, this->m_end);
}

template<typename Type>
inline typename btree<Type>::reverse_iterator btree<Type>::rend() const
{
    return reverse_iterator(this->m_rend, this->m_rend, this->m_end);
}

template<typename Type>
inline typename btree<Type>::const_iterator btree<Type>::cbegin() const
{
    return const_iterator(this->m_rend->m_next, this->m_rend, this->m_end);
}

template<typename Type>
inline typename btree<Type>::const_iterator btree<Type>::cend() const
{
    return const_iterator(this->m_end, this->m_rend, this->m_end);
}

template<typename Type>
inline typename btree<Type>::const_reverse_iterator btree<Type>::crbegin() const
{
    return const_reverse_iterator(this->m_end->m_prev, this->m_rend, this->m_end);
}

template<typename Type>
inline typename btree<Type>::const_reverse_iterator btree<Type>::crend() const
{
    return const_reverse_iterator(this->m_rend, this->m_rend, this->m_end);
}

template<typename Type>
inline typename btree<Type>::iterator btree<Type>::find(const Type & elem)
{
    tree_ele_ptr found;
    
    if (this->find(this->m_root, elem, found))
    {
        return iterator(found, this->m_rend, this->m_end);
    }

    return this->end();
}

template<typename Type>
inline typename btree<Type>::const_iterator btree<Type>::find(const Type & elem) const
{
    tree_ele_ptr found;

    if (this->find(this->m_root, elem, found))
    {
        return const_iterator(found, this->m_rend, this->m_end);
    }

    return this->cend();
}


template<typename Type>
inline std::pair<typename btree<Type>::iterator, bool> btree<Type>::insert(const Type & elem)
{
    tree_ele_ptr found;

    if (this->find(this->m_root, elem, found))
    {
        iterator itr{ found, this->m_rend, this->m_end };
        return std::make_pair(itr, false);
    }

    tree_element ele{ elem };
    tree_ele_ptr ele_key = &ele;
    return this->insert(this->m_root, ele_key);
}

template<typename Type>
inline btree<Type>::~btree()
{
    this->destroy();
}

template<typename Type>
inline void btree<Type>::print(std::ostream & os) const
{
    this->print(this->m_root, os, 0);
}

template<typename Type>
inline void btree<Type>::clear_without_destroy()
{
    // Do not forget to reset orignial btree to an empty tree
    this->m_root = new tree_node();
    this->m_rend = new tree_element();
    this->m_end = new tree_element();

    this->m_rend->m_next = this->m_end;
    this->m_end->m_prev = this->m_rend;
    this->m_root->m_left_parent = this->m_rend;
}

template<typename Type>
inline void btree<Type>::destroy()
{
    delete this->m_root;
    this->m_root = nullptr;
    
    delete this->m_rend;
    this->m_rend = nullptr;
    
    delete this->m_end;
    this->m_end = nullptr;
}

template<typename Type>
inline void btree<Type>::copy_children(tree_node_ptr & copy_to, const tree_node_ptr & copy_from)
{
    for (tree_node_ptr child : copy_from->m_children)
    {
        // Copy elements of child to new child
        tree_node_ptr new_child = new tree_node(*child);

        // left parent of foreign child as a search key
        tree_ele_ptr ele_key = child->m_left_parent;
        // Search for my left parent
        tree_ele_ptr my_left_parent = *(copy_to->m_elements.find(ele_key));
        // Confirm my left parent
        new_child->m_left_parent = my_left_parent;
        // Confirm my parent node
        new_child->m_parent_node = copy_to;

        copy_to->m_children.insert(new_child);

        // Recursive call of this function
        copy_children(new_child, child);
    }
}

template<typename Type>
inline void btree<Type>::connect_elements(tree_ele_ptr & prev_ele, tree_node_ptr & current_node)
{
    size_t index = 0;
    for (tree_ele_ptr ele : current_node->m_elements)
    {
        if (0 < index)
        {
            prev_ele->m_next = ele;
            ele->m_prev = prev_ele;
            prev_ele = ele;
        }

        tree_node node{ current_node, ele };
        tree_node_ptr node_key = &node;
        auto itr_found = current_node->m_children.find(node_key);
        if (itr_found != current_node->m_children.end())
        {
            tree_node_ptr child_node = *itr_found;
            connect_elements(prev_ele, child_node);
        }

        ++index;
    }
}

template<typename Type>
inline bool btree<Type>::find(const tree_node_ptr & current_node, const Type & elem, tree_ele_ptr & found) const
{
    tree_element ele{ elem };
    tree_ele_ptr ele_key = &ele;

    auto itr_found = current_node->m_elements.find(ele_key);
    if (itr_found != current_node->m_elements.end())
    {
        found = *itr_found;
        return true;
    }

    auto lower_bound = current_node->m_elements.lower_bound(ele_key);
    tree_ele_ptr left_parent = *(--lower_bound);

    tree_node node{ current_node, left_parent };
    tree_node_ptr node_key = &node;

    auto node_itr_found = current_node->m_children.find(node_key);
    if (node_itr_found != current_node->m_children.end())
    {
        tree_node_ptr node_found = *node_itr_found;
        // Continue to search this element in the child node
        return find(node_found, elem, found);
    }

    return false;
}

template<typename Type>
inline std::pair<typename btree<Type>::iterator, bool> btree<Type>::insert(const tree_node_ptr & current_node, const tree_ele_ptr & ele_key)
{
    if (current_node->m_elements.size() < this->m_size_limit + 1)
    {
        auto lower_bound = current_node->m_elements.lower_bound(ele_key);
        tree_ele_ptr left_ele = *(--lower_bound);

        // If the left neighbor is an abstract element
        // the real left neighbor should be the left parent of this node
        tree_node_ptr node = current_node;
        while (this->m_rend != left_ele && nullptr == left_ele->m_value)
        {
            left_ele = node->m_left_parent;
            node = node->m_parent_node;
        }

        tree_ele_ptr new_ele = new tree_element(*(ele_key->m_value));
        current_node->m_elements.insert(new_ele);
        // Do not forget to update connecionts between neighboring elements
        tree_ele_ptr right_ele = left_ele->m_next;
        left_ele->m_next = new_ele;
        new_ele->m_prev = left_ele;
        new_ele->m_next = right_ele;
        right_ele->m_prev = new_ele;
        

        iterator itr{ new_ele, this->m_rend, this->m_end };
        return std::make_pair(itr, true);
    }
    else
    {
        auto lower_bound = current_node->m_elements.lower_bound(ele_key);
        tree_ele_ptr left_ele = *(--lower_bound);

        // The left_ele could be a real element, or an abstract element whose
        // value is minimum of all elements 
        tree_node node{ current_node, left_ele };
        tree_node_ptr node_key = &node;
        auto itr = current_node->m_children.find(node_key);
        if (itr != current_node->m_children.end())
        {
            tree_node_ptr child_node = *itr;
            return insert(child_node, ele_key);
        }
        else
        {
            tree_node_ptr child_node = new tree_node(current_node, left_ele);

            current_node->m_children.insert(child_node);
            return insert(child_node, ele_key);
        }
    }
}

template<typename Type>
inline void btree<Type>::print(const tree_node_ptr & current_node, std::ostream & os, size_t depth) const
{
    size_t index = 0;
    for (tree_ele_ptr ele : current_node->m_elements)
    {
        if (0 < index)
        {
            os << *(ele->m_value) << "(" << depth << ") ";
        }

        tree_node node{ current_node, ele };
        tree_node_ptr node_key = &node;
        auto itr = current_node->m_children.find(node_key);
        if (itr != current_node->m_children.end())
        {
            tree_node_ptr child_node = *itr;

            this->print(child_node, os, depth + 1);
        }

        ++index;
    }
}

template <typename Type>
std::ostream & operator << (std::ostream & os, const btree<Type> & the_tree)
{
    the_tree.print(os);
    return os;
}

#endif
