#ifndef _ufeSceneNotification
#define _ufeSceneNotification
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "notification.h"
#include "sceneItem.h"

UFE_NS_DEF {

//! \brief Base class for object add scene notifications.
/*!

  This class is the base class for object add scene notifications.
*/
class UFE_SDK_DECL ObjectAdd: public Notification
{
public:
    //! Constructor.
    ObjectAdd(const SceneItem::Ptr& item);

    //! Default copy constructor.
    ObjectAdd(const ObjectAdd&) = default;
    
    //! Destructor.
    ~ObjectAdd() override;

    //! \return Added SceneItem.
    SceneItem::Ptr item() const;

private:
    SceneItem::Ptr fItem;
};

//! \brief Base class for object delete scene notifications.
/*!

  This class is the base class for object delete scene notifications.  There
  are two ObjectDelete derived class notification types, for post-delete and
  pre-delete.  This covers run-times that send notification just before the
  object is deleted (e.g. Maya), and those that send notification just after
  the object is deleted (e.g. USD).  The important point is that regardless of
  whether observers get a pre- or a post- notification, the object is still
  valid when the notification is sent.
*/
class UFE_SDK_DECL ObjectDelete: public Notification
{
public:
    //! Constructor.
    ObjectDelete(const SceneItem::Ptr& item);

    //! Default copy constructor.
    ObjectDelete(const ObjectDelete&) = default;
    
    //! Destructor.
    ~ObjectDelete() override;

    //! \return Deleted SceneItem.
    SceneItem::Ptr item() const;

private:
    SceneItem::Ptr fItem;
};

//! \brief Object delete scene notifications that occur after the object was deleted.
/*!

  The object is still valid, even if the notification is sent after the delete.
*/
class UFE_SDK_DECL ObjectPostDelete: public ObjectDelete
{
public:
    //! Constructor.
    ObjectPostDelete(const SceneItem::Ptr& item);

    //! Default copy constructor.
    ObjectPostDelete(const ObjectPostDelete&) = default;
    
    //! Destructor.
    ~ObjectPostDelete() override;
};

//! \brief Object delete scene notifications that occur before the object is deleted.
/*!

  The object is valid when the notification is sent.
*/
class UFE_SDK_DECL ObjectPreDelete: public ObjectDelete
{
public:
    //! Constructor.
    ObjectPreDelete(const SceneItem::Ptr& item);

    //! Default copy constructor.
    ObjectPreDelete(const ObjectPreDelete&) = default;
    
    //! Destructor.
    ~ObjectPreDelete() override;
};

//! \brief Base class for object path change scene notifications.
/*!

  This is the notification base class for data model changes where no
  object is created or removed, but an existing object sees its path modified,
  or a path to it is added or removed.
*/
class UFE_SDK_DECL ObjectPathChange: public Notification
{
public:
    //! Constructor.
    ObjectPathChange();

    //! Default copy constructor.
    ObjectPathChange(const ObjectPathChange&) = default;
    
    //! Destructor.
    ~ObjectPathChange() override;
};

//! \brief Object renamed scene notification.
/*!

  A rename notification is a path change notification where only the tail
  component differs.  Implicitly, all children of the renamed object have their
  paths changed; no notification is sent for this.
*/
class UFE_SDK_DECL ObjectRename: public ObjectPathChange
{
public:
    //! Constructor.
    ObjectRename(const SceneItem::Ptr& item, const Path& previousPath);

    //! Default copy constructor.
    ObjectRename(const ObjectRename&) = default;
    
    //! Destructor.
    ~ObjectRename() override;

    //! \return Renamed SceneItem.
    SceneItem::Ptr item() const;

    //! \return Previous path.
    Path previousPath() const;

private:
    const SceneItem::Ptr fItem;
    const Path           fPreviousPath;
};

//! \brief Object reparented scene notification.
/*!

  A reparent notification is a path change notification where one or more
  components, including the tail, are changed.  Conceptually, it is sent when
  the tail component object is given a new parent.  However, because child
  names must be unique, the tail component can also be changed if there is an
  existing child with the same name under the new parent.  Implicitly, all
  children of the reparented object have their paths changed; no notification
  is sent for this.

  Should have been implemented using a type alias:

  using ObjectReparent = ObjectRename;

  See sceneItemOps.h for description of pybind11 problems encountered with this
  approach.  PPT, 13-Aug-2018.
*/
class UFE_SDK_DECL ObjectReparent: public ObjectPathChange
{
public:
    //! Constructor.
    ObjectReparent(const SceneItem::Ptr& item, const Path& previousPath);

    //! Default copy constructor.
    ObjectReparent(const ObjectReparent&) = default;
    
    //! Destructor.
    ~ObjectReparent() override;

    //! \return Reparented SceneItem.
    SceneItem::Ptr item() const;

    //! \return Previous path.
    Path previousPath() const;

private:
    const SceneItem::Ptr fItem;
    const Path           fPreviousPath;
};

//! \brief Path added to object scene notification.
/*!

  A path add notification is a path change notification where a new path is
  added to an existing object.  Not all run-times support these semantics: a
  new path is created to the tail component object, and data model observers
  must reflect this, but the tail component object itself is unchanged.
  Implicitly, all children of the object to which a path was added have also
  had a path added to them; no notification is sent for this.
*/
class UFE_SDK_DECL ObjectPathAdd: public ObjectPathChange
{
public:
    //! Constructor.
    ObjectPathAdd(const SceneItem::Ptr& item);

    //! Default copy constructor.
    ObjectPathAdd(const ObjectPathAdd&) = default;
    
    //! Destructor.
    ~ObjectPathAdd() override;

    //! \return Added path to existing SceneItem.
    SceneItem::Ptr item() const;

private:
    const SceneItem::Ptr fItem;
};

//! \brief Path removed from object scene notification.
/*!

  A path remove notification is a path change notification where an existing
  path is removed from an existing object.  Not all run-times support these
  semantics.  Data model observers must reflect the removal of the path, but
  the tail component object itself is unchanged.  When the last path to an
  object is removed, an ObjectDelete notification will be sent.  Implicitly,
  all children of the object from which a path was removed have also had a path
  removed from them; no notification is sent for this.
*/
class UFE_SDK_DECL ObjectPathRemove: public ObjectPathChange
{
public:
    //! Constructor.
    ObjectPathRemove(const Path& removedPath);

    //! Default copy constructor.
    ObjectPathRemove(const ObjectPathRemove&) = default;
    
    //! Destructor.
    ~ObjectPathRemove() override;

    //! \return Removed path.
    Path removedPath() const;

private:
    const Path fRemovedPath;
};

}

#endif /* _ufeSceneNotification */
