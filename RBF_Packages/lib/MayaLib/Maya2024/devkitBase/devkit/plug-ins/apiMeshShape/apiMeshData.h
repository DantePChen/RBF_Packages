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
// Provides a data type for some arbitrary user geometry.
// 
// A users geometry class can exist in the DAG by creating an
// MPxSurfaceShape (and UI) class for it and can also be passed through
// DG connections by creating an MPxGeometryData class for it.
// 
// MPxGeometryData is the same as MPxData except it provides 
// additional methods to modify the geometry data via an iterator.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _apiMeshData
#define _apiMeshData

#include <maya/MPxGeometryData.h>
#include <maya/MTypeId.h>
#include <maya/MString.h>
#include "apiMeshGeom.h"

#include <iosfwd>

class apiMeshData : public MPxGeometryData
{
public:
	//////////////////////////////////////////////////////////////////
	//
	// Overrides from MPxData
	//
	//////////////////////////////////////////////////////////////////

	apiMeshData();
	~apiMeshData() override;

	MStatus			readASCII( const MArgList& argList, unsigned& idx ) override;
	MStatus			readBinary( std::istream& in, unsigned length ) override;
	MStatus			writeASCII( std::ostream& out ) override;
	MStatus			writeBinary( std::ostream& out ) override;

		void			copy ( const MPxData& ) override;

	MTypeId         typeId() const override;
	MString         name() const override;

	//////////////////////////////////////////////////////////////////
	//
	// Overrides from MPxGeometryData
	//
	//////////////////////////////////////////////////////////////////

	MPxGeometryIterator* iterator( MObjectArray & componentList,
											MObject & component,
											bool useComponents) override;
	MPxGeometryIterator* iterator( MObjectArray & componentList,
											MObject & component,
											bool useComponents,
											bool world) const override;

	bool	updateCompleteVertexGroup( MObject & component ) const override;

	//////////////////////////////////////////////////////////////////
	//
	// Helper methods
	//
	//////////////////////////////////////////////////////////////////

	MStatus					readVerticesASCII( const MArgList&, unsigned& );
	MStatus					readNormalsASCII( const MArgList&, unsigned& );
	MStatus					readFacesASCII( const MArgList&, unsigned& );
	MStatus				    readUVASCII( const MArgList&, unsigned& ); 

	MStatus					writeVerticesASCII( std::ostream& out );
	MStatus					writeNormalsASCII( std::ostream& out );
	MStatus					writeFacesASCII( std::ostream& out );
	MStatus					writeUVASCII( std::ostream& out );

	static void * creator();

public:
	static const MString typeName;
	static const MTypeId id;

	// This is the geometry our data will pass though the DG
	//
	apiMeshGeom* fGeometry;
};

#endif /* apimeshData */
