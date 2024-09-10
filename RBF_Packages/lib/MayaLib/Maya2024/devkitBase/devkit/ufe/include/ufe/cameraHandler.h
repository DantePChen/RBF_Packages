#ifndef _cameraHandler
#define _cameraHandler
// ===========================================================================
// Copyright 2020 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "camera.h"
#include "sceneItem.h"
#include "selection.h"

UFE_NS_DEF {

class Path;

//! \brief Factory base class for Camera interface.
/*!

  This base class defines an interface for factory objects that runtimes
  can implement to create a Camera interface object.
*/

class UFE_SDK_DECL CameraHandler
{
public:
    typedef std::shared_ptr<CameraHandler> Ptr;
    //! Constructor.
    CameraHandler();
    //! Default copy constructor.
    CameraHandler(const CameraHandler&) = default;
    //! Destructor.
    virtual ~CameraHandler();

    /*!
        Creates a Camera interface on the given SceneItem.
        \param item SceneItem to use to retrieve its Camera interface.
        \return Camera interface of given SceneItem. Returns a null
        pointer if no Camera interface can be created for the item.
    */
    virtual Camera::Ptr camera(
        const SceneItem::Ptr& item) const = 0;

    /*!
        Find cameras which are at path or descendants of path and in the scene segment of path.
        If path is a gateway node then search the scene segment which is an immediate child of path.
        \param path Path to inclusive root of hierarchy under which cameras should be found, but only in the path's scene segment.
        \return Selection of all cameras at or under path, in path's scene segment.
    */
    virtual Selection find_(const Path& path) const = 0;

    /*!
        Find cameras which are at path or descendants of path and in the scene segment of path.
        If path is a gateway node then search the scene segment which is an immediate child of path.
        \param path Path to inclusive root of hierarchy under which cameras should be found, but only in the path's scene segment.
        \return Selection of all cameras at or under path, in path's scene segment.
    */
    static Selection find(const Path& path);

    /*!
        Find all the cameras which are at path or descendants of path.
        \param path Path to search from (inclusive).
        \return Selection of all cameras.
    */
    static Selection findAll(const Path& path);
};

}

#endif /* _cameraHandler */
