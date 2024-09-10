#ifndef _ufeTransform3dNotification
#define _ufeTransform3dNotification
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

//! \brief Base class for all Transform3d notifications.
/*!

  This class is the base class for all Transform3d changed notifications.
*/

class UFE_SDK_DECL Transform3dChanged: public Notification
{
public:
    //! Constructor.
    //! \param item Transform3dChanged Notification on the given SceneItem.
    Transform3dChanged(const SceneItem::Ptr& item);
    //! Default copy constructor.
    Transform3dChanged(const Transform3dChanged&) = default;
    //! Destructor.
    ~Transform3dChanged() override;
    //! \return SceneItem of this Notification.
    SceneItem::Ptr item() const;

private:
    SceneItem::Ptr fItem;
};

}

#endif /* _ufeTransform3dNotification */
