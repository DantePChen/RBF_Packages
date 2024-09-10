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
//! \file synColor/stockTemplate.h
//!
//! \brief List of available stock templates.
//!

#ifndef _SYNCOLOR_STOCK_TEMPLATE_H
#define _SYNCOLOR_STOCK_TEMPLATE_H

#include <synColor/synExport.h>
#include <synColor/synColorNamespace.h>

SYN_VISIBILITY_PUSH

namespace SYNCOLOR
{
  //! \brief List of available stock templates.
  //!
  //! \sa SYNCOLOR::loadStockTemplate()
  //!
  enum StockTemplate
  {
    STOCK_TEMPLATE_RAW //!< An identity template for returning a raw transform.
                       //!< \sa SYNCOLOR::STOCK_TRANSFORM_RAW.
  };
};

SYN_VISIBILITY_POP

#endif // _SYNCOLOR_BIT_DEPTH_H
