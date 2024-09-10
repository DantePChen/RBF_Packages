#ifndef _transform3d
#define _transform3d
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "common/ufeExport.h"
#include "sceneItem.h"
#include "observer.h"
#include "transform3dUndoableCommands.h"

#include <memory>
#include <array>

UFE_NS_DEF {

//! \brief Matrix class for 3D transforms.
/*!
    Matrix class to allow matrix operations across different run-times.
*/
struct UFE_SDK_DECL Matrix4d {
    std::array<std::array<double, 4>, 4> matrix;

    //! Star operator overload to multiply matrices.
    Matrix4d operator*(const Matrix4d& rhs) const;
};

//! \brief 3D vector class.
struct UFE_SDK_DECL Vector3d {
    std::array<double, 3> vector;

    Vector3d(double x, double y, double z);

    double x() const;
    double y() const;
    double z() const;
};

//! \brief Abstract base class for 3D transform interface.
/*!

  This base class defines the interface that run-times can implement to
  transform objects in 3D, for a given selection item.  These include
  operations like:
  - translating an item
  - rotating an item
  - scaling an item
  - translating an item's rotate pivot point
  - translating an item's scale pivot point
  - returning the inclusive matrix of the item (including the item's local
    transformation)
  - returning the exclusive matrix of the item (excluding the item's local
    transformation)
   
  Each operation has two calls, one with undo capability, and one without.
  Providing undo capability typically involves code complexity, and using undo
  capability incurs run-time cost in processing and memory.  Therefore, non-
  interactive use of this interface should use calls without undo capability.

  All operations on the item are done in its local coordinate system, and are
  absolute.  It is the responsibility of the application to convert operations
  done in any other coordinate system (e.g. world, or another object's
  coordinate system) back into a UFE item's local coordinates.  Similarly,
  the application must convert relative transformations back into absolute for
  the UFE item.

  As with the rest of UFE, observation is done through the [Observer
  Pattern](https://en.wikipedia.org/wiki/Observer_pattern).  Observers can be
  added to an individual scene item, to be notified when an object's
  transformation changes.

  To avoid the memory-consuming "one proxy object per scene object" approach,
  Transform3d interface objects should be obtained and used within a local
  scope, and not stored.  Transform3d interfaces should be considered
  stateless, and can be bound to new selection items.
*/

class UFE_SDK_DECL Transform3d
{
public:
    typedef std::shared_ptr<Transform3d> Ptr;
    
    /*!
        Convenience method that calls the Transform3d method on the Transform3d
        handler for the item.  Returns a null pointer if the argument is null,
        or has an empty path.
        \param item SceneItem's Transform3d to retrieve
        \return Transform3d of the given SceneItem
    */
    static Ptr transform3d(const SceneItem::Ptr& item);

    /*!
        Add observation on the argument item for Transform3d changes.
        \param item SceneItem to observe.
        \param obs Observer to add.
        \return True if the observer is added. Add does nothing and returns 
         false if the observer is already present.
    */
    static bool addObserver(
        const SceneItem::Ptr& item, const Observer::Ptr& obs);
    /*!
        Remove observation on the argument item for transform changes.
        \param item SceneItem to remove observation on.
        \param obs Observer to remove.
        \return True if the observer is removed. False if the observer isn't
        found.
    */
    static bool removeObserver(
        const SceneItem::Ptr& item, const Observer::Ptr& obs);

    /*!
        Number of observed on the given SceneItem.
        \param item SceneItem to count observes on.
        \return Number of observers on SceneItem.
    */
    static std::size_t nbObservers(const SceneItem::Ptr& item);

    /*!
        Query observation on argument item for transform changes.
        \param item SceneItem to check if has observation.
        \param obs Observer to query.
        \return True if there is observation on argument item
        for transform changes.
    */
    static bool hasObserver(
        const SceneItem::Ptr& item, const Observer::Ptr& obs);

    //! \param path Path to verify if being observed.
    //! \return True if the given path is being observed.
    static bool hasObservers(const Path& path);

    //! Helper query for run-times, to determine if any path they are
    //! responsible for is being observed.
    //! \param runTimeId run-time Id to find observers on.
    //! \return True if any path of given run-time Id are being observed.
    static bool hasObservers(Rtid runTimeId);

    //! Notify all observers of the item with this path.  If no observer exists,
    //! does nothing.
    //! \param path Path to use on the notify action.
    static void notify(const Path& path);

    //! Constructor.
    Transform3d();
    //! Default copy constructor.
    Transform3d(const Transform3d&) = default;
    //! Destructor.
    virtual ~Transform3d();

    //! \return the object's Path.
    virtual const Path& path() const = 0;

    //! \return the object's SceneItem.
    virtual SceneItem::Ptr sceneItem() const = 0;

    //! Create an undoable translate command
    //! \return Undoable command whose undo restores the translate's position
    virtual TranslateUndoableCommand::Ptr translateCmd() = 0;

    //! Translate object to the given position.
    //! \param x value to translate to the X-axis.
    //! \param y value to translate to the Y-axis.
    //! \param z value to translate to the Z-axis.
    virtual void translate(double x, double y, double z) = 0;

    //! Retrieve the translate value set for the object.
    //! \return The object's translate vector.
    virtual Vector3d translation() const = 0;

    //! Create an undoable rotate command
    //! \return Undoable command whose undo restores the rotation's position
    virtual RotateUndoableCommand::Ptr rotateCmd() = 0;

    //! Rotate object to given value.
    //! \param x value to rotate on the X-axis.
    //! \param y value to rotate on the Y-axis.
    //! \param z value to rotate on the Z-axis.
    virtual void rotate(double x, double y, double z) = 0;

    //! Create an undoable scale command
    //! \return Undoable command whose undo restores the scale's value
    virtual ScaleUndoableCommand::Ptr scaleCmd() = 0;

    //! Scale object to given values.
    //! \param x value to scale on the X-axis.
    //! \param y value to scale on the Y-axis.
    //! \param z value to scale on the Z-axis.
    virtual void scale(double x, double y, double z) = 0;

    //! Create an undoable command to translate the rotate pivot point.
    //! \return Undoable command whose undo restores the rotate pivot's position
    virtual TranslateUndoableCommand::Ptr rotatePivotTranslateCmd() = 0;

    //! Translate the rotate pivot point.
    //! \param x value to translate on the X-axis.
    //! \param y value to translate on the Y-axis.
    //! \param z value to translate on the Z-axis.
    virtual void rotatePivotTranslate(double x, double y, double z) = 0;

    //! Retrieve the rotate pivot point for the  object.
    //! \return The rotate pivot point.
    virtual Vector3d rotatePivot() const = 0;

    //! Create an undoable command to translate the scale pivot point.
    //! \return Undoable command whose undo restores the scale pivot's position
    virtual TranslateUndoableCommand::Ptr scalePivotTranslateCmd() = 0;

    //! Translate the scale pivot point.
    //! \param x value to translate on the X-axis.
    //! \param y value to translate on the Y-axis.
    //! \param z value to translate on the Z-axis.
    virtual void scalePivotTranslate(double x, double y, double z) = 0;

    //! Retrieve the scale pivot point for the  object.
    //! \return The scale pivot point.
    virtual Vector3d scalePivot() const = 0;

    //! Retrieve the inclusive Matrix of the object.  This matrix includes
    //! the transformation's inclusive matrix of this object's parent, and the object itself.
    //! \return Inclusive Matrix4d of the SceneItem.
    Matrix4d inclusiveMatrix() const; 

    //! Retrieve the exclusive Matrix of the object.  This matrix includes
    //! the transformation's inclusive matrix of this object's parent, without the object itself.
    //! \return Exclusive Matrix4d of the SceneItem.
    Matrix4d exclusiveMatrix() const;

    //! Retrieve the inclusive Matrix of the object, for all objects in the tail
    //! segment of the object's path, including the object itself.  The segment
    //! inclusive matrices of a path are multiplied together by
    //! inclusiveMatrix(), from head to tail, to obtain the full transformation.
    //! In the case of a trivial single segment path, the segmentInclusiveMatrix
    //! and the inclusiveMatrix are the same.
    //! \return The PathSegment's transform matrix
    virtual Matrix4d segmentInclusiveMatrix() const = 0; 

    //! Retrieve the exclusive Matrix of the object, for all objects in the tail
    //! segment of the object's path, excluding the object itself. The tail segment
    //! exclusive matrix and inclusive matrices of other segments of a path are 
    //! multiplied together by exclusiveMatrix(), from head to tail, to obtain the 
    //! full transformation. In the case of a trivial single segment path, the 
    //! segmentExclusiveMatrix and the exclusiveMatrix are the same.
    //! \return The PathSegment's exclusive transform matrix
    virtual Matrix4d segmentExclusiveMatrix() const = 0;
};

}

#endif /* _transform3d */
