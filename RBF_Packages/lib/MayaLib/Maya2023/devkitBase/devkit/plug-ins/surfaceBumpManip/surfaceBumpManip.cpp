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

////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
// 
// This example demonstrates how to use the PointOnSurface manipulator in the
// API. This example uses three classes to accomplish this task: First, a 
// context command (surfaceBumpContext) is provided to create instances of 
// the context. Next, a custom selection context (SurfaceBumpContext) is 
// created to manage the surface manipulator. Finally, the surface 
// manipulator is provided as a custom node class.
//	
// Loading and unloading:
//
// The surface bump manipulator context can be created with the following 
// MEL commands:
//
//	surfaceBumpContext;
//	setToolTo surfaceBumpContext1;
//
// If the preceding commands were used to create the manipulator context, 
// the following commands can destroy it:
//
//	deleteUI surfaceBumpContext1;
//	deleteUI surfaceBumpManip;
//
// If the plugin is loaded and unloaded frequently (such as during testing),
// it is useful to make these command sequences into shelf buttons.
//
// To create the tool button for the plug-in:
//	(1) Create a new shelf named "Shelf1".
//	(2) Execute the following MEL commands to create the tool button in this shelf:
//
//	surfaceBumpContext;
//	setParent Shelf1;
//	toolButton -cl toolCluster -t surfaceBumpContext1 -i1 "moveManip.xpm";
//
// How to use:
//
// Select the tool button, and then click on a NURBS surface object. The PointOnSurface 
// manipulator should appear on the object. Then, drag the manipulator over
// the surface of the object and a bump should appear on the CV nearest to 
// the manipulator. If the bump is too small, try adjusting the BUMP_SCALE
// value and recompile.
//	
////////////////////////////////////////////////////////////////////////

#include <maya/MIOStream.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <maya/MFn.h>
#include <maya/MPxNode.h>
#include <maya/MPxManipContainer.h>
#include <maya/MPxSelectionContext.h>
#include <maya/MPxContextCommand.h>
#include <maya/MModelMessage.h>
#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>
#include <maya/MItSelectionList.h>
#include <maya/MPoint.h>
#include <maya/MVector.h>
#include <maya/MDagPath.h>
#include <maya/MManipData.h>

#include <maya/MItDependencyNodes.h>
#include <maya/MItDependencyGraph.h>
#include <maya/MFnNurbsSurface.h>
#include <maya/MFnNumericAttribute.h>

// Manipulators
#include <maya/MFnPointOnSurfaceManip.h>

// BUMP_SCALE is the factor indicating how far the bump created by the 
// manipulator should protrude from the object surface.  The value is treated
// as a distance in object space.  Note that this value is how far the CV is
// moved away from the surface, not the actual distance that the surface is
// moved at the point.
//
static const double BUMP_SCALE = 0.5;

// This is a utility method used to find the surface plug that should be used
// to connect a pointOnSurface manipulator to a surface.  The method takes the
// input node (which must be a DAG node whose child is the nurbs shape) and
// finds the nurbs shape node (shape) and the surface plug (plug).
//
MStatus findSurfacePlug(const MObject& node, MPlug& plug, MObject& shape) {
	if (!node.hasFn(MFn::kDagNode))
	{
		MGlobal::displayError("Nodes passed to findSurfacePlug must be DAG"
			" nodes");
		return MS::kFailure;
	}

	MFnDagNode nodeFn(node);
	if (nodeFn.childCount() != 1)
	{
		MGlobal::displayError("DAG node must have 1 child");
		return MS::kFailure;
	}

	shape = nodeFn.child(0);
	if (!shape.hasFn(MFn::kNurbsSurface))
	{
		MGlobal::displayError("Child node is not a nurbs surface");
		shape = MObject::kNullObj;
		return MS::kFailure;
	}

	// Find the connection to the create attribute on the shape node.
	//
	MFnDependencyNode shapeNode(shape);
	plug = shapeNode.findPlug("create", true);

	return MS::kSuccess;
}

/////////////////////////////////////////////////////////////
//
// surfaceBumpManip
//
// This class implements the example pointOnSurface manipulator
//
/////////////////////////////////////////////////////////////

