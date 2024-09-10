#ifndef _hierarchy
#define _hierarchy
// ===========================================================================
// Copyright 2020 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "common/ufeExport.h"
#include "sceneItemList.h"
#include "sceneItem.h"
#include "undoableCommand.h"

#include <memory>
#include <string>

UFE_NS_DEF {

class Selection;

//! \brief Structure to describe child filtering.
/*!

  Provides a simple method to describe a children filter flag.
*/
struct UFE_SDK_DECL ChildFilterFlag {
    ChildFilterFlag() = default;
    ChildFilterFlag(
        const std::string& filterName,
        const std::string& filterLabel,
        bool               filterValue);

    //! The implementation-specific name of this child filter flag.
    std::string name;

    //! The user-visible string, possibly localized.
    std::string label;

    //! The value for this child filter flag.
    bool value{false};
};


//! \brief Abstract base class for scene hierarchy interface.
/*!

  This base class defines the interface that runtimes can implement to
  navigate the scene hierarchy, for a given scene item.  The Hierarchy
  interface supports parent-child hierarchy semantics and queries, with the
  following characteristics:
  - an item has a single parent, or none if it is not in a hierarchy, or is a
    hierarchy root.
  - an item has 0 or more children.
  Certain DCC data models (e.g. Maya) allow objects to have multiple parents in
  the case of instancing.  This does not affect UFE single-parent semantics.
  The different UFE paths to a Maya instanced object uniquely identify each
  instance, in exact correspondence to how a Maya MDagPath uniquely identifies
  the instance to Maya.

  To avoid the memory-consuming "one proxy object per scene object" approach,
  Hierarchy interface objects should be obtained and used within a local
  scope, and not stored.  Hierarchy interfaces should be considered
  stateless, and can be bound to new selection items.
*/

class UFE_SDK_DECL Hierarchy
{
public:
    typedef std::shared_ptr<Hierarchy> Ptr;
    typedef std::list<ChildFilterFlag> ChildFilter;

    /*! Convenience method that calls the hierarchy method on the hierarchy
        handler for the item.  Returns a null pointer if the argument is null,
        or has an empty path.
        \param item SceneItem to build Hierarchy on
        \return Hierarchy of given SceneItem.
    */
    static Ptr hierarchy(const SceneItem::Ptr& item);

    /*! Convenience method that calls the createItem method on the hierarchy
        handler for the item.  Note that this method does not create an object
        in the runtime; rather, it creates a UFE scene item for an existing
        runtime object.  Returns a null pointer if the argument is an empty
        path.
        \param path Path of SceneItem to create
        \return SceneItem of argument Path
     */
    static SceneItem::Ptr createItem(const Path& path);

    //! Constructor
    Hierarchy();

    //! Default copy constructor
    Hierarchy(const Hierarchy&) = default;

    //! Destructor
    virtual ~Hierarchy();

    //! Scene item accessor.
    virtual SceneItem::Ptr sceneItem() const = 0;

    /*! Predicate to return if the SceneItem has children in the hierarchy.
        \return True if SceneItem has children
     */
    virtual bool hasChildren() const = 0;

    //! \return Ordered list of children for this Hierarchy's SceneItem.
    virtual SceneItemList children() const = 0;

    //! \return Ordered list of children filtered by input child filter for this Hierarchy's SceneItem.
    virtual SceneItemList filteredChildren(const ChildFilter&) const = 0;

    //! \return The SceneItem's parent.  Returns a null pointer if the item is the world root, and has no parent.
    virtual SceneItem::Ptr parent() const = 0;

    //! Return the parent to which this SceneItem should be appended, if it is
    //! removed from its current parent (unparent).  This will typically be the
    //! scene root, or a meaningful sub-root of a sub-hierarchy.
    //! \return The SceneItem's default parent.  Returns a null pointer if the item cannot be reparented to a default parent.
    virtual SceneItem::Ptr defaultParent() const = 0;

    //! Remove the argument child from its existing parent, and append it as a
    //! child of this SceneItem, inserting before SceneItem pos.  If pos is a
    //! null pointer, child is appended.  Because of name uniqueness, the child
    //! may be renamed.
    //! \param child Child item to be inserted.
    //! \param pos   Item before which child is to be inserted.
    //! \return SceneItem for the appended child, at its new path.
    virtual SceneItem::Ptr insertChild(
        const SceneItem::Ptr& child, const SceneItem::Ptr& pos) = 0;

    //! Create a command to remove the argument child from its existing parent,
    //! and insert it as a child of this SceneItem, inserting before SceneItem
    //! pos.  If pos is a null pointer, child is appended.  Because of name 
    //! uniqueness, the child may be renamed.  The command is not executed.
    //! \param child Child item to be inserted.
    //! \param pos   Item before which child is to be inserted.
    //! \return Command whose execution will reparent the child.
    virtual InsertChildCommand::Ptr insertChildCmd(
        const SceneItem::Ptr& child, const SceneItem::Ptr& pos) = 0;

    //! Create a command to remove the argument child from its existing parent,
    //! and append it as a child of this SceneItem.  Because of name
    //! uniqueness, the child may be renamed.  The command is not executed.
    //! \param child Child item to be appended.
    //! \return Command whose execution will reparent the child.
    InsertChildCommand::Ptr appendChildCmd(const SceneItem::Ptr& child) {
        return insertChildCmd(child, nullptr);
    }

    /*! Create a new group object as a child of this object.
        Returned SceneItem will be the newly created parent.
        The argument component is the desired name of the created
        group. The resulting name will be different if an existing sibling
        already has that name.
        \param name Desired name of new group to create
        \return SceneItem for the new group. Returns a null pointer on failure.
    */
    virtual SceneItem::Ptr createGroup(const PathComponent& name) const = 0;

    /*! Create a command to create a new group object as a child of this object.
        The argument component is the desired name of the created
        group. The resulting name will be different if an existing sibling.
        The command is not executed.
        already has that name.
        \param name Desired name of new group to create
        \return Command whose execution will create a new group and move the selection under it.
    */
    virtual InsertChildCommand::Ptr createGroupCmd(const PathComponent& name) const = 0;

    /*! Create a command to reorder the children of this scene item.
        \param orderedList list of already reordered children.
        \return Command whose execution will reorder the child(s).
    */
    virtual UndoableCommand::Ptr reorderCmd(const Ufe::SceneItemList& orderedList) const = 0;

    /*! Reorder the children of this scene item. Default implementation calls reorder command and executes.
        \param orderedList list of already reordered children.
        \return Command whose execution will reorder the child(s).
    */
    virtual bool reorder(const Ufe::SceneItemList& orderedList) {
        auto cmd = reorderCmd(orderedList);
        if (cmd) {
            cmd->execute();
            return true;
        }
        return false;
    }

    /*! Create a command to ungroup the specified objects.
        \return Command whose execution will ungroup the specified objects.
    */
    virtual UndoableCommand::Ptr ungroupCmd() const = 0;

    /*! Ungroup the specified objects. Default implementation calls ungroup command and executes.
    */
    virtual bool ungroup() {
        auto cmd = ungroupCmd();
        if (cmd) {
            cmd->execute();
            return true;
        }
        return false;
    }
};

}

#endif /* _hierarchy */

