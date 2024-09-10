#ifndef _batchOpsHandler
#define _batchOpsHandler
// ===========================================================================
// Copyright 2022 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "undoableCommand.h"
#include "value.h"

UFE_NS_DEF {

class Selection;

/*! \brief Provides services to properly manage complex sequences of operations
           using bracketed guards.

  This handler provides begin/end callbacks for operations that will be done
  in batch on a large selection, providing services to allow storing options to
  be used on each operations. This will also allow doing extra processing at the
  end of the batch.
*/
class UFE_SDK_DECL BatchOpsHandler
{
public:
    typedef std::shared_ptr<BatchOpsHandler> Ptr;
    //! Constructor.
    BatchOpsHandler();
    //! Default copy constructor.
    BatchOpsHandler(const BatchOpsHandler&) = default;
    //! Destructor.
    virtual ~BatchOpsHandler();

    /*!
      Asks all registered BatchOpsHandlers to prepare a command that can duplicate in batch the
      items in the selection list that belong to their runtimes. These sub commands will be gathered
      in a composite command that will be returned here. The returned command is not executed.
      \param selection The list of items to duplicate.
      \param duplicateOptions Options affecting the batched duplicate operation.
    */
    static SelectionUndoableCommand::Ptr
    duplicateSelectionCmd(
      const Selection& selection,
      const ValueDictionary& duplicateOptions);

protected:
    /*!
      Ask a runtime handler to create a command that can be executed to duplicate all the items in
      the selection list that are supported by that runtime. The returned command is not executed.

      Runtimes are expected to:
        - Only process SceneItems from their own runtime.
        - Return nullptr if none of the items in the selection can be duplicated.
      \param selection The list of items to duplicate.
      \param duplicateOptions Options affecting the batched duplicate operation.
    */
    virtual SelectionUndoableCommand::Ptr
    duplicateSelectionCmd_(
      const Selection& selection,
      const ValueDictionary& duplicateOptions) = 0;
};

}

#endif /* _batchOpsHandler */
