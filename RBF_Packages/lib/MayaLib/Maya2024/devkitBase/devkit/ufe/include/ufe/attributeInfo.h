#ifndef _ufe_attributeInfo
#define _ufe_attributeInfo

// =======================================================================
// Copyright 2022 Autodesk, Inc. All rights reserved.
//
// This computer source code and related instructions and comments are the
// unpublished confidential  and proprietary information of Autodesk, Inc.
// and are protected under applicable copyright and trade secret law. They 
// may not be disclosed to, copied  or used by any third party without the 
// prior written consent of Autodesk, Inc.
// =======================================================================

#include "common/ufeExport.h"

#include "attribute.h"

#include <memory>
#include <string>

UFE_NS_DEF {

/*!
  \class AttributeInfo
  \brief This class keeps information of an attribute.

  This class keeps the information of an attribute when long-term usage is mandatory such
  as an undo/redo commands for example. The information remains valid even if the node
  owning this attribute is deleted. The Attribute class keeps a handle associated to a
  specific node instance so, deleting the node instance invalidates the Attribute instance.

*/
class UFE_SDK_DECL AttributeInfo
{
public:
    AttributeInfo(const Attribute::Ptr& attr);
    AttributeInfo(const Path& path, const std::string& name);
    AttributeInfo(const AttributeInfo& attr) = default;
    ~AttributeInfo();

    inline Rtid runTimeId() const { return fNodePath.runTimeId(); }
    inline Path path() const { return fNodePath; }
    inline std::string name() const { return fAttributeName; }

    /*!
     * \brief Create the Attribute from the AttributeInfo.
     * \note This method could have a performance hit.
     */
    Attribute::Ptr attribute() const;

private:
    const Path fNodePath;
    const std::string fAttributeName;
};

}

#endif /* _ufe_attributeInfo */
