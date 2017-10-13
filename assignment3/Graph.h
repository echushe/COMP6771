/******************************************************************
*
*                COMP 6771 Assignment 3
*                    Chunnan Sheng
*               Student Code: 5100764
*
*******************************************************************/

#pragma once
#include <memory>
#include <algorithm>
#include <vector>
#include <set>
#include <iostream>
// Namespace
namespace gdwg
{
    // All integers will be defined as long long here
    typedef long long lint;

    template <typename NodeValue, typename EdgeWeight>
    class Graph
    {
    private:
        /***********************************************************
        *
        *      Pre-definitions of all frequently used data types
        *   Make typedef shotcuts of them to make code more readable
        *
        ************************************************************/
        class GraphNode;
        class NodeSharedPtrLess;

        typedef std::shared_ptr<GraphNode> NodeSharedPtr;
        typedef std::weak_ptr<GraphNode> NodeWeakPtr;

        // A set of shared pointers of GraphNode
        // Use callable class "NodeSharedPtrLess" as "<" comparison 
        typedef std::set<NodeSharedPtr, NodeSharedPtrLess> NodeSPContainer;

        class GraphEntry;
        class EntrySharedPtrLess;
        typedef std::shared_ptr<GraphEntry> EntrySharedPtr;

        // A set of shared pointers of GraphEntry
        // Use callable class "EntrySharedPtrLess" as "<" comparison
        typedef std::set<EntrySharedPtr, EntrySharedPtrLess> EntrySPContainer;
        
        /************************************************************
        *
        *      Nested class Graph Node
        *      A graph node as a set of entries to all its child nodes
        *      For each child node there may be multiple weights
        *      representing multiple edges to the same node
        *
        *************************************************************/
        struct GraphNode
        {
            NodeValue m_value;
            EntrySPContainer m_children;
            lint m_in_degree;
            lint m_out_degree;

            GraphNode(const NodeValue & value);

            // Copy constructor is forbidden for graph node
            GraphNode(const GraphNode &other) = delete;

            // Move constructor is forbidden for graph node
            GraphNode(GraphNode &&other) = delete;

            // 
            ~GraphNode();

            // Copy assignment is forbidden for graph node
            GraphNode & operator = (const GraphNode &other) = delete;

            // Move assignment is fordidden for graph node
            GraphNode & operator = (GraphNode && other) = delete;

        };

        /***********************************************************
        *
        *      A callable comparing class that is used by a set of 'NodeSharedPtr's.
        *      The shared ptr whose GraphNode value is "larger" will
        *      be considered "larger"
        *
        ************************************************************/
        class NodeSharedPtrLess
        {
        public:
            bool operator()(const NodeSharedPtr &left, const NodeSharedPtr &right) const
            {
                return left->m_value < right->m_value;
            }
        };

        /***********************************************************
        *
        *      A graph entry is an entrance of the grah node entering into its
        *      child node. Each entry may include mutiple weights representing
        *      multiple Graph edges.
        *
        ************************************************************/
        struct GraphEntry
        {
            NodeWeakPtr m_node_ptr;
            std::set<EdgeWeight> m_edges;

            GraphEntry(const NodeWeakPtr & dst);

            GraphEntry(const NodeWeakPtr & dst, const std::set<EdgeWeight> & edges);

            // Copy constructor is forbidden for graph node
            GraphEntry(const GraphEntry &other) = delete;

            // Move constructor is forbidden for graph node
            GraphEntry(GraphEntry &&other) = delete;

            // 
            ~GraphEntry();

            // Copy assignment is forbidden for graph node
            GraphEntry & operator = (const GraphEntry &other) = delete;

            // Move assignment is fordidden for graph node
            GraphEntry & operator = (GraphEntry && other) = delete;
        };

        class EntrySharedPtrLess
        {
        public:
            bool operator()(const EntrySharedPtr &left, const EntrySharedPtr &right) const
            {
                return left->m_node_ptr.lock()->m_value < right->m_node_ptr.lock()->m_value;
            }
        };

    private:
        /*****************************************
        *
        *      Private data members
        *
        ******************************************/

