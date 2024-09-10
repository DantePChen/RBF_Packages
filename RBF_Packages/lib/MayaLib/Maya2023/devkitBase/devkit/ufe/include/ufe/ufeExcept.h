#ifndef _ufeExcept
#define _ufeExcept
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "rtid.h"
#include "path.h"
#include "pathSegment.h"

#include <stdexcept>

UFE_NS_DEF {

//! \brief Exception class to signal unknown runtime ID.
//
// This class is fully inline, as its base class is in the Standard Library.
// On Windows, exporting a class from a DLL requires exporting all its bases.
// This post
//
// https://stackoverflow.com/questions/24511376/how-to-dllexport-a-class-derived-from-stdruntime-error
//
// suggests that different Standard Library exception class implementations
// have different export linkage characteristics, so we avoid these differences
// with a fully inline implementation, which is therefore not exported.
//
// This does cause clang to warn about the class having "no out-of-line virtual
// method definitions; its vtable will be emitted in every translation unit
// [-Werror,-Wweak-vtables]".  As per
//
// http://llvm.org/docs/CodingStandards.html#provide-a-virtual-method-anchor-for-classes-in-headers
//
// this will cause the vtable and RTTI information to be emitted into every .o
// (translation unit) that incluces this header, increasing .o size and link
// time, so this header file should be included only as necessary.

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
#endif

class InvalidRunTimeName : public std::runtime_error
{
public:
    InvalidRunTimeName(const std::string& rtName) :
        std::runtime_error(rtName) {}
    InvalidRunTimeName(const InvalidRunTimeName&) = default;
    ~InvalidRunTimeName() override {}
};

class InvalidRunTimeId : public std::out_of_range
{
public:
    InvalidRunTimeId(const Rtid& rtId) :
        std::out_of_range(std::to_string(rtId)) {}
    InvalidRunTimeId(const InvalidRunTimeId&) = default;
    ~InvalidRunTimeId() override {}
};

//! \brief Exception class to signal an invalid path.
//
// This class is fully inline.  See InvalidRunTimeId class description for
// details.

class InvalidOperationOnPath : public std::out_of_range
{
public:
    InvalidOperationOnPath(const Path& path) :
        std::out_of_range(path.string()) {}
    InvalidOperationOnPath(const InvalidOperationOnPath&) = default;
    ~InvalidOperationOnPath() override {}
};

//! \brief Exception class to signal an invalid path.
//
// This class is fully inline.  See InvalidRunTimeId class description for
// details.

class InvalidOperationOnPathSegment : public std::out_of_range
{
public:
    InvalidOperationOnPathSegment(const PathSegment& pathSegment) :
        std::out_of_range(pathSegment.string()) {}
    InvalidOperationOnPathSegment(const InvalidOperationOnPathSegment&) = default;
    ~InvalidOperationOnPathSegment() override {}
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

}

#endif /* _ufeExcept */
