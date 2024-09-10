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
//! \file synColor/synColorFactory.h
//!
//! \brief Transform and Template factories.
//!

#ifndef _SYNCOLOR_FACTORY_H
#define _SYNCOLOR_FACTORY_H

#include <synColor/synExport.h>

#include <synColor/bitdepth.h>
#include <synColor/synColor.h>
#include <synColor/half.h>
#include <synColor/basePath.h>
#include <synColor/template.h>
#include <synColor/templateTypes.h>
#include <synColor/stockTemplate.h>
#include <synColor/stockTransform.h>
#include <synColor/transformTypes.h>

SYN_VISIBILITY_PUSH

namespace SYNCOLOR
{ 
  //! \brief Create a color transform from CDL SOP and Sat values.
  //!
  //! Utility factory method that builds a CDL color transform from the
  //! Slope, Offset, Power and Saturation values provided.
  //!
  //! The ASC spec v1.2 imposes the following restrictions:
  //!
  //! slope >= 0, power > 0, sat >= 0, (offset unbounded).  The parameters are validated and
  //! an error will be returned if any of them are illegal.
  //!
  //! \param inBitDepth Input bit depth.  See SYNCOLOR::BitDepth.
  //! \param outBitDepth Output bit depth. See SYNCOLOR::BitDepth.
  //! \param ccId Color correction identifier string
  //! \param slope Slope RGB array values
  //! \param offset Offset RGB array values
  //! \param power Power RGB array values
  //! \param saturation Saturation value
  //! \param style CDL style to use
  //! \param pTransform Returned transform.
  //!
  //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error occurred.
  //!
  SYN_EXPORT SynStatus CreateCDLTransform(BitDepth inBitDepth,
                                          BitDepth outBitDepth,
                                          const char* ccId,
                                          const double slope[],
                                          const double offset[],
                                          const double power[],
                                          const double saturation,
                                          CDLStyle style,
                                          TransformPtr& pTransform);
  
  //! \brief Create a color transform containing a single 3x 1D LUT operator from short values.
  //!
  //! Utility factory method that builds a 1D LUT color transform from the
  //! lists of unsigned short coefficients provided.
  //!
  //! \param inBitDepth Input bit depth.  See SYNCOLOR::BitDepth.
  //! \param outBitDepth Output bit depth. See SYNCOLOR::BitDepth.
  //! \param lutSize Number of coefficients in each of the r, g, and b LUT
  //!                arrays.
  //! \param rLut 1D LUT for the red component.
  //! \param gLut 1D LUT for the green component.
  //! \param bLut 1D LUT for the blue component.
  //! \param halfFlags Half Flags for 1D LUT content.  See SYNCOLOR::HalfFlags
  //! \param pTransform Returned transform.
  //!
  //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error occurred.
  //!
  SYN_EXPORT SynStatus Create1DLutTransformFromShort(BitDepth inBitDepth,
                                                     BitDepth outBitDepth,
                                                     unsigned lutSize,
                                                     const unsigned short rLut[],
                                                     const unsigned short gLut[],
                                                     const unsigned short bLut[],
                                                     HalfFlags halfFlags,
                                                     TransformPtr& pTransform);

  //! \brief Invert a half-domain 1D Lut.
  //!
  //! Utility factory method that that calculates the the inverse of a 1D LUT
  //! that has a half-domain (input depth == 16f, output depth == 16i).
  //! The returned LUT will lookup a 16i value and return a raw half float.
  //!
  //! \param rLut 1D LUT for the red component (65536 entries).
  //! \param gLut 1D LUT for the green component (65536 entries).
  //! \param bLut 1D LUT for the blue component (65536 entries).
  //! \param rInvLut 1D LUT for the inverted red component (65536 entries).
  //! \param gInvLut 1D LUT for the inverted green component (65536 entries).
  //! \param bInvLut 1D LUT for the inverted blue component (65536 entries).
  //!
  //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error occurred.
  //!
  SYN_EXPORT SynStatus InvertHalfDomain1DLutFromShort(const unsigned short rLut[],
                                                      const unsigned short gLut[],
                                                      const unsigned short bLut[],
                                                      unsigned short rInvLut[],
                                                      unsigned short gInvLut[],
                                                      unsigned short bInvLut[]);

  //! \brief Create a color transform containing a single 3x 1D LUT operator from integer values.
  //!
  //! Utility factory method that builds a 1D LUT color transform from the
  //! lists of unsigned integer coefficients provided.
  //!
  //! \param inBitDepth Input bit depth.  See SYNCOLOR::BitDepth.
  //! \param outBitDepth Output bit depth. See SYNCOLOR::BitDepth.
  //! \param lutSize Number of coefficients in each of the r, g, and b LUT
  //!                arrays.
  //! \param rLut 1D LUT for the red component.
  //! \param gLut 1D LUT for the green component.
  //! \param bLut 1D LUT for the blue component.
  //! \param halfFlags Half Flags for 1D LUT content.  See SYNCOLOR::HalfFlags
  //! \param pTransform Returned transform.
  //!
  //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error occurred.
  //!
  SYN_EXPORT SynStatus Create1DLutTransformFromInt(BitDepth inBitDepth,
                                                   BitDepth outBitDepth,
                                                   unsigned lutSize,
                                                   const unsigned int rLut[],
                                                   const unsigned int gLut[],
                                                   const unsigned int bLut[],
                                                   HalfFlags halfFlags,
                                                   TransformPtr& pTransform);

