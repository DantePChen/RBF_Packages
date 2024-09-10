#ifndef _ufe_connection
#define _ufe_connection

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

#include "attributeInfo.h"

#include <memory>

UFE_NS_DEF {

/*!
  \class Connection
  \brief This class represents a connection.

  This class represents a connection with only one source and destination attribute.
  If an attribute is connected to several other attributes then there are several
  connections.
*/
class UFE_SDK_DECL Connection
{
public:
    typedef std::shared_ptr<Connection> Ptr;

    //! Constructor.
    Connection(const AttributeInfo& srcAttr, const AttributeInfo& dstAttr);
    //! Destructor.
    virtual ~Connection();

    //@{
    //! No copy or move constructor/assignment.
    Connection(const Connection&) = delete;
    Connection& operator=(const Connection&) = delete;
    Connection(Connection&&) = delete;
    Connection& operator=(Connection&&) = delete;
    //@}

    //! Get the source attribute.
    const AttributeInfo& src() const { return fSrc; }
    //! Get the destination attribute.
    const AttributeInfo& dst() const { return fDst; }

private:
    const AttributeInfo fSrc, fDst;
};


}

#endif /* _ufe_connection */
