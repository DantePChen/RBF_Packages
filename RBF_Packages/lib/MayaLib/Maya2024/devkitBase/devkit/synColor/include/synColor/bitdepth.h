//
// ==================================================================
// Copyright 2014 Autodesk, Inc.  All rights reserved.
// 
// This computer source code  and related  instructions and comments are
// the unpublished confidential and proprietary information of Autodesk,
// Inc. and are  protected  under applicable  copyright and trade secret
// law. They may not  be disclosed to, copied or used by any third party
// without the prior written consent of Autodesk, Inc.
// ==================================================================
//
//! \file synColor/bitdepth.h
//!
//! \brief List of input and output bit depths supported by color
//!        transform, string/enum conversion utilities, and bit depth
//!        property utilities.
//!

#ifndef _SYNCOLOR_BIT_DEPTH_H
#define _SYNCOLOR_BIT_DEPTH_H

#include <synColor/synExport.h>
#include <synColor/synColorNamespace.h>

SYN_VISIBILITY_PUSH

namespace SYNCOLOR
{
  //----------------------------------------------------------------------------
  //! \brief Supported bit depth list.
  //!
  //! Each bit depth is represented by a bit flags.  These
  //! flags are a used to encode bit depth in entries of the
  //! SYNCOLOR::PixelFormat list.
  //!
  enum BitDepth
  {
    BIT_DEPTH_UNKNOWN = 0,         //!< Bit depth invalid or not set.

    BIT_DEPTH_8i      = 0x01000,   //!<  8-bit integer.
    BIT_DEPTH_10i     = 0x02000,   //!< 10-bit integer.
    BIT_DEPTH_12i     = 0x04000,   //!< 12-bit integer.
    BIT_DEPTH_16i     = 0x08000,   //!< 16-bit integer.
    BIT_DEPTH_16f     = 0x10000,   //!< 16-bit float.
    BIT_DEPTH_32f     = 0x20000    //!< 32-bit float.
  };

  //----------------------------------------------------------------------------
  //! \brief Bit depth mask.
  //!
  //! This mask is useful for extracting the bit depth portion of a
  //! SYNCOLOR::PixelFormat.
  //!
  enum BitDepthHelper
  {
    BIT_DEPTH_MASK    = 0xFF000    //!< Set of bits corresponding to bit depth flags.
  };

  //----------------------------------------------------------------------------
  //! \brief Convert the bit depth string to its corresponding enum value.
  //
  //! \param name String representing the bit depth (8i, 16f, etc.)
  //! \return BitDepth value for 'name'.
  //!         Returns SYNCOLOR::BIT_DEPTH_UNKNOWN if the string is 
  //!         not recognized.
  //
  SYN_EXPORT BitDepth getBitDepth(const char* name);

  //----------------------------------------------------------------------------
  //! \brief Convert the BitDepth value to a string.
  //
  //! \param bitDepth BitDepth value
  //! \return String corresponding to bitDepth (8i, 16f, etc.)
  //
  SYN_EXPORT const char* getBitDepthName(BitDepth bitDepth);

  //----------------------------------------------------------------------------
  //! \brief Get the maximum code value of a bit depth.
  //
  //! \param bitDepth BitDepth value.
  //! \return The maximum code value for bitDepth.
  //!         Returns 0.0f for SYNCOLOR::BIT_DEPTH_UNKNOWN, and 1.0f
  //!         for all floating point bit depths.
  //
  SYN_EXPORT float getBitDepthMaxValue(BitDepth bitDepth);

  //----------------------------------------------------------------------------
  //! \brief Get the minimum code value of a bit depth.
  //
  //! \param bitDepth BitDepth value.
  //! \return The minimum code value for bitDepth.  This value is 0.0f for all
  //!         bit depths.
  //
  SYN_EXPORT float getBitDepthMinValue(BitDepth bitDepth);

  //----------------------------------------------------------------------------
  //! \brief Get the value range of a bit depth
  //
  //! \param bitDepth BitDepth value.
  //! \return The range for bitDepth.  Returns the difference between the maximum
  //!         and minimum bit depth values, or 0.0f for SYNCOLOR::BIT_DEPTH_UNKNOWN.
  //
  SYN_EXPORT float getBitDepthValueRange(BitDepth bitDepth);

  //----------------------------------------------------------------------------
  //! \brief True for a floating point bit depth.
  //
  //! \param bitDepth BitDepth value.
  //! \return True if bitDepth is floating point.
  //!
  SYN_EXPORT bool isFloatDepth(BitDepth bitDepth);

  //----------------------------------------------------------------------------
  //! \brief True for an integer bit depth.
  //
  //! \param bitDepth BitDepth value.
  //! \return True if bitDepth is integer.
  //!
  SYN_EXPORT bool isIntegerDepth(BitDepth bitDepth);

  //----------------------------------------------------------------------------
  //! \brief Number of bits available for encoding a color value.
  //
  //!
  //! For floating point bit depths, the precision is the number of bits
  //! to encode the entire value: i.e. the mantissa and the exponent.
  //!
  //! \param bitDepth BitDepth value.
  //! \return The number of bits available for encoding a color value.
  //!
  SYN_EXPORT unsigned getBitDepthPrecision(const BitDepth bitDepth);

};

SYN_VISIBILITY_POP

#endif // _SYNCOLOR_BIT_DEPTH_H
