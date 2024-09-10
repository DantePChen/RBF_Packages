#ifndef _pathMappingHandler
#define _pathMappingHandler

// ===========================================================================
// Copyright 2021 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "common/ufeExport.h"

#include "sceneItem.h"

UFE_NS_DEF {

/*!
  This base class defines an interface that runtimes can implement to provide
  a path mapping service. A runtime can have paths that are aliases to one
  another. This service allows conversion from the path to the aliased path
  and vice-versa.
*/

class UFE_SDK_DECL PathMappingHandler
{
public:
    typedef std::shared_ptr<PathMappingHandler> Ptr;

    //@{
    //! Convenience method to retrieve the path mapping handler.
    //! The handler interface will remain valid as long as the given runtime
    //! remains set in the runtime manager.
    static Ptr pathMappingHandler(Rtid);
    static Ptr pathMappingHandler(const SceneItem::Ptr&);
    //@}

    //! Constructor.
    PathMappingHandler() = default;
    //! Default copy constructor.
    PathMappingHandler(const PathMappingHandler&) = default;
    //! Destructor.
    virtual ~PathMappingHandler() = default;

    //! Maps a runtime path into its host alias equivalent.
    virtual Path toHost(const Path&) const = 0;

    //! Maps a host alias of the path into its runtime equivalent.
    virtual Path fromHost(const Path&) const = 0;
};

}

#endif // _pathMappingHandler
