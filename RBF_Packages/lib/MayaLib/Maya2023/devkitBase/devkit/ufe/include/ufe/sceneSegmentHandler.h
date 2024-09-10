#ifndef _sceneSegmentHandler
#define _sceneSegmentHandler
// ===========================================================================
// Copyright 2022 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "path.h"
#include "selection.h"

UFE_NS_DEF {

/*! \brief Provides services that operate on a scene segment.
    This base class defines an interface that runtimes can implement to provide
    services for a scene segment.

    A scene segment is a group of related scene items which are all in the same
    runtime. A scene segment can be nested within another scene segment under a
    specific scene item. A scene item which nests a scene segment is called a 
    gateway item. All the scene segments in the scene collectively form a tree.
    
    A nested scene segment will typically belong to a different runtime. It is
    possible to have a runtime that defines paths with more than one path segment,
    all with the same runTimeId, to represent peculiarities of paths in its data
    model, in which case a nested scene segment would have the same runTimeId.
    This is not expected to be a common case.

    When a scene item has children whose runTimeId differs from its own runTimeId,
    that scene item is a gateway node and those children are in a different scene
    segment. All of the children of a scene item have the same runTimeId.

    When consecutive path segments in a path have a different runTimeIds, then
    by definition that is a boundary between scene segments. When consecutive
    path segments in a path have the same runTimeId, then it may or may not be
    a boundary between scene segments, at the discretion of the runtime.
*/

class UFE_SDK_DECL SceneSegmentHandler
{
public:
    typedef std::shared_ptr<SceneSegmentHandler> Ptr;

    /*!
        Convenience method to retrieve the scene segment handler.
    */
    static Ptr sceneSegmentHandler(Rtid);

    //! Constructor.
    SceneSegmentHandler();
    //! Default copy constructor.
    SceneSegmentHandler(const SceneSegmentHandler&) = default;
    //! Destructor.
    virtual ~SceneSegmentHandler();

    /*!
        \param path The path of interest
        \return true if path is to a gateway item
    */
   virtual bool isGateway_(const Path& path) const = 0;

    /*!
        \param path The path of interest
        \return true if path is to a gateway item
    */
   static bool isGateway(const Path& path);

    /*!
        Find the gateway items which are descendants of path within path's scene segment.
        If path is a gateway node then search the scene segment which is an immediate child of path.
        \param path Path to root of hierarchy under which gateway items should be found, but only in the path's scene segment.
        \return Selection of all gateway items in path's scene segment.
    */
    virtual Selection findGatewayItems_(const Path& path) const = 0;

    /*!
        Find the gateway items which are descendants of path within path's scene segment.
        If path is a gateway node then search the scene segment which is an immediate child of path.
        \param path Path to root of hierarchy under which gateway items should be found, but only in the path's scene segment.
        \return Selection of all gateway items in path's scene segment.
    */
    static Selection findGatewayItems(const Path& path);
};

}

#endif /* _sceneSegmentHandler */
