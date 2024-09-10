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

// Integer identifier for a UFE run-time.
//
// The run-time ID is allocated by the UFE run time manager when a run-time is
// registered by name.  Run-times may change from run to run, depending on the
// load order of multiple run-times, and therefore should not be persisted.
//
// ID 0 is used to represent an invalid run-time ID.

typedef uint32_t Rtid;

}

#endif /* _ufeRtid */
