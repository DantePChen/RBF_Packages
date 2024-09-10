#ifndef _nodeDefHandler
#define _nodeDefHandler
// ===========================================================================
// Copyright 2022 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "common/ufeExport.h"
#include "rtid.h"
#include "sceneItem.h"
#include "nodeDef.h"

#include <memory>

UFE_NS_DEF {

//! \brief Base class for NodeDefHandler interface.
/*!

  This base class defines an interface for retrieving
  node definition interfaces.

  Implementers of this class will need to provide a means of
  retrieving classes derived from NodeDef. A specific
  instance of this derived NodeDef will need to be
  retrievable through the definition function which can be
  called either by SceneItem::Ptr or by type. Additionally,
  all available derived NodeDef classes will need to be
  obtainable through the definitions function.
*/

class UFE_SDK_DECL NodeDefHandler
{
public:
    typedef std::shared_ptr<NodeDefHandler> Ptr;

    static constexpr char kNodeDefCategoryAll[] = "All";

    //! Constructor.
    NodeDefHandler();
    //! Default copy constructor.
    NodeDefHandler(const NodeDefHandler&) = default;
    //! Destructor.
    virtual ~NodeDefHandler();

    //! Returns the node definition interface for a provided SceneItem. Returns a
    // nullptr if no such node definition can be found.
    virtual Ufe::NodeDef::Ptr definition(const Ufe::SceneItem::Ptr& item) const = 0;
    //! Returns the node definition interface for a provided type. Returns a nullptr
    // if no such node definition can be found.
    virtual Ufe::NodeDef::Ptr definition(const std::string& type) const = 0;
    //! Returns the available node definitions for the provided category.
    virtual Ufe::NodeDefs definitions(const std::string& category=kNodeDefCategoryAll) const = 0;
};

}

#endif /* _nodeDefHandler */
