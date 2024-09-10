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
//! \file synColor/stockTransform.h
//!
//! \brief List of available stock transforms.
//!

#ifndef _SYNCOLOR_STOCK_TRANSFORM_H
#define _SYNCOLOR_STOCK_TRANSFORM_H

#include <synColor/synExport.h>


SYN_VISIBILITY_PUSH

namespace SYNCOLOR
{
  //! \brief List of available stock transforms.
  //!
  //! \sa SYNCOLOR::createStockTransform()
  //!
  enum StockTransform
  {
    STOCK_TRANSFORM_RAW,      //!< An identity transform with dynamic exposure contrast controls.
    STOCK_TRANSFORM_IDENTITY  //!< An identity transform.
  };
};

SYN_VISIBILITY_POP

#endif // _SYNCOLOR_BIT_DEPTH_H
