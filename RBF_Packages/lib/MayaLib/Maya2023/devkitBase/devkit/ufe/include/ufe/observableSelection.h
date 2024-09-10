#ifndef _ufeObservableSelection
#define _ufeObservableSelection
// ===========================================================================
// Copyright 2020 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "selection.h"
#include "subject.h"

#include <memory>               // unique_ptr

UFE_NS_DEF {

class SelectionCompositeNotification;

//! \brief Observable selection for objects in the scene.
/*!

This class extends the base class selection to provide notifications on change.
It acts as a Subject, so that Observers can register to it and receive
SelectionChanged notifications.

*/

class UFE_SDK_DECL ObservableSelection : public Selection, public Subject
{
public:

    //! Constructor
    ObservableSelection();

    //! Destructor
    ~ObservableSelection() override;

    //@{
    //! Copy and move assignment and construction.  These methods do not assign
    //! or move observers from the right hand ObservableSelection (source) to
    //! the destination.  For assignment, the observers of both objects are
    //! unchanged.  For construction, the observers of the source are
    //! unchanged, and the destination has no observers.  See \ref
    //! Ufe::Selection base class.
    ObservableSelection(const Selection&);
    ObservableSelection(const ObservableSelection&);
    ObservableSelection(Selection&&);
    ObservableSelection(ObservableSelection&&);
    ObservableSelection& operator=(const Selection&);
    ObservableSelection& operator=(const ObservableSelection&);
    ObservableSelection& operator=(Selection&& rhs);
    ObservableSelection& operator=(ObservableSelection&& rhs);
    //@}

    //! Composite notification predicate.
    //! \return True if this subject is collecting notifications into a composite notification.
    bool inCompositeNotification() const override;

protected:

    //@{
    //! Subject Notification guard interface.  Implementation in this class
    //! creates a SelectionCompositeNotification on begin, and notifies
    //! observers with the composite notification on end.
    void beginNotificationGuard() override;
    void endNotificationGuard() override;
    //@}

    /*!
        Method called after adding an item to the observable selection
        \param item Added SceneItem to the observable selection
    */
    void postAppend(const SceneItem::Ptr& item) override;
   
    /*!
        Method called after removing an item from the observable selection
        \param item Removed SceneItem from the observable selection
    */
    void postRemove(const SceneItem::Ptr& item) override;
    
    /*!
        Method called after inserting an item into the observable selection
        \param position SceneItem following the inserted item
        \param item SceneItem inserted into the observable selection
    */
    void postInsert(
        const SceneItem::Ptr& position, const SceneItem::Ptr& item
    ) override;

    //! Method called after clearing the observable selection
    void postClear() override;

    //! Method called after replacing the observable selection
    void postReplaceWith() override;

private:

    typedef Selection BaseClass;

    //! Composite notification, when within the scope of a guard.
    std::unique_ptr<SelectionCompositeNotification> fCompositeNotification;
};

}

#endif /* _ufeObservableSelection */
