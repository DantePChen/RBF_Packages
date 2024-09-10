#ifndef _ufeTrie
#define _ufeTrie
// =======================================================================
// Copyright 2019 Autodesk, Inc. All rights reserved.
//
// This computer source code and related instructions and comments are the
// unpublished confidential  and proprietary information of Autodesk, Inc.
// and are protected under applicable copyright and trade secret law. They 
// may not be disclosed to, copied  or used by any third party without the 
// prior written consent of Autodesk, Inc.
// =======================================================================

#include "pathComponent.h"
#include "path.h"

#include <memory>
#include <unordered_map>

UFE_NS_DEF {

//! \brief Node for Universal Front End trie.
/*!

Each node in the trie represents a path component in the graph, except the trie
root node, which is empty.  Each node has a dictionary of children trie nodes.

It holds the name of a node in a runtime, a dictionary of children trie nodes,
and optionally data.  The runtime node is the parent of children runtime
nodes, each of which have an associated trie node.

Trie nodes created to represent components that are iternal to the trie won't
have data.  For example, in a trivial trie, for a node with path "a|b|c|d" that
has data, trie nodes for path components a, b, and c will not have data, while
the trie node for d will.  Data can later be added to runtime node a, b, or c.
*/

template<typename T> class Trie;

template<typename T>
class TrieNode : public std::enable_shared_from_this< TrieNode<T> >
{
    friend class Trie<T>;
public:
    typedef std::shared_ptr<TrieNode> Ptr;

    TrieNode(const PathComponent& component);

    // Create a trie node with a null component.
    TrieNode();

    Ptr parent() const;

    // Child addition and removal.
    void add(const Ptr& child);
    void remove(const Ptr& child);

    // Child lookup.
    bool contains(const PathComponent& child) const;
    Ptr operator[](const PathComponent& child) const;

    // Returns the PathComponents of all the children of this node.
    std::vector<PathComponent> childrenComponents() const;

    // Number of children TrieNode's.
    std::size_t size() const;

    // Number of nodes in the tree rooted at this node, including this node.
    std::size_t treeSize() const;

    // Convenience method for size() == 0.
    bool empty() const;

    PathComponent component() const;

    // Change the component of this node, adjusting the parent accordingly.
    void rename(const PathComponent& component);

    // Change the component & parent of this node, adjusting the old and new parent accordingly.
    void move(const PathComponent& component, const Ptr& newParent);

    // Copy the data into the node, and set the has data flag.
    void setData(const T& data);

    // Access the node's data.  The hasData() method must return true for data
    // to be valid, else the return data is stale.
    const T& data() const;

    bool hasData() const;

    // Called to find the depth of the closest common ancestor to all nodes
    // with data.  Calls itself recursively looking for either multiple children
    // or the first node with associated data.
    // Increments the depth parameter as it moves down the trie which can then
    // be used to create a path that matches the specific node.
    int closestCommonAncestor( int depth ) const;

private:

    typedef std::unordered_map<PathComponent, Ptr> Children;
    typedef std::weak_ptr<TrieNode>                ParentPtr;

    // Clearing out of the TrieNode should be managed by the Trie only,
    // so keeping these private.
    void clear();
    void clearData();

    void setParent(Ptr parent);

    PathComponent fComponent;
    ParentPtr     fParent;
    Children      fChildren;
    bool          fHasData;
    T             fData;
};

//! \brief Trie.
/*!
The trie is a prefix tree data structure to allow fast hierarchical searching
by path.  The path can represent containment, or a 3D hierarchy.

Trie nodes can have data.  A trie node without data is an internal node, and
only exists to serve as parent of one or more descendant trie node(s).
*/

template<typename T>
class Trie
{
public:

    Trie();

    Trie(const Trie&) = delete;
    Trie& operator=(const Trie&) = delete;

    // Move assignment and construction.
    Trie(Trie&&);
    Trie& operator=(Trie&&);

    typename TrieNode<T>::Ptr root() const;

    typename TrieNode<T>::Ptr add(const Path& path, const T& data);
    typename TrieNode<T>::Ptr remove(const Path&);
    // Move the entire hierarchy rooted at oldPath to newPath. Returns the resulting
    // trie node if successful, else a null pointer.
    typename TrieNode<T>::Ptr move(const Path& oldPath, const Path& newPath);
    void                      clear();

    // Lookup. contains() is a convenience for bool(find(path)).
    typename TrieNode<T>::Ptr find(const Path& path) const;
    bool contains(const Path& path) const;

    // Return the trie node corresponding to the path, regardless of whether it
    // has data or not.  If the trie has no such node, returns a null pointer.
    typename TrieNode<T>::Ptr node(const Path& path) const;

    // Query whether the trie contains a descendant of the argument path.  Must
    // be a strict descendant: if the trie contains only the argument
    // itself, returns false.  If the argument path is empty, returns false.
    bool containsDescendant(const Path& path) const;

    // convenience for contains(path) ||constainsDescendant(path)
    bool containsDescendantInclusive(const Path& path) const;

    // Query whether the trie contains an ancestor of the argument path.
    // Must be a strict ancestor: if the trie contains only the argument
    // itself, returns false.  If the argument path is empty, returns false.
    bool containsAncestor(const Path& path) const;

    // convenience for contains(path) || containsAncestor(path)
    bool containsAncestorInclusive(const Path& path) const;

    // Returns the depth of the closest common ancestor of all leaf nodes.
    // Return -1 if the trie is empty.
    // Return 0 if the common ancestor is the root of the scene graph.
    int closestCommonAncestor() const;

    // Number of nodes in the trie, including the root node.  For testing
    // and debugging purposes, as it traverses the trie and counts nodes.
    std::size_t size() const;

    //! Returns true when the trie is empty.  Since a trie always has a root
    //! node, this method is a convenience for root()->empty().
    //! \complexity O(1).
    bool empty() const;

private:

    typename TrieNode<T>::Ptr createNode(const Path& path);
    void cleanUpNode(const typename TrieNode<T>::Ptr& node);
    template<bool INCLUDE_DESCENDANT>
    bool containsAncestorHelper(const Path& path) const;
    template<bool INCLUDE_ANCESTOR>
    bool containsDescendantHelper(const Path& path) const;

    typename TrieNode<T>::Ptr fRoot;
};

}

#endif  /* _ufeTrie */
