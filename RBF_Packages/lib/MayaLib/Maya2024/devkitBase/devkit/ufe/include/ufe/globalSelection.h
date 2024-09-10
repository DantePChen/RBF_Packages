#ifndef _globalSelection
#define _globalSelection
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "common/ufeExport.h"

#include <memory>

UFE_NS_DEF {

class ObservableSelection;

/*! \namespace GlobalSelection Support for an application-wide selection singleton.

  This namespace allows an application to initialize and access its
  globally-observable selection singleton.  This is an optional service; an
  application is not required by UFE to have a globally-observable selection
  singleton.

  This namespace has 2 function members :
  - \ref Ufe::GlobalSelection::get accesses the global selection.
  - \ref Ufe::GlobalSelection::initializeInstance initializes the global selection.


*/
namespace GlobalSelection {

typedef std::shared_ptr<ObservableSelection> Ptr;

/*!
  Initialize the observable selection singleton.  This selection is useful for
  applications where user operations are done on a single selection that is
  shared by all viewers and editors.  Initializing the global selection should
  be done on startup, and for proper selection lifescope management should be
  cleared with a null pointer on exit.  If the current global selection is
  non-null, the argument must be null, to prevent initializing the global
  selection more than once.
  \param globalSelection The single global selection for the application.
  \exception std::invalid_argument if the argument and current global selection are both non-null.
*/
UFE_SDK_DECL void initializeInstance(const Ptr& globalSelection);

//! \return The global observable selection
UFE_SDK_DECL const Ptr& get();
}

}

#endif /* _globalSelection */
