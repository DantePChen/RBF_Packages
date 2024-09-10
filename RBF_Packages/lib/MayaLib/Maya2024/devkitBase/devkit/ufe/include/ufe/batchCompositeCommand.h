#ifndef _ufeBatchCompositeCommand
#define _ufeBatchCompositeCommand
// ===========================================================================
// Copyright 2023 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "undoableCommand.h"

#include "codeWrapper.h"
#include "selection.h"

#include <string>

UFE_NS_DEF {

class Selection;

//! \brief Composite command affecting a batch (selection) of items.
//!        For each item run-time, allow the run-time to wrap the execution
//!        of the composite commmand with arbitrary prelude and cleanup code.
//!
//! \details Intended usage of the class in this file and in codeWrapperContext:
//!
//! BatchCompositeCommand
//!     Holds wrappers to do prelude and cleanup around the execution, undoing
//!     or redoing of a composite command.
//!
//! CodeWrapperContext (from codeWrapperContext.h)
//!     Supports the prelude and cleanup when executing sub-commands before
//!     adding them to a composite command.
//!
//! The necessity of two classes is to support the two coding patterns when
//! creating a composite command:
//!
//!     - Creating all sub-commands and adding them to the composite without
//!       executing them, and then executing the whole composite command.
//!
//!     - Creating all sub-commands and executing them immediately *before*
//!       adding them to the composite command and *not* executing the
//!       composite command.
//!
//! So, if the sub-commands are all created but not executed and are added
//! to the composite command, then the composite command will automatically
//! do the prelude and cleanup and thus, with this coding pattern, the context
//! class is *not* needed. In pseudo-code:
//!
//!    auto composite = new BatchCompositeCommand(selection, "op-name")
//!    for each sub-command:
//!        auto subCmd = new SomeOtherUfeCommand();
//!        composite->append(subCmd);
//!    composite->execute()
//!
//! In contrast, if the individual sub-commands are executed before adding
//! them to the composite command, then the context class must be used in
//! order for these individually-executed commands to be properly surrounded
//! by the intended prelude and cleanup code. In pseudo-code, the pattern
//! this supports is:
//!
//!    auto composite = new BatchCompositeCommand(selection, "op-name")
//!    CodeWrapperContext ctx(composite->codeWrappers(), BatchCompositeCommand::executeSubOperation);
//!    for each sub-command:
//!        auto subCmd = new SomeOtherUfeCommand();
//!        subCmd->execute()
//!        composite->append(subCmd);
//!
//! \link ufeCodeWrapper see the full documentation \endlink
class UFE_SDK_DECL BatchCompositeCommand : public CompositeUndoableCommand
{
public:
    //! \brief Creates a BatchCompositeCommand.
    //! \param selection the selection of items that is affected by the command.
    //! \param operationName the name of the composite command.
    static std::shared_ptr<BatchCompositeCommand> create(
      const Selection& selection,
      const std::string& operationName);

    BatchCompositeCommand(const BatchCompositeCommand&) = delete;
    const BatchCompositeCommand& operator=(const BatchCompositeCommand&) = delete;

    //! \brief Constructor.
    //! \param selection the selection of items that is affected by the command.
    //! \param operationName the name of the composite command.
    BatchCompositeCommand(
      const Selection& selection,
      const std::string& operationName);

    //! Retrieves the container of code-wrappers, one per run-time found in the selection.
    const CodeWrapperContainer& codeWrappers() const {
      return fWrappers;
    }

    //! The sub-operations for the code wrappers associated with the execute, undo and redo.
    static const std::string execSubOp;
    static const std::string undoSubOp;
    static const std::string redoSubOp;
      
    //! Wraps execution of sub-command with the command wrappers.
    void execute() override;

    //! Wraps undoing of sub-command with the command wrappers.
    void undo() override;

    //! Wraps redoing of sub-command with the command wrappers.
    void redo() override;

private:
    CodeWrapperContainer fWrappers;
};

}

#endif /* _ufeBatchCompositeCommand */
