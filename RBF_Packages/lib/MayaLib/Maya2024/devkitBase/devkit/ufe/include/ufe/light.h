#ifndef _light
#define _light
// ===========================================================================
// Copyright 2022 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "common/ufeExport.h"
#include "sceneItem.h"
#include "observer.h"
#include "baseUndoableCommands.h"
#include "types.h"

#include <memory>
#include <array>

UFE_NS_DEF
{

    //! \brief Abstract base class for light interface.
    /*!

  This base class defines the interface that runtimes can implement for
  lights.

  The Light interface provides members that are common to all light types.  To
  access interfaces that are appropriate for different light types, the Light
  interface provides accessors to sub-interfaces:
  - DirectionalInterface
  - SphereInterface
  - ConeInterface
  - AreaInterface

  For example, the SphereInterface is available for Light::Spot and
  Light::Point types.

  To determine the transform of a light, attach a transform3d interface to
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
  Light interface objects should be obtained and used within a local
  scope, and not stored.  Light interfaces should be considered
  stateless, and can be bound to new selection items.
*/

    class UFE_SDK_DECL Light
    {
    public:
        typedef std::shared_ptr<Light> Ptr;

        enum Type
        {
            Invalid,
            Directional,
            Point,
            Spot,
            Area
        };

        struct SphereProps
        {
            float radius = 0;
            bool asPoint = true;
        };

        struct ConeProps
        {
            // Higher focus values pull light towards the center and narrow the spread
            float focus = 0;
            // Angular limit off the primary axis to restrict the light spread
            float angle = 40;
            // Controls the cutoff softness for cone angle
            float softness = 0;
        };

        using IntensityUndoableCommand = SetFloatUndoableCommand;
        using ColorUndoableCommand = SetColor3fUndoableCommand;
        using ShadowEnableUndoableCommand = SetBoolUndoableCommand;
        using ShadowColorUndoableCommand = SetColor3fUndoableCommand;
        using DiffuseUndoableCommand = SetFloatUndoableCommand;
        using SpecularUndoableCommand = SetFloatUndoableCommand;
        using AngleUndoableCommand = SetFloatUndoableCommand;
        using SpherePropsUndoableCommand = SetValueUndoableCommand<const SphereProps &>;
        using ConePropsUndoableCommand = SetValueUndoableCommand<const ConeProps &>;
        using NormalizeUndoableCommand = SetBoolUndoableCommand;

        /*!
            Convenience method that calls the \ref Ufe::LightHandler::light()
            method on the Light handler for the item. Returns a null pointer
            if the argument is null, is an empty path or if the item does not
            support the Light interface.
            \param item SceneItem's Light to retrieve
            \return Light of the given SceneItem
        */
        static Ptr light(const SceneItem::Ptr &item);

        /*!
            Add observation on the argument item for Light changes.
            \param item SceneItem to observe.
            \param obs Observer to add.
            \return True if the observer is added. Add does nothing and returns 
            false if the observer is already present.
        */
        static bool addObserver(
            const SceneItem::Ptr &item, const Observer::Ptr &obs);
        /*!
            Remove observation on the argument item for Light changes.
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
            Query observation on argument item for light changes.
            \param item SceneItem to check if has observation.
            \param obs Observer to query.
            \return True if there is observation on argument item
            for light changes.
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
        Light();
        //! Default copy constructor.
        Light(const Light &) = default;
        //! Destructor.
        virtual ~Light();

        //! \return the object's Path.
        virtual const Path &path() const = 0;

        //! \return the object's SceneItem.
        virtual SceneItem::Ptr sceneItem() const = 0;

        //! \return the light type
        virtual Type type() const = 0;

        /*************************************************
            Light intensity attribute. 
            Valid for the following light types: [all]
        *************************************************/

        //! Create an undoable command to set the intensity. The command is not executed.
        //! \param li the new intensity value.
        //! \return Undoable command to set the intensity
        virtual IntensityUndoableCommand::Ptr intensityCmd(float li) = 0;

        //! Set the intensity. Default implementation uses IntensityUndoableCommand.
        //! \param li the new intensity value.
        virtual void intensity(float li) {
            if (auto cmd = intensityCmd(li)) {
                cmd->execute();
            }
        }

        //! Retrieve the intensity for the light
        //! \return the intensity value
        virtual float intensity() const = 0;

        /*************************************************
            Light color attribute, defined in energy-linear terms
            Valid for the following light types: [all]
        *************************************************/

        //! Create an undoable command to set the color. The command is not executed.
        //! \param r Red component of the new color value.
        //! \param g Green component of the new color value.
        //! \param b Blue component of the new color value.
        //! \return Undoable command to set the color
        virtual ColorUndoableCommand::Ptr colorCmd(float r, float g, float b) = 0;

        //! Set the color. Default implementation uses ColorUndoableCommand.
        //! \param r red component \param g green component \param b blue component
        virtual void color(float r, float g, float b) {
            if (auto cmd = colorCmd(r, g, b)) {
                cmd->execute();
            }
        }

        //! Retrieve the color for the light
        //! \return the color value
        virtual Color3f color() const = 0;

        /*************************************************
            Light shadow enable attribute.
            Valid for the following light types: [all]
        *************************************************/

        //! Create an undoable command to set the shadow enable flag. The command is not executed.
        //! \param se the new shadow enable flag value.
        //! \return Undoable command to set the shadow enable flag
        virtual ShadowEnableUndoableCommand::Ptr shadowEnableCmd(bool se) = 0;

        //! Set the shadow enable flag. Default implementation uses ShadowEnableUndoableCommand.
        //! \param se the new shadow enable flag value.
        virtual void shadowEnable(bool se) {
            if (auto cmd = shadowEnableCmd(se)) {
                cmd->execute();
            }
        }

        //! Retrieve the shadow enable flag
        //! \return the shadow enable flag value
        virtual bool shadowEnable() const = 0;

        /*************************************************
            Shadow color attribute.
            Valid for the following light types: [all]
        *************************************************/

        //! Create an undoable command to set the shadow color. The command is not executed.
        //! \param r Red component of the new shadow color value.
        //! \param g Green component of the new shadow color value.
        //! \param b Blue component of the new shadow color value.
        //! \return Undoable command to set the shadow color
        virtual ShadowColorUndoableCommand::Ptr shadowColorCmd(float r, float g, float b) = 0;

        //! Set the shadow color. Default implementation uses ShadowColorUndoableCommand.
        //! \param r red component \param g green component \param b blue component
        virtual void shadowColor(float r, float g, float b) {
            if (auto cmd = shadowColorCmd(r, g, b)) {
                cmd->execute();
            }
        }

        //! Retrieve the shadow color for the light
        //! \return the shadow color value
        virtual Color3f shadowColor() const = 0;

        /*************************************************
            Light diffuse attribute, a multiplier for the effect 
            of this light on the diffuse response of materials.
            Valid for the following light types: [all]
        *************************************************/

        //! Create an undoable command to set the diffuse. The command is not executed.
        //! \param ld the new diffuse value.
        //! \return Undoable command to set the diffuse
        virtual DiffuseUndoableCommand::Ptr diffuseCmd(float ld) = 0;

        //! Set the diffuse. Default implementation uses DiffuseUndoableCommand.
        //! \param ld the new diffuse value.
        virtual void diffuse(float ld) {
            if (auto cmd = diffuseCmd(ld)) {
                cmd->execute();
            }
        }

        //! Retrieve the diffuse for the light
        //! \return the diffuse value
        virtual float diffuse() const = 0;

        /*************************************************
            Light specular attribute, a multiplier for the effect
            of this light on the specular response of materials.
            Valid for the following light types: [all]
        *************************************************/

        //! Create an undoable command to set the specular. The command is not executed.
        //! \param ls the new specular value.
        //! \return Undoable command to set the specular
        virtual SpecularUndoableCommand::Ptr specularCmd(float ls) = 0;

        //! Set the specular. Default implementation uses SpecularUndoableCommand.
        //! \param ls the new specular value.
        virtual void specular(float ls) {
            if (auto cmd = specularCmd(ls)) {
                cmd->execute();
            }
        }

        //! Retrieve the specular for the light
        //! \return the specular value
        virtual float specular() const = 0;

        /*************************************************
            Directional interface.
            Valid for the following light types: [Directional]
        *************************************************/

        class UFE_SDK_DECL DirectionalInterface
        {
        public:
            virtual ~DirectionalInterface();

            //! Create an undoable command to set the angle. The command is not executed.
            //! \param la the new angle value.
            //! \return Undoable command to set the angle
            virtual AngleUndoableCommand::Ptr angleCmd(float la) = 0;

            //! Set the angle. Default implementation uses AngleUndoableCommand.
            //! \param la the new angle value.
            virtual void angle(float la) {
                if (auto cmd = angleCmd(la)) {
                    cmd->execute();
                }
            }

            //! Retrieve the angle for the light
            //! \return the angle value
            virtual float angle() const = 0;
        };

        //! Retrieve the directional interface for the light
        //! \return the directional interface
        std::shared_ptr<DirectionalInterface> directionalInterface();

        /*************************************************
            Sphere interface.
            Valid for the following light types: [Point, Spot]
        *************************************************/

        class UFE_SDK_DECL SphereInterface
        {
        public:
            virtual ~SphereInterface();

            //! Create an undoable command to set the sphere properties. The command is not executed.
            //! \param radius the new sphere radius.
            //! \param asPoint the flag specifying whether to treat the light as a point 
            //! \return Undoable command to set the sphere properties.
            virtual SpherePropsUndoableCommand::Ptr spherePropsCmd(float radius, bool asPoint) = 0;

            //! Set the sphere properties. Default implementation uses SpherePropsUndoableCommand.
            //! \param radius the new sphere radius.
            //! \param asPoint the flag specifying whether to treat the light as a point
            virtual void sphereProps(float radius, bool asPoint) {
                if (auto cmd = spherePropsCmd(radius, asPoint)) {
                    cmd->execute();
                }
            }

            //! Retrieve the sphere properties for the light
            //! \return the sphere properties
            virtual SphereProps sphereProps() const = 0;
        };

        //! Retrieve the sphere interface for the light
        //! \return the sphere interface
        std::shared_ptr<SphereInterface> sphereInterface();

        /*************************************************
            Cone interface.
            Valid for the following light types: [Spot]
        *************************************************/

        class UFE_SDK_DECL ConeInterface
        {
        public:
            virtual ~ConeInterface();

            //! Create an undoable command to set the cone properties. The command is not executed.
            //! \param focus the rate at which light intensity decreases from the center of the cone.
            //! \param angle the cone angle.
            //! \param softness defines the rate at which penumbra kicks in
            //! \return Undoable command to set the cone properties.
            virtual ConePropsUndoableCommand::Ptr conePropsCmd(float focus, float angle, float softness) = 0;

            //! Set the cone properties. Default implementation uses ConePropsUndoableCommand.
            //! \param focus the rate at which light intensity decreases from the center of the cone.
            //! \param angle the cone angle.
            //! \param softness defines the rate at which penumbra kicks in
            virtual void coneProps(float focus, float angle, float softness) {
                if (auto cmd = conePropsCmd(focus, angle, softness)) {
                    cmd->execute();
                }
            }

            //! Retrieve the cone properties for the light
            //! \return the cone properties
            virtual ConeProps coneProps() const = 0;
        };

        //! Retrieve the cone interface for the light
        //! \return the cone interface
        std::shared_ptr<ConeInterface> coneInterface();

        /*************************************************
            Area interface.
            Valid for the following light types: [Area]
        *************************************************/

        class UFE_SDK_DECL AreaInterface
        {
        public:
            virtual ~AreaInterface();

            //! Normalize attribute. Normalizes power by the surface area of the light.

            //! Create an undoable command to set the normalize flag. The command is not executed.
            //! \param nl the new normalize flag value.
            //! \return Undoable command to set the normalize flag
            virtual NormalizeUndoableCommand::Ptr normalizeCmd(bool nl) = 0;

            //! Set the normalize flag. Default implementation uses NormalizeUndoableCommand.
            //! \param nl the new normalize flag value.
            virtual void normalize(bool nl) {
                if (auto cmd = normalizeCmd(nl)) {
                    cmd->execute();
                }
            }

            //! Retrieve the normalize flag for the light
            //! \return the normalize flag value
            virtual bool normalize() const = 0;
        };

        //! Retrieve the area interface for the light
        //! \return the area interface
        std::shared_ptr<AreaInterface> areaInterface();
    
    protected:
        virtual std::shared_ptr<DirectionalInterface> directionalInterfaceImpl() = 0;
        virtual std::shared_ptr<SphereInterface> sphereInterfaceImpl() = 0;
        virtual std::shared_ptr<ConeInterface> coneInterfaceImpl() = 0;
        virtual std::shared_ptr<AreaInterface> areaInterfaceImpl() = 0;
    };
}

#endif /* _light */