        // All nodes of this graph are stored in an ordered set (RB tree)
        // A unique ptr encapsulizes this RB tree so that move operation can
        // be easily implemented and executed with almost no cost
        std::unique_ptr<NodeSPContainer> m_all_nodes;

        // Current position iterator
        mutable typename NodeSPContainer::const_iterator m_itr;

    public:
        /***********************************************************
        *
        *                  Constructors
        *
        ************************************************************/

        // Default constructor
        Graph <NodeValue, EdgeWeight>();

        // Copy constructor
        Graph <NodeValue, EdgeWeight>(const Graph <NodeValue, EdgeWeight> &other);

        // Move constructor
        Graph <NodeValue, EdgeWeight>(Graph <NodeValue, EdgeWeight> &&other);

        // Assignments
    public:
        /***********************************************************
        *
        *                  Assignments
        *
        ************************************************************/
        // Copy assignment
        Graph <NodeValue, EdgeWeight> & operator = (const Graph <NodeValue, EdgeWeight> &other);

        // Move assignment
        Graph <NodeValue, EdgeWeight> & operator = (Graph <NodeValue, EdgeWeight> &&other);

    public:
        /***********************************************************
        *
        *                  All public operations
        *
        *    Please refer to the assignment specification for details
        *    about these public functions
        *
        ************************************************************/

        bool addNode(const NodeValue &val);

        bool addEdge(const NodeValue &src, const NodeValue &dst, const EdgeWeight &w);

        bool replace(const NodeValue &old_data, const NodeValue &new_data);

        void mergeReplace(const NodeValue &old_data, const NodeValue &new_data);

        void deleteNode(const NodeValue &val) noexcept;

        void deleteEdge(const NodeValue &src, const NodeValue &dst, const EdgeWeight &weight) noexcept;

        void clear() noexcept;

        bool isNode(const NodeValue &val) const;

        bool isConnected(const NodeValue &src, const NodeValue &dst) const;

        void printNodes() const;

        void printNodes(std::ostream &out) const;

        void printEdges(const NodeValue &val) const;

        void printEdges(const NodeValue &val, std::ostream &out) const;

        void printAll() const;

        void begin() const;

        bool end() const;

        void next() const;

        const NodeValue& value() const;

    public:
        ~Graph();

    private:
        /***********************************************************
        *
        *                  All private operations
        *
        ************************************************************/

        // This function is used by both copy constructor and copy assignment to
        // replicate all the data from the other graph to this graph
        void copy(NodeSPContainer & copy_to, const NodeSPContainer & copy_from);
    };

    /**************************************************
    *
    *    All functions of GraphNode
    *
    ***************************************************/
    template<typename NodeValue, typename EdgeWeight>
    inline Graph<NodeValue, EdgeWeight>::GraphNode::GraphNode(const NodeValue & value)
        : m_value{ value }, m_children{}, m_in_degree{ 0 }, m_out_degree{ 0 }
    {}

    template<typename NodeValue, typename EdgeWeight>
    inline Graph<NodeValue, EdgeWeight>::GraphNode::~GraphNode()
    {}

    /**************************************************
    *
    *    All functions of GraphEntry
    *
    ***************************************************/
    template<typename NodeValue, typename EdgeWeight>
    inline Graph<NodeValue, EdgeWeight>::GraphEntry::GraphEntry(const NodeWeakPtr & dst)
        : m_node_ptr{ dst }
    {}

    template<typename NodeValue, typename EdgeWeight>
    inline Graph<NodeValue, EdgeWeight>::GraphEntry::GraphEntry(const NodeWeakPtr & dst, const std::set<EdgeWeight>& edges)
        : m_node_ptr{ dst }, m_edges{ edges }
    {}

    template<typename NodeValue, typename EdgeWeight>
    inline Graph<NodeValue, EdgeWeight>::GraphEntry::~GraphEntry()
    {}

    /**************************************************
    *
    *    All functions of Graph
    *
    ***************************************************/

    template<typename NodeValue, typename EdgeWeight>
    inline Graph<NodeValue, EdgeWeight>::Graph()
        // m_all_nodes is initialized as a unique pointer of empty set
        : m_all_nodes{ std::make_unique<NodeSPContainer>() }
    {
        // Move the iterator to the end
        this->m_itr = this->m_all_nodes->end();
    }

