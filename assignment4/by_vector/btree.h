/**********************************************************
*
*               COMP 6771 Assignment 4
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
#include <vector>
#include <queue>
#include <map>

// we better include the iterator
#include "btree_iterator.h"

// we do this to avoid compiler errors about non-template friends
// what do we do, remember? :)

template <typename Type> 
class btree
{
private:
    //==================================
    // Pre-declaration of nested classes
    //==================================
    struct tree_node;
    struct tree_element;
    class tree_ele_less;
    
public:

    //==================================================
    // All iterators should be friend classes of btree
    //==================================================
    friend class btree_iterator<Type>;
    friend class const_btree_iterator<Type>;
    friend class btree_reverse_iterator<Type>;
    friend class const_btree_reverse_iterator<Type>;

    //==================================================
    // Rename iterators to STL standard iterator names
    //==================================================
    typedef btree_iterator<Type> iterator;
    typedef btree_reverse_iterator<Type> reverse_iterator;
    typedef const_btree_iterator<Type> const_iterator;
    typedef const_btree_reverse_iterator<Type> const_reverse_iterator;

public:

    //==================================================
    // Typedef of all smart pointers
    //==================================================
    typedef std::unique_ptr<Type> type_u_ptr;

    typedef std::shared_ptr<tree_node> tree_node_s_ptr;
    typedef std::weak_ptr<tree_node> tree_node_w_ptr;
    typedef std::unique_ptr<tree_node> tree_node_u_ptr;

    typedef std::shared_ptr<tree_element> tree_ele_s_ptr;
    typedef std::weak_ptr<tree_element> tree_ele_w_ptr;

    typedef std::vector<tree_ele_s_ptr> tree_ele_container;

    //===================================================
    //
    //       Definitions Nested classes          
    //
    //===================================================
private:
    /*
       Each tree node consists of (maxNodeElems + 1) tree elemenets.
       The left most element of each tree node is an empty element.
       for example:
       -------------------------------------
       | empty | C | D | F | G | H | P | Q |
       -------------------------------------
               /                   \
       -----------------        -------------
       | empty | A | B |        | empty | J |
       -----------------        -------------
       This empty element is to make sure each child node has an accessible left parent.
       If you want each child node to have an accessible right parent,
       there should be a right most empty node as well:
       ---------------------------------------------
       | empty | A | D | F | G | H | P | Q | empty |
       ---------------------------------------------
       Only left parent is implemented in this assignment so far.
    */
    struct tree_node
    {
        // Reference to the parent node of this tree node
        tree_node_w_ptr m_parent_node;

        // Reference to left parent of this tree node
        tree_ele_w_ptr m_left_parent;

        // All elements of this node is stored in this container
        // Values of elements as keys for insertion and finding
        // We assume that number of elements in this container is limited (maxNodeElems + 1).
        // So std::vector can be used here, 
        // eventhough insertion of elements may be a little slower than std::set or std::unordered_set
        // The finding will use dichotomy algorithm whose time complexity is the same as
        // searching for an element in an set::set (log(n))
        tree_ele_container m_elements;

        // Constructor 1
        tree_node(const tree_node_w_ptr & parent, const tree_ele_w_ptr & left_p);

        // Constructor 2
        tree_node(const tree_node_w_ptr & parent);

        // The default constructor
        tree_node();

        // Copy Constructor
        tree_node(const tree_node & other);

        // Move constructor
        tree_node(tree_node && other) = delete;

        ~tree_node();

        // Copy assignment
        tree_node & operator = (const tree_node & other) = delete;

        // Move assignment
        tree_node & operator = (tree_node && other) = delete;

        // dichotomy algorithm is used to search for a specific element in set::vector
        bool find_element(const Type & key, tree_ele_s_ptr & ele_found, tree_ele_s_ptr & left) const;

        // dichotomy algorithm is used to search for a left neighbor of a specific element in set::vector
        void find_left_element(const Type & key, tree_ele_s_ptr & left);

        // dichotomy algorithm is used to insert a new element to set::vector to make sure all
        // elements are in incremental order
        bool insert_element(const tree_ele_s_ptr & to_insert);
    };

    // Each tree element is connected with both its left and right
    // neighors to guarantee efficiency of iterations.
    // Each tree element has a reference of right child node.
    // If this element does not has a child node, this reference will
    // be a null pointer.
    struct tree_element
    {
        // Reference to the left neighbor
        tree_ele_w_ptr m_prev;

        // Reference to the element value
        type_u_ptr m_value;

        // Reference to the right neighbor
        tree_ele_w_ptr m_next;

        // Reference to the right child node
        tree_node_s_ptr m_right_child;

        // Constructor 1
        tree_element(const Type &val);

        // Constructor 2
        tree_element();

        tree_element(const tree_element & other) = delete;

        tree_element(tree_element && other) = delete;

        ~tree_element();

        tree_element & operator = (const tree_element & other) = delete;

        tree_element & operator = (tree_element && other) = delete;
    };

    // This callable class is to compare two tree element references according to their values.
    // The element of empty value (nullptr) will always become the left most
    // member of a tree node.
    class tree_ele_less
    {
    public:
        bool operator() (const tree_ele_s_ptr & left, const tree_ele_s_ptr & right) const
        {
            // The element of null value will always become the smallest
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

        bool operator() (const tree_ele_s_ptr & left, const Type & right) const
        {
            // The element of null value will always become the smallest
            if (left->m_value == nullptr)
            {
	            return true;
            }

            // Compare value
            return *(left->m_value) < right;
        }

        bool operator() (const Type & left, const tree_ele_s_ptr & right) const
        {
            // The element of null value will always become the smallest
            if (right->m_value == nullptr)
            {
	            return false;
            }

            // Compare value
            return left < *(right->m_value);
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

    // Print the entire tree using in-order deep first search.
    void print(std::ostream & os) const;

    // Clear the entire tree to make it empty.
    // The tree should continue to be usable after it is cleared.
    void clear();
  
private:
    // Size limit of each B-tree node
    size_t m_size_limit;

    // The root node of this B-tree
    tree_node_s_ptr m_root;

    // rend is an empty tree element before the first tree element.
    // rend is also the left parent of root node.
    // rend's right child reference points to the root node.
    tree_ele_s_ptr m_rend;

    // end is an empty tree element after the last tree element.
    tree_ele_s_ptr m_end;

    // Recursive function of copy
    void copy_children(tree_node_s_ptr & copy_to, const tree_node_s_ptr & copy_from);

    // Recursive function to connect all elements of this tree preparing for iteration
    void connect_elements(tree_ele_s_ptr & prev_ele, tree_node_s_ptr & current_node);

    // Recursive function of find
    bool find(const tree_node_s_ptr & node, const Type & key, tree_ele_s_ptr & found) const;

    // Recursive function to insert a new element knowing that
    // this element does not exist in this b-tree
    std::pair<iterator, bool> insert(const tree_node_s_ptr & node, const tree_ele_s_ptr & key);

    // Recursive function to print all elements of this tree via in-order deep first search
    void depth_first_print(const tree_node_s_ptr & current_node, std::ostream & os, size_t depth) const;

    void breadth_first_print(std::ostream & os) const;
};

//===========================================================
//
//           All function definitions of tree_element
//
//===========================================================

template<typename Type>
inline btree<Type>::tree_element::tree_element(const Type & val)
    : m_value{ std::make_unique<Type>(val) }
{}

template<typename Type>
inline btree<Type>::tree_element::tree_element()
{}

template<typename Type>
inline btree<Type>::tree_element::~tree_element()
{}

//===========================================================
//
//           All function definitions of tree_node
//
//===========================================================

template<typename Type>
inline btree<Type>::tree_node::tree_node(const tree_node_w_ptr & parent, const tree_ele_w_ptr & left_p)
    : m_parent_node{ parent }, m_left_parent {left_p}
{
    // The left most element is an element of null value
    this->insert_element(std::make_shared<tree_element>());
}

template<typename Type>
inline btree<Type>::tree_node::tree_node()
{
    // The left most element is an element of null value
    this->insert_element(std::make_shared<tree_element>());
}

template<typename Type>
inline btree<Type>::tree_node::tree_node(const tree_node_w_ptr & parent)
    : m_parent_node{ parent }
{
    // The left most element is an element of null value
    this->insert_element(std::make_shared<tree_element>());
}

// The copy constructor of tree node.
// Only values of elements are copied here.
// References or connections between elements or nodes would
// be implemented elsewhere.
template<typename Type>
inline btree<Type>::tree_node::tree_node(const tree_node & other)
{
    for (tree_ele_s_ptr ele : other.m_elements)
    {
        if (nullptr == ele->m_value)
        {
            tree_ele_s_ptr new_ele = std::make_shared<tree_element>();
            this->insert_element(new_ele);
        }
        else
        {
            tree_ele_s_ptr new_ele = std::make_shared<tree_element>(*(ele->m_value));
            this->insert_element(new_ele);
        }
    }
}

template<typename Type>
inline btree<Type>::tree_node::~tree_node()
{}

// Implementation of dichotomy algorithm to search for an element in a tree_node
// The time complexity is log(n), n is number of elements in the node.
template<typename Type>
inline bool btree<Type>::tree_node::find_element(const Type & key, tree_ele_s_ptr & ele_found, tree_ele_s_ptr & left) const
{
    if (m_elements.empty())
    {
        return false;
    }

    size_t min = 0;
    size_t max = this->m_elements.size() - 1;

    while (true)
    {
        size_t index = (min + max) / 2;
        tree_ele_s_ptr test_ele = this->m_elements.at(index);

        tree_ele_less less;
        if (less(test_ele, key))
        {
            if (min + 1 == max)
            {
                min = max;
            }
            else if (min == max)
            {
                left = test_ele;
                break;
            }
            else
            {
                min = index;
            }
        }
        else if (less(key, test_ele))
        {
            if (min == max)
            {
                left = this->m_elements.at(index - 1);
                break;
            }
            else
            {
                max = index;
            }
        }
        else // found
        {
            ele_found = test_ele;
            return true;
        }
    }

    return false;
}

// Implementation of dichotomy algorithm to search for left neighbor of an element in a tree_node
// The time complexity is log(n), n is number of elements in the node.
template<typename Type>
inline void btree<Type>::tree_node::find_left_element(const Type & key, tree_ele_s_ptr & left)
{
    tree_ele_s_ptr to_find;
    this->find_element(key, to_find, left);
}

// Implementation of dichotomy algorithm to
// insert a new tree element into a right place in the tree_node
// The time complexity is log(n), n is number of elements in the node.
template<typename Type>
inline bool btree<Type>::tree_node::insert_element(const tree_ele_s_ptr & to_insert)
{   
    if (m_elements.empty())
    {
        this->m_elements.push_back(to_insert);
        return true;
    }

    size_t min = 0;
    size_t max = this->m_elements.size() - 1;

    while (true)
    {
        size_t index = (min + max) / 2;
        tree_ele_s_ptr test_ele = this->m_elements.at(index);

        tree_ele_less less;
        if (less(test_ele, to_insert))
        {
            if (min + 1 == max)
            {
                min = max;
            }
            else if (min == max)
            {
                this->m_elements.insert(this->m_elements.begin() + index + 1, to_insert);
                break;
            }
            else
            {
                min = index;
            }
        }
        else if (less(to_insert, test_ele))
        {
            if (min == max)
            {
                this->m_elements.insert(this->m_elements.begin() + index, to_insert);
                break;
            }
            else
            {
                max = index;
            }
        }
        else // found
        {
            return false;
        }
    }

    return true;
}


//===========================================================
//
//          All function definitions of btree
//
//===========================================================
template<typename Type>
inline btree<Type>::btree(size_t maxNodeElems)
    : m_size_limit{ maxNodeElems },
    // Root should be an existing empty node
    m_root{ std::make_shared<tree_node>() },
    m_rend{ std::make_shared<tree_element>() },
    m_end{ std::make_shared<tree_element>() }

{
    // "rend" and "end" of an empty tree will
    // be connected to each other.
    // New tree elements will be inserted between them.
    this->m_rend->m_next = this->m_end;
    this->m_end->m_prev = this->m_rend;
    this->m_root->m_left_parent = this->m_rend;
    this->m_rend->m_right_child = this->m_root;
}


template<typename Type>
inline btree<Type>::btree(const btree<Type>& original)
    : m_size_limit{ original.m_size_limit },
    m_root{ std::make_shared<tree_node>(*(original.m_root)) },
    m_rend{ std::make_shared<tree_element>() },
    m_end{ std::make_shared<tree_element>() }
{
    // Copy all nodes and elements
    this->m_root->m_left_parent = this->m_rend;
    this->m_rend->m_right_child = this->m_root;
    this->copy_children(this->m_root, original.m_root);

    // Connect all elements
    // Illustration: (m_rend as beginning and m_end as ending)
    // m_rend <-> ele_1 <-> ... <-> ele_n <-> m_end
    tree_ele_s_ptr prev_ele = this->m_rend;
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
    original.clear();
}

template<typename Type>
inline btree<Type>& btree<Type>::operator=(const btree<Type>& original)
{
    // clear up all original data and copy the root node
    this->m_size_limit = original.m_size_limit;
    this->m_root = std::make_shared<tree_node>(*(original.m_root));
    this->m_rend = std::make_shared<tree_element>();
    this->m_end = std::make_shared<tree_element>();

    // Copy all nodes and elements
    this->m_root->m_left_parent = this->m_rend;
    this->m_rend->m_right_child = this->m_root;
    this->copy_children(this->m_root, original.m_root);

    // Connect all elements
    // Illustration: (m_rend as beginning and m_end as ending)
    // m_rend <-> ele_1 <-> ... <-> ele_n <-> m_end
    tree_ele_s_ptr prev_ele = this->m_rend;
    this->connect_elements(prev_ele, this->m_root);
    prev_ele->m_next = this->m_end;
    this->m_end->m_prev = prev_ele;

    return *this;
}

template<typename Type>
inline btree<Type>& btree<Type>::operator=(btree<Type>&& other)
{
    // Smart ptrs can clear up original data, so do not worry
    // Assign all smart ptrs of the other b-tree to this b-tree
    this->m_size_limit = other.m_size_limit;
    this->m_root = other.m_root;
    this->m_rend = other.m_rend;
    this->m_end = other.m_end;

    // Do not forget to reset orignial btree to an empty tree
    other.clear();

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

// This find calls the internal recursive find function
template<typename Type>
inline typename btree<Type>::iterator btree<Type>::find(const Type & elem)
{
    tree_ele_s_ptr found;
    if (this->find(this->m_root, elem, found))
    {
        return iterator(found, this->m_rend, this->m_end);
    }

    return this->end();
}

// This find calls the internal recursive find function
template<typename Type>
inline typename btree<Type>::const_iterator btree<Type>::find(const Type & elem) const
{
    tree_ele_s_ptr found;
    if (this->find(this->m_root, elem, found))
    {
        return const_iterator(found, this->m_rend, this->m_end);
    }

    return this->cend();
}

// This insert function calls the internal recursive insert function
template<typename Type>
inline std::pair<typename btree<Type>::iterator, bool> btree<Type>::insert(const Type & elem)
{
    tree_ele_s_ptr found;
    if (this->find(this->m_root, elem, found))
    {
        // std::cout << " " << elem << " exists! ";
        iterator itr{ found, this->m_rend, this->m_end };
        return std::make_pair(itr, false);
    }
    
	tree_ele_s_ptr to_insert = std::make_shared<tree_element>(elem);
    return this->insert(this->m_root, to_insert);
}

template<typename Type>
inline btree<Type>::~btree()
{}

template<typename Type>
inline void btree<Type>::print(std::ostream & os) const
{
    this->breadth_first_print(os);
}

template<typename Type>
inline void btree<Type>::clear()
{
    // Do not forget to reset orignial btree to an empty tree
    this->m_root = std::make_shared<tree_node>();
    this->m_rend = std::make_shared<tree_element>();
    this->m_end = std::make_shared<tree_element>();

    // rend and end of the empty tree should still be functional
    this->m_rend->m_next = this->m_end;
    this->m_end->m_prev = this->m_rend;
    this->m_root->m_left_parent = this->m_rend;
    this->m_rend->m_right_child = this->m_root;
}

// Recursive function to copy all child nodes from the other tree 
template<typename Type>
inline void btree<Type>::copy_children(tree_node_s_ptr & copy_to, const tree_node_s_ptr & copy_from)
{
    for (size_t i = 0; i < copy_from->m_elements.size(); ++i)
    {
		tree_ele_s_ptr ele = copy_from->m_elements.at(i);
        if (nullptr != ele->m_right_child)
        {
            // Copy elements of child to new child
            tree_node_s_ptr new_child = std::make_shared<tree_node>(*(ele->m_right_child));

            // my ele is left parent of my new child
			tree_ele_s_ptr my_ele = copy_to->m_elements.at(i);
            
            // Confirm my left parent
            new_child->m_left_parent = my_ele;
            // Confirm my parent node
            new_child->m_parent_node = copy_to;

            my_ele->m_right_child = new_child;

            // Recursive call of this function
            copy_children(new_child, ele->m_right_child);
        }
    }
}

// This function should be called in copy constructors or copy assignments to
// re-connect all tree elements after the copy job.
template<typename Type>
inline void btree<Type>::connect_elements(tree_ele_s_ptr & prev_ele, tree_node_s_ptr & current_node)
{
    size_t index = 0;
    for (tree_ele_s_ptr ele : current_node->m_elements)
    {
        if (0 < index)
        {
            prev_ele->m_next = ele;
            ele->m_prev = prev_ele;
            prev_ele = ele;
        }

        if (nullptr != ele->m_right_child)
        {
            tree_node_s_ptr child_node = ele->m_right_child;

            connect_elements(prev_ele, child_node);
        }

        ++index;
    }
}

template<typename Type>
inline bool btree<Type>::find(const tree_node_s_ptr & node, const Type & key, tree_ele_s_ptr & found) const
{
    tree_ele_s_ptr left_parent;
    if (node->find_element(key, found, left_parent))
    {
        return true;
    }

    if (nullptr != left_parent->m_right_child)
    {
        // If this tree element is left parent of a child node
        // Continue to search this element in the child node
        return find(left_parent->m_right_child, key, found);
    }

    return false;
}

template<typename Type>
inline std::pair<typename btree<Type>::iterator, bool> btree<Type>::insert(const tree_node_s_ptr & current_node, const tree_ele_s_ptr & to_insert)
{
    // In this case the tree node is not full, so we can insert elements
    // directly into this tree node.
    if (current_node->m_elements.size() < this->m_size_limit + 1)
    {
        tree_ele_s_ptr left_ele;
        current_node->find_left_element(*(to_insert->m_value), left_ele);

        // If the left neighbor is an abstract(empty) element,
        // the real left neighbor should be the left parent of this node.
        // If the left parent is still empty (left most element of the mother node),
        // continue to go up to the left parent until:
        // 1. the left parent is a real element;
        // 2. the left parent is "rend" of this btree;
        tree_node_s_ptr node = current_node;
        while (this->m_rend != left_ele && nullptr == left_ele->m_value)
        {
            left_ele = node->m_left_parent.lock();
            node = node->m_parent_node.lock();
        }

        current_node->insert_element(to_insert);
        // Do not forget to update connecionts between neighboring elements
        tree_ele_s_ptr right_ele = left_ele->m_next.lock();
        left_ele->m_next = to_insert;
        to_insert->m_prev = left_ele;
        to_insert->m_next = right_ele;
        right_ele->m_prev = to_insert;
        //std::cout << " new insert: " << *(to_insert->m_value) << " ";
        

        iterator itr{ to_insert, this->m_rend, this->m_end };
        return std::make_pair(itr, true);
    }
    // In this case the tree node is already full.
    // Then we should create a child node or go into an existing child node
    // to insert the new element
    else
    {
        // Search for the left parent of the potential child node
        tree_ele_s_ptr left_ele;
        current_node->find_left_element(*(to_insert->m_value), left_ele);

        // The left parent has a right child (child node).
        // Just insert the new element to the child node.
        if (nullptr != left_ele->m_right_child)
        {
            return insert(left_ele->m_right_child, to_insert);
        }
        // The left parent does not have a right child.
        // We should create a new child node here and
        // then insert the new element.
        else
        {
            tree_node_s_ptr child_node = std::make_shared<tree_node>(current_node, left_ele);
            left_ele->m_right_child = child_node;

            return insert(child_node, to_insert);
        }
    }
}

template<typename Type>
inline void btree<Type>::depth_first_print(const tree_node_s_ptr & current_node, std::ostream & os, size_t depth) const
{
    size_t index = 0;
    for (tree_ele_s_ptr ele : current_node->m_elements)
    {
        if (0 < index)
        {
            if (ele->m_next.lock() != this->m_end)
            {
                os << *(ele->m_value) << " ";
            }
            else
            {
                os << *(ele->m_value);
            }
        }

        if (nullptr != ele->m_right_child)
        {
            tree_node_s_ptr child_node = ele->m_right_child;

            this->depth_first_print(child_node, os, depth + 1);
        }
        ++index;
    }
}

template<typename Type>
inline void btree<Type>::breadth_first_print(std::ostream & os) const
{
    std::queue<tree_ele_s_ptr> ele_queue;
    ele_queue.push(this->m_rend);

    while (!ele_queue.empty())
    {
        tree_ele_s_ptr ele = ele_queue.front();
        ele_queue.pop();

        // All elements including empty ones may have right children
        if (nullptr != ele->m_right_child)
        {
            for (tree_ele_s_ptr child_ele : ele->m_right_child->m_elements)
            {
                ele_queue.push(child_ele);
            }
        }

        // The element may be the left most one which is an empty element
        // If it is empty, DO NOT print it
        if (nullptr != ele->m_value)
        {
            os << *(ele->m_value);
            if (!ele_queue.empty())
            {
                os << ' ';
            }
        }
    }
}

template <typename Type>
std::ostream & operator << (std::ostream & os, const btree<Type> & the_tree)
{
    the_tree.print(os);
    return os;
}

#endif
