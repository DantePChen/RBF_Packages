#ifndef _ufe_attributesHandler
#define _ufe_attributesHandler

// =======================================================================
// Copyright 2019 Autodesk, Inc. All rights reserved.
//
// This computer source code and related instructions and comments are the
// unpublished confidential  and proprietary information of Autodesk, Inc.
// and are protected under applicable copyright and trade secret law. They 
// may not be disclosed to, copied  or used by any third party without the 
// prior written consent of Autodesk, Inc.
// =======================================================================

#include "common/ufeExport.h"
#include "attributes.h"
#include "sceneItem.h"

#include <memory>

UFE_NS_DEF {

/*!
  \class AttributesHandler
  \brief Factory base class for Attributes interface.

  This base class defines an interface for factory objects that runtimes
  can implement to create the attributes interface object.
*/

class UFE_SDK_DECL AttributesHandler
{
public:
    typedef std::shared_ptr<AttributesHandler> Ptr;

    //! Constructor.
    AttributesHandler();
    //! Default copy constructor.
    AttributesHandler(const AttributesHandler&) = default;
    //! Destructor.
    virtual ~AttributesHandler();

    /*!
        Creates an Attributes interface on the given SceneItem.
        \param item SceneItem to use to retrieve its Attributes interface.
        \return Attributes interface of given SceneItem. Returns a null
        pointer if no Attribute interface can be created for the item.
    */
    virtual Attributes::Ptr attributes(const SceneItem::Ptr& item) const = 0;
};

}

#endif /* _ufe_attributesHandler */
