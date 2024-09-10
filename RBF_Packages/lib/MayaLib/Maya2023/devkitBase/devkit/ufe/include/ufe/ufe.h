#ifndef _ufeUfe
#define _ufeUfe
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#define UFE_MAJOR_VERSION 3
#define UFE_MINOR_VERSION 3
#define UFE_PATCH_LEVEL   0

// UFE public namespace string will never change.
#define UFE_NS Ufe
// C preprocessor trickery to expand arguments.
#define UFE_CONCAT(A, B) UFE_CONCAT_IMPL(A, B)
#define UFE_CONCAT_IMPL(A, B) A##B
// Versioned namespace includes the major version number.
#define UFE_VERSIONED_NS UFE_CONCAT(UFE_NS, _v3)
 
namespace UFE_VERSIONED_NS {}
// With a using namespace declaration, pull in the versioned namespace into the
// Ufe public namespace, to allow client code to use the plain Ufe namespace,
// e.g. Ufe::Path.
namespace UFE_NS {
    using namespace UFE_VERSIONED_NS;
}
 
// Macros to place the UFE symbols in the versioned namespace, which is how
// they will appear in the shared library, e.g. Ufe_v1::Path.
#ifdef DOXYGEN
#define UFE_NS_DEF namespace UFE_NS
#else
#define UFE_NS_DEF namespace UFE_VERSIONED_NS
#endif

// Special symbols which can be used for checking when features were added.
#define UFE_V1_FEATURES_AVAILABLE
#define UFE_V2_FEATURES_AVAILABLE
#define UFE_V3_FEATURES_AVAILABLE

// Convenience macro for UFE conditional compilation.
#ifdef UFE_V2_FEATURES_AVAILABLE
#define UFE_V2(...) __VA_ARGS__
#else
#define UFE_V2(...)
#endif
#ifdef UFE_V3_FEATURES_AVAILABLE
#define UFE_V3(...) __VA_ARGS__
#else
#define UFE_V3(...)
#endif

#endif /* _ufeUfe */
