#ifndef _ufeTrie_imp
#define _ufeTrie_imp
// =======================================================================
// Copyright 2019 Autodesk, Inc. All rights reserved.
//
// This computer source code and related instructions and comments are the
// unpublished confidential  and proprietary information of Autodesk, Inc.
// and are protected under applicable copyright and trade secret law. They 
// may not be disclosed to, copied  or used by any third party without the 
// prior written consent of Autodesk, Inc.
// =======================================================================

#include "trie.h"
#include "ufeAssert.h"

#include <numeric>

UFE_NS_DEF {

//==============================================================================
// CLASS Ufe::TrieNode
//==============================================================================

template<typename T>
TrieNode<T>::TrieNode(const PathComponent& component)
    : fComponent(component), fParent(), fChildren(), fHasData(false), 
      fData()
{}

template<typename T>
TrieNode<T>::TrieNode() : TrieNode(PathComponent())
{}

template<typename T>
void TrieNode<T>::add(const Ptr& child)
{
    // Child must not exist yet.
    UFE_ASSERT_MSG(fChildren.find(child->component()) == fChildren.end(),
                   "Child trie node already exists.");

    fChildren[child->component()] = child;
    // Go through this pointer to prevent "no arguments to ‘shared_from_this’
    // that depend on a template parameter" error.
    child->setParent(this->shared_from_this());
}

template<typename T>
void TrieNode<T>::remove(const Ptr& child)
{
    UFE_ASSERT_COMPILED(typename Children::size_type nbErased = )
        fChildren.erase(child->component());
    UFE_ASSERT_MSG(nbErased == 1, "Child trie node does not exist.");
    child->setParent(Ptr());
}

template<typename T>
void TrieNode<T>::clear()
{
    fComponent = PathComponent();
    fParent = ParentPtr();
    fChildren.clear();
    fHasData = false;
}

template<typename T>
typename TrieNode<T>::Ptr TrieNode<T>::parent() const
{
    return fParent.lock(); 
}

template<typename T>
bool TrieNode<T>::contains(const PathComponent& component) const
{
    return fChildren.find(component) != fChildren.end();
}

template<typename T>
typename TrieNode<T>::Ptr TrieNode<T>::operator[](const PathComponent& component) const
{
    typename Children::const_iterator const found = fChildren.find(component);
    return found == fChildren.end() ? Ptr() : found->second;
}

template<typename T>
int TrieNode<T>::closestCommonAncestor( int depth ) const
{
    std::size_t sz = size();

    // If this node has data or it's a leaf node then return
    // before incrementing.  The common ancestor is our
    // parent.
    //
    // In practice both of these conditions should be the same,
    // but we can call this function with an empty trie in which
    // case we will have size 0 and no data in the empty root node.
    if ( hasData() || 0 == sz) {
        return depth;
    }

    depth++;

    // If this node has multiple children then it is the 
    // common ancestor.
    if (1 < sz) {
        return depth;
    }

    // Else we need to keep going...
    typename Children::const_iterator const next = fChildren.begin();
    return next->second->closestCommonAncestor(depth);
}

template<typename T>
std::size_t TrieNode<T>::size() const
{
    return fChildren.size();
}

template<typename T>
std::size_t TrieNode<T>::treeSize() const
{
    return std::accumulate(
        fChildren.begin(), fChildren.end(), std::size_t(1),
        [](std::size_t treeSize, const typename Children::value_type& child) {
            return treeSize+child.second->treeSize();
        });
}

template<typename T>
bool TrieNode<T>::empty() const
{
    return size() == 0;
}

template<typename T>
PathComponent TrieNode<T>::component() const
{
    return fComponent;
}

template<typename T>
void TrieNode<T>::rename(const PathComponent& component)
{
    UFE_ASSERT_MSG(parent()->contains(fComponent),
                   "Cannot rename child trie node, not found in parent.");
    auto self = this->shared_from_this();
    auto p = parent();
    p->remove(self);
    fComponent = component;
    p->add(self);
}

template<typename T>
void TrieNode<T>::move(const PathComponent& component, const Ptr& newParent)
{
    auto oldParent = parent();
    if (newParent == oldParent)
        return;
    UFE_ASSERT_MSG(!newParent->contains(component), "Cannot reparent child trie node, new component already exists in new parent.");
    UFE_ASSERT_MSG(oldParent->contains(fComponent), "Cannot reparent child trie node, not found in parent.");
    auto self = this->shared_from_this();
    oldParent->remove(self);
    fComponent = component;
    newParent->add(self);
}

template<typename T>
void TrieNode<T>::setData(const T& data)
{
    fHasData = true;
    fData = data;
}

template<typename T>
void TrieNode<T>::clearData()
{
    fHasData = false;
}

template<typename T>
const T& TrieNode<T>::data() const
{
    return fData;
}

template<typename T>
bool TrieNode<T>::hasData() const
{
    return fHasData;
}

template<typename T>
void TrieNode<T>::setParent(Ptr parent)
{
    fParent = ParentPtr(parent);
}

//==============================================================================
// CLASS Ufe::Trie
//==============================================================================

template<typename T>
Trie<T>::Trie() : fRoot(std::make_shared< TrieNode<T> >())
{}

template<typename T>
Trie<T>::Trie(Trie&& rhs) : fRoot(std::move(rhs.fRoot))
{
    rhs.fRoot = std::make_shared< TrieNode<T> >();
}

template<typename T>
Trie<T>& Trie<T>::operator=(Trie&& rhs)
{
    fRoot = std::move(rhs.fRoot);
    rhs.fRoot = std::make_shared< TrieNode<T> >();
    return *this;
}

template<typename T>
typename TrieNode<T>::Ptr Trie<T>::root() const
{
    return fRoot;
}

template<typename T>
typename TrieNode<T>::Ptr Trie<T>::createNode(const Path& path)
{
    // Walk down the path inside the tree, adding trie nodes if required.
    typename TrieNode<T>::Ptr trieNode = root();
    for (const PathComponent& c : path) {
        typename TrieNode<T>::Ptr child = (*trieNode)[c];
        if (!child) {
            child = std::make_shared< TrieNode<T> >(c);
            trieNode->add(child);
        }
        trieNode = child;
    }

    return trieNode;
}

template<typename T>
typename TrieNode<T>::Ptr Trie<T>::add(const Path& path, const T& data)
{
    typename TrieNode<T>::Ptr trieNode = createNode(path);

    // Last trie node gets the data.
    trieNode->setData(data);
    return trieNode;
}

template<typename T>
typename TrieNode<T>::Ptr Trie<T>::find(const Path& path) const
{
    // Walk down the path inside the tree.
    typename TrieNode<T>::Ptr trieNode = node(path);
    if (trieNode && trieNode->hasData())
        return trieNode;
    return typename TrieNode<T>::Ptr();
}

template<typename T>
typename TrieNode<T>::Ptr Trie<T>::node(const Path& path) const
{
    // Walk down the path inside the tree.
    typename TrieNode<T>::Ptr trieNode = root();
    for (const PathComponent& c : path) {
        typename TrieNode<T>::Ptr child = (*trieNode)[c];
        if (!child) {
            return typename TrieNode<T>::Ptr();
        }
        trieNode = child;
    }
    return trieNode;
}

template<typename T>
bool Trie<T>::contains(const Path& path) const
{
    return bool(find(path));
}

template<typename T>
bool Trie<T>::containsDescendant(const Path& path) const
{
    // Algorithm summary: walk down the path tree to the end of the argument
    // path. If that trie node has children, a descendant is in the trie. 

    // By definition, an empty path has no descendants.
    if (path.empty()) {
        return false;
    }

    // Walk down the complete path inside the tree, if possible.
    typename TrieNode<T>::Ptr trieNode = root();
    for (const PathComponent& c : path) {
        typename TrieNode<T>::Ptr child = (*trieNode)[c];
        // If we've reached a trie leaf node before the end of our path, there
        // cannot be any descendants in the trie.
        if (!child) {
            return false;
        }
        trieNode = child;
    }
    // We reached the end of the argument path.  Whether the trieNode we
    // reached has data or not, return true if it has descendants.  To
    // implement a containsDescendantInclusive, which would include the
    // argument path, we would simply return true: at this point we've reached
    // either an internal node, which by definition has children, or a leaf
    // node with no descendants, which by definition has data.
    return !trieNode->empty();
}

template<typename T>
template<bool INCLUDE_DESCENDANT>
bool Trie<T>::containsAncestorHelper(const Path& descendantPath) const
{
    // Algorithm summary: walk down the path tree trying to find a node
    // with data.

    // By definition, an empty path has no ancestors.
    if (descendantPath.empty()) {
        return false;
    }

    // Walk down the path inside the tree.  As soon as we find a trieNode with
    // data, return true.
    typename TrieNode<T>::Ptr trieNode = root();
    auto pathEndIt = descendantPath.cend();
    // When we are not including descendentPath then finding the last PathComponent of
    // descendentPath in the trie does not count. Set up the iterator so that we don't
    // check the final PathComponent.
    if (!INCLUDE_DESCENDANT) {
        pathEndIt = std::prev(pathEndIt);
    }
    for (auto pathIt = descendantPath.cbegin(); pathIt != pathEndIt; ++pathIt) {
        const PathComponent& c = *pathIt;
        typename TrieNode<T>::Ptr child = (*trieNode)[c];
        // If we've reached a trie leaf node before the end of our path, there
        // is no trie node with data as ancestor of the path.
        if (!child) {
            return false;
        }
        trieNode = child;

        // Found a trieNode with data.
        if (trieNode->hasData()) {
            return true;
        }
    }
    // We reached the end of the parent path without returning true, therefore
    // there are no ancestors.
    return false;
}

template<typename T>
bool Trie<T>::containsAncestor(const Path& descendantPath) const
{
    return containsAncestorHelper<false>(descendantPath);
}

template<typename T>
bool Trie<T>::containsAncestorInclusive(const Path& descendantPath) const
{
    return containsAncestorHelper<true>(descendantPath);
}

template<typename T>
int Trie<T>::closestCommonAncestor() const
{
    return root()->closestCommonAncestor(-1);
}

template<typename T>
typename TrieNode<T>::Ptr Trie<T>::remove(const Path& p)
{
    typename TrieNode<T>::Ptr found = find(p);
    if (!found) {
        return found;
    }

    // First, clear the data from the trie node.
    UFE_ASSERT_MSG(found->hasData(), "Trie node has no associated data.");
    found->clearData();

    // Next, clean up trie if required.  If trie node has no children and
    // no data, remove it, and recurse up to its parent.
    cleanUpNode(found);

    return found;
}

template<typename T>
void Trie<T>::cleanUpNode(const typename TrieNode<T>::Ptr& node)
{
    typename TrieNode<T>::Ptr child = node;
    while (child->empty() && !child->hasData() && child != root()) {
        typename TrieNode<T>::Ptr parent = child->parent();
        parent->remove(child);
        child = parent;
    }
}


template<typename T>
typename TrieNode<T>::Ptr Trie<T>::move(const Path& oldPath, const Path& newPath)
{
    typename TrieNode<T>::Ptr newParentNode = createNode(newPath.pop());
    typename TrieNode<T>::Ptr trieNode = node(oldPath);
    UFE_ASSERT_MSG(trieNode, "Trie does not contain path");
    typename TrieNode<T>::Ptr oldParentNode = trieNode->parent();

    trieNode->move(newPath.back(), newParentNode);
    cleanUpNode(oldParentNode);

    return trieNode;
}

template<typename T>
void Trie<T>::clear()
{
    root()->clear();
}

template<typename T>
std::size_t Trie<T>::size() const
{
    return root()->treeSize();
}

template<typename T>
bool Trie<T>::empty() const
{
    return size() == 0;
}

}

#endif  /* _ufeTrie_imp */
