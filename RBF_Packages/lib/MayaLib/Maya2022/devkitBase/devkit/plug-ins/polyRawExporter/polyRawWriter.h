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

#ifndef __POLYRAWWRITER_H
#define __POLYRAWWRITER_H

// polyRawWriter.h

//
// *****************************************************************************
//
// CLASS:    polyRawWriter
//
// *****************************************************************************
//
// CLASS DESCRIPTION (polyRawWriter)
// 
// polyRawWriter is a class derived from polyWriter.  It currently outputs in
// raw text format the following polygonal mesh data:
// - faces and their vertex components
// - vertex coordinates
// - colors per vertex
// - normals per vertex
// - current uv set and coordinates
// - component sets
// - file textures (for the current uv set)
// - other uv sets and coordinates
//
// *****************************************************************************

#include "polyWriter.h"

#include <iosfwd>

//Used to store UV set information
//
struct UVSet {
	MFloatArray	uArray;
	MFloatArray	vArray;
	MString		name;
	UVSet*		next;
};

class polyRawWriter : public polyWriter {

	public:
						polyRawWriter (const MDagPath& dagPath, MStatus& status);
					~polyRawWriter () override;
				MStatus extractGeometry () override;
				MStatus writeToFile (std::ostream& os) override;

	private:
		//Functions
		//
				MStatus	outputSingleSet (std::ostream& os, 
										 MString setName, 
										 MIntArray faces, 
										 MString textureName) override;
				MStatus outputFaces (std::ostream& os);
				MStatus outputVertices (std::ostream& os);
				MStatus	outputVertexInfo (std::ostream& os);
				MStatus	outputNormals (std::ostream& os);
				MStatus	outputTangents (std::ostream& os);
				MStatus	outputBinormals (std::ostream& os);
				MStatus	outputColors (std::ostream& os);
				MStatus	outputUVs (std::ostream& os);

		//Data Member
		//for storing UV information
		//
		UVSet*	fHeadUVSet;
};

#endif /*__POLYRAWWRITER_H*/
