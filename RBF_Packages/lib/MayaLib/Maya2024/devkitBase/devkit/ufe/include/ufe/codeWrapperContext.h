#ifndef _ufeCodeWrapperContext
#define _ufeCodeWrapperContext
// ===========================================================================
// Copyright 2023 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "codeWrapper.h"

#include <string>
#include <vector>

UFE_NS_DEF {

class Selection;

//! \class CodeWrapperContext
//! \brief Wraps execution of other code by calling the prelude and cleanup
//!        code wrappers for given sub-operation.
class UFE_SDK_DECL CodeWrapperContext
{
public:
    //! \brief Creates a context that immediately calls all the prelude functions
    //!        of the code wrapper kept in the given container for the given sub-operation.
    //! \param container the container of code wrappers.
    //! \param subOperation the name of the sub-operation being wrapped.
    CodeWrapperContext(
        const CodeWrapperContainer& container,
        const std::string& subOperation);
    ~CodeWrapperContext();

    // Do not allow copying since the CodeWrapper prelude and cleanup
    // must only be executed once.
    CodeWrapperContext(const CodeWrapperContext&) = delete;
    const CodeWrapperContext& operator=(const CodeWrapperContext&) = delete;

    CodeWrapperContext(CodeWrapperContext&&) = delete;
    const CodeWrapperContext& operator=(CodeWrapperContext&&) = delete;

    //! \brief Invoke the cleanup of the wrappers immediately, before the destructor.
    //!        After this call, the destructor will do nothing instead of doing cleanup.
    void cleanup();

private:
    // This function is called during construction.
    void prelude();

    // The CodeWrapperContext can only be created with a valid container of
    // code wrapper but does not manage the lifetime of that container. The
    // container is held by another class, for example a BatchCompositeCommand,
    // which is also the class that uses the context. See BatchCompositeCommand.cpp
    // for an example.
    //
    // Note: we need to keep it as a pointer so we can clear it out when cleanup()
    //       is explicitly called.
    const CodeWrapperContainer* fContainer;
    std::string                 fSubOperation;
};

}

#endif /* _ufeCodeWrapperContext */
