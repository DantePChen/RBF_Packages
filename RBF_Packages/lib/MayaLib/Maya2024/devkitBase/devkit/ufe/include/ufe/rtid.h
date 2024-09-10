#ifndef _ufeRtid
#define _ufeRtid
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "ufe.h"

#include <cstdint>

UFE_NS_DEF {

// Integer identifier for a UFE runtime.
//
// The runtime ID is allocated by the UFE runtime manager when a runtime is
// registered by name.  Runtimes may change from run to run, depending on the
// load order of multiple runtimes, and therefore should not be persisted.
//
// ID 0 is used to represent an invalid runtime ID.

typedef uint32_t Rtid;

}

#endif /* _ufeRtid */
