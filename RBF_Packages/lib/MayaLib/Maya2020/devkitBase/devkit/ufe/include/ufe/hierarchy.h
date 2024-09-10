#ifndef _hierarchy
#define _hierarchy
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "common/ufeExport.h"
#include "sceneItemList.h"
#include "sceneItem.h"

#include <memory>

UFE_NS_DEF {

//! \brief Result for child item appended to parent.
/*!

  This struct provides simple aggregation for the result of
  Hierarchy::appendChild.
*/
struct UFE_SDK_DECL AppendedChild {

    //! Construct.  The scene item argument is the reparented child.  The path
    //! argument stores the child's previous parent and its previous name, as
    //! it may have been renamed under its new parent.  The integer argument is
    //! the child's position in its former parent's children list (0 is first).
    AppendedChild(const SceneItem::Ptr&, const Path&, const int);

    //! Root of the appended hierarchy.
    SceneItem::Ptr child;
    //! Child's previous path.
    Path           previousPath;
    //! Child's index position in its previous parent's children list.
    int            previousIndex;
};

//! \brief Abstract base class for scene hierarchy interface.
/*!

  This base class defines the interface that run-times can implement to
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

    /*! Convenience method that calls the hierarchy method on the hierarchy
        handler for the item.  Returns a null pointer if the argument is null,
        or has an empty path.
        \param item SceneItem to build Hierarchy on
        \return Hierarchy of given SceneItem.
    */
    static Ptr hierarchy(const SceneItem::Ptr& item);

    /*! Convenience method that calls the createItem method on the hierarchy
        handler for the item.  Returns a null pointer if the argument is an
        empty path.
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

    //! \return The SceneItem's parent.  Returns a null pointer if the item is the world root, and has no parent.
    virtual SceneItem::Ptr parent() const = 0;

    //! Remove the argument child from its existing parent, and append it
    //! as a child of this SceneItem.  Because of name uniqueness, the
    //! child may be renamed.
    //! \return AppendedChild object containing a SceneItem for the appended
    //! child, the child's previous path, and its previous index position.
    virtual AppendedChild appendChild(const SceneItem::Ptr& child) = 0;
};

}

#endif /* _hierarchy */
