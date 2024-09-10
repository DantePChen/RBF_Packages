#ifndef _transform3dUndoableCommands
#define _transform3dUndoableCommands
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "common/ufeExport.h"
#include "undoableCommand.h"
#include "sceneItem.h"

#include <memory>

UFE_NS_DEF {

//! \brief Base class for undoable transform operation commands.
class UFE_SDK_DECL BaseTransformUndoableCommand : public UndoableCommand {
public:
    BaseTransformUndoableCommand(const SceneItem::Ptr& item);
    virtual ~BaseTransformUndoableCommand();
    //! \return the object's SceneItem.
    SceneItem::Ptr sceneItem() const;

private:
    const SceneItem::Ptr fItem;
};

//! \brief Abstract class for undoable translate command.
class UFE_SDK_DECL TranslateUndoableCommand : public BaseTransformUndoableCommand
{
public:
    typedef std::shared_ptr<TranslateUndoableCommand> Ptr;

    //! Constructor.
    TranslateUndoableCommand(const SceneItem::Ptr& item);
    //! Default copy constructor.
    TranslateUndoableCommand(const TranslateUndoableCommand&) = default;
    //! Destructor.
    virtual ~TranslateUndoableCommand();

    /*!
         Translate the object by the given x, y, z values.
         The semantics of the translation, whether absolute or relative, and the
         coordinate system in which it is done (object local, parent, world) is up
         to the concrete derived class.
         \param x value to translate on the X-axis.
         \param y value to translate on the Y-axis.
         \param z value to translate on the Z-axis.
     */
    virtual bool translate(double x, double y, double z) = 0;

};

//! \brief Abstract class for undoable rotate command.
class UFE_SDK_DECL RotateUndoableCommand : public BaseTransformUndoableCommand
{
public:
    typedef std::shared_ptr<RotateUndoableCommand> Ptr;

    //! Constructor.
    RotateUndoableCommand(const SceneItem::Ptr& item);
    //! Default copy constructor.
    RotateUndoableCommand(const RotateUndoableCommand&) = default;
    //! Destructor.
    virtual ~RotateUndoableCommand();

    /*!
        Rotate the object by the given x, y, z values.
        The semantics of the rotation, whether absolute or relative, and the
        coordinate system in which it is done (object local, parent, world) is up
        to the rotator (Usually the DCC).
        //! \param x value to rotate on the X-axis.
        //! \param y value to rotate on the Y-axis.
        //! \param z value to rotate on the Z-axis.
    */
    virtual bool rotate(double x, double y, double z) = 0;

};

//! \brief Abstract class for undoable scale command.
class UFE_SDK_DECL ScaleUndoableCommand : public BaseTransformUndoableCommand
{
public:
    typedef std::shared_ptr<ScaleUndoableCommand> Ptr;

    //! Constructor.
    ScaleUndoableCommand(const SceneItem::Ptr& item);
    //! Default copy constructor.
    ScaleUndoableCommand(const ScaleUndoableCommand&) = default;
    //! Destructor.
    virtual ~ScaleUndoableCommand();

    /*!
    Scale the object by the given x, y, z values.
    The semantics of the scale, whether absolute or relative, and the
    coordinate system in which it is done (object local, parent, world) is up
    to the concrete derived class.
    //! \param x value to scale on the X-axis.
    //! \param y value to scale on the Y-axis.
    //! \param z value to scale on the Z-axis.
    */
    virtual bool scale(double x, double y, double z) = 0;

};



}

#endif /* _transform3dUndoableCommands */
