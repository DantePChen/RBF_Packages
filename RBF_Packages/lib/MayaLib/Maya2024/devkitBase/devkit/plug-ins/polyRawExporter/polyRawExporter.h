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

#ifndef __POLYRAWEXPORTER_H
#define __POLYRAWEXPORTER_H

// polyRawExporter.h

// *****************************************************************************
//
// CLASS:    polyRawExporter
//
// *****************************************************************************
//
// CLASS DESCRIPTION (polyRawExporter)
// 
// polyRawExporter is a class derived from polyExporter.  It allows the export
// of polygonal mesh data in raw text format.  The file extension for this type
// is ".raw".
//
// *****************************************************************************

#include "polyExporter.h"

#include <iosfwd>

class polyRawExporter : public polyExporter {

	public:
								polyRawExporter(){}
							~polyRawExporter() override;

		static	void*			creator();
				MString			defaultExtension () const override;
				MStatus			initializePlugin(MObject obj);
				MStatus			uninitializePlugin(MObject obj);


	private:	
				polyWriter*		createPolyWriter(const MDagPath dagPath, MStatus& status) override;
				void			writeHeader(std::ostream& os) override;
};

#endif /*__POLYEXPORTER_H*/
