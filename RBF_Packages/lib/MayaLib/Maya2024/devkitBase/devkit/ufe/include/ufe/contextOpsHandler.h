#ifndef _contextOpsHandler
#define _contextOpsHandler
// ===========================================================================
// Copyright 2020 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "contextOps.h"
#include "sceneItem.h"

UFE_NS_DEF {

//! \brief Factory base class for ContextOps interface.
/*!

  This base class defines an interface for factory objects that runtimes
  can implement to create a ContextOps interface object.
*/

class UFE_SDK_DECL ContextOpsHandler
{
public:
    typedef std::shared_ptr<ContextOpsHandler> Ptr;
    //! Constructor.
    ContextOpsHandler();
    //! Default copy constructor.
    ContextOpsHandler(const ContextOpsHandler&) = default;
    //! Destructor.
    virtual ~ContextOpsHandler();

    /*!
        Creates a ContextOps interface on the given SceneItem.
        \param item SceneItem to use to retrieve its ContextOps interface.
        \return ContextOps interface of given SceneItem. Returns a null
        pointer if no ContextOps interface can be created for the item.
    */
    virtual ContextOps::Ptr contextOps(
        const SceneItem::Ptr& item) const = 0;
};

}

#endif /* _contextOpsHandler */
