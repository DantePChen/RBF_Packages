#ifndef _ufeCameraNotification
#define _ufeCameraNotification
// ===========================================================================
// Copyright 2020 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "notification.h"
#include "sceneItem.h"

UFE_NS_DEF {

//! \brief Base class for all Camera notifications.
/*!

  This class is the base class for all Camera changed notifications.
*/

class UFE_SDK_DECL CameraChanged: public Notification
{
public:
    //! Constructor.
    //! \param item CameraChanged Notification on the given SceneItem.
    CameraChanged(const SceneItem::Ptr& item);
    //! Default copy constructor.
    CameraChanged(const CameraChanged&) = default;
    //! Destructor.
    ~CameraChanged() override;
    //! \return SceneItem of this Notification.
    SceneItem::Ptr item() const;

private:
    SceneItem::Ptr fItem;
};

}

#endif /* _ufeCameraNotification */
