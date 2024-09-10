#ifndef _sceneItemOps
#define _sceneItemOps
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "common/ufeExport.h"

#include "sceneItem.h"
#include "undoableCommand.h"

#include <memory>

UFE_NS_DEF {

//! \brief Result for undoable duplicate item.
/*!

  This struct provides simple pair-like aggregation for the result of
  SceneItemOps::duplicateItemCmd.
*/
struct UFE_SDK_DECL Duplicate {
    //! Initialize the item and the undoable command to null.
    Duplicate();

    //! Construct.
    Duplicate(const SceneItem::Ptr&, const UndoableCommand::Ptr&);

    //! Root of the duplicate hierarchy.
    SceneItem::Ptr       item;
    //! Undoable command whose undo removes the duplicated object(s).
    UndoableCommand::Ptr undoableCommand;
};

//! \brief Result for undoable rename item.
/*!

  This struct provides simple pair-like aggregation for the result of
  SceneItemOps::renameItemCmd.
*/

struct UFE_SDK_DECL Rename {
    //! Initialize the item and the undoable command to null.
    Rename();

    //! Construct.
    Rename(const SceneItem::Ptr&, const UndoableCommand::Ptr&);

    //! Renamed item.
    SceneItem::Ptr       item;
    //! Undoable command whose undo restores the original item's name.
    UndoableCommand::Ptr undoableCommand;
};

//! \brief Abstract base class for scene item operations interface.
/*!

  This base class defines the interface that run-times can implement to
  operate on a given selection item as a whole.  These include operations like:
  - deleting an item
  - renaming an item
  - duplicating an item and its sub-hierarchy of items

  Each operation has two calls, one with undo capability, and one without.
  Providing undo capability typically involves code complexity, and using undo
  capability incurs run-time cost in processing and memory.  Therefore, non-
  interactive use of this interface should use calls without undo capability.

  To avoid the memory-consuming "one proxy object per scene object" approach,
  SceneItemOps interface objects should be obtained and used within a local
  scope, and not stored.  SceneItemOps interfaces should be considered
  stateless, and can be bound to new selection items.
*/

class UFE_SDK_DECL SceneItemOps
{
public:
    typedef std::shared_ptr<SceneItemOps> Ptr;
    
    /*!
        Convenience method that calls the sceneItemOps method on the 
        SceneItemOps handler for the item.  Returns a null pointer if the
        argument is null, or has an empty path.
        \param item SceneItem's SceneItemOps to retrieve
        \return SceneItemOps of the given SceneItem
    */
    static Ptr sceneItemOps(const SceneItem::Ptr& item);

    //! Constructor.
    SceneItemOps();
    //! Default copy constructor.
    SceneItemOps(const SceneItemOps&) = default;
    //! Destructor.
    virtual ~SceneItemOps();

    //! Scene item accessor.
    virtual SceneItem::Ptr sceneItem() const = 0;

    //! Remove the item from participation in the scene.
    //! \return Pointer to an undoable command object whose undo method will restore the object.
    virtual UndoableCommand::Ptr deleteItemCmd() = 0;

    //! Remove the item from participation in the scene.
    //! \return True for success.
    virtual bool deleteItem() = 0;
    
    //! Duplicate the item and all its descendants.  If duplication cannot be
    //! done, both the item and the undoable command will be null pointers.
    //! \return Duplicate object containing the root of the duplicate hierarchy and a pointer to an undoable command whose undo removes the duplicated object(s).
    virtual Duplicate duplicateItemCmd() = 0;

    //! Duplicate the item and all its descendants.
    //! \return The root of the duplicate hierarchy, if successful, else a null pointer.
    virtual SceneItem::Ptr duplicateItem() = 0;
    
    //! Rename the scene item by changing the tail of its path.  After the
    //! rename, this interface object will be updated to the new scene item.
    //! \return Rename object containing the renamed item and a pointer to an undoable command object whose undo method restores the original name.
    virtual Rename renameItemCmd(const PathComponent& newName) = 0;

    //! Rename the scene item by changing the tail of its path.  After the
    //! rename, this interface object will be updated to the new scene item.
    //! \return The renamed item, if successful, else a null pointer.
    virtual SceneItem::Ptr renameItem(const PathComponent& newName) = 0;
};

}

#endif /* _sceneItemOps */
