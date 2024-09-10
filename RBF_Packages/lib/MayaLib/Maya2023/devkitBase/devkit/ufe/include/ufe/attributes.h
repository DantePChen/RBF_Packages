#ifndef _ufe_attributes
#define _ufe_attributes

// =======================================================================
// Copyright 2019 Autodesk, Inc. All rights reserved.
//
// This computer source code and related instructions and comments are the
// unpublished confidential  and proprietary information of Autodesk, Inc.
// and are protected under applicable copyright and trade secret law. They 
// may not be disclosed to, copied  or used by any third party without the 
// prior written consent of Autodesk, Inc.
// =======================================================================

#include "common/ufeExport.h"
#include "attribute.h"
#include "sceneItem.h"
#include "observer.h"
#include "attributesNotification.h"

#include <memory>
#include <vector>
#include <string>

UFE_NS_DEF {

/*!
  \class Attributes
  \brief Abstract base class for Attributes interface.

  This base class defines an interface for factory objects that runtimes
  can implement to create attribute interface objects as well as query
  attribute names, attribute type and attribute existence.  The Attributes
  interface object can be considered as a factory, because it provides an
  interface to create an Attribute object per attribute, given an attribute
  name.  The Attribute object actually performs the work of reading
  from and writing to the attribute.

  As with the rest of UFE, attribute observation is done through the [Observer
  Pattern](https://en.wikipedia.org/wiki/Observer_pattern).  Observers can be
  added to an individual scene item, to be notified when an attribute
  changes.

  To avoid the memory-consuming "one proxy object per scene object" approach,
  attributes interface objects should be obtained and used within a local
  scope, and not stored.
*/

class UFE_SDK_DECL Attributes
{
public:
    typedef std::shared_ptr<Attributes> Ptr;

    /*!
        Convenience method that calls the attributes method on the attribute
        handler for the item.  Returns a null pointer if the argument is null,
        or has an empty path.
        \param item SceneItem's Attributes interface to retrieve.
        \return Attributes interface of the given SceneItem.
    */
    static Ptr attributes(const SceneItem::Ptr& item);

    /*!
        Add observation on the argument item for attribute changes.
        \param item SceneItem to observe.
        \param obs Observer to add.
        \return True if the observer is added. Add does nothing and returns 
         false if the observer is already present.
    */
    static bool addObserver(
        const SceneItem::Ptr& item, const Observer::Ptr& obs);
    /*!
        Add observation for attribute changes on any item in the scene (global
        observation).
        \param obs Observer to add.
        \return True if the observer is added. Add does nothing and returns 
         false if the observer is already present.
    */
    static bool addObserver(const Observer::Ptr& obs);
    /*!
        Remove observation on the argument item for attribute changes.
        \param item SceneItem to remove observation on.
        \param obs Observer to remove.
        \return True if the observer is removed. False if the observer isn't
        found.
    */
    static bool removeObserver(
        const SceneItem::Ptr& item, const Observer::Ptr& obs);

    /*!
        Remove observation for attribute changes on any item in the scene
        (global observation).
        \param obs Observer to remove.
        \return True if the observer is removed. False if the observer isn't
        found.
    */
    static bool removeObserver(const Observer::Ptr& obs);

    /*!
        Number of observers on the given SceneItem.
        \param item SceneItem on which to count observers.
        \return Number of observers on SceneItem.
    */
    static std::size_t nbObservers(const SceneItem::Ptr& item);

    /*!
        Number of observers for attribute changes on any item in the scene
        (global observers).
        \return Number of observers.
    */
    static std::size_t nbObservers();

    /*!
        Query observation on argument item for attribute changes.
        \param item SceneItem to check if has observation.
        \param obs Observer to query.
        \return True if there is observation on argument item
        for attribute changes.
    */
    static bool hasObserver(
        const SceneItem::Ptr& item, const Observer::Ptr& obs);

    /*!
        Query observation for attribute changes on any item in the scene
        (global observers).
        \param obs Observer to query.
        \return True if the observer observes attribute changes on any item
        in the scene.
    */
    static bool hasObserver(const Observer::Ptr& obs);

    //! \param path Path to verify if being observed.
    //! \return True if the given path is being observed.
    static bool hasObservers(const Path& path);

    //! Helper query for runtimes, to determine if a path they are responsible
    //! for is being observed specifically.  This only considers observation of
    //! that specific path, and does not consider observation of any item in
    //! the scene (global observation), which is in addition.
    //! \param runTimeId runtime Id to find observers on.
    //! \return True if any path of given runtime Id are being observed.
    static bool hasObservers(Rtid runTimeId);

    //! Notify all observers, including global observers.
    //! The order in which observers are notified is unspecified.
    //! If no observer exists, does nothing.
    //! \param notification Attribute notification to send.
    static void notify(const AttributeChanged& notification);

    //! Constructor.
    Attributes();
    //! Default copy constructor.
    Attributes(const Attributes&) = default;
    //! Destructor.
    virtual ~Attributes();

    //! \return The object's SceneItem.
    virtual SceneItem::Ptr sceneItem() const = 0;

    //! \return The attribute type for the attribute matching the input name.
    virtual Attribute::Type attributeType(const std::string& name) = 0;

    /*!
        Creates an Attribute interface for the given attribute name.
        \param name Name of the attribute to retrieve.
        \return Attribute interface for the given name. Returns a null
        pointer if no attribute exists for the given name.
    */
    virtual Attribute::Ptr attribute(const std::string& name) = 0;

    //! \return List of all the attribute names for this object.
    virtual std::vector<std::string> attributeNames() const = 0;

    /*!
        Queries whether an attribute exists with the given name.
        \param name The attribute name to check.
        \return True if the object contains an attribute matching the input name.
    */
    virtual bool hasAttribute(const std::string& name) const = 0;

}; // end class Attributes


} // end namespace

#endif /* _ufe_attributes */