class surfaceBumpManip : public MPxManipContainer
{
public:
	surfaceBumpManip();
	~surfaceBumpManip() override;
	
	static void * creator();
	static MStatus initialize();
	MStatus createChildren() override;
	MStatus connectToDependNode(const MObject &node) override;

	void draw(M3dView &view, 
					  const MDagPath &path, 
					  M3dView::DisplayStyle style,
					  M3dView::DisplayStatus status) override;

	// Callback function
	MManipData surfacePointChangedCallback(unsigned index);

public:
	static MTypeId id;

private:
	MDagPath fPointOnSurfaceManip;
	MObject fSurfaceShape;

	// Previous parameters
	int saved_u,saved_v;
	// Previous position of the CV
	MPoint savedPoint;

	unsigned dummyPlugIndex;
};


MTypeId surfaceBumpManip::id( 0x80023 );

surfaceBumpManip::surfaceBumpManip() : saved_u(-1),saved_v(-1)
{ 
	// The constructor must not call createChildren for user-defined
	// manipulators.
}

surfaceBumpManip::~surfaceBumpManip() 
{
}


void *surfaceBumpManip::creator()
{
	 return new surfaceBumpManip();
}


MStatus surfaceBumpManip::initialize()
{
	return MPxManipContainer::initialize();
}


MStatus surfaceBumpManip::createChildren()
{
	MStatus stat = MStatus::kSuccess;

	// Add the pointOnSurface manip
	//
	fPointOnSurfaceManip = addPointOnSurfaceManip("surfaceBumpManip", "point");
	MFnPointOnSurfaceManip pointOnSurfaceManip(fPointOnSurfaceManip);
	pointOnSurfaceManip.setDrawSurface(true);
	pointOnSurfaceManip.setDrawArrows(true);
	pointOnSurfaceManip.setParameters(0.0,0.0);
	
	return stat;
}


MStatus surfaceBumpManip::connectToDependNode(const MObject &node)
{
	MStatus stat;

	MFnPointOnSurfaceManip pointOnSurfaceManip(fPointOnSurfaceManip);

	// Connect to the nurbs surface plug.  The surface plug indicates the 
	// surface that should be used as the source for the uv manipulation.
	//
	MPlug surfacePlug;
	findSurfacePlug(node, surfacePlug, fSurfaceShape);
	if (!surfacePlug.isNull())
	{
		MGlobal::displayInfo(MString("Using surface plug: ") + surfacePlug.name());
		stat = pointOnSurfaceManip.connectToSurfacePlug(surfacePlug);
		if (stat != MStatus::kSuccess) {
			MGlobal::displayError("Could not connect surface plug");
			return stat;
		}
	}
	else {
		MGlobal::displayError("Error finding surface plug");
		return MS::kFailure;
	}

	// Add a dynamic attribute to the node to be computed by the manipToPlug 
	// conversion function.  For this manipulator, the manipToPlug conversion 
	// function is used as a callback so this plug is just a placeholder.
	//
	// Note that this plugin leaves the dummy plugs on the nodes it manipulates.
	//
	MFnDependencyNode nodeFn(node);
	MPlug dummyPlug = nodeFn.findPlug("dummyPlug",  true,  &stat);
	if (dummyPlug.isNull())
	{
		MFnNumericAttribute attributeFn;
		MObject attr = attributeFn.create("dummyPlug", "dp", MFnNumericData::k3Double);
		nodeFn.addAttribute(attr);
		dummyPlug = nodeFn.findPlug("dummyPlug", true, &stat);
		if (dummyPlug.isNull())
		{
			MGlobal::displayError("Could not find dummyPlug on the manipulator.");
			return MS::kFailure;
		}
	}

	// Create a manipToPlug callback for the param plug.  For now, the plug
	// that is computed is irrelevant (we only care about the callback being
	// invoked), so we pass in the dummy plug.
	//
	dummyPlugIndex = addManipToPlugConversionCallback(dummyPlug, 
		(manipToPlugConversionCallback) 
		&surfaceBumpManip::surfacePointChangedCallback);

	// Retrieve the transformation from the object and set it in the 
	// manipulator.  This allows the manipulator to display with the object
	// it is manipulating.
	//
	MFnTransform transform(node);
	MTransformationMatrix matrix = transform.transformation();
	pointOnSurfaceManip.set(matrix);

	finishAddingManips();
	MPxManipContainer::connectToDependNode(node);
	return stat;
}