    template<typename NodeValue, typename EdgeWeight>
    inline Graph<NodeValue, EdgeWeight>::Graph(const Graph<NodeValue, EdgeWeight>& other)
        : m_all_nodes{ std::make_unique<NodeSPContainer>() }
    {
        this->copy(*(this->m_all_nodes), *(other.m_all_nodes));
        // Move the iterator to the end
        this->m_itr = this->m_all_nodes->end();
    }

    template<typename NodeValue, typename EdgeWeight>
    inline Graph<NodeValue, EdgeWeight>::Graph(Graph<NodeValue, EdgeWeight>&& other)
        // The move operation has almost no cost
        : m_all_nodes{ std::move(other.m_all_nodes) }
    {
        // Do not forget to recreate a new empty RB tree for the
        // other Graph so that it continues to be usable
        other.m_all_nodes = std::make_unique<NodeSPContainer>();
    }

    template<typename NodeValue, typename EdgeWeight>
    inline Graph<NodeValue, EdgeWeight>& Graph<NodeValue, EdgeWeight>::operator = (const Graph<NodeValue, EdgeWeight>& other)
    {
        // Do not copy to itself
        if (&other == this)
        {
            return *this;
        }

        // Clear the RB tree of myself
        this->m_all_nodes->clear();
        // Copy all nodes of the other graph to my RB tree
        this->copy(*(this->m_all_nodes), *(other.m_all_nodes));

        return *this;
    }

    template<typename NodeValue, typename EdgeWeight>
    inline Graph<NodeValue, EdgeWeight>& Graph<NodeValue, EdgeWeight>::operator = (Graph<NodeValue, EdgeWeight>&& other)
    {
        // Do not copy to itself
        if (&other == this)
        {
            return *this;
        }

        // The move operation has almost no cost
        this->m_all_nodes = std::move(other.m_all_nodes);

        // Do not forget to recreate a new empty RB tree for the
        // other Graph so that it continues to be usable
        other.m_all_nodes = std::make_unique<NodeSPContainer>();

        return *this;
    }

    template<typename NodeValue, typename EdgeWeight>
    inline void Graph<NodeValue, EdgeWeight>::copy(NodeSPContainer & copy_to, const NodeSPContainer & copy_from)
    {
        // Copy all raw nodes from the other graph to this graph
        for (NodeSharedPtr n_ptr : copy_from)
        {
            NodeSharedPtr my_n_ptr = std::make_shared<GraphNode>(n_ptr->m_value);
            copy_to.insert(my_n_ptr);
        }

        // Copy all edges (entries) from the other graph to this graph
        for (NodeSharedPtr n_ptr : copy_from)
        {
            NodeSharedPtr my_n_ptr = *(copy_to.find(n_ptr));

            for (EntrySharedPtr e_ptr : n_ptr->m_children)
            {
                NodeSharedPtr my_n_child = *(copy_to.find(e_ptr->m_node_ptr.lock()));
                EntrySharedPtr new_e_ptr = std::make_shared<GraphEntry>(my_n_child, e_ptr->m_edges);
                my_n_ptr->m_children.insert(new_e_ptr);

                my_n_ptr->m_out_degree += new_e_ptr->m_edges.size();
                my_n_child->m_in_degree += new_e_ptr->m_edges.size();
            }
        }
    }

    template<typename NodeValue, typename EdgeWeight>
    inline bool Graph<NodeValue, EdgeWeight>::addNode(const NodeValue & val)
    {
        NodeSharedPtr new_n_ptr = std::make_shared<GraphNode>(val);
        if (0 != this->m_all_nodes->count(new_n_ptr))
        {
            return false;
        }

        this->m_all_nodes->insert(new_n_ptr);
        return true;
    }

