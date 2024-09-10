#ifndef _lightHandler
#define _lightHandler
// ===========================================================================
// Copyright 2022 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "light.h"
#include "sceneItem.h"

UFE_NS_DEF {

class Path;

//! \brief Factory base class for Light interface.
/*!

  This base class defines an interface for factory objects that runtimes
  can implement to create a Light interface object.
*/

class UFE_SDK_DECL LightHandler
{
public:
    typedef std::shared_ptr<LightHandler> Ptr;
    //! Constructor.
    LightHandler();
    //! Default copy constructor.
    LightHandler(const LightHandler&) = default;
    //! Destructor.
    virtual ~LightHandler();

    /*!
        Creates a Light interface on the given SceneItem.
        \param item SceneItem to use to retrieve its Light interface.
        \return Light interface of given SceneItem. Returns a null
        pointer if no Light interface can be created for the item.
    */
    virtual Light::Ptr light(
        const SceneItem::Ptr& item) const = 0;

};

}

#endif /* _lightHandler */
