#ifndef _ufe_connections
#define _ufe_connections

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

#include "connection.h"

#include <memory>

UFE_NS_DEF {

/*!
  \class Connections
  \brief The class represents a list of existing connections.
*/
class UFE_SDK_DECL Connections
{
public:
    typedef std::shared_ptr<Connections> Ptr;

    enum AttributeType
    {
        ATTR_IS_SOURCE      = 0x01,
        ATTR_IS_DESTINATION = 0x02,
        ATTR_ANY            = 0x03
    };

    //! Constructor.
    Connections();
    //! Destructor.
    virtual ~Connections();

    //@{
    //! No copy or move constructor/assignment.
    Connections(const Connections&) = delete;
    Connections& operator=(const Connections&) = delete;
    Connections(Connections&&) = delete;
    Connections& operator=(Connections&&) = delete;
    //@}

    //! \return Return all the connections where an attribute is part of the connection.
    virtual std::vector<Connection::Ptr> allConnections() const = 0;

    /*!
        Queries whether at least one connection exists with the given attribute.
        \note It provides a default implementation which can be overridden by the
              underlying data model if needed.
        \complexity Amortized O(m) for an attribute in a list of m attributes.
        \param attr The attribute to check.
        \param attrType AttributeType defines if the attribute is the source or the destination of the connection.
        \return True if the scene item contains a connection matching the given attribute.
    */
    virtual bool hasConnection(const Attribute::Ptr& attr, AttributeType attrType) const;

    /*!
        Returns all the connections for a specific attribute.
        \note It provides a default implementation which can be overridden by the
              underlying data model if needed.
        \complexity Amortized O(m) for an attribute in a list of m attributes.
        \param attr Attribute to look for connections.
        \param attrType AttributeType defines if the attribute is the source or the destination of the connection.
        \return vector of source connections for a given attribute.
    */
    virtual std::vector<Connection::Ptr> connections(const Attribute::Ptr& attr, AttributeType attrType) const;
};

}

#endif