  //! \brief Create a color transform containing a single 3x 1D LUT operator from float values.
  //!
  //! Utility factory method that builds a 1D LUT color transform from the
  //! lists of floating point coefficients provided.
  //!
  //! \param inBitDepth Input bit depth.  See SYNCOLOR::BitDepth.
  //! \param outBitDepth Output bit depth. See SYNCOLOR::BitDepth.
  //! \param lutSize Number of coefficients in each of the r, g, and b LUT
  //!                arrays.
  //! \param rLut 1D LUT for the red component.
  //! \param gLut 1D LUT for the green component.
  //! \param bLut 1D LUT for the blue component.
  //! \param halfFlags Half Flags for 1D LUT content.  See SYNCOLOR::HalfFlags
  //! \param pTransform Returned transform.
  //! \brief Create a color transform containing a single 3x 1D LUT.
  //!
  //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error occurred.
  //!
  SYN_EXPORT SynStatus Create1DLutTransformFromFloat(BitDepth inBitDepth,
                                                    BitDepth outBitDepth,
                                                    unsigned lutSize,
                                                    const float rLut[],
                                                    const float gLut[],
                                                    const float bLut[],
                                                    HalfFlags halfFlags,
                                                    TransformPtr& pTransform);

  //! \brief Create a color transform containing a single 3D LUT operator from short values.
  //!
  //! Utility factory method that builds a 3D LUT color transform from the
  //! lists of unsigned short coefficients provided.
  //!
  //! \param inBitDepth Input bit depth.  See SYNCOLOR::BitDepth.
  //! \param outBitDepth Output bit depth. See SYNCOLOR::BitDepth.
  //! \param dim LUT axis dimension.
  //! \param data RGB output value array. Linear set of RGB values organized with increasing B,
  //!             then G, then R index (blue index changes the fastest).
  //! \param pTransform Returned transform.
  //!
  //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error occurred.
  //!
  SYN_EXPORT SynStatus Create3DLutTransformFromShort(BitDepth inBitDepth,
                                                     BitDepth outBitDepth,
                                                     unsigned dim,
                                                     const unsigned short* data,
                                                     TransformPtr& pTransform);

  //! \brief Create a color transform containing a single 3D LUT operator from integer values.
  //!
  //! Utility factory method that builds a 3D LUT color transform from the
  //! lists of unsigned integer coefficients provided.
  //!
  //! \param inBitDepth Input bit depth.  See SYNCOLOR::BitDepth.
  //! \param outBitDepth Output bit depth. See SYNCOLOR::BitDepth.
  //! \param dim LUT axis dimension.
  //! \param data RGB output value array. Linear set of RGB values organized with increasing B,
  //!             then G, then R index (blue index changes the fastest).
  //! \param pTransform Returned transform.
  //!
  //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error occurred.
  //!
  SYN_EXPORT SynStatus Create3DLutTransformFromInt(BitDepth inBitDepth,
                                                  BitDepth outBitDepth,
                                                  unsigned dim,
                                                  const unsigned int* data,
                                                  TransformPtr& pTransform);

  //! \brief Create a color transform containing a single 3D LUT operator from float values.
  //!
  //! Utility factory method that builds a 3D LUT color transform from the
  //! lists of floating point coefficients provided.
  //!
  //! \param inBitDepth Input bit depth.  See SYNCOLOR::BitDepth.
  //! \param outBitDepth Output bit depth. See SYNCOLOR::BitDepth.
  //! \param dim LUT axis dimension.
  //! \param data RGB output value array. Linear set of RGB values organized with increasing B,
  //!             then G, then R index (blue index changes the fastest).
  //! \param pTransform Returned transform.
  //!
  //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error occurred.
  //!
  SYN_EXPORT SynStatus Create3DLutTransformFromFloat(BitDepth inBitDepth,
                                                     BitDepth outBitDepth,
                                                     unsigned dim,
                                                     const float* data,
                                                     TransformPtr& pTransform);

  //! \brief Create a color transform containing a single matrix operator.
  //!
  //! Utility factory method that builds a matrix color transform from the
  //! float coefficients provided.
  //!
  //! \param inBitDepth Input bit depth.  See SYNCOLOR::BitDepth.
  //! \param outBitDepth Output bit depth. See SYNCOLOR::BitDepth.
  //! \param mtxCoeffs Matrix coefficient array.  This array needs to be row dominant, i.e.
  //!                  coefficients are ordered by row.
  //! \param offsetCoeffs Offset coefficient array.
  //! \param isRGBA Whether the coefficient arrays are RGB or RGBA.  If _true_ the _mtxCoeffs_
  //!               must have 16 elements and the _offsetCoeffs_ must have 4.  Otherwise
  //!               _mtxCoeffs_ must have 9 elements and _offsetCoeffs_ must have 3,
  //! \param pTransform Returned transform.
  //!
  //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error occurred.
  //!
  SYN_EXPORT SynStatus CreateMatrixTransform(BitDepth inBitDepth,
                                            BitDepth outBitDepth,
                                            const float* mtxCoeffs,
                                            const float* offsetCoeffs,
                                            bool isRGBA,
                                            TransformPtr& pTransform);

