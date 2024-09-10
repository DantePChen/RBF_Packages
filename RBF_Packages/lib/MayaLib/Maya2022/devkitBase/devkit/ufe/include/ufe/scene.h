#ifndef _scene
#define _scene
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "common/ufeExport.h"

#include "sceneItem.h"
#include "subject.h"

#include <memory>

UFE_NS_DEF {

class ObjectAdd;
class ObjectDelete;
class ObjectPathChange;
class SubtreeInvalidate;
class Path;
class SceneCompositeNotification;

//! \brief Singleton class to manage UFE scenes.
/*!

  This singleton class can be thought of as a "container" of scene objects, and
  so is responsible notifications which affect its objects.

  It also has post-notification hooks, which are indented to be implemented by
  the DCC app.

  The application must provide the scene singleton, and initialize it in UFE
  with the initializeInstance() call.  There is no Python binding for
  initializing the instance, which must therefore be done in C++.
*/

class UFE_SDK_DECL Scene : public Subject
{
public:

    typedef std::shared_ptr<Scene> Ptr;

    //! \return The scene singleton instance.
    static Scene& instance();

    //! Initialize the scene singleton.  The application should call this once,
    //! on startup, to initialize, and to clean up once on shut down, with a
    //! null pointer.
    //! \param instance The scene instance.
    //! \exception std::invalid_argument if the scene instance is initialized with a non-null pointer more than once.
    static void initializeInstance(const Ptr& instance);

    //! Destructor
    ~Scene() override;

    //! Cannot copy the scene singleton.
    Scene(const Scene&) = delete;
    //! Cannot assign the scene singleton.
    Scene& operator=(const Scene&) = delete;

    //! Notify observers.
    void notify(const Notification& notification) override;

    //! Composite notification predicate.
    //! \return True if the scene is collecting notifications into a composite notification.
    bool inCompositeNotification() const override;

protected:

    //! Only derived classes can create a scene.
    Scene();

    /*! notifyObjectAdd() hook method, called after observer notification.
      Implementation in this class is empty.
      \param notification Object add notification for the added object.
    */
    virtual void postNotifyObjectAdd(const ObjectAdd& notification);

    /*! notifyObjectDelete() hook method, called after observer notification.
      Implementation in this class is empty.
      \param notification Object delete notification for the deleted object.
    */
    virtual void postNotifyObjectDelete(const ObjectDelete& notification);

    /*! notifyObjectPathChange() hook method, called after observer notification.
      Implementation in this class is empty.
      \param notification Object path change notification for the object whose path was changed.
    */
    virtual void postNotifyObjectPathChange(const ObjectPathChange& notification);

    /*! notifySubtreeInvalidate() hook method, called after observer notification.
      Implementation in this class is empty.
      \param notification Subtree invalidate notification for the subtree which was invalidated.
    */
    virtual void postNotifySubtreeInvalidate(const SubtreeInvalidate& notification);

    /*! notifySceneCompositeChange() hook method, called after observer notification.
      Implementation in this class is empty.
      \param notification scene composite notification.
    */
    virtual void postNotifySceneCompositeChange(const SceneCompositeNotification& notification);

    //@{
    //! Scene Notification guard interface.
    //! creates a SceneCompositeNotification on begin, and notifies
    //! observers with the composite notification on end.
    void beginNotificationGuard() override;
    void endNotificationGuard() override;
    //@}

private:
    //! Composite notification, when within the scope of a guard.
    std::unique_ptr<SceneCompositeNotification> fCompositeNotification;
};

}

#endif /* _scene */
