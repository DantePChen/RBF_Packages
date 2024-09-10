#ifndef _namedSelection
#define _namedSelection
// ===========================================================================
// Copyright 2020 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "common/ufeExport.h"

#include <memory>
#include <string>

UFE_NS_DEF {

class Selection;

/*! \namespace NamedSelection Support for an application-wide selection singleton(s).

  This namespace allows an application to get a selection singleton.
  This is an optional service; an application is not required by UFE
  to use any selection singletons.

  This namespace has 2 function members:
  - \ref Ufe::NamedSelection::get returns a pointer to named selection singleton
         that will be automatically created upon first request.
  - \ref Ufe::NamedSelection::remove if the named selection exists remove it.
*/
namespace NamedSelection {

typedef std::shared_ptr<Selection> Ptr;

/*!
  This function can be used to create a named selection singleton that
  can be shared and used across all viewers and editors. The first call
  to get with a given name will create the selection singleton. Subsequent
  calls to get with the same name will return this selection singleton.
  This selection object can be used to pass around UFE data.

  \param name Name of the selection singleton.
  \return The named selection.
*/
UFE_SDK_DECL const Ptr& get(const std::string& name);

/*!
  If a named selection exists with the given name, it is removed from
  the internal cache. When the final shared_ptr to this selection is
  released it will be automatically destroyed.

  \param name Name of the selection singleton to remove.
 */
UFE_SDK_DECL void remove(const std::string& name);
}

}

#endif /* _namedSelection */
