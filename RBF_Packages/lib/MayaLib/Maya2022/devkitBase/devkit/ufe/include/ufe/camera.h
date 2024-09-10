#ifndef _camera
#define _camera
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
#include "cameraUndoableCommands.h"
#include "types.h"

#include <memory>
#include <array>

UFE_NS_DEF
{

    //! \brief Abstract base class for camera interface.
    /*!

  This base class defines the interface that runtimes can implement for
  cameras.

  To determine the transform of a camera attach a transform3d interface to
  the scene item, or to the first ancestor scene item which is a Transform3d.

  Each operation has two calls, one with undo capability, and one without.
  Providing undo capability typically involves code complexity, and using undo
  capability incurs runtime cost in processing and memory.  Therefore, non-
  interactive use of this interface should use calls without undo capability.

  As with the rest of UFE, observation is done through the [Observer
  Pattern](https://en.wikipedia.org/wiki/Observer_pattern).  Observers can be
  added to an individual scene item, to be notified when an object's
  transformation changes.

  To avoid the memory-consuming "one proxy object per scene object" approach,
  Camera interface objects should be obtained and used within a local
  scope, and not stored.  Camera interfaces should be considered
  stateless, and can be bound to new selection items.
*/

    class UFE_SDK_DECL Camera
    {
    public:
        typedef std::shared_ptr<Camera> Ptr;

        enum Projection
        {
            Perspective,
            Orthographic
        };

        /*!
            Convenience method that calls the \ref Ufe::CameraHandler::camera()
            method on the Camera handler for the item.  Returns a null pointer
            if the argument is null, is an empty path or if the item does not
            support the Camera interface.
            \param item SceneItem's Camera to retrieve
            \return Camera of the given SceneItem
        */
        static Ptr camera(const SceneItem::Ptr &item);

        /*!
            Add observation on the argument item for Camera changes.
            \param item SceneItem to observe.
            \param obs Observer to add.
            \return True if the observer is added. Add does nothing and returns 
            false if the observer is already present.
        */
        static bool addObserver(
            const SceneItem::Ptr &item, const Observer::Ptr &obs);
        /*!
            Remove observation on the argument item for transform changes.
            \param item SceneItem to remove observation on.
            \param obs Observer to remove.
            \return True if the observer is removed. False if the observer isn't
            found.
        */
        static bool removeObserver(
            const SceneItem::Ptr &item, const Observer::Ptr &obs);

        /*!
            Number of observers on the given SceneItem.
            \param item SceneItem for which to count observers.
            \return Number of observers on SceneItem.
        */
        static std::size_t nbObservers(const SceneItem::Ptr &item);

        /*!
            Query observation on argument item for camera changes.
            \param item SceneItem to check if has observation.
            \param obs Observer to query.
            \return True if there is observation on argument item
            for camera changes.
        */
        static bool hasObserver(
            const SceneItem::Ptr &item, const Observer::Ptr &obs);

        //! \param path Path to verify if being observed.
        //! \return True if the given path is being observed.
        static bool hasObservers(const Path &path);

        //! Helper query for runtimes, to determine if any path they are
        //! responsible for is being observed.
        //! \param runTimeId runtime Id to find observers on.
        //! \return True if any path of given runtime Id are being observed.
        static bool hasObservers(Rtid runTimeId);

        //! Notify all observers of the item with this path.  If no observer exists,
        //! does nothing.
        //! \param path Path to use on the notify action.
        static void notify(const Path &path);

        //! Constructor.
        Camera();
        //! Default copy constructor.
        Camera(const Camera &) = default;
        //! Destructor.
        virtual ~Camera();

        //! \return the object's Path.
        virtual const Path &path() const = 0;

        //! \return the object's SceneItem.
        virtual SceneItem::Ptr sceneItem() const = 0;

        //! Create an undoable command to set the horizontal aperture.
        //! The command is not executed.
        //! \param ha the new horizontal aperture in inches.
        //! \return Undoable command whose undo restores the camera's horizontal aperture
        virtual HorizontalApertureUndoableCommand::Ptr horizontalApertureCmd(
            float ha) = 0;

        //! Set the horizontal aperture.  Default implementation calls
        //! horizontalApertureCmd() and executes the returned command, if non-null.
        //! \param ha the new horizontal aperture in inches.
        virtual void horizontalAperture(float ha)
        {
            auto cmd = horizontalApertureCmd(ha);
            if (cmd)
            {
                cmd->execute();
            }
        }

        //! Retrieve the horizontal aperture for the camera
        //! \return the horizontal aperture in inches
        virtual float horizontalAperture() const = 0;

        //! Create an undoable command to set the vertical aperture.
        //! The command is not executed.
        //! \param va the new vertical aperture in inches.
        //! \return Undoable command whose undo restores the camera's vertical aperture
        virtual VerticalApertureUndoableCommand::Ptr verticalApertureCmd(
            float va) = 0;

        //! Set the vertical aperture.  Default implementation calls
        //! verticalApertureCmd() and executes the returned command, if non-null.
        //! \param va the new vertical aperture in inches.
        virtual void verticalAperture(float va)
        {
            auto cmd = verticalApertureCmd(va);
            if (cmd)
            {
                cmd->execute();
            }
        }

        //! Retrieve the vertical aperture for the camera
        //! \return the vertical aperture in inches
        virtual float verticalAperture() const = 0;

        //! Create an undoable command to set the horizontal aperture offset.
        //! The command is not executed.
        //! \param hao the new horizontal aperture offset in inches.
        //! \return Undoable command whose undo restores the camera's horizontal aperture offset
        virtual HorizontalApertureOffsetUndoableCommand::Ptr horizontalApertureOffsetCmd(
            float hao) = 0;

        //! Set the horizontal aperture offset.  Default implementation calls
        //! horizontalApertureOffsetCmd() and executes the returned command, if non-null.
        //! \param hao the new horizontal aperture offset in inches.
        virtual void horizontalApertureOffset(float hao)
        {
            auto cmd = horizontalApertureOffsetCmd(hao);
            if (cmd)
            {
                cmd->execute();
            }
        }

        //! Retrieve the horizontal aperture offset for the camera
        //! \return the horizontal aperture offset in inches
        virtual float horizontalApertureOffset() const = 0;

        //! Create an undoable command to set the vertical aperture offset.
        //! The command is not executed.
        //! \param vao the new vertical aperture offset in inches.
        //! \return Undoable command whose undo restores the camera's vertical aperture offset
        virtual VerticalApertureOffsetUndoableCommand::Ptr verticalApertureOffsetCmd(
            float vao) = 0;

        //! Set the vertical aperture offset.  Default implementation calls
        //! verticalApertureOffsetCmd() and executes the returned command, if non-null.
        //! \param vao the new vertical aperture offset in inches.
        virtual void verticalApertureOffset(float vao)
        {
            auto cmd = verticalApertureOffsetCmd(vao);
            if (cmd)
            {
                cmd->execute();
            }
        }

        //! Retrieve the vertical aperture offset for the camera
        //! \return the vertical aperture offset in inches
        virtual float verticalApertureOffset() const = 0;

        //! Create an undoable command to set the fStop.
        //! The command is not executed.
        //! \param fs the new fStop in millimeters.
        //! \return Undoable command whose undo restores the camera's fStop
        virtual FStopUndoableCommand::Ptr fStopCmd(
            float fs) = 0;

        //! Set the fStop.  Default implementation calls
        //! fStopCmd() and executes the returned command, if non-null.
        //! \param fs the new fStop in millimeters.
        virtual void fStop(float fs)
        {
            auto cmd = fStopCmd(fs);
            if (cmd)
            {
                cmd->execute();
            }
        }

        //! Retrieve the fStop for the camera
        //! \return the fStop in millimeters
        virtual float fStop() const = 0;

        //! Create an undoable command to set the focal length.
        //! The command is not executed.
        //! \param fl the new focal length in millimeters.
        //! \return Undoable command whose undo restores the camera's focal length
        virtual FocalLengthUndoableCommand::Ptr focalLengthCmd(
            float fl) = 0;

        //! Set the focal length.  Default implementation calls
        //! focalLengthCmd() and executes the returned command, if non-null.
        //! \param fl the new focal length in millimeters.
        virtual void focalLength(float fl)
        {
            auto cmd = focalLengthCmd(fl);
            if (cmd)
            {
                cmd->execute();
            }
        }

        //! Retrieve the focal length for the camera
        //! \return the focal length in millimeters
        virtual float focalLength() const = 0;

        //! Create an undoable command to set the focus distance.
        //! The command is not executed.
        //! \param fd the new focus distance in millimeters.
        //! \return Undoable command whose undo restores the camera's focus distance
        virtual FocusDistanceUndoableCommand::Ptr focusDistanceCmd(
            float fd) = 0;

        //! Set the focus distance.  Default implementation calls
        //! focusDistanceCmd() and executes the returned command, if non-null.
        //! \param fd the new focus distance in millimeters.
        virtual void focusDistance(float fd)
        {
            auto cmd = focusDistanceCmd(fd);
            if (cmd)
            {
                cmd->execute();
            }
        }

        //! Retrieve the focus distance for the camera
        //! \return the focus distance in millimeters
        virtual float focusDistance() const = 0;

        //! Create an undoable command to set the near clip plane.
        //! The command is not executed.
        //! \param ncp the new near clip plane.
        //! \return Undoable command whose undo restores the camera's near clip plane
        virtual NearClipPlaneUndoableCommand::Ptr nearClipPlaneCmd(
            float ncp) = 0;

        //! Set the near clip plane.  Default implementation calls
        //! nearClipPlaneCmd() and executes the returned command, if non-null.
        //! \param ncp the new near clip plane.
        virtual void nearClipPlane(float ncp)
        {
            auto cmd = nearClipPlaneCmd(ncp);
            if (cmd)
            {
                cmd->execute();
            }
        }

        //! Retrieve the near clip plane for the camera
        //! \return the near clip plane.
        virtual float nearClipPlane() const = 0;

        //! Create an undoable command to set the far clip plane.
        //! The command is not executed.
        //! \param fcp the new far clip plane.
        //! \return Undoable command whose undo restores the camera's far clip plane
        virtual FarClipPlaneUndoableCommand::Ptr farClipPlaneCmd(
            float fcp) = 0;

        //! Set the far clip plane.  Default implementation calls
        //! farClipPlaneCmd() and executes the returned command, if non-null.
        //! \param fcp the new far clip plane.
        virtual void farClipPlane(float fcp)
        {
            auto cmd = farClipPlaneCmd(fcp);
            if (cmd)
            {
                cmd->execute();
            }
        }

        //! Retrieve the far clip plane for the camera
        //! \return the far clip plane.
        virtual float farClipPlane() const = 0;

        //! Create an undoable command to set the projection.
        //! The command is not executed.
        //! \param p the new projection.
        //! \return Undoable command whose undo restores the camera's projection
        virtual ProjectionUndoableCommand::Ptr projectionCmd(
            Projection p) = 0;

        //! Set the projection.  Default implementation calls
        //! projectionCmd() and executes the returned command, if non-null.
        //! \param p the new projection.
        virtual void projection(Projection p)
        {
            auto cmd = projectionCmd(p);
            if (cmd)
            {
                cmd->execute();
            }
        }

        //! Retrieve the projection for the camera
        //! \return the projection
        virtual Projection projection() const = 0;
    };
}

#endif /* _camera */
