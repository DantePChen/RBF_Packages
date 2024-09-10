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

////////////////////////////////////////////////////////////////////////////////
//
// Point iterator for control-point based geometry
//
// This is used by the translate/rotate/scale manipulators to 
// determine where to place the manipulator when components are
// selected.
//
// As well deformers use this class to deform points of the shape.
//
////////////////////////////////////////////////////////////////////////////////

#include <maya/MPxGeometryIterator.h>
#include <maya/MPoint.h>
#include "apiMeshGeom.h"

class apiMeshGeomIterator : public MPxGeometryIterator
{
public:
	apiMeshGeomIterator( void * userGeometry, MObjectArray & components );
	apiMeshGeomIterator( void * userGeometry, MObject & components );
    ~apiMeshGeomIterator() override;

    //////////////////////////////////////////////////////////
	//
	// Overrides
	//
    //////////////////////////////////////////////////////////

	void		reset() override;
	MPoint		point() const override;
	void		setPoint( const MPoint & ) const override;
	int			iteratorCount() const override;
	bool        hasPoints() const override;

public:
	apiMeshGeom * 		geometry;
};
