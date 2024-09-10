#ifndef _uiNodeGraphNodeHandler
#define _uiNodeGraphNodeHandler
// ===========================================================================
// Copyright 2022 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "uiNodeGraphNode.h"

UFE_NS_DEF {

//! \brief Factory base class for UINodeGraphNode interface.
/*!

  This base class defines an interface for factory objects that runtimes
  can implement to create a UINodeGraphNode interface object.
*/

class UFE_SDK_DECL UINodeGraphNodeHandler
{
public:
    typedef std::shared_ptr<UINodeGraphNodeHandler> Ptr;

    //! Constructor
    UINodeGraphNodeHandler() = default;

    //! Default copy constructor
    UINodeGraphNodeHandler(const UINodeGraphNodeHandler&) = default;

    //! Destructor
    virtual ~UINodeGraphNodeHandler() = default;

    /*! Create a UINodeGraphNode_v4_1 interface for the argument scene item.
        \param item SceneItem to use for the UINodeGraphNode
        \return UINodeGraphNode interface of given SceneItem. Returns a null pointer if no UINodeGraphNode
        interface can be created for the argument SceneItem
    */
    virtual UINodeGraphNode::Ptr uiNodeGraphNode(const SceneItem::Ptr& item) const = 0;
};

}

#endif /* _uiNodeGraphNodeHandler */
