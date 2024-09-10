#ifndef _undoableCommandMgr
#define _undoableCommandMgr
// ===========================================================================
// Copyright 2022 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "common/ufeExport.h"

#include "undoableCommand.h"

#include <memory>

UFE_NS_DEF {

class UndoableCommandGuard;

//! \brief Abstract base class for undoable command manager.
/*!
  Certain UFE clients will make changes to a data model by using Ufe::UndoableCommand and want
  those changes to generate application-level undoable commands. This singleton class allows
  an application to provide this functionality by registering a concrete implementation
  of UndableCommandMgr.

  If the application doesn't support an undo queue, the concrete implementation only has to execute
  the command which is the default behaviour of the class.

  Note that there will be only one instance of the undoable command manager in the application.
*/
class UFE_SDK_DECL UndoableCommandMgr
{
public:

    typedef std::shared_ptr<UndoableCommandMgr> Ptr;

    //@{
    //! Constructors.
    UndoableCommandMgr(const UndoableCommandMgr&) = delete;
    UndoableCommandMgr& operator= (const UndoableCommandMgr&) = delete;
    //@}

    //! Destructor.
    virtual ~UndoableCommandMgr();

    //! \return The command manager singleton instance.
    static UndoableCommandMgr& instance();

    //! Initialize the command manager singleton.  The application should call this once at
    //! startup to initialize the instance and once at shut down for clean-up.
    //! \param instance The command manager instance or null to remove the existing instance.
    //! \exception std::invalid_argument if the instance is initialized with a non-null pointer more than once.
    static void initializeInstance(const Ptr& instance);

    //! \brief The method directly executes the command i.e., not undoable execution.
    //! \note The application implementation should execute the command and add it in its undo queue
    //! mechanism if it exists; otherwise, it can use the default implementation.
    virtual void executeCmd(const UndoableCommand::Ptr& cmd) const;

protected:

    friend class UndoableCommandGuard;

    //! \brief The method registers the command (i.e. without executing it) to the undo queue.
    //! \note The application implementation should only register the command to its undo queue
    //! mechanism if it exists; otherwise, it does nothing. The command is not executed.
    virtual void registerCmd(const UndoableCommand::Ptr& cmd) const;

    //! Only derived classes can create an undoable command manager.
    UndoableCommandMgr();
};


class UFE_SDK_DECL UndoableCommandGuard
{
public:

    //@{
    //! Constructors.
    explicit UndoableCommandGuard(const std::string& commandString);
    UndoableCommandGuard(const UndoableCommandGuard&) = delete;
    UndoableCommandGuard& operator= (const UndoableCommandGuard&) = delete;
    //@}

    //! Destructor.
    virtual ~UndoableCommandGuard();

    //! Set execution as a success.
    void setSuccess();

private:
    UndoableCommandMgr::Ptr fPreviousCommandMgr;
};

}

#endif /* _undoableCommandMgr */
