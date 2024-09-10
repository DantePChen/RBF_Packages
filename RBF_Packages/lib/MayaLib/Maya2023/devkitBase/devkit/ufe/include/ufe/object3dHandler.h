#ifndef _object3dHandler
#define _object3dHandler
// ===========================================================================
// Copyright 2019 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "object3d.h"
#include "sceneItem.h"

UFE_NS_DEF {

//! \brief Factory base class for Object3d interface.
/*!

  This base class defines an interface for factory objects that runtimes
  can implement to create an Object3d interface object.
*/

class UFE_SDK_DECL Object3dHandler
{
public:
    typedef std::shared_ptr<Object3dHandler> Ptr;
    //! Constructor.
    Object3dHandler();
    //! Default copy constructor.
    Object3dHandler(const Object3dHandler&) = default;
    //! Destructor.
    virtual ~Object3dHandler();

    /*!
        Creates an Object3d interface on the given SceneItem.
        \param item SceneItem to use to retrieve its Object3d interface.
        \return Object3d interface of given SceneItem. Returns a null
        pointer if no Object3d interface can be created for the item.
    */
    virtual Object3d::Ptr object3d(
        const SceneItem::Ptr& item) const = 0;
};

}

#endif /* _object3dHandler */