void surfaceBumpManip::draw(M3dView & view, 
					 const MDagPath & path, 
					 M3dView::DisplayStyle style,
					 M3dView::DisplayStatus status)
{
	// Uses default manipulator drawing to draw the pointOnSurface manip
	//
	MPxManipContainer::draw(view, path, style, status);
}

MManipData surfaceBumpManip::surfacePointChangedCallback(unsigned index) {

	// Always return (0.0,0.0,0.0) to the dummy plug.
	//
	MFnNumericData numericData;
	MObject obj = numericData.create( MFnNumericData::k3Double );
	numericData.setData(0.0,0.0,0.0);

	if (index != dummyPlugIndex)
	{
		MGlobal::displayError("Invalid index in surface point changed callback!");
		return obj;
	}

	MFnNurbsSurface nurbsSurface(fSurfaceShape);

	// Get the parameter values from the node
	//
	double u = 0.0;
	double v = 0.0;
	MFnPointOnSurfaceManip pointOnSurfaceManip(fPointOnSurfaceManip);
	pointOnSurfaceManip.getParameters(u,v);

	// Uncomment to see the parameters that are received by the callback.
/*
	MGlobal::displayInfo(MString("Num CVs are: ") + 
		nurbsSurface.numSpansInU() + "," + nurbsSurface.numSpansInV());
	MGlobal::displayInfo(MString("Parameters are: ") + u + "," + v);
*/

	//
	// Snap the parameters to the nearest CV.  This code assumes that CVs are 
	// a unit distance apart in both the U and V directions.
	//

	int u_int = 0;
	int v_int = 0;
	if (nurbsSurface.formInU() == MFnNurbsSurface::kPeriodic)
	{
		u_int = (int)(floor(u+1.5)) % nurbsSurface.numSpansInU();
		if (u_int < 0)
			u_int += nurbsSurface.numSpansInU();
	}
	else {
		u_int = (int)(floor(u+1.5));
	}

	if (nurbsSurface.formInV() == MFnNurbsSurface::kPeriodic)
	{
		v_int = (int)(floor(v+1.5)) % nurbsSurface.numSpansInV();
		if (v_int < 0)
			v_int += nurbsSurface.numSpansInV();
	}
	else {
		v_int = (int)(floor(v+1.5));
	}

	// Uncomment to see the CVs that were determined from the parameters.
/*
	MGlobal::displayInfo(MString("Int params are: ") + u_int + "," + v_int);
*/

	// Is it the same CV?
	if (u_int == saved_u && v_int == saved_v)
	{
		return MManipData(obj);
	}

	// Is it the first one?  We need a special case to initialize the class
	// variables.
	if (saved_u == -1)
	{
		nurbsSurface.getCV(u_int,v_int,savedPoint,MSpace::kObject);
		saved_u = u_int;
		saved_v = v_int;
	}

	// Set the old CV back
	nurbsSurface.setCV(saved_u,saved_v,savedPoint,MSpace::kObject);

	// Get the position for this CV and store the u and v values
	nurbsSurface.getCV(u_int,v_int,savedPoint,MSpace::kObject);
	saved_u = u_int;
	saved_v = v_int;

	// Move the CV out by the normal
	MPoint perturbedPosition = savedPoint + 
		BUMP_SCALE*nurbsSurface.normal((double)u_int,(double)v_int,MSpace::kObject);

	nurbsSurface.setCV(u_int,v_int,perturbedPosition,MSpace::kObject);
	
	return MManipData(obj);
}

/////////////////////////////////////////////////////////////
//
// SurfaceBumpContext
//
// This class is a simple context for supporting a pointOnSurface manip.
//
/////////////////////////////////////////////////////////////

class SurfaceBumpContext : public MPxSelectionContext
{
public:
	SurfaceBumpContext();
	void	toolOnSetup(MEvent &event) override;
	void	toolOffCleanup() override;

	// Callback issued when selection list changes
	static void updateManipulators(void * data);

private:
	MCallbackId id1;
};

SurfaceBumpContext::SurfaceBumpContext()
{
	MString str("Plugin Surface Bump Manipulator");
	setTitleString(str);
}


