//
// ==================================================================
// Copyright 2014 Autodesk, Inc.  All rights reserved.
// 
// This computer source code  and related  instructions and comments are
// the unpublished confidential and proprietary information of Autodesk,
// Inc. and are  protected  under applicable  copyright and trade secret
// law. They may not  be disclosed to, copied or used by any third party
// without the prior written consent of Autodesk, Inc.
// ==================================================================
//
//! \file synColor/basePath.h
//!
//! \brief List of supported base paths for transforms referenced by relative
//!        paths, and string/enum conversion utilities.
//!

#ifndef _SYNCOLOR_BASE_PATH_H
#define _SYNCOLOR_BASE_PATH_H

#include <synColor/synExport.h>
#include <synColor/synColorNamespace.h>

SYN_VISIBILITY_PUSH

namespace SYNCOLOR
{
    //! List of supported base paths to use to interpret relative file paths.
    enum BasePath
    {
      BASE_PATH_NONE = 0,  //!< No base path set.
      BASE_PATH_AUTODESK,  //!< Autodesk SynColor transform collection path.
      BASE_PATH_SHARED     //!< User transform path set in the SynColor preferences.
    };

    //! \brief Convert the base path string to its corresponding enum value.
    //! \param name String representing the base path name.
    //! \return BasePath value for 'name'.
    //!         Returns SYNCOLOR::BASE_PATH_NONE if the name is not recognized.
    SYN_EXPORT BasePath getBasePath(const char* name);

    //! \brief Convert the BasePath value to a string.
    //! \param basePath BasePath value.
    //! \return String corresponding to basePath.
    SYN_EXPORT const char* getBasePathName(BasePath basePath);
};

SYN_VISIBILITY_POP

#endif // _SYNCOLOR_BASE_PATH_H
