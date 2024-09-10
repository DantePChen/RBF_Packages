#ifndef _CacheAlembicUtil_h_
#define _CacheAlembicUtil_h_

//-
//**************************************************************************/
// Copyright 2012 Autodesk, Inc. All rights reserved. 
//
// Use of this software is subject to the terms of the Autodesk 
// license agreement provided at the time of installation or download, 
// or which otherwise accompanies this software in either electronic 
// or hard copy form.
//**************************************************************************/
//+

// Includes
#include <string>
#include <mutex>

namespace CacheAlembicUtil{
	//
	// Big Alembic Mutex for all calls into Alembic library.
	// This mutex should be the last mutex to lock and the first mutex to unlock.
	// This mutex is not reentrant.
	// For example,
	//    {
	//        std::unique_lock<std::recursive_mutex> lock(fMutex);
	//        ... access to this class's internal data structure ...
	//
	//        {
	//            std::lock_guard<std::mutex> alembicLock(gsAlembicMutex);
	//            ... calls to Alembic library ...
	//        }
	//    }
	//
	extern std::mutex gsAlembicMutex;

	extern const std::string kCustomPropertyWireIndices;
	extern const std::string kCustomPropertyWireIndicesOld;
	extern const std::string kCustomPropertyShadingGroupSizes;
    extern const std::string kCustomPropertyDiffuseColor;
	extern const std::string kCustomPropertyCreator;
	extern const std::string kCustomPropertyCreatorValue;
	extern const std::string kCustomPropertyVersion;
	extern const std::string kCustomPropertyVersionValue;

    extern const std::string kMaterialsObject;
    extern const std::string kMaterialsGpuCacheTarget;
    extern const std::string kMaterialsGpuCacheType;
}

#endif


