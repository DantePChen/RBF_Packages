#ifndef _stringUtils
#define _stringUtils
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "common/ufeExport.h"

#include <string>
#include <vector>

UFE_NS_DEF {
/*!
We need to export these functions to be able to run our unit tests, although
this header file is not exposed with the other public header files.
*/

/*!
  Return a copy of the string with the leading toStrip character removed.
  If toStrip is '\0', the src argument is returned unchanged.

  \param[in] src     string to be stripped.
  \param[in] toStrip character to be stripped from the beginning of src.
  \return Copy of src with leading toStrip removed.
*/
UFE_SDK_DECL std::string lstrip(std::string src, char toStrip);

/*!
  Split src into separate strings.  If sep is '\0', a single-element vector
  with the src argument is returned.

  \param[in] src string to be split.
  \param[in] sep separator character.
  \return src split on sep into separate strings.
*/
UFE_SDK_DECL std::vector<std::string> split(const std::string& src, char sep);

/*!
  Split src into separate strings, using a separator string.  If the separator
  is an empty string, a single-element vector with the src argument is returned.

  \param[in] src string to be split.
  \param[in] sep separator string.
  \return src split on sep into separate strings.
*/
UFE_SDK_DECL std::vector<std::string> split(
    const std::string& src,
    const std::string& sep
);

/*!
  Returns true if the argument string ends with the suffix string.

  \param[in] str  argument string.
  \param[in] suffix suffix string.
  \return true if str ends with suffix.
*/
UFE_SDK_DECL bool endsWith(const std::string& str, const std::string& suffix);

}

#endif /* _stringUtils */
