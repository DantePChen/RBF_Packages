#ifndef _ufeSubject
#define _ufeSubject
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "observer.h"

#include <set>

UFE_NS_DEF {

class Notification;
class NotificationGuard;

//! \brief Observer pattern Subject class.
/*!

  This class implements the Subject in the Observer Pattern:

  https://en.wikipedia.org/wiki/Observer_pattern

  Observers are held by weak pointer.  The order in which observers are
  notified is unspecified, implementation dependent, and can change at any
  time.  Observers should NOT rely on the order in which they are added to a
  Subject and assume they will be called in the same order.

  The semantics of Subject under copy, assignment, move, and move assignment
  are that observers are not copied or moved.  As compared to preventing
  copying, this provides flexibility for derived classes to support the
  value-based semantics of copy, as well as efficient support of temporary
  objects with move.
*/

class UFE_SDK_DECL Subject
{
public:

    //! Constructor.
    Subject();
    
    //! Copy constructor.  Observers are not copied.
    Subject(const Subject&);

    //! Move constructor.  Observers are not moved.
    Subject(Subject&&);

    //! Destructor.
    virtual ~Subject();

    //! Assignment operator.  Does nothing, as observers are not copied.
    Subject& operator=(const Subject&);

    //! Move assignment operator.  Does nothing, as observers are not moved.
    Subject& operator=(Subject&&);

    /*!
        Add the argument Observer to this subject.  Does nothing and returns
        false if the Observer is already present.
        \param obs Observer to add to Subject.
        \return True if the Observer has been added to Subject.
    */
    bool addObserver(const Observer::Ptr& obs);

    /*!
        Remove the argument Observer from this subject.  Returns false if
        the observer isn't found.
        \param obs Observer to remove from Subject.
        \return True if the Observer has been removed from Subject.
    */
    bool removeObserver(const Observer::Ptr& obs);

    //! \return True if this subject has the argument observer.
    bool hasObserver(const Observer::Ptr& obs) const;

    //! \return Number of observers.
    std::size_t nbObservers() const;

    //! Notify observers. Not protected to simplify Python bindings.
    //! \param notification Notification to notify.
    virtual void notify(const Notification& notification);

    //! Composite notification predicate.  Implementation in this class returns
    //! false.
    //! \return True if this subject is collecting notifications into a composite notification.
    virtual bool inCompositeNotification() const;

protected:
    //@{
    //! Notification guard interface.  Implementation in this class is a no-op.
    virtual void beginNotificationGuard();
    virtual void endNotificationGuard();
    //@}

private:
    friend class NotificationGuard;

    // Weak pointers cannot be ordered by default, as they can expire at any
    // time.  However, they can be ordered by the address of the control block,
    // which stores the number of shared_ptr AND weak_ptr, and is deleted only
    // when the last weak_ptr expires.
    typedef std::set<Observer::WeakPtr, std::owner_less<Observer::WeakPtr> >
        Observers;

    //! Remove dead observers.
    //! \param dead Observers to remove.
    void cleanObservers(const Observers& dead);

    Observers fObservers;
};

//! \brief Notification guard for Subject.
/*!

  This class implements a guard that delays notifications sent by Subject until
  expiration of the guard.  When the guard expires, the Subject will send out a
  composite notification with all notifications that occurred within the scope
  of the guard.

  Only the top-most guard is considered: any nested guards are ignored.
*/

class UFE_SDK_DECL NotificationGuard {
public:

    NotificationGuard(Subject& subject);
    ~NotificationGuard();

    //@{
    //! Cannot be copied or assigned.
    NotificationGuard(const NotificationGuard&) = delete;
    const NotificationGuard& operator&(const NotificationGuard&) = delete;
    //@}

private:
    Subject* const fSubject;
};

}

#endif /* _ufeSubject */
