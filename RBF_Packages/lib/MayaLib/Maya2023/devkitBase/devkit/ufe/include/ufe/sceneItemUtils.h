#ifndef _ufeSceneItemUtils
#define _ufeSceneItemUtils
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "sceneItem.h"
#include "attributes.h"

UFE_NS_DEF {

    UFE_SDK_DECL
    uint64_t getRawAddress(const SceneItem::Ptr& item);

    UFE_SDK_DECL
    SceneItem::Ptr getSceneItemFromRaw(uint64_t raw);

    UFE_SDK_DECL
    uint64_t getRawAddress(const Attributes::Ptr& item);

    UFE_SDK_DECL
    Attributes::Ptr getAttributesFromRaw(uint64_t raw);

}

#endif /* _ufeSceneItemUtils */