    template<typename NodeValue, typename EdgeWeight>
    inline bool Graph<NodeValue, EdgeWeight>::addEdge(const NodeValue & src, const NodeValue & dst, const EdgeWeight & weight)
    {
        // Create some arbitrary shared ptrs as key to search in the binary tree
        NodeSharedPtr src_key = std::make_shared<GraphNode>(src);
        NodeSharedPtr dst_key = std::make_shared<GraphNode>(dst);
        if (0 == this->m_all_nodes->count(src_key) || 0 == this->m_all_nodes->count(dst_key))
        {
            throw std::runtime_error("Src or Dst of this edge does not exist!");
        }

        NodeSharedPtr src_ptr = *(this->m_all_nodes->find(src_key));
        NodeSharedPtr dst_ptr = *(this->m_all_nodes->find(dst_key));

        EntrySharedPtr new_c_entry = std::make_shared<GraphEntry>(dst_ptr);
        if (0 != src_ptr->m_children.count(new_c_entry))
        {
            EntrySharedPtr c_entry = *(src_ptr->m_children.find(new_c_entry));
            if (0 != c_entry->m_edges.count(weight))
            {
                // Edge of this weight already exists between these two graph nodes
                return false;
            }
            else // The weight does not exist
            {
                c_entry->m_edges.insert(weight);
            }
        }
        else // There is no connection between these graph nodes
             // add a new entry including the new weight
        {
            new_c_entry->m_edges.insert(weight);
            src_ptr->m_children.insert(new_c_entry);
        }

        ++src_ptr->m_out_degree;
        ++dst_ptr->m_in_degree;

        return true;
    }

    template<typename NodeValue, typename EdgeWeight>
    inline bool Graph<NodeValue, EdgeWeight>::replace(const NodeValue & old_data, const NodeValue & new_data)
    {
        NodeSharedPtr old_key = std::make_shared<GraphNode>(old_data);
        NodeSharedPtr new_key = std::make_shared<GraphNode>(new_data);
        if (0 == this->m_all_nodes->count(old_key))
        {
            throw std::runtime_error("The old data does not exist!");
        }

        if (0 != this->m_all_nodes->count(new_key))
        {
            return false;
        }

        // Get the node instance
        NodeSharedPtr node_ptr = (*(this->m_all_nodes->find(old_key)));
        EntrySharedPtr entry_key = std::make_shared<GraphEntry>(node_ptr);

        // Connections (src -> dst) affected
        std::vector<std::pair<NodeSharedPtr, EntrySharedPtr>> affected_entries;

        if (node_ptr->m_in_degree > 0)
        {
            // Traverse the graph to find affected parent nodes
            // Time complexity may be poor for graphs whose vertices' degrees are low
            for (NodeSharedPtr n_ptr : *(this->m_all_nodes))
            {
                if (0 != n_ptr->m_children.count(entry_key))
                {
                    EntrySharedPtr affected_entry = *(n_ptr->m_children.find(entry_key));
                    affected_entries.push_back(std::pair<NodeSharedPtr, EntrySharedPtr>(n_ptr, affected_entry));
                    // Remove this entry since value of its destination node will change.
                    // The changed value will make the data set corrupt.
                    // The removed entry will be restored afterwards
                    n_ptr->m_children.erase(entry_key);
                }
            }
        }

        // Delete the node with the old value from the table
        this->m_all_nodes->erase(old_key);
        node_ptr->m_value = new_data;
        // Insert the node to the table after its value is changed
        this->m_all_nodes->insert(node_ptr);
        
        // Go through all affected connections to recover all affected entries
        for (std::pair<NodeSharedPtr, EntrySharedPtr> entry : affected_entries)
        {
            entry.first->m_children.insert(entry.second);
        }

        return true;
    }


