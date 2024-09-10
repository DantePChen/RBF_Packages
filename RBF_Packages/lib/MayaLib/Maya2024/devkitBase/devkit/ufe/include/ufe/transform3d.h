#ifndef _transform3d
#define _transform3d
// ===========================================================================
// Copyright 2020 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "common/ufeExport.h"
#include "sceneItem.h"
#include "observer.h"
#include "transform3dUndoableCommands.h"
#include "types.h"

#include <memory>
#include <array>

UFE_NS_DEF {

//! \brief Hint class for Transform3d editTransform3d.
/*!
  This class defines an extensible hint to transfer information into the 
  \ref Ufe::Transform3d::editTransform3d() method, so that the caller
  can provide information about the context in which the Transform3d
  interface is being created.
*/
class UFE_SDK_DECL EditTransform3dHint
{
public:

    // The None enumerant is deprecated, and may be removed in a future
    // major version of UFE.
    enum Type {NoHint, None = NoHint, Translate, Rotate, Scale, RotatePivot, ScalePivot};
    EditTransform3dHint() = default;
    EditTransform3dHint(const EditTransform3dHint&) = delete;
    EditTransform3dHint& operator=(const EditTransform3dHint&) = delete;
    EditTransform3dHint(Type);
    virtual ~EditTransform3dHint();

    inline Type type() const { return fType; }

private:
    
    const Type fType{None};
};

//! \brief Read-only abstract base class for 3D transform interface.

class UFE_SDK_DECL Transform3dRead
{
public:
    typedef std::shared_ptr<Transform3dRead> Ptr;
    
    /*!
        Convenience method that calls the \ref Ufe::Transform3dHandler::transform3dRead()
        method on the Transform3d handler for the item.  Returns a null
        pointer if the argument is null, or has an empty path.
        \param item SceneItem on which to create Transform3dRead interface.
        \return Transform3dRead for the given SceneItem
    */
    static Ptr transform3dRead(const SceneItem::Ptr& item);

    //! Constructor.
    Transform3dRead();

    //! Default copy constructor.
    Transform3dRead(const Transform3dRead&) = default;

    //! Destructor.
    virtual ~Transform3dRead();

    //! \return the object's Path.
    virtual const Path& path() const = 0;

    //! \return the object's SceneItem.
    virtual SceneItem::Ptr sceneItem() const = 0;

    //! Retrieve the object's local transform matrix.
    //! \return The object's transform matrix.
    virtual Matrix4d matrix() const = 0;

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

//! \brief Abstract base class for 3D transform interface.
/*!

  This base class defines the interface that runtimes can implement to
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
  capability incurs runtime cost in processing and memory.  Therefore, non-
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

class UFE_SDK_DECL Transform3d : public Transform3dRead
{
public:
    typedef std::shared_ptr<Transform3d> Ptr;
    
    /*!
        Convenience method that calls the \ref Ufe::Transform3dHandler::transform3d()
        method on the Transform3d handler for the item.  Returns a null pointer
        if the argument is null, or has an empty path.
        \param item SceneItem on which to create Transform3d interface.
        \return Transform3d for the given SceneItem
    */
    static Ptr transform3d(const SceneItem::Ptr& item);

    /*!
        Convenience method that calls the
        \ref Ufe::Transform3d::editTransform3d() method on the Transform3d
        handler for the item.  Returns a null pointer if the argument is null,
        or has an empty path.
        \param item SceneItem on which to create Transform3d interface.
        \param hint Contextual information for Transform3d interface creation.
        \return Transform3d for the given SceneItem
    */
    static Ptr editTransform3d(
        const SceneItem::Ptr&      item,
        const EditTransform3dHint& hint = EditTransform3dHint()
    );

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

    typedef bool (*RemoveObserverItemType)(const SceneItem::Ptr& item, const Observer::Ptr& obs);

    /*!
        Remove observation on the argument path for transform changes. This
        method can remove observation from a Path when the corresponding SceneItem
        is already invalid.
        \param path path to remove observation on.
        \param obs Observer to remove.
        \return True if the observer is removed. False if the observer isn't
        found.
    */
    static bool removeObserver(
        const Path& path, const Observer::Ptr& obs);

    typedef bool (*RemoveObserverPathType)(const Path& path, const Observer::Ptr& obs);

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

    //! Helper query for runtimes, to determine if any path they are
    //! responsible for is being observed.
    //! \param runTimeId runtime Id to find observers on.
    //! \return True if any path of given runtime Id are being observed.
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
    virtual ~Transform3d() override;

    //! Create an undoable translate command.  The command is not executed.
    //! \param x X-axis translation.
    //! \param y Y-axis translation.
    //! \param z Z-axis translation.
    //! \return Undoable command whose undo restores the original translation.
    virtual TranslateUndoableCommand::Ptr translateCmd(
        double x, double y, double z) = 0;

    inline TranslateUndoableCommand::Ptr translateCmd() {
        return translateCmd(0, 0, 0); }

    //! Translate object to the given position.  Default implementation calls
    //! translateCmd() and executes the returned command, if non-null.
    //! \param x value to translate to the X-axis.
    //! \param y value to translate to the Y-axis.
    //! \param z value to translate to the Z-axis.
    virtual void translate(double x, double y, double z) {
        auto cmd = translateCmd(x, y, z);
        if (cmd) {
            cmd->execute();
        }
    }

    //! Retrieve the translate value set for the object.
    //! \return The object's translate vector.
    virtual Vector3d translation() const = 0;

    //! Create an undoable rotate command.  Euler angle rotation order is XYZ.
    //! The command is not executed.
    //! \param x value to rotate on the X-axis, in degrees.
    //! \param y value to rotate on the Y-axis, in degrees.
    //! \param z value to rotate on the Z-axis, in degrees.
    //! \return Undoable command whose undo restores the original rotation.
    virtual RotateUndoableCommand::Ptr rotateCmd(
        double x, double y, double z) = 0;

    inline RotateUndoableCommand::Ptr rotateCmd() {
        return rotateCmd(0, 0, 0); }

    //! Rotate object to given value.  Default implementation calls
    //! rotateCmd() and executes the returned command, if non-null
    //! \param x value to rotate on the X-axis, in degrees.
    //! \param y value to rotate on the Y-axis, in degrees.
    //! \param z value to rotate on the Z-axis, in degrees.
    virtual void rotate(double x, double y, double z) {
        auto cmd = rotateCmd(x, y, z);
        if (cmd) {
            cmd->execute();
        }
    }

    //! Retrieve the rotation value set for the object, in degrees.  Euler 
    //! angle rotation order is XYZ.
    //! \return The object's rotation, in degrees.
    virtual Vector3d rotation() const = 0;

    //! Create an undoable scale command.  The command is not executed.
    //! \param x value to scale on the X-axis.
    //! \param y value to scale on the Y-axis.
    //! \param z value to scale on the Z-axis.
    //! \return Undoable command whose undo restores the scale's value
    virtual ScaleUndoableCommand::Ptr scaleCmd(
        double x, double y, double z) = 0;

    inline ScaleUndoableCommand::Ptr scaleCmd() {
        return scaleCmd(1, 1, 1); }

    //! Scale object to given values.  Default implementation calls
    //! scaleCmd() and executes the returned command, if non-null.
    //! \param x value to scale on the X-axis.
    //! \param y value to scale on the Y-axis.
    //! \param z value to scale on the Z-axis.
    virtual void scale(double x, double y, double z) {
        auto cmd = scaleCmd(x, y, z);
        if (cmd) {
        cmd->execute();
        }
    }

    //! Retrieve the scale value set for the object.
    //! \return The object's scale.
    virtual Vector3d scale() const = 0;

    //! Create an undoable command to translate the rotate pivot point.  The
    //! command is not executed.
    //! \param x X-axis translation.
    //! \param y Y-axis translation.
    //! \param z Z-axis translation.
    //! \return Undoable command whose undo restores the rotate pivot's position
    virtual TranslateUndoableCommand::Ptr rotatePivotCmd(
        double x, double y, double z) = 0;

    inline TranslateUndoableCommand::Ptr rotatePivotCmd() {
        return rotatePivotCmd(0, 0, 0); }

    //! Translate the rotate pivot point.  Default implementation calls
    //! rotatePivotCmd() and executes the returned command, if non-null.
    //! \param x value to translate on the X-axis.
    //! \param y value to translate on the Y-axis.
    //! \param z value to translate on the Z-axis.
    virtual void rotatePivot(double x, double y, double z) {
        auto cmd = rotatePivotCmd(x, y, z);
        if (cmd) {
            cmd->execute();
        }
    }

    //! Retrieve the rotate pivot point for the object.
    //! \return The rotate pivot point.
    virtual Vector3d rotatePivot() const = 0;

    //! Create an undoable command to translate the scale pivot point.  The
    //! command is not executed.
    //! \param x X-axis translation.
    //! \param y Y-axis translation.
    //! \param z Z-axis translation.
    //! \return Undoable command whose undo restores the scale pivot's position
    virtual TranslateUndoableCommand::Ptr scalePivotCmd(
        double x, double y, double z) = 0;

    inline TranslateUndoableCommand::Ptr scalePivotCmd() {
      return scalePivotCmd(0, 0, 0); }

    //! Translate the scale pivot point.  Default implementation calls
    //! scalePivotCmd() and executes the returned command, if non-null.
    //! \param x value to translate on the X-axis.
    //! \param y value to translate on the Y-axis.
    //! \param z value to translate on the Z-axis.
    virtual void scalePivot(double x, double y, double z)  {
        auto cmd = scalePivotCmd(x, y, z);
        if (cmd) {
            cmd->execute();
        }
    }

    //! Retrieve the scale pivot point for the object.
    //! \return The scale pivot point.
    virtual Vector3d scalePivot() const = 0;

    //! Create an undoable command to change the rotate pivot correction.  This
    //! is used when it is desired to move the rotate pivot point without
    //! affecting the overall transformation matrix.  The command is not
    //! executed.  The default implementation in this class is to return a null
    //! pointer.
    //! \param x X-axis translation.
    //! \param y Y-axis translation.
    //! \param z Z-axis translation.
    //! \return Undoable command whose undo restores the rotate pivot correction
    virtual TranslateUndoableCommand::Ptr translateRotatePivotCmd(
        double x, double y, double z) {
        (void) x; (void) y; (void) z;
        return nullptr;
    }

    inline TranslateUndoableCommand::Ptr translateRotatePivotCmd() {
      return translateRotatePivotCmd(0, 0, 0); }

    //! Change the rotate pivot correction.  Default implementation calls
    //! translateRotatePivotCmd() and executes the returned command, if
    //! non-null.
    //! \param x value to translate on the X-axis.
    //! \param y value to translate on the Y-axis.
    //! \param z value to translate on the Z-axis.
    virtual void translateRotatePivot(double x, double y, double z)  {
        auto cmd = translateRotatePivotCmd(x, y, z);
        if (cmd) {
            cmd->execute();
        }
    }

    //! Retrieve the rotate pivot correction for the object.  The default
    //! implementation in this class is to return a zero vector.
    //! \return The rotate pivot correction.
    virtual Vector3d rotatePivotTranslation() const {
        return Vector3d(0, 0, 0);
    }

    //! Create an undoable command to change the scale pivot correction.  This
    //! is used when it is desired to move the scale pivot point without
    //! affecting the overall transformation matrix.  The command is not
    //! executed.  The default implementation in this class is to return a null
    //! pointer.
    //! \param x X-axis translation.
    //! \param y Y-axis translation.
    //! \param z Z-axis translation.
    //! \return Undoable command whose undo restores the scale pivot correction
    virtual TranslateUndoableCommand::Ptr translateScalePivotCmd(
        double x, double y, double z) {
        (void) x; (void) y; (void) z;
        return nullptr;
    }

    inline TranslateUndoableCommand::Ptr translateScalePivotCmd() {
      return translateScalePivotCmd(0, 0, 0); }

    //! Change the scale pivot correction.  Default implementation calls
    //! translateScalePivotCmd() and executes the returned command, if non-null.
    //! \param x value to translate on the X-axis.
    //! \param y value to translate on the Y-axis.
    //! \param z value to translate on the Z-axis.
    virtual void translateScalePivot(double x, double y, double z)  {
        auto cmd = translateScalePivotCmd(x, y, z);
        if (cmd) {
            cmd->execute();
        }
    }

    //! Retrieve the scale pivot correction for the object.  The default
    //! implementation in this class is to return a zero vector.
    //! \return The scale pivot correction.
    virtual Vector3d scalePivotTranslation() const {
        return Vector3d(0, 0, 0);
    }

    //! Create an undoable command to set the rotate axis, an extra rotation to
    //! adjust the local axis prior to applying the rotate attribute.  Euler
    //! angle rotation order is XYZ.  The command is not executed.  The default
    //! implementation in this class is to return a null pointer.
    //! \param x value to rotate on the X-axis, in degrees.
    //! \param y value to rotate on the Y-axis, in degrees.
    //! \param z value to rotate on the Z-axis, in degrees.
    //! \return Undoable command whose undo restores the original rotation.
    virtual RotateUndoableCommand::Ptr rotateAxisCmd(
        double x, double y, double z) {
        (void) x; (void) y; (void) z;
        return nullptr;
    }

    inline RotateUndoableCommand::Ptr rotateAxisCmd() {
        return rotateAxisCmd(0, 0, 0); }

    //! Set the rotate axis to given value.  Default implementation calls
    //! rotateAxisCmd() and executes the returned command, if non-null.
    //! \param x value to rotate on the X-axis, in degrees.
    //! \param y value to rotate on the Y-axis, in degrees.
    //! \param z value to rotate on the Z-axis, in degrees.
    virtual void rotateAxis(double x, double y, double z) {
        auto cmd = rotateAxisCmd(x, y, z);
        if (cmd) {
            cmd->execute();
        }
    }

    //! Retrieve the Euler angle XYZ rotation axis, in degrees.
    //! \return The object's rotate axis, in degrees.
    virtual Vector3d rotateAxis() const {
        return Vector3d(0, 0, 0);
    }

    //! Create an undoable command to change the shearing in (xy, xz, yz).  The
    //! command is not executed.  The default implementation in this class is
    //! to return a null pointer.
    //! \param xy xy shearing.
    //! \param xz xz shearing.
    //! \param yz yz shearing.
    //! \return Undoable command whose undo restores the original shearing.
    virtual ShearUndoableCommand::Ptr shearCmd(
        double xy, double xz, double yz) {
        (void) xy; (void) xz; (void) yz;
        return nullptr;
    }

    inline ShearUndoableCommand::Ptr shearCmd() {
      return shearCmd(0, 0, 0); }

    //! Change the shear.  Default implementation calls shearCmd() and executes
    //! the returned command, if non-null.
    //! \param xy xy shearing.
    //! \param xz xz shearing.
    //! \param yz yz shearing.
    virtual void shear(double xy, double xz, double yz)  {
        auto cmd = shearCmd(xy, xz, yz);
        if (cmd) {
            cmd->execute();
        }
    }

    //! Retrieve the shear for the object.  The default implementation in this
    //! class is to return a zero vector.
    //! \return The (xy, xz, yz) shear.
    virtual Vector3d shear() const {
        return Vector3d(0, 0, 0);
    }

    //! Create an undoable command to set the object's local transform to the
    //! argument matrix.  The command is not executed.
    //! \param m Complete local transformation for the object.
    //! \return Undoable command whose undo restores the original matrix.
    virtual SetMatrix4dUndoableCommand::Ptr setMatrixCmd(const Matrix4d& m) = 0;

    //! Set the object's local transform to the argument matrix.
    //! Implementation in this class executes the command returned by
    //! setMatrixCmd(), if non-null.
    //! \param m Complete local transformation for the object.
    virtual void setMatrix(const Matrix4d& m) {
        auto cmd = setMatrixCmd(m);
        if (cmd) {
            cmd->execute();
        }
    }
};

}

#endif /* _transform3d */
