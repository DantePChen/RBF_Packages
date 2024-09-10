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

#include <list>
#include <bitset>

UFE_NS_DEF {

//! \brief Base class for all scene notifications.
/*!

  This class is the base class for all scene changed notifications.
*/
class UFE_SDK_DECL SceneChanged: public Notification
{
public:
    enum OpType : unsigned char {
        ObjectAdd,
        ObjectDelete,
        ObjectPathChange,
        SubtreeInvalidate,
        SceneCompositeNotification
    };

    using SubOpStorageType = unsigned char;
    constexpr static SubOpStorageType SubOpTypeNone = 0;

    //! Default copy constructor.
    SceneChanged(const SceneChanged&) = default;

    //! Destructor.
    ~SceneChanged() override;

    //! \return Path to the object that was changed.  In the case of a
    //! composite notification, this path will be empty.
    virtual Path changedPath() const;

    //! \return OpType of the notification.
    OpType opType() const;

    //! \return SubOpType of the notification specific to the OpType.
    virtual SubOpStorageType subOpType() const;

protected:
    //! Constructor.
    SceneChanged(OpType opType);
private:
    const OpType fOpType;
};

//! \brief Base class for object add scene notifications.
/*!

  This class is the base class for object add scene notifications.
*/
class UFE_SDK_DECL ObjectAdd: public SceneChanged
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

    //! \return Path to the added object.
    Path changedPath() const override;

private:
    const SceneItem::Ptr fItem;
};

//! \brief Base class for object delete scene notifications.
/*!

  This class is the base class for object delete scene notifications.  There
  are three ObjectDelete derived class notification types: post-delete, 
  pre-delete and destroyed.  This covers runtimes that send notification just before the
  object is deleted (e.g. Maya), and those that send notification just after
  the object is deleted (e.g. USD).
*/
class UFE_SDK_DECL ObjectDelete: public SceneChanged
{
public:
    enum SubOpType : SubOpStorageType {
        None = SubOpTypeNone,
        ObjectPostDelete,
        ObjectPreDelete,
        ObjectDestroyed
    };

    //! Constructor.
    ObjectDelete(const Path& path);

    //! Default copy constructor.
    ObjectDelete(const ObjectDelete&) = default;

    //! Destructor.
    ~ObjectDelete() override;

    //! \return Path to the object that was deleted.
    Path path() const;

    //! \return Calls path().
    Path changedPath() const override;

    //! \return SubOpType of the notification specific to ObjectDelete.
    SubOpStorageType subOpType() const override;

protected:
    //! Constructor.
    ObjectDelete(const Path& path, SubOpType objectDeleteOpType);

private:
    const Path fPath;
    const SubOpType fSubOpType;
};

//! \brief Object delete scene notifications that occur after the object was deleted.
/*!

  The object is still valid, even if the notification is sent after the delete.
  \sa ObjectPreDelete, ObjectDestroyed
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

    //! \return Deleted SceneItem.
    SceneItem::Ptr item() const;

private:
    const SceneItem::Ptr fItem;
};

//! \brief Object delete scene notifications that occur before the object is deleted.
/*!

  The object is valid when the notification is sent.
  \sa ObjectPostDelete, ObjectDestroyed
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

    //! \return Deleted SceneItem.
    SceneItem::Ptr item() const;

private:
    const SceneItem::Ptr fItem;
};

//! \brief Object destroyed scene notification that occurs once an object is destroyed.
/*!

  The object is no longer valid, so no hierarchy interface can be obtained for it.
  \sa ObjectPreDelete, ObjectPostDelete
*/
class UFE_SDK_DECL ObjectDestroyed: public ObjectDelete
{
public:
    //! Constructor.
    ObjectDestroyed(const Path& path);

    //! Default copy constructor.
    ObjectDestroyed(const ObjectDestroyed&) = default;

    //! Destructor.
    ~ObjectDestroyed() override;
};

//! \brief Base class for object path change scene notifications.
/*!

  This is the notification base class for data model changes where no
  object is created or removed, but an existing object sees its path modified,
  or a path to it is added or removed.
*/
class UFE_SDK_DECL ObjectPathChange: public SceneChanged
{
public:
    enum SubOpType : SubOpStorageType {
        None = SubOpTypeNone,
        ObjectRename,
        ObjectReparent,
        ObjectPathAdd,
        ObjectPathRemove
    };

    //! Constructor.
    ObjectPathChange();

    //! Default copy constructor.
    ObjectPathChange(const ObjectPathChange&) = default;

    //! Destructor.
    ~ObjectPathChange() override;

