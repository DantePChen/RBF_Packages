#ifndef _ufeSelectionNotification
#define _ufeSelectionNotification
// ===========================================================================
// Copyright 2020 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "notification.h"
#include "sceneItem.h"

#include <list>
#include <bitset>

UFE_NS_DEF {

//! \brief Base class for all selection notifications.
/*!

  This class is the base class for all selection changed notifications.
*/

class UFE_SDK_DECL SelectionChanged: public Notification
{
public:
    enum OpType {Append, Remove, Insert, Clear, ReplaceWith, SelectionCompositeNotification};

    //! Constructor.
    SelectionChanged(OpType opType);

    //! Default copy constructor.
    SelectionChanged(const SelectionChanged&) = default;
    
    //! Destructor.
    ~SelectionChanged() override;

    /*! 
        \return the type of this notification
    */
    OpType opType() const;
private:
    const OpType fOpType;
};

//! \brief Item added to selection notification.
/*!

  This class notifies observers that the argument item has been added to the
  selection.
*/

class UFE_SDK_DECL SelectionItemAppended: public SelectionChanged
{
public:

    //! Constructor.
    //! \param item SceneItem appended.
    SelectionItemAppended(const SceneItem::Ptr& item);
    
    //! Default copy constructor.
    SelectionItemAppended(const SelectionItemAppended&) = default;

    //! Destructor.
    ~SelectionItemAppended() override;

    //! \return SceneItem appended.
    SceneItem::Ptr item() const;

private:
    SceneItem::Ptr fItem;
};

//! \brief Item removed from selection notification.
/*!

  This class notifies observers that the argument item has been removed from the
  selection.
*/

class UFE_SDK_DECL SelectionItemRemoved: public SelectionChanged
{
public:
    //! Constructor
    //! \param item SceneItem removed.
    SelectionItemRemoved(const SceneItem::Ptr& item);
    
    //! Default copy constructor.
    SelectionItemRemoved(const SelectionItemRemoved&) = default;

    //! Destructor.
    ~SelectionItemRemoved() override;

    //! \return SceneItem removed.
    SceneItem::Ptr item() const;

private:
    SceneItem::Ptr fItem;
};

//! \brief Item inserted into selection notification.
/*!

  This class notifies observers that the argument item has been inserted into
  the selection before item position.  Since the insertion was successful, the
  insert position can also be determined with \ref Ufe::Selection::next(), but
  is provided here for convenience.
*/

class UFE_SDK_DECL SelectionItemInserted: public SelectionChanged
{
public:
    //! Constructor
    //! \param position SceneItem following the inserted item, null if item is last in the selection.
    //! \param item SceneItem inserted.
    SelectionItemInserted(
        const SceneItem::Ptr& position,
        const SceneItem::Ptr& item
    );
    
    //! Default copy constructor.
    SelectionItemInserted(const SelectionItemInserted&) = default;

    //! Destructor.
    ~SelectionItemInserted() override;

    //! \return SceneItem inserted.
    SceneItem::Ptr item() const;

    //! \return SceneItem following inserted item.  Will be null if the item is last in the selection.
    SceneItem::Ptr position() const;

private:
    SceneItem::Ptr fPosition;
    SceneItem::Ptr fItem;
};

//! \brief Selection cleared notification.
/*!

  This class notifies observers that the selection has been cleared.
*/

class UFE_SDK_DECL SelectionCleared: public SelectionChanged
{
public:
    //! Constructor.
    SelectionCleared();
    
    //! Default copy constructor.
    SelectionCleared(const SelectionCleared&) = default;

    //! Destructor.
    ~SelectionCleared() override;
};

//! \brief Selection replaced notification.
/*!

  This class notifies observers that the existing selection has been replaced
  with a new selection.
*/

class UFE_SDK_DECL SelectionReplaced: public SelectionChanged
{
public:

    //! Constructor.
    SelectionReplaced();
    
    //! Default copy constructor.
    SelectionReplaced(const SelectionReplaced&) = default;

    //! Destructor.
    ~SelectionReplaced() override;
};

//! \brief Selection composite notification.
/*!

  This class collects multiple selection change operations inside a single
  notification.  The selection change operations are stored as a list, in the
  same order as they were performed.
*/

class UFE_SDK_DECL SelectionCompositeNotification : public SelectionChanged
{
public:

    struct UFE_SDK_DECL Op {
        Op(OpType opTypeArg) : opType(opTypeArg) {}
        Op(OpType opTypeArg, const SceneItem::Ptr& itemArg)
            : opType(opTypeArg), item(itemArg) {}
        Op(OpType                opTypeArg, 
           const SceneItem::Ptr& positionArg,
           const SceneItem::Ptr& itemArg)
            : opType(opTypeArg), item(itemArg), position(positionArg) {}

        Op(const SelectionChanged&);

        OpType opType; 
        SceneItem::Ptr item{nullptr};
        SceneItem::Ptr position{nullptr};
    };

    typedef std::list<Op> Ops;

    //! Constructor.
    SelectionCompositeNotification();

    //! Default copy constructor
    SelectionCompositeNotification(const SelectionCompositeNotification&) = default;

    //! Destructor.
    ~SelectionCompositeNotification() override;

    //! Append an append operation to the list of operations.
    void appendAppendOp(const SceneItem::Ptr& item);

    //! Append a remove operation to the list of operations.
    void appendRemoveOp(const SceneItem::Ptr& item);

    //! Append an insert operation to the list of operations.
    void appendInsertOp(
        const SceneItem::Ptr& position,
        const SceneItem::Ptr& item
    );

    //! Append a clear operation to the list of operations.
    void appendClearOp();

    //! Append a replace with operation to the list of operations.  Because the
    //! selection is completely replaced, it is assumed clients of this
    //! notification will simply consider the final state of the selection and
    //! ignore operations after a replace with operation.
    void appendReplaceWithOp();

    //! \return Size of operations in the composite notification
    std::size_t size() const;
    
    //! \return True if size() == 0.
    bool empty() const;
    
    //@{
    //! Iteration interface on operations.
    Ops::const_iterator cbegin() const;
    Ops::const_iterator begin() const;
    Ops::const_iterator cend() const;
    Ops::const_iterator end() const;
    Ops::iterator       begin();
    Ops::iterator       end();
    //@}

private:

    Ops fOps;
};

}

#endif /* _ufeSelectionNotification */
