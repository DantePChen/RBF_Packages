#ifndef _ufeSceneItemList
#define _ufeSceneItemList
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "ufe.h"

#include <list>
#include <memory>

UFE_NS_DEF {

class SceneItem;

typedef std::list<std::shared_ptr<SceneItem> > SceneItemList;

}

#endif /* _ufeSceneItemList */
