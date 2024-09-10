#ifndef _undoableCommand
#define _undoableCommand
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "common/ufeExport.h"

#include <memory>
#include <list>
#include <initializer_list>

UFE_NS_DEF {

//! \brief Abstract base class for undoable commands.
/*!

  This base class defines the trivial interface to implement the Command pattern

  https://en.wikipedia.org/wiki/Command_pattern

  for interactive multi-level undo.
*/

class UFE_SDK_DECL UndoableCommand
{
public:
    typedef std::shared_ptr<UndoableCommand> Ptr;

    //! Destructor.
    virtual ~UndoableCommand();

    //! Initial execution of the command, which should be called exactly once.
    //! Implementation in this class calls redo().
    virtual void execute();

    virtual void undo() = 0;
    virtual void redo() = 0;

protected:
    //! Constructor.
    UndoableCommand();
    //! Deleting copy constructor.
    UndoableCommand(const UndoableCommand&) = delete;
};

//! \brief Composite undoable command.
/*!

  This base class implements the Composite pattern

  https://en.wikipedia.org/wiki/Composite_pattern

  for undoable commands.
*/

class UFE_SDK_DECL CompositeUndoableCommand : public UndoableCommand
{
public:
    
    //! Create the composite command and append the argument commands to it.
    //! \return Pointer to the composite undoable command.
    static std::shared_ptr<CompositeUndoableCommand> create(
        std::initializer_list<Ptr> undoableCommands
    );

    //@{
    //! Constructors.
    CompositeUndoableCommand();
    CompositeUndoableCommand(std::initializer_list<Ptr> undoableCommands);
    CompositeUndoableCommand(const std::list<Ptr>& undoableCommands);
    //@}
    //! Destructor.
    ~CompositeUndoableCommand() override;

    //! Calls execute() on each command in the list, in forward order.
    void execute() override;

    //! Calls undo() on each command in the list, in reverse order.
    void undo() override;

    //! Calls redo() on each command in the list, in forward order.
    void redo() override;

    //! Append the argument to the list of commands.
    void append(const Ptr& cmd);

private:
    //! Deleting copy constructor.
    CompositeUndoableCommand(const CompositeUndoableCommand&) = delete;

    typedef std::list<Ptr> CmdList;

    CmdList fCmds;
};

}

#endif /* _undoableCommand */
