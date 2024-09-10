#ifndef _versionInfo
#define _versionInfo
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "common/ufeExport.h"

UFE_NS_DEF {

    //! \brief Version information of UFE
    /*!
        Interface to retrieve version information of UFE
    */

    class UFE_SDK_DECL VersionInfo
    {
    public:

        //! \return Major version of UFE
        static int getMajorVersion();

        //! \return Minor version of UFE
        static int getMinorVersion();

        //! \return Patch version of UFE
        static int getPatchLevel();

        //! \return Build number of UFE
        static int getBuildNumber();

        //! \return Git Commit SHA of UFE
        static char const* getGitCommit();
        
        //! \return Git branch name of UFE
        static char const* getGitBranch();
        
        //! \return Git Build Date of UFE
        static char const* getBuildDate();
    };

}

#endif /* _versionInfo */
