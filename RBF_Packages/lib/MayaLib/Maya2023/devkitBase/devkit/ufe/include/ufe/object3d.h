#ifndef _object3d
#define _object3d
// ===========================================================================
// Copyright 2019 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "common/ufeExport.h"

#include "sceneItem.h"
#include "observer.h"
#include "undoableCommand.h"
#include "notification.h"

#include <memory>

UFE_NS_DEF {

struct BBox3d;

//! \brief Abstract base class for 3D object interface.
/*!

  This base class defines the interface that runtimes can implement to
  provide basic 3D object behavior.  3D objects have visibility, and have 3D
  space bounds.

  To avoid the memory-consuming "one proxy object per scene object" approach,
  Object3d interface objects should be obtained and used within a local
  scope, and not stored.  Object3d interfaces should be considered
  stateless, and can be bound to new selection items.
*/

class UFE_SDK_DECL Object3d
{
public:
    typedef std::shared_ptr<Object3d> Ptr;
    
    /*!
        Convenience method that calls the object3d method on the 
        Object3d handler for the item.  Returns a null pointer if the
        argument is null, or has an empty path.
        \param item SceneItem for which Object3d interface is desired.
        \return Object3d interface of the given SceneItem.
    */
    static Ptr object3d(const SceneItem::Ptr& item);

    /*!
        Add observation for Object3d changes on any item in the scene (global
        observation).
        \param obs Observer to add.
        \return True if the observer is added. Add does nothing and returns 
         false if the observer is already present.
    */
    static bool addObserver(const Observer::Ptr& obs);

    /*!
        Remove observation for Object3d changes on any item in the scene
        (global observation).
        \param obs Observer to remove.
        \return True if the observer is removed. False if the observer isn't
        found.
    */
    static bool removeObserver(const Observer::Ptr& obs);

    /*!
        Number of observers for Object3d changes on any item in the scene
        (global observers).
        \return Number of observers.
    */
    static std::size_t nbObservers();

    /*!
        Query observation for Object3d changes on any item in the scene
        (global observers).
        \param obs Observer to query.
        \return True if the observer observes Object3d changes on any item
        in the scene.
    */
    static bool hasObserver(const Observer::Ptr& obs);

    //! Notify all global observers.  The order in which observers are notified
    //! is unspecified.
    //! If no observer exists, does nothing.
    //! \param notif The Object3d notification to send.
    static void notify(const Notification& notif);

    //! Constructor.
    Object3d();
    //! Default copy constructor.
    Object3d(const Object3d&) = default;
    //! Destructor.
    virtual ~Object3d();

    //! Scene item accessor.
    virtual SceneItem::Ptr sceneItem() const = 0;

    //! Return the local space, axis-aligned 3D extents of the object.
    //! \return Object bounding box.
    virtual BBox3d boundingBox() const = 0;

    //! Return the visibility state of the object.
    //! \return true if the object is visible
    virtual bool visibility() const = 0;

    //! Set the visibility state of the object.
    virtual void setVisibility(bool vis) = 0;

    //! Create an undoable visible command.
    virtual Ufe::UndoableCommand::Ptr setVisibleCmd(bool vis) = 0;
};

}

#endif /* _object3d */
