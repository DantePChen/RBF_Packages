#ifndef _hierarchyHandler
#define _hierarchyHandler
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "hierarchy.h"
#include "sceneItem.h"

UFE_NS_DEF {

class Path;

//! \brief Factory base class for scene hierarchy interface.
/*!

  This base class defines an interface for factory objects that run-times
  can implement to create a scene hierarchy interface object.
*/

class UFE_SDK_DECL HierarchyHandler
{
public:
    typedef std::shared_ptr<HierarchyHandler> Ptr;

    //! Constructor
    HierarchyHandler();

    //! Default copy constructor
    HierarchyHandler(const HierarchyHandler&) = default;

    //! Destructor
    virtual ~HierarchyHandler();

    /*! 
        \param item SceneItem to use for the hierarchy
        \return Hierarchy interface of given SceneItem. Returns a null pointer if no Hierarchy interface can be created 
        for the argument SceneItem
    */
    virtual Hierarchy::Ptr hierarchy(const SceneItem::Ptr& item) const = 0;

    /*!
        \param path Path of item to create
        \return SceneItem for the given Path. Returns a null pointer if no SceneItem can be created for the Path
    */
    virtual SceneItem::Ptr createItem(const Path& path) const = 0;
};

}

#endif /* _hierarchyHandler */
