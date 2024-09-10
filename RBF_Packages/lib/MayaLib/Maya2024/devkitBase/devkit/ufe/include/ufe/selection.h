#ifndef _ufeSelection
#define _ufeSelection
// ===========================================================================
// Copyright 2020 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "sceneItemList.h"
#include "sceneItem.h"
#include "selectionTrieFwd.h"

UFE_NS_DEF {

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
        Insert item just before the position item.  If the item is already in
        the list, do nothing and return false.  If position is the null pointer,
        append.  Will throw an exception if the position argument is not found.
        \param position SceneItem before which to insert into selection list.
        \param item SceneItem to insert into selection list.
        \return True if the item was inserted into selection list.
        \exception std::invalid_argument if item is a null pointer, or if position is not found.
        \complexity Amortized O(m) for an item with a path of m components.
    */
    bool insert(const SceneItem::Ptr& position, const SceneItem::Ptr& item);

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
      Query whether the selection contains the argument path or a descendant of
      the argument path. If the argument path is empty, returns false.
      \param path ancestor Path
      \return True if the argument Path or a descendant of the argument Path is in the selection
      \complexity Amortized O(m) for an argument path of m components.
    */
    bool containsDescendantInclusive(const Path& path) const;

    /*!
      Query whether the selection contains an ancestor of the argument path.
      Must be a strict ancestor: if the selection contains only the argument
      itself, returns false.  If the argument path is empty, returns false.
      \param path descendant Path
      \return True if an ancestor of the argument Path is in the selection
      \complexity Amortized O(m) for an argument path of m components.
    */
    bool containsAncestor(const Path& path) const;

    /*!
      Query whether the selection contains the argument path or an ancestor of
      the argument path. If the argument path is empty, returns false.
      \param path descendant Path
      \return True if the argument Path or an ancestor of the argument Path is in the selection
      \complexity Amortized O(m) for an argument path of m components.
    */
    bool containsAncestorInclusive(const Path& path) const;

    /*!
      Finds the single common parent of the existing selection.
      \return Common parent SceneItem if it exists, null pointer if not.
      \exception std::invalid_argument if the selection is empty.
      \complexity Amortized O(m) for a selection with m components.
    */
    SceneItem::Ptr closestCommonAncestor() const;

    /*!
      Finds the item which follows the argument item.  This can be used to undo
      the effect of remove, by using insert.
      \param item SceneItem whose successor must be found.
      \return Successor SceneItem, or null pointer if the argument is the last item in the selection.
      \exception std::invalid_argument if the argument item is not found.
      \complexity Amortized O(m) for a selection with m components.
    */
    SceneItem::Ptr next(const SceneItem::Ptr& item) const;

    //@{
    //! Element access.
    const SceneItem::Ptr& front() const;
    const SceneItem::Ptr& back() const;
    //@}

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
    //! Engine methods for append, remove, insert, clear, and replaceWith, to
    //! implement the Template Method design pattern.  See
    //!
    //! https://en.wikipedia.org/wiki/Template_method_pattern
    //!    
    bool doAppend(const SceneItem::Ptr& item);
    bool doRemove(const SceneItem::Ptr& item);
    bool doInsert(const SceneItem::Ptr& position, const SceneItem::Ptr& item);
    void doClear();
    void doReplaceWith(const Selection& selection);
    //@}

    //@{ 
    // Hook methods for append, remove, insert, clear, and replaceWith.  
    // postAppend(), postRemove(), and postInsert() are only called if
    // doAppend(), doRemove(), and doInsert() return true, respectively.
    // Implementations in this class do nothing.
    virtual void postAppend(const SceneItem::Ptr& item);
    virtual void postRemove(const SceneItem::Ptr& item);
    virtual void postInsert(
        const SceneItem::Ptr& position, const SceneItem::Ptr& item
    );
    virtual void postClear();
    virtual void postReplaceWith();
    //@}

    //@{ 
    //! Trie-based acceleration structure access.
    SelectionTrie&              trie();
    const SelectionTrie&        trie() const;
    //@}

    //! Internal SceneItem list structure access.
    //! Do not change the order of this list as the internal
    //! trie saves iterators into fList.
    inline SceneItemList&       list() { return fList; }

private:
    SceneItemList fList;

    //! Hidden trie-based untyped storage.
    //! The untyped storage avoids a dynamic memory allocation.
    void* fTrieStorage[2];
};

}

#endif /* _ufeSelection */
