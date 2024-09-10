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
//! \file synColor/optimizer.h
//!
//! \brief List of transform optimization levels.
//!

#ifndef _SYNCOLOR_OPT_FLAGS_H
#define _SYNCOLOR_OPT_FLAGS_H

#include <synColor/synExport.h>
#include <synColor/synColorNamespace.h>

SYN_VISIBILITY_PUSH

namespace SYNCOLOR
{
  //! \brief List of transform optimization levels.
  //!
  //! When finalizing a color transform, an optimization pass is performed on
  //! the list of operators within the transform. These flags control what
  //! actions the optimizer may perform.   The different levels provide different
  //! speed and accuracy trade-offs.
  //!
  //! All levels except the OPTIMIZER_NONE level remove the identity color operators.
  //! In the case where the transform being finalized has dynamic properties and
  //! these dynamic properties are needed, then the
  //! OPTIMIZER_KEEP_DYNAMIC_OPS_FLAG bit needs to be set.  For example:
  //!
  //! SYNCOLOR::OptimizerFlags optFlags = SYNCOLOR::OPTIMIZER_GOOD | SYNCOLOR::OPTIMIZER_KEEP_DYNAMIC_OPS_FLAG
  //!
  enum OptimizerFlags
  {
    OPTIMIZER_NONE        = 1000, //!< No transform optimizations applied.
    OPTIMIZER_LOSSLESS    =   90, //!< Removal of non-dynamic bypassed ops and
                                  //!< non-dynamic identity ops.  Composition of
                                  //!< consecutive matrix ops, and of clamp-only range
                                  //!< ops with the following 1D lut, 3D lut or range op.
    OPTIMIZER_VERY_GOOD   =   70, //!< Composition of consecutive 1D luts.
    OPTIMIZER_GOOD        =   40, //!< Composition of consecutive 3D luts.
    OPTIMIZER_DRAFT       =   10, //!< For quite lossy optimizations.  Reserved for future implementation.


    //! If set, this flag indicates that the transform should never be added to the internal cache
    //! (e.g. two finalizations could return the same finalized transform if the cache is used). 
    //! A typical case is a transform from a view template where dynamic properties exist. 
    //! As the caller blindly consumes the resulting finalized transform (not knowing if it's shared or not) 
    //! a dynamic property change could affect several transforms at his point of view.
    OPTIMIZER_NOT_ADD_TO_CACHE = 0x40000000,

    //! If set, this flag indicates to the optimizer to preserve dynamic ops
    //! that report being identities.
    OPTIMIZER_KEEP_DYNAMIC_OPS_FLAG = 0x80000000
  };

};

SYN_VISIBILITY_POP

#endif // _SYNCOLOR_OPT_FLAGS_H
