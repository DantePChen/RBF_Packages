//-
// ==========================================================================
// Copyright 2015 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
// ==========================================================================
//+

///////////////////////////////////////////////////////////////////////////////
//
// File Name: geometryCacheBlockBase.cpp
//
// Description : 
//		A base class used for storing and outputing a cache block tag and data.
//
///////////////////////////////////////////////////////////////////////////////

// Project includes
//
#include "geometryCacheBlockBase.h"

// Maya includes
//
#include <maya/MIffTag.h>
#include <iostream>

///////////////////////////////////////////////////////////////////////////////
//
// Methods
//
///////////////////////////////////////////////////////////////////////////////

geometryCacheBlockBase::geometryCacheBlockBase()
///////////////////////////////////////////////////////////////////////////////
//
// Description : ( public method )
//		Constructor
//
///////////////////////////////////////////////////////////////////////////////
{
	blockTag = "";
	group = false;
}

geometryCacheBlockBase::geometryCacheBlockBase( const MString& tag )
	: blockTag(tag)
///////////////////////////////////////////////////////////////////////////////
//
// Description : ( public method )
//		Overloaded Constructor
//
///////////////////////////////////////////////////////////////////////////////
{
	if( tag == "CACH" ||
		tag == "/CACH" ||
		tag == "MYCH" ||
		tag == "/MYCH" )
		group = true;
	else
		group = false;
}

geometryCacheBlockBase::~geometryCacheBlockBase()
///////////////////////////////////////////////////////////////////////////////
//
// Description : ( public method )
//		Destructor
//
///////////////////////////////////////////////////////////////////////////////
{
}

const bool& geometryCacheBlockBase::isGroup()
///////////////////////////////////////////////////////////////////////////////
//
// Description : ( public method )
//		Returns a boolean that indicates if this block is a group block.
//
///////////////////////////////////////////////////////////////////////////////
{
	return group;
}

const MString& geometryCacheBlockBase::tag()
///////////////////////////////////////////////////////////////////////////////
//
// Description : ( public method )
//		Returns the block tag as an MString
//
///////////////////////////////////////////////////////////////////////////////
{
	return blockTag;
}

void geometryCacheBlockBase::outputToAscii( std::ostream& os )
///////////////////////////////////////////////////////////////////////////////
//
// Description : ( public method )
//		Outputs the data of this block to Ascii
//
///////////////////////////////////////////////////////////////////////////////
{
	MString tabs = "";
	if( !group )
		tabs = "\t";

	os << tabs << "[" << blockTag << "]\n";
}
