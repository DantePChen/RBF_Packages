#ifndef _sceneItemOpsHandler
#define _sceneItemOpsHandler
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "sceneItemOps.h"
#include "sceneItem.h"

UFE_NS_DEF {

class Path;

//! \brief Factory base class for SceneItemOps interface.
/*!

  This base class defines an interface for factory objects that run-times
  can implement to create a SceneItemOps interface object.
*/

class UFE_SDK_DECL SceneItemOpsHandler
{
public:
    typedef std::shared_ptr<SceneItemOpsHandler> Ptr;
    //! Constructor.
    SceneItemOpsHandler();
    //! Default copy constructor.
    SceneItemOpsHandler(const SceneItemOpsHandler&) = default;
    //! Destructor.
    virtual ~SceneItemOpsHandler();

    /*!
        Retrieves a SceneItemOps interface on the given SceneItem.
        \param item SceneItem to use to retrieve its SceneItemOps interface.
        \return SceneItemOps interface of given SceneItem. Returns a null
        pointer if no SceneItemOps interface can be created for the item.
    */
    virtual SceneItemOps::Ptr sceneItemOps(
        const SceneItem::Ptr& item) const = 0;
};

}

#endif /* _sceneItemOpsHandler */
