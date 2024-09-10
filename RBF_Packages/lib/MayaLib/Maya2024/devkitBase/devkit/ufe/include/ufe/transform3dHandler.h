#ifndef _transform3dHandler
#define _transform3dHandler
// ===========================================================================
// Copyright 2022 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "transform3d.h"
#include "sceneItem.h"

UFE_NS_DEF {

class Path;

//! \brief Factory base class for Transform3d interface.
/*!

  This base class defines an interface for factory objects that runtimes
  can implement to create a Transform3d interface object.
*/

class UFE_SDK_DECL Transform3dHandler
{
public:
    typedef std::shared_ptr<Transform3dHandler> Ptr;
    //! Constructor.
    Transform3dHandler();
    //! Default copy constructor.
    Transform3dHandler(const Transform3dHandler&) = default;
    //! Destructor.
    virtual ~Transform3dHandler();

    /*!
        Creates a Transform3dRead interface on the given SceneItem.  By
        default, calls transform3d().
        \param item SceneItem to use to retrieve its Transform3dRead interface.
        \return Transform3dRead interface of given SceneItem. Returns a null
        pointer if no Transform3dRead interface can be created for the item.
    */
    virtual Transform3dRead::Ptr transform3dRead(
        const SceneItem::Ptr& item) const;

    /*!
        Creates a Transform3d interface on the given SceneItem.
        \param item SceneItem to use to retrieve its Transform3d interface.
        \return Transform3d interface of given SceneItem. Returns a null
        pointer if no Transform3d interface can be created for the item.
    */
    virtual Transform3d::Ptr transform3d(
        const SceneItem::Ptr& item) const = 0;

    /*! Return an interface to be used to edit the 3D transformation of the
        object.  By default, returns the normal Transform3d interface.  The
        edit transform object may have a different local transformation and a
        different \ref Ufe::Transform3d::segmentInclusiveMatrix() and
        \ref Ufe::Transform3d::segmentExclusiveMatrix() than the normal
        Transform3d interface associated with a scene item.  All changes made
        through the edit transform 3D object will be visible through the normal
        \ref Ufe::Transform3d::transform3d() interface.
        \param item SceneItem to use to retrieve its Transform3d interface.
        \param hint Contextual information for Transform3d interface creation.
        \return Transform3d interface of given SceneItem. Returns a null
        pointer if no Transform3d interface can be created for the item.
    */
    virtual Transform3d::Ptr editTransform3d(
        const SceneItem::Ptr&      item,
        const EditTransform3dHint& hint = EditTransform3dHint()
    ) const {
        (void) hint;
        return transform3d(item);
    }
};

}

#endif /* _transform3dHandler */