    template<typename NodeValue, typename EdgeWeight>
    inline void Graph<NodeValue, EdgeWeight>::mergeReplace(const NodeValue & old_data, const NodeValue & new_data)
    {
        // Create some arbitrary shared ptrs as key to search in the binary tree
        if (!(old_data < new_data) && !(new_data < old_data))
        {
            return;
        }

        NodeSharedPtr old_key = std::make_shared<GraphNode>(old_data);
        NodeSharedPtr new_key = std::make_shared<GraphNode>(new_data);
        if (0 == this->m_all_nodes->count(old_key) || 0 == this->m_all_nodes->count(new_key))
        {
            throw std::runtime_error("Either old data or new data does not exist!");
        }

        // Get the node instance via the old value
        NodeSharedPtr old_node_ptr = *(this->m_all_nodes->find(old_key));
        // Get the node instance via the new value
        NodeSharedPtr new_node_ptr = *(this->m_all_nodes->find(new_key));

        // Backup all the in-edges and out-edges
        EntrySharedPtr old_node_entry_key = std::make_shared<GraphEntry>(old_node_ptr);
        std::vector<std::pair<NodeSharedPtr, EntrySharedPtr>> affected_entries;

        // The child nodes affected should distract in_degree, backup these removed edges
        if (old_node_ptr->m_out_degree > 0)
        {
            for (EntrySharedPtr entry_ptr : old_node_ptr->m_children)
            {
                affected_entries.push_back(std::pair<NodeSharedPtr, EntrySharedPtr>(old_node_ptr, entry_ptr));
            }
        }

        // The parent nodes affected should remove and backup their affected edges
        if (old_node_ptr->m_in_degree > 0)
        {
            for (NodeSharedPtr n_ptr : *(this->m_all_nodes))
            {
                if (0 != n_ptr->m_children.count(old_node_entry_key))
                {
                    EntrySharedPtr affected_entry = *(n_ptr->m_children.find(old_node_entry_key));

                    affected_entries.push_back(std::pair<NodeSharedPtr, EntrySharedPtr>(n_ptr, affected_entry));
                }
            }
        }

        // Go over all the edges, update src or dst of edges and
        // update all nodes related to the new node
        for (std::pair<NodeSharedPtr, EntrySharedPtr> entry : affected_entries)
        {
            NodeSharedPtr src_ptr = entry.first;
            NodeSharedPtr dst_ptr = entry.second->m_node_ptr.lock();

            // Remove original edges (entries)
            if (dst_ptr == old_node_ptr)
            {
                src_ptr->m_children.erase(entry.second);
                src_ptr->m_out_degree -= entry.second->m_edges.size();
            }
            if (src_ptr == old_node_ptr)
            {
                dst_ptr->m_in_degree -= entry.second->m_edges.size();
            }

            // Update destination node of entries
            if (dst_ptr == old_node_ptr)
            {
                dst_ptr = new_node_ptr;
                entry.second->m_node_ptr = new_node_ptr;
            }
            if (src_ptr == old_node_ptr)
            {
                src_ptr = new_node_ptr;
            }

            // Child nodes clash (There is an entry of the same child node already existing)
            // Merge the edges of different weights
            if (0 != src_ptr->m_children.count(entry.second))
            {
                EntrySharedPtr existing_entry_ptr = *(src_ptr->m_children.find(entry.second));

                for (EdgeWeight weight : entry.second->m_edges)
                {
                    if (0 == existing_entry_ptr->m_edges.count(weight))
                    {
                        existing_entry_ptr->m_edges.insert(weight);
                    }
                }
            }
            else // If child nodes do not clash
                 // Add the new entry directly
            {
                src_ptr->m_children.insert(entry.second);
            }

            src_ptr->m_out_degree += entry.second->m_edges.size();
            dst_ptr->m_in_degree += entry.second->m_edges.size();
        }

        // Remove the old node from the table
        this->m_all_nodes->erase(old_key);
    }



    template<typename NodeValue, typename EdgeWeight>
    inline void Graph<NodeValue, EdgeWeight>::deleteNode(const NodeValue & val) noexcept
    {
        NodeSharedPtr delete_key = std::make_shared<GraphNode>(val);
        if (0 == this->m_all_nodes->count(delete_key))
        {
            return;
        }

        NodeSharedPtr to_delete_ptr = *(this->m_all_nodes->find(delete_key));
        EntrySharedPtr entry_delete_key = std::make_shared<GraphEntry>(to_delete_ptr);
        // All affected connections (src -> dst) will be stored in this vector
        std::vector<std::pair<NodeSharedPtr, EntrySharedPtr>> affected_entries;

        // All affected parent nodes
        if (to_delete_ptr->m_in_degree > 0)
        {
            for (NodeSharedPtr src_ptr : *(this->m_all_nodes))
            {
                if (0 != src_ptr->m_children.count(entry_delete_key))
                {
                    EntrySharedPtr affected_entry = *(src_ptr->m_children.find(entry_delete_key));
                    affected_entries.push_back(std::pair<NodeSharedPtr, EntrySharedPtr>(src_ptr, affected_entry));
                }
            }
        }

        // All affected child nodes
        if (to_delete_ptr->m_out_degree > 0)
        {
            for (EntrySharedPtr entry_ptr : to_delete_ptr->m_children)
            {
                affected_entries.push_back(std::pair<NodeSharedPtr, EntrySharedPtr>(to_delete_ptr, entry_ptr));
            }
        }

        // Go over all the edges, update src or dst of edges and
        // update all edges related to the deleted node
        for (std::pair<NodeSharedPtr, EntrySharedPtr> entry : affected_entries)
        {
            NodeSharedPtr src_ptr = entry.first;
            NodeSharedPtr dst_ptr = entry.second->m_node_ptr.lock();

            // Remove relevant edges
            if (dst_ptr == to_delete_ptr)
            {
                src_ptr->m_children.erase(entry.second);
                src_ptr->m_out_degree -= entry.second->m_edges.size();
            }
            if (src_ptr == to_delete_ptr)
            {
                dst_ptr->m_in_degree -= entry.second->m_edges.size();
            }
        }

        this->m_all_nodes->erase(to_delete_ptr);
    }