  //! \brief Create a color transform containing a single dither operator.
  //!
  //! Utility factory method that builds a dither transform.
  //!
  //! \param inBitDepth Input bit depth.  See SYNCOLOR::BitDepth.
  //! \param outBitDepth Output bit depth. See SYNCOLOR::BitDepth.
  //! \param ditherBitDepth Dither bit depth, i.e. bit depth to which dithering will be performed.
  //! \param pTransform Returned transform.
  //!
  //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error occurred.
  //!
  SYN_EXPORT SynStatus CreateDitherTransform(BitDepth inBitDepth,
                                             BitDepth outBitDepth,
                                             BitDepth ditherBitDepth,
                                             TransformPtr& pTransform);

  //! \brief Create a color transform containing a single alias reference operator.
  //!
  //! Utility factory method that builds a color transform that references an alias.
  //!
  //! \param inBitDepth Input bit depth.  See SYNCOLOR::BitDepth.
  //! \param outBitDepth Output bit depth. See SYNCOLOR::BitDepth.
  //! \param alias Alias name.  Must exist in the SynColor preferences.
  //! \param pTransform Returned transform.
  //!
  //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error occurred.
  //!
  //! \sa SYNCOLOR::findAliasInPreferences, SYNCOLOR::addAliasInPreferences,
  //!
  SYN_EXPORT SynStatus CreateReferenceTransformFromAlias(
      BitDepth inBitDepth,
      BitDepth outBitDepth,
      const char* alias,
      TransformPtr& pTransform);

  //! \brief Create a color transform containing a single file path reference operator.
  //!
  //! Utility factory method that builds a color transform that references a file.
  //!
  //! \param inBitDepth Input bit depth.  See SYNCOLOR::BitDepth.
  //! \param outBitDepth Output bit depth. See SYNCOLOR::BitDepth.
  //! \param path File path to the transform to reference.
  //! \param pTransform Returned transform.
  //! \param basePath Base path to use if _path_ is relative.  See SYNCOLOR::BasePath.
  //!
  //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error occurred.
  //!
  SYN_EXPORT SynStatus CreateReferenceTransformFromPath(
      BitDepth inBitDepth,
      BitDepth outBitDepth,
      const char* path,
      BasePath basePath,
      TransformPtr& pTransform);

  //! \brief Concatenate two color transforms.
  //
  //! \param pTransform1 First transform.
  //! \param pTransform2 Second transform.
  //! \param pTransform  Returned transform containing a copy of both transforms in sequence.
  //!
  //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error occurred.
  //!
  SYN_EXPORT SynStatus concatenateTransforms(const TransformPtr& pTransform1,
                                             const TransformPtr& pTransform2,
                                             TransformPtr& pTransform);

  //! \brief Create one of the available stock transforms.
  //
  //! \param type Stock transform type. See SYNCOLOR::StockTransform.
  //! \param pTransform  Returned transform.
  //
  //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error occurred.
  //!
  SYN_EXPORT SynStatus createStockTransform(StockTransform type,
                                            TransformPtr& pTransform);

  //! \brief Create a native color transform template of the specified type.
  //!
  //! \param type Template type. See SYNCOLOR::TemplateType.
  //! \param pTemplate Returned template instance.
  //!
  //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error occurred.
  //!
  //! \sa Detailed description of SYNCOLOR::Template.
  //!
  SYN_EXPORT SynStatus loadNativeTemplate(TemplateType type, TemplatePtr& pTemplate);

  //! \brief Create a stock transform template of the specified type.
  //!
  //! \param type Stock template type.  See SYNCOLOR::StockTemplate.
  //! \param pTemplate Returned template instance.
  //!
  //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error occurred.
  //!
  //! \sa Detailed description of SYNCOLOR::Template.
  //!
  SYN_EXPORT SynStatus loadStockTemplate(StockTemplate type, TemplatePtr& pTemplate);

  //! \brief Create a transform template from an OpenColorIO configuration file.
  //!
  //! \param type Template type. See SYNCOLOR::TemplateType.
  //! \param filename Configuration file name.
  //! \param pTemplate Returned template instance.
  //!
  //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error occurred.
  //!
  //! \sa Detailed description of SYNCOLOR::Template.
  //!
  SYN_EXPORT SynStatus loadOCIOTemplate(TemplateType type, const char* filename, 
                                        TemplatePtr& pTemplate);
};

SYN_VISIBILITY_POP

#endif
