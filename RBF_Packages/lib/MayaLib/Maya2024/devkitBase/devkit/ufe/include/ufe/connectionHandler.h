#ifndef _ufe_connectionHandler
#define _ufe_connectionHandler

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

#include "connections.h"
#include "sceneItem.h"

#include <memory>

UFE_NS_DEF {

class UndoableCommand;
class ConnectionResultUndoableCommand;

/*!
  \class ConnectionHandler
  \brief Factory base class for connection interface.

  This base class defines an interface that runtimes must implement to manage
  source connections i.e. list, create and delete.

  \note A source connection is a connection where the source attribute is an
  ouput type attribute and the destination attribute is an input type from the
  SceneItem.
*/

class UFE_SDK_DECL ConnectionHandler
{
public:
    typedef std::shared_ptr<ConnectionHandler> Ptr;

    //! Constructor.
    ConnectionHandler();
    //! Default copy constructor.
    ConnectionHandler(const ConnectionHandler &) = default;
    //! Destructor.
    virtual ~ConnectionHandler();

    /*!
        \brief Get the source Connections on the given SceneItem.

        \param item SceneItem to use to retrieve its source Connections.
        \return all the Connections of given SceneItem and a null pointer
        if no Connections interface can be created for the item.
    */
    virtual Connections::Ptr sourceConnections(const SceneItem::Ptr &item) const = 0;

    //@{
    /*!
        Connects a source attribute to a destination attribute.

        \returns The resulting connection. Note that the original source and target can be remapped
        by the client so this will be the actual attributes that ended up being connected.

        \note It throws an exception if the connection cannot be created or if it already exists.

        \note To avoid the performance hit of the conversion from AttributeInfo
        to Attribute use the method with Attribute arguments when available.
    */
    //! \param srcInfo source attribute (from AttributeInfo) to be connected.
    //! \param dstInfo destination attribute (from AttributeInfo) to be connected.
    Connection::Ptr connect(const AttributeInfo &srcInfo, const AttributeInfo &dstInfo) const;
    //! \param srcAttr source attribute to be connected.
    //! \param dstAttr destination attribute to be connected.
    Connection::Ptr connect(const Attribute::Ptr &srcAttr, const Attribute::Ptr &dstAttr) const;
    //@}

    //@{
    /*!
        Disconnects a source attribute from a destination attribute.

        \note It throws an exception if the connection cannot be removed or if it doesn't exist.

        \note To avoid the performance hit of the conversion from AttributeInfo
        to Attribute use the method with Attribute arguments when available.
    */
    //! \param srcInfo source attribute (from AttributeInfo) to be disconnected.
    //! \param dstInfo destination attribute (from AttributeInfo) to be disconnected.
    void disconnect(const AttributeInfo &srcInfo, const AttributeInfo &dstInfo) const;
    //! \param srcAttr source attribute to be disconnected.
    //! \param dstAttr destination attribute to be disconnected.
    void disconnect(const Attribute::Ptr &srcAttr, const Attribute::Ptr &dstAttr) const;
    //@}

    /*!
      Get the command to connect two attributes. The returned command is not executed.
      \param srcAttr source attribute to be connected.
      \param dstAttr destination attribute to be connected.
      \returns a command that, when executed, will connect the attributes and return the
      resulting connection. Note that the original source and target can be remapped by the client
      and the connection returned by the command will be the actual attributes that ended up being
      connected.
    */
    virtual std::shared_ptr<ConnectionResultUndoableCommand> createConnectionCmd(
        const Attribute::Ptr &srcAttr, const Attribute::Ptr &dstAttr) const = 0;

    /*!
      Get the command to disconnect two attributes. The returned command is not executed.
      \param srcAttr source attribute to be disconnected.
      \param dstAttr destination attribute to be disconnected.
      \returns a command that, when executed, will disconnect the attributes.
    */
    virtual std::shared_ptr<UndoableCommand> deleteConnectionCmd(
        const Attribute::Ptr &srcAttr, const Attribute::Ptr &dstAttr) const = 0;
};
}

#endif
