#ifndef _selectionUndoableCommands
#define _selectionUndoableCommands
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "undoableCommand.h"
#include "globalSelection.h"
#include "sceneItem.h"
#include "path.h"

UFE_NS_DEF {

//! \brief Append an item to the argument selection.
/*!
  This class appends an item from the argument selection on execution, and
  removes it on undo.
*/

class UFE_SDK_DECL SelectionAppendItem : public UndoableCommand
{
public:

    //! Convenience method to create the undoable command and execute it.
    //! \return Pointer to the undoable command.
    static Ptr createAndExecute(
        const GlobalSelection::Ptr& sn,
        const SceneItem::Ptr&       item
    );

    //! Constructor.
    SelectionAppendItem(
        const GlobalSelection::Ptr& sn,
        const SceneItem::Ptr&       item
    );

    //! Undo the append (i.e. remove).
    void undo() override;
    //! Redo the append.
    void redo() override;

private:

    const GlobalSelection::Ptr fSn;
    // Store path instead of scene item.
    // A valid scene item at command creation may be invalidated by a sequence
    // of undos, followed by corresponding redos.
    const Path                 fPath;
};

//! \brief Remove an item from the argument selection.
/*!
  This class removes an item from the argument selection on execution, and
  appends it on undo.
*/

class UFE_SDK_DECL SelectionRemoveItem : public UndoableCommand
{
public:

    //! Convenience method to create the undoable command and execute it.
    //! \return Pointer to the undoable command.
    static Ptr createAndExecute(
        const GlobalSelection::Ptr& sn,
        const SceneItem::Ptr&       item
    );

    //! Constructor.
    SelectionRemoveItem(
        const GlobalSelection::Ptr& sn,
        const SceneItem::Ptr&       item
    );

    //! Undo the remove (i.e. append).
    void undo() override;
    //! Redo the remove.
    void redo() override;

private:

    const GlobalSelection::Ptr fSn;
    // Store path instead of scene item.
    // A valid scene item at command creation may be invalidated by a sequence
    // of undos, followed by corresponding redos.
    const Path                 fPath;
};

}

#endif /* _selectionUndoableCommands */