void SurfaceBumpContext::toolOnSetup(MEvent &)
{
	MString str("Drag the manipulator around the surface");
	setHelpString(str);

	updateManipulators(this);
	MStatus status;
	id1 = MModelMessage::addCallback(MModelMessage::kActiveListModified,
									 updateManipulators, 
									 this, &status);
	if (!status) {
		MGlobal::displayError("Model addCallback failed");
	}
}


void SurfaceBumpContext::toolOffCleanup()
{
	MStatus status;
	status = MModelMessage::removeCallback(id1);
	if (!status) {
		MGlobal::displayError("Model remove callback failed");
	}
	MPxContext::toolOffCleanup();
}


void SurfaceBumpContext::updateManipulators(void * data)
{
	MStatus stat = MStatus::kSuccess;
	
	SurfaceBumpContext * ctxPtr = (SurfaceBumpContext *) data;
	ctxPtr->deleteManipulators(); 

	MSelectionList list;
	stat = MGlobal::getActiveSelectionList(list);
	MItSelectionList iter(list, MFn::kInvalid, &stat);

	if (MS::kSuccess == stat) {
		for (; !iter.isDone(); iter.next()) {

			// Make sure the selection list item is a depend node and has the
			// required plugs before manipulating it.
			//
			MObject dependNode;
			iter.getDependNode(dependNode);
			if (dependNode.isNull() || !dependNode.hasFn(MFn::kDependencyNode))
			{
				MGlobal::displayWarning("Object in selection list is not "
					"a depend node.");
				continue;
			}

			// Add manipulator to the selected object
			//
			MString manipName ("surfaceBumpManip");
			MObject manipObject;
			surfaceBumpManip* manipulator =
				(surfaceBumpManip *) surfaceBumpManip::newManipulator(
					manipName, 
					manipObject);

			if (NULL != manipulator) {
				// Add the manipulator
				//
				ctxPtr->addManipulator(manipObject);

				// Connect the manipulator to the object in the selection list.
				//
				if (!manipulator->connectToDependNode(dependNode))
				{
					MFnDependencyNode dependNodeFn(dependNode);
					MGlobal::displayWarning("Error connecting manipulator to"
						" object: " + dependNodeFn.name());
				}
			} 
		}
	}
}


/////////////////////////////////////////////////////////////
//
// surfaceBumpContext
//
// This is the command that will be used to create instances
// of our context.
//
/////////////////////////////////////////////////////////////

class surfaceBumpContext : public MPxContextCommand
{
public:
	surfaceBumpContext() {};
	MPxContext * makeObj() override;

public:
	static void* creator();
};

MPxContext *surfaceBumpContext::makeObj()
{
	return new SurfaceBumpContext();
}

void *surfaceBumpContext::creator()
{
	return new surfaceBumpContext;
}


///////////////////////////////////////////////////////////////////////
//
// The following routines are used to register/unregister
// the context and manipulator
//
///////////////////////////////////////////////////////////////////////

MStatus initializePlugin(MObject obj)
{
	MStatus status;
	MFnPlugin plugin(obj, PLUGIN_COMPANY, "6.0", "Any");

	status = plugin.registerContextCommand("surfaceBumpContext",
										   &surfaceBumpContext::creator);
	if (!status) {
		MGlobal::displayError("Error registering surfaceBumpContext command");
		return status;
	}

	status = plugin.registerNode("surfaceBumpManip", surfaceBumpManip::id, 
								 &surfaceBumpManip::creator, &surfaceBumpManip::initialize,
								 MPxNode::kManipContainer);
	if (!status) {
		MGlobal::displayError("Error registering surfaceBumpManip node");
		return status;
	}

	return status;
}


MStatus uninitializePlugin(MObject obj)
{
	MStatus status;
	MFnPlugin plugin(obj);

	status = plugin.deregisterContextCommand("surfaceBumpContext");
	if (!status) {
		MGlobal::displayError("Error deregistering surfaceBumpContext command");
		return status;
	}

	status = plugin.deregisterNode(surfaceBumpManip::id);
	if (!status) {
		MGlobal::displayError("Error deregistering surfaceBumpManip node");
		return status;
	}

	return status;
}
