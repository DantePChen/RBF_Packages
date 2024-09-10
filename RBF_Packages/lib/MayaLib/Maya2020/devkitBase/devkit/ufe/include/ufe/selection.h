#ifndef _ufeSelection
#define _ufeSelection
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "sceneItemList.h"
#include "sceneItem.h"

UFE_NS_DEF {

class SelectionTrie;

//! \brief Selection list for objects in the scene.
/*!

This class provides an ordered list composed of selection items.  It uses a
backing trie data structure so that on average, for a path with c components,
addition, lookup, and removal from the selection can be done in O(c) time.

*/

class UFE_SDK_DECL Selection
{
public:

    typedef SceneItemList::const_iterator const_iterator;
    typedef SceneItemList::const_reverse_iterator const_reverse_iterator;

    //! Constructor.
    Selection();
    
    //! Default copy constructor.
    Selection(const Selection&);

    //! Destructor.
    virtual ~Selection();

    //! Syntactic convenience that calls replaceWith.
    Selection& operator=(const Selection&);

    //! Move construction.  postClear is called on the argument.
    Selection(Selection&&);

    //! Move assignment.  postClear is first called on rhs, then postReplaceWith on this.
    Selection& operator=(Selection&& rhs);

    //! \return Size of scene items in the selection.
    std::size_t size() const;
    
    //! \return True if size() == 0.
    bool empty() const;
    
    /*!
        If the item is not on the selection list, append it at the end and return
        true.  If the item is already in the list, do nothing and return false.
        \param item SceneItem to append to the selection list.
        \return True if the item was added to selection list.
        \exception std::invalid_argument if the argument is a null pointer.
        \complexity Amortized O(m) for an item with a path of m components.
    */
    bool append(const SceneItem::Ptr& item);

    /*!
        Remove the item from the selection list, if present, and return true.  If
        no such item is on the list, return false.
        \param item SceneItem to remove from selection list.
        \return True if the item was removed from selection list.
        \exception std::invalid_argument if the argument is a null pointer.
        \complexity Amortized O(m) for an item with a path of m components.
    */
    bool remove(const SceneItem::Ptr& item);

    /*!
        Remove all SceneItems from the selection list.
        \complexity O(n) for a selection with n items.
    */
    void clear();

    /*!
        Replace all SceneItems in the selection list with those from the argument
        selection list.  The items are shared between both lists (shallow copy).
        \param selection Selection to replace with.
        \complexity O(n) for an existing selection and argument selection of n items. 
    */
    void replaceWith(const Selection& selection);
    
    //! \param path Path to verify if is in selection list.
    //! \return True if the given Path is in the selection list.
    //! \complexity Amortized O(m) for a path of m components.
    bool contains(const Path& path) const;

    /*!
      Query whether the selection contains a descendant of the argument path.
      Must be a strict descendant: if the selection contains only the argument
      itself, returns false.  If the argument path is empty, returns false.
      \param path ancestor Path
      \return True if a descendant of the argument Path is in the selection
      \complexity Amortized O(m) for an argument path of m components.
    */
    bool containsDescendant(const Path& path) const;

    /*!
      Query whether the selection contains an ancestor of the argument path.
      Must be a strict ancestor: if the selection contains only the argument
      itself, returns false.  If the argument path is empty, returns false.
      \param path descendant Path
      \return True if an ancestor of the argument Path is in the selection
      \complexity Amortized O(m) for an argument path of m components.
    */
    bool containsAncestor(const Path& path) const;

    //@{
    //! Iteration interface on SceneItems.
    const_iterator cbegin() const;
    const_iterator begin() const;
    const_iterator cend() const;
    const_iterator end() const;
    const_reverse_iterator crbegin() const;
    const_reverse_iterator rbegin() const;
    const_reverse_iterator crend() const;
    const_reverse_iterator rend() const;
    //@}
protected:

    //@{
    //! Engine methods for append, remove, clear, and replaceWith, to implement
    //! the Template Method design pattern.  See
    //!
    //! https://en.wikipedia.org/wiki/Template_method_pattern
    //!    
    bool doAppend(const SceneItem::Ptr& item);
    bool doRemove(const SceneItem::Ptr& item);
    void doClear();
    void doReplaceWith(const Selection& selection);
    //@}

    //@{ 
    // Hook methods for append, remove, clear, and replaceWith.  postAppend()
    // and postRemove() are only called if doAppend() and doRemove() return
    // true, respectively.  Implementations in this class do nothing.
    virtual void postAppend(const SceneItem::Ptr& item);
    virtual void postRemove(const SceneItem::Ptr& item);
    virtual void postClear();
    virtual void postReplaceWith();
    //@}
private:

    SceneItemList fList;

    //@{
    //! Hidden trie-based acceleration structure access and untyped storage.
    //! The untyped storage avoids a dynamic memory allocation.
    inline SelectionTrie&       trie();
    inline const SelectionTrie& trie() const;
    
    void* fTrieStorage[2];
    //@}
};

}

#endif /* _ufeSelection */