    //! \return SubOpType of the notification specific to ObjectPathChange.
    SubOpStorageType subOpType() const override;

protected:
    //! Constructor.
    ObjectPathChange(SubOpType objectPathChangeOpType);
    const SubOpType fSubOpType;
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

    //! \return Calls previousPath().
    Path changedPath() const override;

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

    //! \return Calls previousPath().
    Path changedPath() const override;

private:
    const SceneItem::Ptr fItem;
    const Path           fPreviousPath;
};

//! \brief Path added to object scene notification.
/*!

  A path add notification is a path change notification where a new path is
  added to an existing object.  Not all runtimes support these semantics: a
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

    //! \return Returns the item's path.
    Path changedPath() const override;

private:
    const SceneItem::Ptr fItem;
};

//! \brief Path removed from object scene notification.
/*!

  A path remove notification is a path change notification where an existing
  path is removed from an existing object.  Not all runtimes support these
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

    //! \return Calls removedPath().
    Path changedPath() const override;

private:
    const Path fRemovedPath;
};

//! \brief Subtree invalidate notification.
/*!
  An invalidate notification is for data model changes where all
  descendants of the root will be invalidated. This could mean the
  entire hierarchy has been removed and/or an entirely new hierarchy attached.

  The root item contained in this notification is valid, but all its
  descendants are stale (have been invalidated).

  Synchronously within this notification you cannot query for the
  replacement of the invalidated hierarchy because it might not be
  there yet.
*/
class UFE_SDK_DECL SubtreeInvalidate : public SceneChanged
{
public:
    //! Constructor.
    SubtreeInvalidate(const SceneItem::Ptr& item);

    //! Default copy constructor.
    SubtreeInvalidate(const SubtreeInvalidate&) = default;

    //! Destructor.
    ~SubtreeInvalidate() override;

    //! \return The root SceneItem of the invalidated subtree.
    SceneItem::Ptr root() const;

    //! \return Returns the path of the invalidated subtree root.
    Path changedPath() const override;

private:
    const SceneItem::Ptr fRoot;
};


//! \brief SceneChanged composite notification.
/*!

  This class collects multiple scene change operations inside a single
  notification. The scene change operations are stored as a list, in the
  same order as they were performed.
*/

class UFE_SDK_DECL SceneCompositeNotification : public SceneChanged
{
public:
    struct UFE_SDK_DECL Op {
        Op(OpType opTypeArg, SubOpStorageType subOpTypeArg)
          : opType(opTypeArg), subOpType(subOpTypeArg){}

        Op(OpType opTypeArg, const SceneItem::Ptr& itemArg)
          : opType(opTypeArg), item(itemArg), path(itemArg->path()) {}

        Op(OpType opTypeArg, SubOpStorageType subOpTypeArg, const SceneItem::Ptr& itemArg)
          : opType(opTypeArg), subOpType(subOpTypeArg), item(itemArg), path(itemArg->path()) {}

        Op(OpType opTypeArg, const Path& pathArg)
          : opType(opTypeArg), path(pathArg){}

        Op(OpType opTypeArg, SubOpStorageType subOpTypeArg, const Path& pathArg)
          : opType(opTypeArg), subOpType(subOpTypeArg), path(pathArg){}

        Op(OpType opTypeArg, SubOpStorageType subOpTypeArg, const SceneItem::Ptr& itemArg, const Path& pathArg)
            : opType(opTypeArg), subOpType(subOpTypeArg), item(itemArg), path(pathArg){}

        Op(const SceneChanged& sceneChanged);

        OpType opType;
        SubOpStorageType subOpType;
        SceneItem::Ptr item;
        Path path;
    };

    typedef std::list<Op> Ops;

    //! Constructor.
    SceneCompositeNotification();

    //! Default copy constructor
    SceneCompositeNotification(const SceneCompositeNotification&) = default;

    //! Destructor.
    ~SceneCompositeNotification() override;

    //! \return Returns an empty path.  Each op should be queried for its
    //! changed path.
    Path changedPath() const override;

    void appendSceneChanged(const SceneChanged& sceneChanged);

    void appendOp(const Op& op);

    //! \return copy of operation's list
    inline Ops opsList() const { return fOps; }

    //! \return Size of operations in the composite notification
    std::size_t size() const;

    //! \return True if size() == 0.
    bool empty() const;

    //@{
    //! Iteration interface on operations.
    Ops::const_iterator cbegin() const;
    Ops::const_iterator begin() const;
    Ops::const_iterator cend() const;
    Ops::const_iterator end() const;
    Ops::iterator       begin();
    Ops::iterator       end();
    //@}

private:
    Ops fOps;
};

}

#endif /* _ufeSceneNotification */
