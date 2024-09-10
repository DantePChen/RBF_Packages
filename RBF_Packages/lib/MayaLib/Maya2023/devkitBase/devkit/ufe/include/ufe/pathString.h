#ifndef _ufePathString
#define _ufePathString
// ===========================================================================
// Copyright 2021 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "path.h"               // For Path::Segment

#include <string>
#include <functional>
#include <list>

UFE_NS_DEF {

/*! \namespace PathString Support for UFE string to path conversion.

  This namespace provides translation from a parsable UFE path string to a UFE
  path object.  A UFE path string has its segments separated by a path
  separator string.  Converting a UFE path to a parsable UFE path string is
  also provided.

  To determine each path segment's runtime ID, PathString assumes the first
  character of each path segment begins with a single-character path component
  separator, e.g. ".", or "/", or "|".  Each runtime can register one (or
  more) path component separators it understands.  If more than one runtime is
  associated with a path component separator, they will be asked in turn if the
  path segment corresponds to a valid object, using the \ref Ufe::Hierarchy
  interface.

  An application can change the path segment separator string.  The default
  path separator string is the single character comma string, ",".

  An application can add a path creation callback, to customize path creation
  from an input list of path segments.  The default path creation function
  simply calls the Path constructor with the vector of path segments.  The
  following example shows a custom path creation callback.

  \snippet test/testPathString.cpp Custom Path Creation Callback

  An application can add a single-segment path creation callback, to customize
  path creation from a single path segment, without any path segment separator.
  This can allow the application to support its native path string syntax as
  input to create a UFE path.

  An application can add a string callback, to customize string output for
  path to string conversion.  The default string function returns a string
  with each path segment separated by the path segment separator.  The
  following example shows a custom string callback.

  \snippet test/testPathString.cpp Custom String Callback
*/
namespace PathString {

typedef std::function<Path(const Path::Segments&)> CreatePathFn;
typedef std::function<Path(const std::string&)>    CreateSingleSegmentPathFn;
typedef std::function<std::string(const Path&)>    StringFn;

//! \param pathString A string with path segments separated by the path segment separator.
//! \exception EmptyPathSegment If the path segment is an empty string, or a degenerate single path component separator (e.g. "/a/b,|", where "|" is a degenerate path segment).
//! \exception InvalidPathComponentSeparator If the leading character or the character following the path segment separator is not a path component separator (e.g. "/a/b,foo", where "f" is not a path component separator).
//! \exception InvalidPath If multiple runtimes register the same path component separator, each will be asked to create a scene item for the argument path string.  If none succeeds, InvalidPath is thrown.
//! \return The UFE path corresponding to the input string.
UFE_SDK_DECL Path path(const std::string& pathString);

//! \param path Path to a scene item.
//! \return The path string corresponding to the input path, with path segments separated by the path segment separator.
UFE_SDK_DECL std::string string(const Path& path);

//! \return The current path segment string separator.
UFE_SDK_DECL std::string pathSegmentSeparator();

//! Set the path segment string separator.  Cannot be an empty string.
//! \exception std::invalid_argument If separator is empty.
UFE_SDK_DECL void setPathSegmentSeparator(const std::string& separator);

//! \return The general path creation function.
UFE_SDK_DECL CreatePathFn createPathFn();

//! Set the general path creation function.  It cannot be empty.
//! \exception std::invalid_argument If fn is empty.
UFE_SDK_DECL void setCreatePathFn(const CreatePathFn& fn);

//! Return the single-segment path creation function for the argument runtime.
//! \param rtId the ID of the runtime
//! \return rtId The single segment path creator for the argument runtime.
//! \exception std::out_of_range If the argument rtId has no creator function.
UFE_SDK_DECL CreateSingleSegmentPathFn createSingleSegmentPathFn(
    const Rtid& rtId);

//! Set the single-segment path creation function for the argument runtime.
//! \param rtId the ID of the runtime for the single segment path creator.
//! \param fn   the single-segment path creation function.
//! \exception InvalidRunTimeId Thrown if argument runtime ID does not exist.
UFE_SDK_DECL void setCreateSingleSegmentPathFn(
    const Rtid&                      rtId,
    const CreateSingleSegmentPathFn& fn);

//! \return The path to string creation function.
UFE_SDK_DECL StringFn stringFn();

//! Set the path to string creation function.  It cannot be empty.
//! \exception std::invalid_argument If fn is empty.
UFE_SDK_DECL void setStringFn(const StringFn& fn);

//! Register a path component separator for the argument runtime.  It is legal
//! for multiple runtimes to register the same path component separator.
//! \param rtId the ID of the runtime for the path component separator.
//! \param sep  the path component separator character.
//! \exception InvalidRunTimeId Thrown if argument runtime ID does not exist.
UFE_SDK_DECL void registerPathComponentSeparator(const Rtid& rtId, char sep);

//! Unregister a path component separator for the argument runtime.
//! \param rtId the ID of the runtime for the path component separator.
//! \param sep  the path component separator character.
//! \exception std::out_of_range If no such separator is registered.
UFE_SDK_DECL void unregisterPathComponentSeparator(const Rtid& rtId, char sep);

//! Return the runtimes corresponding to a path component separator.
//! \param sep  the path component separator character.
UFE_SDK_DECL std::list<Ufe::Rtid> pathComponentSeparatorRunTimeIds(char sep);

}

}

#endif /* _ufePathString */
