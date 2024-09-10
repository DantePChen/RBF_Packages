#ifndef _ufeNodeDef
#define _ufeNodeDef
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
#include "attributeDef.h"
#include "rtid.h"
#include "sceneItem.h"
#include "undoableCommand.h"
#include "value.h"

#include <memory>
#include <string>
#include <vector>

UFE_NS_DEF {

typedef std::vector<AttributeDef::ConstPtr> ConstAttributeDefs;

/*!
  \class NodeDef
  \brief Abstract base class for NodeDef interface.

  This base class defines an interface to retrieve node
  definition information.
*/

class UFE_SDK_DECL NodeDef
{
public:
    typedef std::shared_ptr<NodeDef> Ptr;

    //! Destructor.
    virtual ~NodeDef();

    //! \return The type of the node definition.
    virtual std::string type() const = 0;

    /*!
        Queries the number of classification levels available for this node.
        This can vary across runtimes. A biology implementation would have
        species as the "type" and genus, family, order, class, phylum, kingdom
        representing the 6 available levels.
        \return The number of classification levels.
    */
    virtual std::size_t nbClassifications() const = 0;

    /*!
        Gets the classification label applicable to this NodeNef for the
        requested classification level. The most precise classification level
        corresponds to level zero.
        \param level The classification level to query.
        \return The classification label for this node at this level.
    */
    virtual std::string classification(std::size_t level) const = 0;

    //! \return List of all the input names for this node definition.
    virtual std::vector<std::string> inputNames() const = 0;

    /*!
        Queries whether an input exists with the given name.
        \param name The input name to check.
        \return True if the object contains an input matching the name.
    */
    virtual bool hasInput(const std::string& name) const = 0;

    /*!
        Creates an AttributeDef interface for the given input name.
        \param name Name of the input to retrieve.
        \return AttributeDef interface for the given name. Returns a null
        pointer if no input exists for the given name.
    */
    virtual AttributeDef::ConstPtr input(const std::string& name) const = 0;

    //! \return The inputs of the node definition.
    virtual ConstAttributeDefs inputs() const = 0;

    //! \return List of all the output names for this node definition.
    virtual std::vector<std::string> outputNames() const = 0;

    /*!
        Queries whether an output exists with the given name.
        \param name The output name to check.
        \return True if the object contains an output matching the name.
    */
    virtual bool hasOutput(const std::string& name) const = 0;

    /*!
        Creates an AttributeDef interface for the given output name.
        \param name Name of the output to retrieve.
        \return AttributeDef interface for the given name. Returns a null
        pointer if no output exists for the given name.
    */
    virtual AttributeDef::ConstPtr output(const std::string& name) const = 0;

    //! \return The outputs of the node definition.
    virtual ConstAttributeDefs outputs() const = 0;

    //! \return The definition associated with the provided type.
    static Ptr definition(const Rtid& id, const std::string& type);

    /*!
        Get the value of the metadata named key.
        \param[in] key The metadata key to query.
        \return The value of the metadata key. If the key does not exist an empty Value is returned.
    */
    virtual Value getMetadata(const std::string& key) const = 0;

    //! Returns true if metadata key has a non-empty value.
    virtual bool hasMetadata(const std::string& key) const = 0;

    //! Create a new object in the data model using the current node definition as template, and
    //! return a SceneItem pointer for it.
    //! \param parent Item under which the node is to be created.
    //! \param name   Name of the new node.
    //! \return SceneItem for the created node, at its new path. Returns a null pointer on failure.
    virtual SceneItem::Ptr createNode(
        const SceneItem::Ptr& parent, const PathComponent& name) const = 0;

    //! Create a command to create a SceneItem using the current node definition
    //! as template. The command is not executed.
    //! \param parent Item under which the node is to be created.
    //! \param name   Name of the new node.
    //! \return Command whose execution will create the node.
    virtual InsertChildCommand::Ptr createNodeCmd(
        const SceneItem::Ptr& parent, const PathComponent& name) const = 0;

protected:
    //! Constructor.
    NodeDef();
}; // end class

typedef std::vector<NodeDef::Ptr> NodeDefs;

} // end namespace

#endif /* _ufeNodeDef */
