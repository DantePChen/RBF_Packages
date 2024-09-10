// ==================================================================
//
// Copyright (c) 2014 Autodesk, Inc.
// All rights reserved.
//
// This computer source code and related instructions and comments are the
// unpublished confidential and proprietary information of Autodesk, Inc.
// and are protected under applicable copyright and trade secret law.
// They may not be disclosed to, copied or used by any third party without
// the prior written consent of Autodesk, Inc.
// ==================================================================
//
//!
//! \file synColor/language.h
//!
//! \brief List of languages available for SynColor error messages.
//!

#ifndef _SYNCOLOR_LANGUAGE_H
#define _SYNCOLOR_LANGUAGE_H

#include <synColor/synColorNamespace.h>

namespace SYNCOLOR
{
  //! \brief List of languages available for SynColor error messages.
  enum Language
  {
    LANG_EN,             //!< US English: 8-bit ASCII encoding.
    LANG_JP,             //!< Japanese: ja_JP.SJIS encoding.
    LANG_SC,             //!< Simplified Chinese: zh_CN.gb2312 encoding.
    LANG_LAST = LANG_SC  //!< Index of last language available.
  };

}
#endif
