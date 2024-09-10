#ifndef _pathStringExcept
#define _pathStringExcept
// ===========================================================================
// Copyright 2020 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "ufe.h"

#include <stdexcept>
#include <string>

UFE_NS_DEF {

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
#endif

//! \brief Exception class to signal empty path segment in path string.
//
// See \ref Ufe::InvalidRunTimeName for inline implementation details.

class EmptyPathSegment : public std::runtime_error
{
public:
    EmptyPathSegment(const std::string& pathString) :
        std::runtime_error(pathString), fPathString(pathString) {}
    EmptyPathSegment(const EmptyPathSegment&) = default;
    ~EmptyPathSegment() override {}

    const std::string& pathString() const { return fPathString; }

private:
    const std::string fPathString;
};

//! \brief Exception class to signal an unknown path component separator in a path string.
//
// See \ref Ufe::InvalidRunTimeName for inline implementation details.

class InvalidPathComponentSeparator : public std::out_of_range
{
public:
    InvalidPathComponentSeparator(char sep, const std::string& segmentString) :
        std::out_of_range(segmentString), fSeparator(sep),
        fPathSegmentString(segmentString) {}
    InvalidPathComponentSeparator(const InvalidPathComponentSeparator&) = default;
    ~InvalidPathComponentSeparator() override {}

    char separator() const { return fSeparator; }

    const std::string& pathSegmentString() const { return fPathSegmentString; }

private:
    const char        fSeparator;
    const std::string fPathSegmentString;
};

//! \brief Exception class to signal an invalid path.
//
// See \ref Ufe::InvalidRunTimeName for inline implementation details.

class InvalidPath : public std::runtime_error
{
public:
    InvalidPath(const std::string& pathString) :
        std::runtime_error(pathString), fPathString(pathString) {}
    InvalidPath(const InvalidPath&) = default;
    ~InvalidPath() override {}

    const std::string& pathString() const { return fPathString; }

private:
    const std::string fPathString;
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

}

#endif /* _pathStringExcept */
