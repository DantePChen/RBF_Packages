#ifndef _clew_h
#define _clew_h

/*

	Include everything we need to handle the OpenCL extensions we are using.  CLEW
	doesn't have extension support, so we need to add everything we case about here.

	The basic steps for each method are:

	Header file:
	 - declare the function pointer type.  This should exactly match the OpenCL function.
	 - declare a variable of the defined type to hold the entry point into OpenCL
	 - define a macro which exactly matches the OpenCL function name which instead uses the function pointer

	cpp file:
	 - initialize the function pointer variable to NULL
	 - in an init method, initialize the function pointer variable to the proper value

	I will follow the same naming conventions that clew uses:
	 - function pointers called PFNCLFUNCTIONNAME

	Everything defined here should exactly match the signature of OpenCL functions,
	please don't try to do anything beyond that.  If you want to do something more,
	do it in another file.

*/
#include <clew/clew_cl.h>
#include <clew/clew_cl_platform.h>
#include <clew/clew_cl_gl.h>
#include <clew/clew_cl_gl_ext.h>
#include <clew/clew_cl_d3d11.h>
#include <clew/clew_cl_d3d11_ext.h>

/**********************************************************************************
 * Copyright (c) 2008-2012 The Khronos Group Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and/or associated documentation files (the
 * "Materials"), to deal in the Materials without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Materials, and to
 * permit persons to whom the Materials are furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Materials.
 *
 * THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * MATERIALS OR THE USE OR OTHER DEALINGS IN THE MATERIALS.
 **********************************************************************************/

/* $Revision: 11708 $ on $Date: 2010-06-13 23:36:24 -0700 (Sun, 13 Jun 2010) $ */

//-
// ==================================================================
// Copyright 2014 Autodesk, Inc.  All rights reserved.
// 
// This computer source code  and related  instructions and comments are
// the unpublished confidential and proprietary information of Autodesk,
// Inc. and are  protected  under applicable  copyright and trade secret
// law. They may not  be disclosed to, copied or used by any third party
// without the prior written consent of Autodesk, Inc.
// ==================================================================
//+

#endif // _clew_extensions_h
