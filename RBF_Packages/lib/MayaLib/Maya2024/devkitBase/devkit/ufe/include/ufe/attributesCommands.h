#ifndef _ufe_attributes_commands
#define _ufe_attributes_commands

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
#include "attribute.h"
#include "undoableCommand.h"

#include <memory>

UFE_NS_DEF {

//! \brief Command to add an attribute to a scene item.
/*!
  This class provides the interface to retrieve the new attribute, which is
  created on command execution.
*/

class UFE_SDK_DECL AttributeUndoableCommand : public UndoableCommand
{
public:
    typedef std::shared_ptr<AttributeUndoableCommand> Ptr;   
    //! Default constructor.
    AttributeUndoableCommand() = default;

    //! Destructor.
    ~AttributeUndoableCommand() override = default;

    //! Pure virtual method to retrieve the new attribute.
    virtual Attribute::Ptr attribute() const = 0;
};



} // end namespace

#endif /* _ufe_attributes_commands */
