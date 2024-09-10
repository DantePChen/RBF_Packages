#ifndef _cameraUndoableCommands
#define _cameraUndoableCommands
// ===========================================================================
// Copyright 2020 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "common/ufeExport.h"
#include "baseUndoableCommands.h"

UFE_NS_DEF
{

    typedef SetFloatUndoableCommand HorizontalApertureUndoableCommand;
    typedef SetFloatUndoableCommand VerticalApertureUndoableCommand;
    typedef SetFloatUndoableCommand HorizontalApertureOffsetUndoableCommand;
    typedef SetFloatUndoableCommand VerticalApertureOffsetUndoableCommand;
    typedef SetFloatUndoableCommand FStopUndoableCommand;
    typedef SetFloatUndoableCommand FocalLengthUndoableCommand;
    typedef SetFloatUndoableCommand FocusDistanceUndoableCommand;
    typedef SetFloatUndoableCommand NearClipPlaneUndoableCommand;
    typedef SetFloatUndoableCommand FarClipPlaneUndoableCommand;
    typedef SetIntUndoableCommand ProjectionUndoableCommand;
}

#endif /* _cameraUndoableCommands */
