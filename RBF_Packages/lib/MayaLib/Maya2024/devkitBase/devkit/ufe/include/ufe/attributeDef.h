#ifndef _ufe_attributeDef
#define _ufe_attributeDef

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
#include "value.h"

#include <memory>
#include <string>

UFE_NS_DEF {

/*!
  \class AttributeDef
  \brief Base class for Attribute definitions.

  Thin wrapper around an attribute definition. Defines the standard interface to include name, type,
  default value, I/O type, and metadata.
*/
class UFE_SDK_DECL AttributeDef
{
public:
    typedef std::shared_ptr<const AttributeDef> ConstPtr;
    enum IOType { INPUT_ATTR, OUTPUT_ATTR };

    //! Destructor.
    virtual ~AttributeDef();

    //! \return The attributeDef's name.
    virtual std::string name() const = 0;

    //! \return The attributeDef's type.
    virtual std::string type() const = 0;

    //! \return The string representation of the attributeDef's value.
    virtual std::string defaultValue() const = 0;

    //! \return The attribute input/output type.
    virtual IOType ioType() const = 0;

    /*!
        Get the value of the metadata named key.
        \param[in] key The metadata key to query.
        \return The value of the metadata key. If the key does not exist an empty Value is returned.
    */
    virtual Value getMetadata(const std::string& key) const = 0;

    //! Returns true if metadata key has a non-empty value.
    virtual bool hasMetadata(const std::string& key) const = 0;

}; // end class AttributeDef

} // end namespace

#endif /* _ufe_attributeDef */
