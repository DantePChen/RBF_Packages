///////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2012, Industrial Light & Magic, a division of Lucas
// Digital Ltd. LLC
// 
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
// *       Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
// *       Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
// *       Neither the name of Industrial Light & Magic nor the names of
// its contributors may be used to endorse or promote products derived
// from this software without specific prior written permission. 
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
///////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_HALFNAMESPACE_H
#define INCLUDED_HALFNAMESPACE_H

//
// The purpose of this file is to make it possible to specify an
// HALF_INTERNAL_NAMESPACE as a preprocessor definition and have all of the
// half symbols defined within that namespace rather than the standard
// half namespace.  Those symbols are made available to client code through
// the HALF_NAMESPACE in addition to the HALF_INTERNAL_NAMESPACE.
//
// To ensure source code compatibility, the HALF_NAMESPACE defaults to half
// and then "using namespace HALF_INTERNAL_NAMESPACE;" brings all of the
// declarations from the HALF_INTERNAL_NAMESPACE into the HALF_NAMESPACE.
// This means that client code can continue to use syntax like half,
// but at link time it will resolve to a mangled symbol based on the
// HALF_INTERNAL_NAMESPACE.
//
// As an example, if one needed to build against a newer version of half and
// have it run alongside an older version in the same application, it is now
// possible to use an internal namespace to prevent collisions between the
// older versions of half symbols and the newer ones.  To do this, the
// following could be defined at build time:
//
// HALF_INTERNAL_NAMESPACE = half_v2
//
// This means that declarations inside half headers look like this (after
// the preprocessor has done its work):
//
// namespace half_v2 {
//     ...
//     class declarations
//     ...
// }
//
// namespace half {
//     using namespace half_v2;
// }
//

//
// Open Source version of this file pulls in the IlmBaseConfig.h file
// for the configure time options.
//
#include "IlmBaseConfig.h"

#ifndef HALF_NAMESPACE
#define HALF_NAMESPACE Half
#endif

#ifndef HALF_INTERNAL_NAMESPACE
#define HALF_INTERNAL_NAMESPACE HALF_NAMESPACE
#endif

//
// We need to be sure that we import the internal namespace into the public one.
// To do this, we use the small bit of code below which initially defines
// HALF_INTERNAL_NAMESPACE (so it can be referenced) and then defines
// HALF_NAMESPACE and pulls the internal symbols into the public
// namespace.
//

//namespace HALF_INTERNAL_NAMESPACE {}
//namespace HALF_NAMESPACE {
//       using namespace HALF_INTERNAL_NAMESPACE;
//}


//need this for now to avoid source changes in OIIO
//using namespace HALF_NAMESPACE;

//
// There are identical pairs of HEADER/SOURCE ENTER/EXIT macros so that
// future extension to the namespace mechanism is possible without changing
// project source code.
//

//#define HALF_INTERNAL_NAMESPACE_HEADER_ENTER namespace HALF_INTERNAL_NAMESPACE {
//#define HALF_INTERNAL_NAMESPACE_HEADER_EXIT }
#define HALF_INTERNAL_NAMESPACE_HEADER_ENTER
#define HALF_INTERNAL_NAMESPACE_HEADER_EXIT

//#define HALF_INTERNAL_NAMESPACE_SOURCE_ENTER namespace HALF_INTERNAL_NAMESPACE {
//#define HALF_INTERNAL_NAMESPACE_SOURCE_EXIT }
#define HALF_INTERNAL_NAMESPACE_SOURCE_ENTER
#define HALF_INTERNAL_NAMESPACE_SOURCE_EXIT

#endif /* INCLUDED_HALFNAMESPACE_H */