    template<typename NodeValue, typename EdgeWeight>
    inline void Graph<NodeValue, EdgeWeight>::deleteEdge(const NodeValue & src, const NodeValue & dst, const EdgeWeight & weight) noexcept
    {
        NodeSharedPtr src_key = std::make_shared<GraphNode>(src);
        NodeSharedPtr dst_key = std::make_shared<GraphNode>(dst);

        if (0 == this->m_all_nodes->count(src_key))
        {
            return;
        }

        if (0 == this->m_all_nodes->count(dst_key))
        {
            return;
        }

        NodeSharedPtr src_ptr = *(this->m_all_nodes->find(src_key));
        NodeSharedPtr dst_ptr = *(this->m_all_nodes->find(dst_key));


        EntrySharedPtr entry_key = std::make_shared<GraphEntry>(dst_ptr);
        if (0 != src_ptr->m_children.count(entry_key))
        {
            EntrySharedPtr c_entry = *(src_ptr->m_children.find(entry_key));
            if (0 != c_entry->m_edges.count(weight))
            {
                c_entry->m_edges.erase(weight);
                if (c_entry->m_edges.empty())
                {
                    src_ptr->m_children.erase(c_entry);
                }

                --src_ptr->m_out_degree;
                --dst_ptr->m_in_degree;
            }
            else
            {
                return;
            }
        }
        else
        {
            return;
        }
    }

    template<typename NodeValue, typename EdgeWeight>
    inline void Graph<NodeValue, EdgeWeight>::clear() noexcept
    {
        this->m_all_nodes->clear();
    }

    template<typename NodeValue, typename EdgeWeight>
    inline bool Graph<NodeValue, EdgeWeight>::isNode(const NodeValue & val) const
    {
        NodeSharedPtr node_key = std::make_shared<GraphNode>(val);
        if (0 == this->m_all_nodes->count(node_key))
        {
            return false;
        }

        return true;
    }

    template<typename NodeValue, typename EdgeWeight>
    inline Graph<NodeValue, EdgeWeight>::~Graph()
    {
    }

    template<typename NodeValue, typename EdgeWeight>
    inline bool Graph<NodeValue, EdgeWeight>::isConnected(const NodeValue & src, const NodeValue & dst) const
    {
        NodeSharedPtr src_key = std::make_shared<GraphNode>(src);
        NodeSharedPtr dst_key = std::make_shared<GraphNode>(dst);
        if (0 == this->m_all_nodes->count(src_key) || 0 == this->m_all_nodes->count(dst_key))
        {
            throw std::runtime_error("Either src or dst does not exist in this graph.");
        }

        NodeSharedPtr src_ptr = *(this->m_all_nodes->find(src_key));
        NodeSharedPtr dst_ptr = *(this->m_all_nodes->find(dst_key));
        EntrySharedPtr entry_key = std::make_shared<GraphEntry>(dst_ptr);

        if (0 == src_ptr->m_children.count(entry_key))
        {
            return false;
        }
        return true;
    }

