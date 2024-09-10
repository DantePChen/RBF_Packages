#ifndef _ufePathSubject
#define _ufePathSubject
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "common/ufeExport.h"

#include "subject.h"
#include "sceneItem.h"
#include "path.h"

#include <memory>
#include <vector>
#include <functional>

UFE_NS_DEF {

class Notification;

//! \brief Helper to make a Path observable
/*!
    PathSubject observes all SceneItems in its path and forwards all notifications
    to the observers of PathSubject.
    
    In Ufe a SceneItem can have multiple subjects to observe. PathSubject observes
    the SceneItem subjects which are registered through the AddSubject function.

    As the observed path changes (through reparent operations) the set of
    observed SceneItem subjects is automatically updated to reflect the new path.
*/
class UFE_SDK_DECL PathSubject : public Subject
{
public:
    typedef std::shared_ptr<PathSubject> Ptr;

    // Prevent the re-definition of the private helper methods from hiding
    // the public methods on Subject that we want to remain callable.
    using Subject::addObserver;
    using Subject::removeObserver;

    /*! \brief Callable function to add an observer to a SceneItem

        \param item The SceneItem to observe
        \param obs The observer to add
        \return True if the observer is added.
    */
    using AddObserverFcn = std::function<bool(const SceneItem::Ptr& item, const Observer::Ptr& obs)>;

    /*! \brief Callable function to remove an observer from a Path

        \param path The Path to remove observation on
        \param obs The observer to remove
        \return True if the observer is removed.
    */
    using RemoveObserverFcn = std::function<bool(const Path& Path, const Observer::Ptr& obs)>;

    //! Destructor
    virtual ~PathSubject();
    
    /*! Constructor

        \param path Path to make observable
        \param addObserverFcn Callable function to add an observer to a scene item
        \param removeObserverFcn Callable function to remove an observer from a path
    */
    PathSubject(const Path& path, AddObserverFcn addObserverFcn, RemoveObserverFcn removeObserverFcn);

private:
    // Use for scene changes
    void addObserver(const Path& path);
    void removeObserver(const Path& path);
    void addObservers(const Path& path, const Path& endAncestor);
    void removeObservers(const Path& path, const Path& endAncestor);

    /*!
        Add or Remove an observer from Path.
        \tparam ADD_OBSERVER True to add an Observer, False to remove an Observer.
        \param path Path to add or remove an Observer from.
    */
    template<bool ADD_OBSERVER>
    void addRemoveObserverHelper(const Path& path);

    /*!
        Add or Remove an observers from Path and each ancestor of path to endAncestor,
        not including endAncestor itself.
        \tparam ADD_OBSERVER True to add an Observer, False to remove an Observer.
        \param path Path to add or remove an Observer from.
        \param endAncestor Final ancestor to add or remove an Observer from.
    */
    template<bool ADD_OBSERVER>
    void addRemoveObserversHelper(Path path, const Path& endAncestor);

    void reparent(const Path& oldPath, const Path& newPath);
    void rename(const Path& oldPath, const Path& newPath);
    void objectDelete(const Path& path);

    class SceneChangedObserver;

    Path _path;
    const AddObserverFcn _addObserverFcn;
    const RemoveObserverFcn _removeObserverFcn;
    const Observer::Ptr _forwardingObserver;
    const Observer::Ptr _sceneChangedObserver;
    bool _attached;
};

}

#endif /* _ufePathSubject */
