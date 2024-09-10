#ifndef _ufeSelectionTrieFwd
#define _ufeSelectionTrieFwd
// =======================================================================
// Copyright 2020 Autodesk, Inc. All rights reserved.
//
// This computer source code and related instructions and comments are the
// unpublished confidential  and proprietary information of Autodesk, Inc.
// and are protected under applicable copyright and trade secret law. They 
// may not be disclosed to, copied  or used by any third party without the 
// prior written consent of Autodesk, Inc.
// =======================================================================

#include "sceneItemList.h"

UFE_NS_DEF {

template<typename T> class Trie;
typedef Trie<Ufe::SceneItemList::iterator> SelectionTrie;

}

#endif  /* _ufeSelectionTrieFwd */
