#ifndef _baseUndoableCommands
#define _baseUndoableCommands
// ===========================================================================
// Copyright 2020 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "common/ufeExport.h"
#include "undoableCommand.h"
#include "hierarchy.h"
#include "path.h"

#include <memory>

UFE_NS_DEF
{

    struct Matrix4d;

    //! \brief Base class for undoable set operation commands.
    //
    // Set operation commands do not hold onto a SceneItem, because that item
    // might be stale when undo or redo is executed.  This is for two reasons:
    // - redo and undo of commands later in the undo stack may make the item stale
    // - operations between the point of creation of the set command and its
    //   execution may make the item stale.
    //
    class UFE_SDK_DECL BaseUndoableCommand : public UndoableCommand
    {
    public:
        BaseUndoableCommand(const Path &path);
        virtual ~BaseUndoableCommand();

        //! Set the path of the object onto which the command is applied.
        inline void setPath(const Path &path) { fPath = path; }

        //! \return the path of the object onto which the command is applied.
        inline const Path &path() const { return fPath; }

        //! \return the object's SceneItem
        Ufe::SceneItem::Ptr sceneItem() const { return Ufe::Hierarchy::createItem(path()); }

    private:
        Path fPath;
    };

    //! \brief Abstract class for set value command.
    template <typename VALUE_TYPE>
    class UFE_SDK_DECL SetValueUndoableCommand : public BaseUndoableCommand
    {
    public:
        typedef std::shared_ptr<SetValueUndoableCommand> Ptr;
        typedef VALUE_TYPE ValueType;

        //! Constructor.
        SetValueUndoableCommand(const Path &path) : BaseUndoableCommand(path) {}
        //! Destructor.
        virtual ~SetValueUndoableCommand() {}

        /*!
            Set the VALUE_TYPE value to the argument VALUE_TYPE.
            //! \param v new VALUE_TYPE value.
        */
        virtual bool set(VALUE_TYPE v) = 0;
    };

    //! \brief Abstract class for set Value3 command.
    template <typename VALUE_TYPE>
    class UFE_SDK_DECL SetValue3UndoableCommand : public BaseUndoableCommand
    {
    public:
        typedef std::shared_ptr<SetValue3UndoableCommand> Ptr;
        typedef VALUE_TYPE ValueType;

        //! Constructor.
        SetValue3UndoableCommand(const Path &path) : BaseUndoableCommand(path) {}
        //! Destructor.
        virtual ~SetValue3UndoableCommand() {}

        /*!
            Set the Value3
            //! \param x x component of vector.
            //! \param y y component of vector.
            //! \param z z component of vector.
        */
        virtual bool set(VALUE_TYPE x, VALUE_TYPE y, VALUE_TYPE z) = 0;
    };

    using SetFloatUndoableCommand = SetValueUndoableCommand<float>;
    using SetDoubleUndoableCommand = SetValueUndoableCommand<double>;
    using SetMatrix4dUndoableCommand = SetValueUndoableCommand<const Matrix4d &>;
    using SetBoolUndoableCommand = SetValueUndoableCommand<bool>;
    using SetIntUndoableCommand = SetValueUndoableCommand<int>;
    using SetVector3fUndoableCommand = SetValue3UndoableCommand<float>;
    using SetVector3dUndoableCommand = SetValue3UndoableCommand<double>;
}

#endif /* _baseUndoableCommands */
