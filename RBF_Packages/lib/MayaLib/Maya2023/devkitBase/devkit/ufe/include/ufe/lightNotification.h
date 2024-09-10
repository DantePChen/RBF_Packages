#ifndef _ufeLightNotification
#define _ufeLightNotification
// ===========================================================================
// Copyright 2022 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "notification.h"
#include "sceneItem.h"

UFE_NS_DEF {

//! \brief Base class for all Light notifications.
/*!

  This class is the base class for all Light changed notifications.
*/

class UFE_SDK_DECL LightChanged: public Notification
{
public:
    //! Constructor.
    //! \param item LightChanged Notification on the given SceneItem.
    LightChanged(const SceneItem::Ptr& item);
    //! Default copy constructor.
    LightChanged(const LightChanged&) = default;
    //! Destructor.
    ~LightChanged() override;
    //! \return SceneItem of this Notification.
    SceneItem::Ptr item() const;

private:
    const SceneItem::Ptr fItem;
};

}

#endif /* _ufeLightNotification */
