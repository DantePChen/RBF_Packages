#ifndef _contextOps
#define _contextOps
// ===========================================================================
// Copyright 2020 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "common/ufeExport.h"

#include "sceneItem.h"
#include "undoableCommand.h"

#include <memory>
#include <vector>
#include <string>

UFE_NS_DEF {

//! \brief Structure to describe contextual operations.
/*!

  Provides simple aggregation to describe the presentation and characteristics
  a contextual operation.
*/
struct UFE_SDK_DECL ContextItem {

    enum HasChildren { kNoChildren = false, kHasChildren = true };
    enum Checkable { kNotCheckable = false, kCheckable = true };
    enum Checked { kNotChecked = false, kChecked = true };
    enum Exclusive { kNotExclusive = false, kExclusive = true };
    enum Enabled { kDisabled = false, kEnabled = true };

    enum SeparatorTag { kSeparator };

    ContextItem() = default;
    ContextItem(
        const std::string& argItem,
        const std::string& argLabel,
        bool               argHasChildren = false,
        bool               argCheckable = false,
        bool               argChecked = false,
        bool               argExclusive = false,
        bool               argEnabled = true
    );
    //! Convenience constructor for creating an item with image icon.
    ContextItem(
        const std::string& argItem,
        const std::string& argLabel,
        const std::string& argImage,
        bool               argHasChildren = false,
        bool               argCheckable = false,
        bool               argChecked = false,
        bool               argExclusive = false,
        bool               argEnabled = true
    );
    //! Convenience constructor for creating a separator item, with optional label.
    ContextItem(SeparatorTag, const std::string& argLabel = "");

    //! The runtime's string that describes the operation or option.
    std::string item{};

    //! The user-visible string, possibly localized.
    std::string label{};

    //! The filename of the icon associated with the item.
    std::string image{};

    //! Does the item have children, i.e. an operation with options, or an
    //! option with sub-options?
    bool hasChildren{false};

    //! Can the item be in a checked state?
    bool checkable{false};

    //! Is the item checked?
    bool checked{false};

    //! Is the item part of a set of checkable items where a single item can be
    //! checked (mutually exclusive items)?
    bool exclusive{false};

    //! Is the item enabled?  A disabled item will be grayed out.
    bool enabled{true};

    //! Is this item a separator?
    bool separator{false};
};

//! \brief Abstract base class for context operations interface.
/*!

  This base class defines the interface that runtimes can implement to
  provide contextual operation support.  As per

  https://en.wikipedia.org/wiki/Context_menu

  a context menu presents operations available on the selected item, given the
  current state.  This interface can be used to describe and implement
  these contextual operations.  The operations are organized in a
  lazily-populated tree structure, which is oriented to filling in a context
  menu hierarchy.
*/

class UFE_SDK_DECL ContextOps
{
public:
    typedef std::shared_ptr<ContextOps> Ptr;
    typedef std::vector<std::string>    ItemPath;
    typedef std::vector<ContextItem>    Items;
    
    /*!
        Convenience method that calls the contextOps method on the 
        ContextOps handler for the item.  Returns a null pointer if the
        argument is null, or has an empty path.
        \param item SceneItem's ContextOps to retrieve
        \return ContextOps interface for the given SceneItem
    */
    static Ptr contextOps(const SceneItem::Ptr& item);

    //! Constructor.
    ContextOps();
    //! Default copy constructor.
    ContextOps(const ContextOps&) = default;
    //! Destructor.
    virtual ~ContextOps();

    //! Scene item accessor.
    virtual SceneItem::Ptr sceneItem() const = 0;

    //! Get the child items of the argument.  An empty vector means to return
    //! the top-level operations.
    virtual Items getItems(const ItemPath& itemPath) const = 0;

    //! Perform the operation described by the itemPath.  Failure should be
    //! reported by throwing an exception derived from std::exception.
    //! Implementation in this class calls doOpCmd() and executes the return
    //! command, if non-null.
    //! \param itemPath The list of tokens that describes the operator.
    virtual void doOp(const ItemPath& itemPath);

    //! Return a command for undo / redo that performs the operation described
    //! by the itemPath.  The returned command is not executed; it is up to the
    //! called to call execute().  If the operation is not undoable, a null
    //! pointer is returned.  Failure should be reported by throwing an
    //! exception derived from std::exception.
    //! \param itemPath The list of tokens that describes the operator.
    //! \return Undoable command to perform the operation, or null if the
    //!         operation is not undoable.
    virtual UndoableCommand::Ptr doOpCmd(const ItemPath& itemPath) = 0;
};

}

#endif /* _contextOps */