    // Time complexity of this print is n * log (n), n is number of nodes
    template<typename NodeValue, typename EdgeWeight>
    inline void Graph<NodeValue, EdgeWeight>::printNodes() const
    {
        this->printNodes(std::cout);
    }

    // Time complexity of this print is n * log (n), n is number of nodes
    template<typename NodeValue, typename EdgeWeight>
    inline void Graph<NodeValue, EdgeWeight>::printNodes(std::ostream & out) const
    {
        std::vector <NodeSharedPtr> nodes;
        for (NodeSharedPtr node_ptr : *(this->m_all_nodes))
        {
            nodes.push_back(node_ptr);
        }

        // sort by out degree, time complexity n * log(n)
        std::sort(nodes.begin(), nodes.end(),
            [](const NodeSharedPtr &left, const NodeSharedPtr &right)
        {
            return left->m_out_degree < right->m_out_degree;
        });

        for (NodeSharedPtr node_ptr : nodes)
        {
            out << node_ptr->m_value << '\n';
        }
    }


    // Time complexity of print is n * log (n), n is out degree of the node
    template<typename NodeValue, typename EdgeWeight>
    inline void Graph<NodeValue, EdgeWeight>::printEdges(const NodeValue & val) const
    {
        this->printEdges(val, std::cout);
    }

    // Time complexity of print is n * log (n), n is out degree of the node
    template<typename NodeValue, typename EdgeWeight>
    inline void Graph<NodeValue, EdgeWeight>::printEdges(const NodeValue & val, std::ostream &out) const
    {
        NodeSharedPtr src_key = std::make_shared<GraphNode>(val);
        if (0 == this->m_all_nodes->count(src_key))
        {
            throw std::runtime_error("The node does not exist!");
        }

        NodeSharedPtr src_ptr = *(this->m_all_nodes->find(src_key));

        out << "Edges attached to Node " << src_ptr->m_value << '\n';

        if (src_ptr->m_children.empty())
        {
            out << "(null)\n";
            return;
        }

        std::vector<std::pair<NodeSharedPtr, EdgeWeight>> entries;
        for (EntrySharedPtr entry_ptr : src_ptr->m_children)
        {
            for (EdgeWeight weight : entry_ptr->m_edges)
            {
                entries.push_back(std::pair<NodeSharedPtr, EdgeWeight>(entry_ptr->m_node_ptr.lock(), weight));
            }
        }

        // Sort by weight of each edge
        std::sort(entries.begin(), entries.end(),
            [](const std::pair<NodeSharedPtr, EdgeWeight> &left, const std::pair<NodeSharedPtr, EdgeWeight> &right)
        {
            return left.second < right.second;
        });

        for (std::pair<NodeSharedPtr, EdgeWeight> entry : entries)
        {
            out << entry.first->m_value << ' ';
            out << entry.second << '\n';
        }
    }

    template<typename NodeValue, typename EdgeWeight>
    inline void Graph<NodeValue, EdgeWeight>::printAll() const
    {
        std::cout << "********** Print ALL ***********\n";
        std::vector <NodeSharedPtr> nodes;
        for (NodeSharedPtr node_ptr : *(this->m_all_nodes))
        {
            nodes.push_back(node_ptr);
        }

        // Sort by out degree
        std::sort(nodes.begin(), nodes.end(),
            [](const NodeSharedPtr &left, const NodeSharedPtr &right)
        {
            return left->m_out_degree < right->m_out_degree;
        });

        for (NodeSharedPtr node : nodes)
        {
            this->printEdges(node->m_value);
        }
    }

    template<typename NodeValue, typename EdgeWeight>
    inline void Graph<NodeValue, EdgeWeight>::begin() const
    {
        this->m_itr = this->m_all_nodes->begin();
    }

    template<typename NodeValue, typename EdgeWeight>
    inline bool Graph<NodeValue, EdgeWeight>::end() const
    {
        return (this->m_itr == this->m_all_nodes->end());
    }

    template<typename NodeValue, typename EdgeWeight>
    inline void Graph<NodeValue, EdgeWeight>::next() const
    {
        this->m_itr++;
    }

    template<typename NodeValue, typename EdgeWeight>
    inline const NodeValue & Graph<NodeValue, EdgeWeight>::value() const
    {
        return (*(this->m_itr))->m_value;
    }
}
