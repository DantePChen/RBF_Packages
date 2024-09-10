#ifndef _ufeCodeWrapper
#define _ufeCodeWrapper
// ===========================================================================
// Copyright 2023 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "common/ufeExport.h"

#include <memory>
#include <string>
#include <vector>

UFE_NS_DEF {

class Selection;

//! \class CodeWrapper
//! \brief Wrap the execution of some other function, called sub-operation,
//!        with arbitrary prelude and cleanup virtual functions.
//!
//!        For example, it can be used to surround the execute, undo and
//!        redo sub-operations of a command.
class UFE_SDK_DECL CodeWrapper
{
public:
    using Ptr = std::shared_ptr<CodeWrapper>;

    virtual ~CodeWrapper();

    //! \brief Function called before execution of some other code.
    //! \param subOperation the name of the sub-operation being wrapped.
    virtual void prelude(const std::string& subOperation) = 0;

    //! \brief Function called after execution of some other code.
    //! \param subOperation the name of the sub-operation being wrapped.
    virtual void cleanup(const std::string& subOperation) = 0;
};

//! \class CodeWrapperContainer
//! \brief A container of multiple CodeWrapper, one per UFE run-time that is
//!        involved in an operation.
class UFE_SDK_DECL CodeWrapperContainer
{
public:
    using Container = std::vector<CodeWrapper::Ptr>;

    //! \brief Creates a container of wrapper, one for each run-time that is used in the selection.
    //! \param selection the selection of item that will be affected by an operation.
    //! \param operationName the name of the operation being wrapped.
    CodeWrapperContainer(
        const Selection& selection,
        const std::string& operationName
    );

    //! \brief Retrieves the code wrappers that were created in the constructor.
    const Container& codeWrappers() const {
        return fWrappers;
    }

private:
    Container fWrappers;
};

}

#endif /* _ufeCodeWrapper */
