#ifndef _ufeObject3dNotification
#define _ufeObject3dNotification

// =======================================================================
// Copyright 2020 Autodesk, Inc. All rights reserved.
//
// This computer source code and related instructions and comments are the
// unpublished confidential  and proprietary information of Autodesk, Inc.
// and are protected under applicable copyright and trade secret law. They 
// may not be disclosed to, copied  or used by any third party without the 
// prior written consent of Autodesk, Inc.
// =======================================================================

#include "notification.h"
#include "path.h"

UFE_NS_DEF {

//! \brief Visibility changed notification.
/*!

  An object of this class is sent to observers when the visibility on
  an Object3d has changed.
*/

class UFE_SDK_DECL VisibilityChanged: public Notification
{
public:
    //! Constructor.
    //! \param path VisibilityChanged notification for the node at the tail of path.
    VisibilityChanged(const Path& path);
    //! Default copy constructor.
    VisibilityChanged(const VisibilityChanged&) = default;
    //! Destructor.
    ~VisibilityChanged() override;
    //! \return Path of this Notification.
    const Path& path() const;

private:
    Path        fPath;
};

}

#endif /* _ufeObject3dNotification */
