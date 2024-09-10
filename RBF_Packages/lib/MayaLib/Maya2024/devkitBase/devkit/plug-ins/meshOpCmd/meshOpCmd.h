#ifndef _meshOpCmd
#define _meshOpCmd

//-
// ==========================================================================
// Copyright 1995,2006,2008 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
// ==========================================================================
//+

//
// ***************************************************************************
//
// Overview:
//
//		The purpose of the meshOp command is to execute a selected mesh 
//      operation, such as edge split or face subdivision on one or
//      more objects.
//
// How it works:
//
//		This command is based on the polyModifierCmd. It relies on the 
//		polyModifierCmd to manage "how" the effects of the meshOperation 
//		operation are applied (ie. directly on the mesh or through a modifier
//      node). See polyModifierCmd.h for more details
//
//		To understand the algorithm behind the meshOp operation,
//      refer to meshOpFty.h.
//
// Limitations:
//
//		(1) Can only operate on a single mesh at a given time. If there are 
//			more than one mesh with selected components, only the first mesh 
//			found in the selection list is operated on.
//

#include "polyModifierCmd.h"
#include "meshOpFty.h"

// Function Sets
//
#include <maya/MFnComponentListData.h>

// Forward Class Declarations
//

class meshOp : public polyModifierCmd
{

public:
	////////////////////
	// Public Methods //
	////////////////////

				meshOp();
			~meshOp() override;

	static		void* creator();

	bool		isUndoable() const override;

	MStatus		doIt( const MArgList& ) override;
	MStatus		redoIt() override;
	MStatus		undoIt() override;

	/////////////////////////////
	// polyModifierCmd Methods //
	/////////////////////////////

	MStatus		initModifierNode( MObject modifierNode ) override;
	MStatus		directModifier( MObject mesh ) override;

private:
	//////////////////
	// Private Data //
	//////////////////

	// Selected Components
	// Selected Operation
	//
	MObject						fComponentList;
	MIntArray					fComponentIDs;
	MeshOperation				fOperation;

	// meshOp Factory
	//
	meshOpFty				fmeshOpFactory;
};

#endif
