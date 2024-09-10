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
//!
//! \file synColorNamespace.h
//!
//! \brief Versioned SynColor namespace definition.
//!
//! All SynColor SDK methods and classes are part of the SYNCOLOR namespace.
//! In order to ease integration of newer versions of SynColor and allow
//! coexistence of various versions of SynColor in the same runtime the
//! SYNCOLOR name is re-defined to a versioned name.
//!
//! \note When integrating a new version of the SynColor library, it is 
//!       required to recompile with the corresponding set of SynColor SDK
//!       header files.
//! 
#ifndef _SYNCOLOR_NAMESPACE_H
#define _SYNCOLOR_NAMESPACE_H

//! SynColor versioned namespace definition.
#define SYNCOLOR SynColor_2018_0_81

#endif // _SYNCOLOR_NAMESPACE_H
