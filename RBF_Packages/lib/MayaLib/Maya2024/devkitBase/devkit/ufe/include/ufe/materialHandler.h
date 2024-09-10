#ifndef _materialHandler
#define _materialHandler
// ===========================================================================
// Copyright 2022 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "material.h"
#include "sceneItem.h"

UFE_NS_DEF {

//! \brief Factory base class for Material interface.
/*!

  This base class defines an interface for factory objects that runtimes
  can implement to create a Material interface object.
*/

class UFE_SDK_DECL MaterialHandler
{
public:
    typedef std::shared_ptr<MaterialHandler> Ptr;
    
    //! Constructor.
    MaterialHandler();
    //! Default copy constructor.
    MaterialHandler(const MaterialHandler&) = default;
    //! Destructor.
    virtual ~MaterialHandler();

    /*!
        Creates a Material interface on the given SceneItem.
        \param item SceneItem to use to retrieve its Material interface.
        \return Material interface of given SceneItem. Returns a null
        pointer if no Material interface can be created for the item.
    */
    virtual Material::Ptr material(const SceneItem::Ptr& item) const = 0;
};

}

#endif /* _materialHandler */
