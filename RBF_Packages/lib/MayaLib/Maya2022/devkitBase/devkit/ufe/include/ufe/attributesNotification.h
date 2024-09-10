#ifndef _ufeAttributesNotification
#define _ufeAttributesNotification

// =======================================================================
// Copyright 2019 Autodesk, Inc. All rights reserved.
//
// This computer source code and related instructions and comments are the
// unpublished confidential  and proprietary information of Autodesk, Inc.
// and are protected under applicable copyright and trade secret law. They 
// may not be disclosed to, copied  or used by any third party without the 
// prior written consent of Autodesk, Inc.
// =======================================================================

#include "notification.h"
#include "path.h"
#include <string>

UFE_NS_DEF {

//! \brief Base class for all attribute changed notifications.
/*!

    This class is the base class for all attribute changed notifications.
*/

class UFE_SDK_DECL AttributeChanged: public Notification
{
public:
    //! Constructor.
    //! \param path AttributeChanged notification for the node at the tail of path.
    //! \param name Name of the attribute that changed.
    AttributeChanged(const Path& path, const std::string& name);
    //! Default copy constructor.
    AttributeChanged(const AttributeChanged&) = default;
    //! Destructor.
    ~AttributeChanged() override;
    //! \return Path of this Notification.
    const Path& path() const;
    //! \return Attribute name of this Notification.
    const std::string& name() const;

private:
    Path        fPath;
    std::string fName;
};

//! \brief Attribute value changed notification.
/*!

  An object of this class is sent to observers when the value of an attribute
  on a single node has changed.
*/
class UFE_SDK_DECL AttributeValueChanged : public AttributeChanged
{
public:
    //! Constructor
    using AttributeChanged::AttributeChanged;
};

}

#endif /* _ufeAttributesNotification */
