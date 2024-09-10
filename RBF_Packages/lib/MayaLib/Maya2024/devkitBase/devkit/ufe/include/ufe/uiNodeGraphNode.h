#ifndef _ufe_uiNodeGraphNode
#define _ufe_uiNodeGraphNode

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
#include "sceneItem.h"
#include "types.h"
#include "undoableCommand.h"

UFE_NS_DEF {

/*!
  \class UINodeGraphNode
  \brief Abstract base class for UINodeGraphNode interface.

  This base class defines an interface for factory objects that runtimes
  implement to handle UINodeGraphNodes.

*/
class UFE_SDK_DECL UINodeGraphNode
{
public:
    typedef std::shared_ptr<UINodeGraphNode> Ptr;

    /*! Convenience method that calls the uiNodeGraphNode method on the UINodeGraphNode
        handler for the item.  Returns a null pointer if the argument is null,
        or has an empty path.
        \param item SceneItem to build UINodeGraphNode on
        \return UINodeGraphNode of given SceneItem.
    */
    static Ptr uiNodeGraphNode(const SceneItem::Ptr& item);

    //! Constructor.
    UINodeGraphNode() = default;

    // Destructor.
    virtual ~UINodeGraphNode() = default;

    //@{
    //! No copy or move constructor/assignment.
    UINodeGraphNode(const UINodeGraphNode&) = delete;
    UINodeGraphNode& operator=(const UINodeGraphNode&) = delete;
    UINodeGraphNode(UINodeGraphNode&&) = delete;
    UINodeGraphNode& operator=(UINodeGraphNode&&) = delete;
    //@}

    //! \return The UINodeGraphNode's SceneItem.
    virtual SceneItem::Ptr sceneItem() const = 0;

    //! \return Whether the UINodeGraphNode is storing position information
    virtual bool hasPosition() const = 0;

    //! \return The UINodeGraphNode's position.
    virtual Ufe::Vector2f getPosition() const = 0;

    //! Set the UINodeGraphNode's position.
    virtual void setPosition(const Ufe::Vector2f& pos) {
        auto cmd = setPositionCmd(pos);
        if (cmd) {
            cmd->execute();
        }
    }

    inline void setPosition(float x, float y) {
        setPosition(Vector2f(x, y));
    }

    virtual UndoableCommand::Ptr setPositionCmd(const Ufe::Vector2f& pos) = 0;
};

class UFE_SDK_DECL UINodeGraphNode_v4_1 : public UINodeGraphNode
{
public:
    typedef std::shared_ptr<UINodeGraphNode_v4_1> Ptr;

    /*! Convenience method that calls the uiNodeGraphNode method on the UINodeGraphNode
        handler for the item.  Returns a null pointer if the argument is null,
        or has an empty path.
        \param item SceneItem to build UINodeGraphNode on
        \return UINodeGraphNode_v4_1 of given SceneItem.
    */
    static Ptr uiNodeGraphNode(const SceneItem::Ptr& item);

    //! Constructor.
    UINodeGraphNode_v4_1() = default;

    // Destructor.
    ~UINodeGraphNode_v4_1() override = default;

    //@{
    //! No copy or move constructor/assignment.
    UINodeGraphNode_v4_1(const UINodeGraphNode_v4_1&) = delete;
    UINodeGraphNode_v4_1& operator=(const UINodeGraphNode_v4_1&) = delete;
    UINodeGraphNode_v4_1(UINodeGraphNode_v4_1&&) = delete;
    UINodeGraphNode_v4_1& operator=(UINodeGraphNode_v4_1&&) = delete;
    //@}

    //! \return Whether the UINodeGraphNode is storing size information
    virtual bool hasSize() const = 0;

    //! \return The UINodeGraphNode's size.
    virtual Ufe::Vector2f getSize() const = 0;

    //! Set the UINodeGraphNode's size.
    virtual void setSize(const Ufe::Vector2f& size) {
        auto cmd = setSizeCmd(size);
        if (cmd) {
            cmd->execute();
        }
    }

    inline void setSize(float x, float y) {
        setSize(Vector2f(x, y));
    }

    virtual UndoableCommand::Ptr setSizeCmd(const Ufe::Vector2f& size) = 0;
};

}

#endif /* _ufe_uiNodeGraphNode */
