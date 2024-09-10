#ifndef _transform3dUndoableCommands
#define _transform3dUndoableCommands
// ===========================================================================
// Copyright 2020 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "common/ufeExport.h"
#include "baseUndoableCommands.h"

UFE_NS_DEF {

typedef SetVector3dUndoableCommand TranslateUndoableCommand;
typedef SetVector3dUndoableCommand RotateUndoableCommand;
typedef SetVector3dUndoableCommand ScaleUndoableCommand;
typedef SetVector3dUndoableCommand ShearUndoableCommand;

}

#endif /* _transform3dUndoableCommands */
