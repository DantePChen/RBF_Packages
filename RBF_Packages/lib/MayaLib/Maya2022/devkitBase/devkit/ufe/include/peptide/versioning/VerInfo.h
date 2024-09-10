#line 1 "S:/jenkins/workspace/ECP/ufe/ufe-full-windows/ufe/gitmodules/peptide/src/versioning/VerInfo_src.h"
//*****************************************************************************
// Copyright (c) 2022 Autodesk, Inc.
// All rights reserved.
//
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc. and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.
//*****************************************************************************

/// \file  VerInfo.h
/// \brief Peptide::VersionInfo: Information about the version of the product
/// \see   Peptide::VersionInfo

#ifndef PEPTIDE_VER_VERSION_INFO_H
#define PEPTIDE_VER_VERSION_INFO_H

//==============================================================================
// EXTERNAL DECLARATIONS
//==============================================================================

#include <string>

#ifndef PEPTIDE_USE_FOUR_FIELD_VERSION
#define PEPTIDE_USE_FOUR_FIELD_VERSION 0
#endif

namespace Peptide {

//==============================================================================
// CLASS DECLARATIONS
//==============================================================================

//==============================================================================
// CLASS VersionInfo
//==============================================================================

/// \brief Information about the version of the product
class VersionInfo {
public:
    /// \brief Enum that tracks our file format version numbers
    /// \details Add to the enum and update the kCurrent value
    /// as new formats are introduced
    enum class FileFormatVersionNumbers {
        // Jan 2015 : (major,minor) = (1,0)
        kInitial = 100,
        //
        kCurrent = kInitial
    };

    /*----- static member functions -----*/

    /// \brief Returns the components of the product version number.
    ///
    /// The version number is of the form "<major>.<minor>.<patch>"
    /// or "<arch>.<major>.<minor>.<patch>"
    ///
    /// \{
#if PEPTIDE_USE_FOUR_FIELD_VERSION
    static int getArchVersion();
#endif
    static int getMajorVersion();
    static int getMinorVersion();
    static int getPatchLevel();
    /// \}

    /// \brief Returns a short version identifier
    ///
    /// Returns a short version identifier such as for example "3.0.2".
    static std::string getVersionID();

    /// \brief Returns detailed information about the version of the product.
    ///
    /// Returns detailed information about the version of the product such
    /// as for example:
    ///
    ///  "MyProduct version 4.0.0 (Mon 09/28/2015, 201509282010, build 34567,
    ///  commit b5283ef)"
    ///
    /// Note that only the short git hash is included.
    static std::string getVersionInfo();

    /// \brief Returns the version suffix of the libraries
    ///
    /// Returns a short version identifier such as for example "4_0".
    static std::string getLibSuffix();

    /// \brief Returns the file version number
    ///
    /// File format version ID is specified by [major][minor]. The minor
    /// number is two digits and the major is 1 or 2 digits. For
    /// example 100 ( major = 1, minor = 0 )
    static int getFileFormatVersionID();

private:
    /*----- member functions -----*/

    /// \brief Prohibited and not implemented
    ///
    /// Don't allow construction since we only have static member functions.
    VersionInfo() = delete;
};

} // namespace Peptide

#endif
