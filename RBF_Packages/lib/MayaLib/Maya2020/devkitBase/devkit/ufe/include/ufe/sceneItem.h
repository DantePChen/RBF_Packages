#ifndef _ufeSceneItem
#define _ufeSceneItem
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "path.h"

#include <cstdint>
#include <memory>
#include <string>

UFE_NS_DEF {

/**
 * \class SceneItem
 *
 * \brief Universal Front End abstract scene item. 
 *
 * A scene item identifies an object or 3D path in the scene, independent
 * of its underlying run-time.  It consists of a path that identifies an object
 * in the scene.
 *
 * A scene item may optionally represent a property associated with a given
 * scene object.  A property represents data associated with a single node or
 * object.  In different run-times, it is variously known as an attribute, a
 * component, or an underworld path, but is always associated with a single
 * node or object.
 */

class UFE_SDK_DECL SceneItem
{
public:
    typedef std::shared_ptr<SceneItem> Ptr;
    
    //! Constructor.
    //! \param path Path of the scene item to build.
    SceneItem(const Path& path);

    //! Default copy constructor.
    SceneItem(const SceneItem&) = default;

    //! Destructor.
    virtual ~SceneItem();

    //! \return Path of the SceneItem.
    const Path& path() const;

    //! \return Convenience to obtain the run-time ID of the Path.
    Rtid runTimeId() const;

    //@{
    //! Unfortunately no compiler-generated default equality operators.  Items
    //! are equal if their path is equal.
    bool operator==(const SceneItem& rhs) const;
    bool operator!=(const SceneItem& rhs) const;
    //@}

    //! Pure virtual method to return the type of the last node
    //! \return type of node at the tail of the Path.
    virtual std::string nodeType() const = 0;

    //! Return whether this scene item represents a property.  The
    //! implementation in this class returns false.
    //! \return true if this scene item represents a property.
    virtual bool isProperty() const;

private:

    Path fPath;
};

}

#endif  /* _ufeSceneItem */
