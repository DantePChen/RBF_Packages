#ifndef _runTimeMgr
#define _runTimeMgr
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "hierarchyHandler.h"
#include "transform3dHandler.h"
#include "sceneItemOpsHandler.h"
#include "rtid.h"

// Can't forward declare std::list or std::string.
#include <list>
#include <string>

UFE_NS_DEF {

//! \brief Singleton class to manage UFE run-times.
/*!

  This singleton class is where UFE run-times register handlers for different
  interfaces.  Clients that wish to use interfaces ask the run-time manager to
  return the appropriate handler for that interface, for the appropriate
  run-time.
*/

class UFE_SDK_DECL RunTimeMgr
{
public:

    //! \return The run-time manager singleton instance.
    static RunTimeMgr& instance();

    //! Cannot copy the run-time manager singleton.
    RunTimeMgr(const RunTimeMgr&) = delete;
    //! Cannot assign the run-time manager singleton.
    RunTimeMgr& operator=(const RunTimeMgr&) = delete;

    /*! Register a run-time and its handlers to create interfaces.
        register is a reserved C++ keyword, using register_ instead.
      \sa unregister
      \param rtName              the name of the run-time for the handlers.
      \param hierarchyHandler    the Hierarchy interface factory.
      \param transform3dHandler  the Transform3d interface factory.
      \param sceneItemOpsHandler the SceneItemOps interface factory.
      \exception InvalidRunTimeName Thrown if argument run-time name already exists.
      \return The allocated run-time ID.
     */
    Rtid register_(
        const std::string&              rtName,
        const HierarchyHandler::Ptr&    hierarchyHandler,
        const Transform3dHandler::Ptr&  transform3dHandler,
        const SceneItemOpsHandler::Ptr& sceneItemOpsHandler
    );
    /*!
        Unregister the given run-time ID.
        \param rtId   the ID of the run-time for the handlers.
        \return True if the run-time was unregistered.
    */
    bool unregister(const Rtid& rtId);

    //! \exception InvalidRunTimeId Thrown if argument run-time ID does not exist.
    //! \return The run-time name corresponding to the argument ID.
    std::string getName(const Rtid& rtId) const;

    //! \exception InvalidRunTimeName Thrown if argument run-time name does not exist.
    //! \return The run-time ID corresponding to the argument name.
    Rtid getId(const std::string& rtName) const;

    /*!
        Set a HierarchyHandler to a given run-time ID
        \param rtId the ID of the run-time for the handler.
        \param hierarchyHandler the Hierarchy interface factory.
    */
    void setHierarchyHandler(
        const Rtid& rtId, const HierarchyHandler::Ptr& hierarchyHandler
    );

    /*!
        Set a Transform3dHandler to a given run-time ID.
        \param rtId the ID of the run-time for the handler.
        \param transform3dHandler the Transform3d interface factory.
    */
    void setTransform3dHandler(
        const Rtid& rtId, const Transform3dHandler::Ptr& transform3dHandler
    );

    /*!
        Set a SceneItemOpsHandler to a given run-time ID.
        \param rtId the ID of the run-time for the handler.
        \param sceneItemOpsHandler the SceneItemOps interface factory.
    */
    void setSceneItemOpsHandler(
        const Rtid& rtId, const SceneItemOpsHandler::Ptr& sceneItemOpsHandler
    );

    /*!
        Retrieve the HierarchyHandler of a given run-time ID.
        \param rtId the ID of the run-time for the handler.
        \return HierarchyHandler corresponding to run-time type. 
        Null pointer if the argument run-time does not exist
    */
    HierarchyHandler::Ptr hierarchyHandler(const Rtid& rtId) const;

    /*!
        Retrieve the Hierarchy handler of a given run-time ID.
        \param rtId the ID of the run-time for the handler.
        \return HierarchyHandler corresponding to run-time type.
        \exception InvalidRunTimeId Thrown if argument run-time does not exist.
    */
    const HierarchyHandler& hierarchyHandlerRef(const Rtid& rtId) const;

    /*!
        Retrieve the Transform3d handler of a given run-time ID.
        \param rtId the ID of the run-time for the handler.
        \return Transform3dHandler corresponding to run-time type.
        Null pointer if the argument run-time does not exist.
    */
    Transform3dHandler::Ptr transform3dHandler(const Rtid& rtId) const;

    /*!
        Retrieve the SceneItemOps handler of a given run-time ID.
        \param rtId the ID of the run-time for the handler.
        \return SceneItemOpsHandler corresponding to run-time type.
        Null pointer if the argument run-time does not exist.
    */
    SceneItemOpsHandler::Ptr sceneItemOpsHandler(const Rtid& rtId) const;

    //! \return List of all register run-time IDs.
    std::list<Rtid> getIds() const;

private:

    //! Cannot create a run-time manager aside from the singleton instance.
    RunTimeMgr();
};

}

#endif /* _runTimeMgr */
