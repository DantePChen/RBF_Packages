#ifndef _ufeCodeWrapperHandler
#define _ufeCodeWrapperHandler
// ===========================================================================
// Copyright 2023 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "batchOpsHandler.h"
#include "codeWrapper.h"

#include <string>

UFE_NS_DEF {

class Selection;
class CodeWrapperContainer;

//! \class CodeWrapperHandler
//! \brief Provides services to create code wrapper around operations affecting
//!        a selection of scene items.
//!
//!        Derives from BatchOpsHandler to allow compatibility with UFE v4 and
//!        because it affects operations on multiple scene items.
//!
//!        Only the CodeWrapperContainer is able to create the code wrappers.
//!        This ensure proper usage of the handler and the code wrappers it
//!        creates.
class UFE_SDK_DECL CodeWrapperHandler : public BatchOpsHandler
{
public:
    //! \brief Destructor.
    virtual ~CodeWrapperHandler() override;

protected:
    //! \brief Ask a runtime handler to create a code wrapper for the given selection and operation.
    //!        By default returns null to mean that the run-time does not need to wrap the operation.
    //! \param selection The list of items to be affected by the operation.
    //! \param operationName The name of the overall operation, for example "group".
    virtual CodeWrapper::Ptr
    createCodeWrapper(
      const Selection& selection,
      const std::string& operationName);

      friend class CodeWrapperContainer;
};

}

#endif /* _ufeCodeWrapperHandler */
