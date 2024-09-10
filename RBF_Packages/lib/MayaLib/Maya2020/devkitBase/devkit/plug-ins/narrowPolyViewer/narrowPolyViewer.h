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
//	File Name: narrowPolyViewer.h
//
//	Description:
//		A simple test of the MPx3dModelView code.
//		A view that allows multiple cameras to be added is made.
//

#ifndef NARROWPOLYVIEWER_H
#define NARROWPOLYVIEWER_H

#include <maya/MDGModifier.h>

#include <maya/MPx3dModelView.h>
#include <maya/M3dView.h>
#include <maya/MDagPath.h>
#include <maya/MDagPathArray.h>
#include <maya/MStringArray.h>

#define kTypeName "narrowPolyViewer"

class narrowPolyViewer: public MPx3dModelView
{
public:
	narrowPolyViewer();
	~narrowPolyViewer() override;

	MString 	viewType() const override;

	virtual	MStatus		setCameraList(const MDagPathArray &cameraList);
	virtual MStatus		getCameraList(MDagPathArray &cameraList) const;
	virtual MStatus		getCameraList(MStringArray &cameraList) const;

	virtual MStatus		appendCamera(const MDagPath &camera);
	virtual MStatus		removeCamera(const MDagPath &camera);
	virtual MStatus		removeAllCameras();

	MString		getCameraHUDName() override;

	virtual MStatus		setIsolateSelect(MSelectionList &list);
	virtual MStatus		setIsolateSelectOff();

	void		removingCamera(MDagPath &cameraPath) override;

	virtual MStatus		swap(MPx3dModelView &src);
	virtual MStatus		copy(MPx3dModelView &src);

	MStatus				setLightTest(MSelectionList &list);

	static void*		creator();

	double tol;

//	Public data used to report the conditions of the last refresh
//
	MStringArray		fTestCameraList;

protected:
    void        	preMultipleDraw() override;
    void        	postMultipleDraw() override;
    void        	preMultipleDrawPass(unsigned index) override;
    void        	postMultipleDrawPass(unsigned index) override;
    bool        	okForMultipleDraw(const MDagPath &) override;
    unsigned    	multipleDrawPassCount() override;

	MDagPath				fOldCamera;
	MDagPathArray			fCameraList;
	unsigned				fCurrentPass;
	bool					fDrawManips;
	M3dView::DisplayStyle	fOldDisplayStyle;

	bool					fLightTest;
	MDagPathArray			fLightList;
private:
	static const char*  className() { return "narrowPolyViewer"; }
};
#endif
