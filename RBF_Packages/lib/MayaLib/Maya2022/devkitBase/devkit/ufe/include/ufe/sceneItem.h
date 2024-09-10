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
#include <vector>

UFE_NS_DEF {

/**
 * \class SceneItem
 *
 * \brief Universal Front End abstract scene item. 
 *
 * A scene item identifies an object or 3D path in the scene, independent
 * of its underlying runtime.  It consists of a path that identifies an object
 * in the scene.
 *
 * A scene item may optionally represent a property associated with a given
 * scene object.  A property represents data associated with a single node or
 * object.  In different runtimes, it is variously known as an attribute, a
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

    //! \return Convenience to obtain the runtime ID of the Path.
    Rtid runTimeId() const;

    //@{
    //! Unfortunately no compiler-generated default equality operators.  Items
    //! are equal if their path is equal.
    bool operator==(const SceneItem& rhs) const;
    bool operator!=(const SceneItem& rhs) const;
    //@}

    //! \return Convenience to return the name of the node at the tail of the Path.
    std::string nodeName() const;

    //! Pure virtual method to return the type of the last node
    //! \return type of node at the tail of the Path.
    virtual std::string nodeType() const = 0;

    //! Return a list of all ancestor types (including the type of
    //! the scene item itself) in order from closest ancestor to farthest.
    //! The starting type is itself included, as the first element of the results vector.
    //! The implementation in this class returns a vector containing a single
    //! item, the nodeType of this scene item.
    //! \return List of all ancestor types (including this one).
    virtual std::vector<std::string> ancestorNodeTypes() const;

    //! Return whether this scene item represents a property.  The
    //! implementation in this class returns false.
    //! \return true if this scene item represents a property.
    virtual bool isProperty() const;

private:

    Path fPath;
};

}

#endif  /* _ufeSceneItem */
