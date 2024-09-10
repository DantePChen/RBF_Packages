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
//
//! \file synColor/pixelFormat.h
//!
//! \brief List of pixel formats and related utilities.
//!

#ifndef _SYNCOLOR_PIXELFORMAT_H
#define _SYNCOLOR_PIXELFORMAT_H

#include <synColor/synExport.h>
#include <synColor/synColorNamespace.h>

SYN_VISIBILITY_PUSH

namespace SYNCOLOR
{
  // see synColor/roi.h
  class ROI;

  //! \brief List of pixel formats.
  //!
  //! The formatting for the pixel format naming is:
  //! PF_<Pixel Layout>_<Bit Depth>
  //!
  //! A mask can be used to extract the SYNCOLOR::BitDepth from the
  //! PixelFormat.  See SYNCOLOR::BitDepthHelper.
  //! 
  enum PixelFormat 
  {
    PF_UNKNOWN   = 0x0,

    PF_RGB_8i    = 0x01017,
    PF_RGB_10i   = 0x02017,
    PF_RGB_10iP  = 0x02117,
    PF_RGB_12i   = 0x04017,
    PF_RGB_16i   = 0x08017,
    PF_RGB_16f   = 0x10017,
    PF_RGB_32f   = 0x20017,

    PF_RGBA_8i   = 0x0101F,
    PF_RGBA_10i  = 0x0201F,
    PF_RGBA_10iP = 0x0211F,   // Format is deprecated and will be removed
    PF_RGBA_12i  = 0x0401F,
    PF_RGBA_16i  = 0x0801F,
    PF_RGBA_16f  = 0x1001F,
    PF_RGBA_32f  = 0x2001F,

    PF_BGR_8i    = 0x01027,

// (Commenting out formats not currently used, but may be needed in the future.)
//     PF_BGR_10i   = 0x02027,
//     PF_BGR_12i   = 0x04027,
//     PF_BGR_16i   = 0x08027,
//     PF_BGR_16f   = 0x10027,
//     PF_BGR_32f   = 0x20027,
// 
    PF_BGRA_8i   = 0x0102F,
//     PF_BGRA_10i  = 0x0202F,
//     PF_BGRA_12i  = 0x0402F,
    PF_BGRA_16i  = 0x0802F,
//     PF_BGRA_16f  = 0x1002F,
//     PF_BGRA_32f  = 0x2002F,
// 
    PF_ABGR_8i   = 0x0104F,
//     PF_ABGR_10i  = 0x0204F,
//     PF_ABGR_12i  = 0x0404F,
//     PF_ABGR_16i  = 0x0804F,
//     PF_ABGR_16f  = 0x1004F,
//     PF_ABGR_32f  = 0x2004F
  };

  //! \brief Convert a string to a pixel format value.
  //!
  //! \param pfStr String to evaluate.
  //! \return Pixel format for 'pfStr', or PF_UNKNOWN if the string is not
  //!         recognized.
  //!       
  SYN_EXPORT PixelFormat strToPF(const char* pfStr);

  //! \brief Convert a pixel format to its corresponding string.
  //!
  //! \param pf Pixel format value.
  //! \return String corresponding to 'pf'.
  //!       
  SYN_EXPORT const char* pfToStr(PixelFormat pf);

  //! \brief Get the size in bytes of a region of interest (ROI) given the
  //!        PixelFormat.
  //!
  //! \param roi Image region of interest.
  //! \param pf Pixel format value
  //! \return Size of the region in bytes
  //!       
  SYN_EXPORT unsigned getImageSizeInByte(const ROI& roi, PixelFormat pf);

  //! \brief Get the pixel size of the PixelFormat
  //!
  //! \param pf Pixel format value.
  //!
  //! \return Pixel size in bytes.
  //!
  SYN_EXPORT unsigned getPixelSizeInByte(PixelFormat pf);

  //! \brief Get the number of channels of the PixelFormat
  //!
  //! \param pf Pixel format value.
  //!
  //! \return Number of channels.
  SYN_EXPORT unsigned getNumChannels(PixelFormat pf);
}

SYN_VISIBILITY_POP

#endif //_SYNCOLOR_PIXELFORMAT_H
