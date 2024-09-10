#ifndef _ufeTransform3dPathSubject
#define _ufeTransform3dPathSubject
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "common/ufeExport.h"

#include "pathSubject.h"

UFE_NS_DEF {

//! \brief Helper to observe transform3d changes on a path
/*!
    Transform3dPathSubject observes all SceneItems in its path and forwards all
    transform3d notifications to the observers of Transform3dPathSubject.

    As the observed path changes (through reparent operations) the set of
    observed SceneItem subjects is automatically updated to reflect the new path.
*/
class UFE_SDK_DECL Transform3dPathSubject : public PathSubject
{
public:
    typedef std::shared_ptr<Transform3dPathSubject> Ptr;

    //! Destructor
    virtual ~Transform3dPathSubject();

    /*! Constructor

        \param path Path to make observable
    */
    Transform3dPathSubject(const Path& path);
};

}

#endif /* _ufeTransform3dPathSubject */
