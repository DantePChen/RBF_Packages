#line 1 "S:/jenkins/workspace/ECP/ufe/ufe-full-python3.10-windows/ufe/gitmodules/peptide/src/versioning/VerFormat_src.h"
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

/// \file  VerFormat.h
/// \brief Peptide::VersionFormat: Utility class for formatting version
///                                information.
/// \see   Peptide::VersionFormat

#ifndef PEPTIDE_VER_VERSION_FORMAT_H
#define PEPTIDE_VER_VERSION_FORMAT_H

//==============================================================================
// EXTERNAL DECLARATIONS
//==============================================================================

#include <iomanip>
#include <sstream>

#ifndef PEPTIDE_USE_FOUR_FIELD_VERSION
#define PEPTIDE_USE_FOUR_FIELD_VERSION 0
#endif

namespace Peptide {

//==============================================================================
// CLASS DECLARATIONS
//==============================================================================

//==============================================================================
// CLASS VersionFormat
//==============================================================================

/// \brief Utility class for formatting version information.
///
/// The VersionTrait is typically generated using CMake configuration files. It
/// contains all of the version information of given component (library,
/// executable, etc.). This class contains functions to format this version
/// information to a human readable form.
///
/// The VersionTrait must provide the following static member functions:
///
#if PEPTIDE_USE_FOUR_FIELD_VERSION
///     static int         getArchVersion();
#endif
///     static int         getMajorVersion();
///     static int         getMinorVersion();
///     static int         getPatchLevel();
///     static char const* getGitCommit();
///     static char const* getGitBranch();
///     static int         getBuildNumber();
///     static char const* getBuildDate();
///     static char const* getProductName();
///
/// \tparam VersionTrait CMake generated version trait.
template <class VersionTrait>
class VersionFormat {
public:
    /*----- static member functions -----*/

#if PEPTIDE_USE_FOUR_FIELD_VERSION
    /// \brief Returns a short version identifier for the given version trait.
    ///
    /// Returns a short version identifier such as for example "1.3.0.2".
    static std::string getVersionID() {
        std::ostringstream builder;
        builder << VersionTrait::getArchVersion() << "."
                << VersionTrait::getMajorVersion() << "."
                << VersionTrait::getMinorVersion() << "."
                << VersionTrait::getPatchLevel();
        return builder.str();
    }
#else
    /// \brief Returns a short version identifier for the given version trait.
    ///
    /// Returns a short version identifier such as for example "3.0.2".
    static std::string getVersionID() {
        std::ostringstream builder;
        builder << VersionTrait::getMajorVersion() << "."
                << VersionTrait::getMinorVersion() << "."
                << VersionTrait::getPatchLevel();
        return builder.str();
    }
#endif

    /// \brief Returns detailed information for the given version trait.
    ///
    /// Returns detailed information about the version of the binary such
    /// as for example:
    ///
    ///  "MyProduct version 4.0.0 (Mon 09/28/2015, 201509282010, build 34567,
    ///  commit b5283ef, branch origin/master)"
    ///
    /// Note that only the short git hash is included.
    /// Note that the product version may have four fields if
    /// PEPTIDE uses PEPTIDE_ARCH_VERSION.
    static std::string getVersionInfo() {
        std::ostringstream builder;
        builder << VersionTrait::getProductName() << " version "
                << getVersionID() << " (" << VersionTrait::getBuildDate()
                << ", build " << VersionTrait::getBuildNumber() << ", commit "
                << std::setw(7)
                << std::string(VersionTrait::getGitCommit()).substr(0, 7)
                << ", branch " << VersionTrait::getGitBranch() << ")";
        return builder.str();
    }

private:
    /*----- member functions -----*/

    /// \brief Prohibited and not implemented
    ///
    /// Don't allow construction since we only have static member functions.
    VersionFormat() = delete;
};

} // namespace Peptide

#endif
