#ifndef _transform3dHandler
#define _transform3dHandler
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
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

  This base class defines an interface for factory objects that run-times
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
        Retrieves a Transform3d interface on the given SceneItem.
        \param item SceneItem to use to retrieve its Transform3d interface.
        \return Transform3d interface of given SceneItem. Returns a null
        pointer if no Transform3d interface can be created for the item.
    */
    virtual Transform3d::Ptr transform3d(
        const SceneItem::Ptr& item) const = 0;
};

}

#endif /* _transform3dHandler */
