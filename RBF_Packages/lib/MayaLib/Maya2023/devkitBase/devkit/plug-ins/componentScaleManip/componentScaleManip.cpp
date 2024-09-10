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
// This example demonstrates the scale manipulator in the API. This example 
// uses three classes to accomplish this task: First, a context command 
// (componentScaleContext) is provided to create instances of the context.  
// Next, a custom selection context (ComponentScaleContext) is created to 
// manage the component scale manipulator. Finally, the component scale  
// manipulator is provided as a custom node class.
//
// Loading and unloading:
// ----------------------
//
// The component scale manipulator context can be created with the following
// MEL commands:
//
//	componentScaleContext;
//	setToolTo componentScaleContext1;
//
// If the preceding commands were used to create the manipulator context, 
// the following commands can destroy it:
//
//	deleteUI componentScaleContext1;
//	deleteUI componentScaleManip;
//
// If the plug-in is loaded and unloaded frequently (such as during testing),
// it is useful to make these command sequences into shelf buttons.
//
// To create the tool button for the plug-in: 
//	
//	setParent Shelf1;
//	toolButton -cl toolCluster -t componentScaleContext1 -i1 "moveManip.xpm";
//
// How to use:
// -----------
//
// Once the tool button has been created using the script above, select the
// tool button, and then select some object components. If non-components are 
// selected, warning messages will be printed indicating that no manipulatable
// objects are selected. Once components are selected, the scale manipulator
// will appear at the centroid of the components. Then, the manipulator can
// be used much like the built-in scale manipulator.
//
// Currently, this plug-in only works correctly for NURBS surfaces where the 
// parametization has the distance 1.0 corresponding to the distance between CVs.
// This will be the case for NURBS spheres, torus, and cones. Planes and
// cylinders do not have this property.
//
// Note that undo and redo are not supported in this example plug-in.
//	
////////////////////////////////////////////////////////////////////////

#include <maya/MIOStream.h>
#include <stdio.h>
#include <stdlib.h>

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
#include <maya/MSelectionList.h>
#include <maya/MItSurfaceCV.h>
#include <maya/MFnComponent.h>

// Manipulators
#include <maya/MFnScaleManip.h>

// This function is a utility that can be used to extract vector values from
// plugs.
//
MVector vectorPlugValue(const MPlug& plug) {
	if (plug.numChildren() == 3)
	{
		double x,y,z;
		MPlug rx = plug.child(0);
		MPlug ry = plug.child(1);
		MPlug rz = plug.child(2);
		rx.getValue(x);
		ry.getValue(y);
		rz.getValue(z);
		MVector result(x,y,z);
		return result;
	}
	else {
		MGlobal::displayError("Expected 3 children for plug "+MString(plug.name()));
		MVector result(0,0,0);
		return result;
	}
}

/////////////////////////////////////////////////////////////
//
// componentScaleManip
//
// This class implements the component scaling example.
//
/////////////////////////////////////////////////////////////

class componentScaleManip : public MPxManipContainer
{
public:
	componentScaleManip();
	~componentScaleManip() override;
	
	static void * creator();
	static MStatus initialize();
	MStatus createChildren() override;
	MStatus connectToDependNode(const MObject &node) override;

	void draw(M3dView &view, 
					  const MDagPath &path, 
					  M3dView::DisplayStyle style,
					  M3dView::DisplayStatus status) override;

	void setSurfaceDagPath(const MDagPath& dagPath) {
		surfaceDagPath = dagPath;
	}

	void setComponentObject(const MObject& obj) {
		component = obj;
	}

	// Virtual handlers
	MManipData manipToPlugConversion(unsigned index) override;
	MManipData plugToManipConversion(unsigned index) override;

	MDagPath fScaleManip;

public:
	static MTypeId id;

private:

	MDagPath surfaceDagPath;
	MObject component;

	MPoint centroid;

	int numComponents;
	MPoint* initialPositions;
	MPoint* initialControlPoint;

	unsigned dummyPlugIndex;
};


MTypeId componentScaleManip::id( 0x80021 );

componentScaleManip::componentScaleManip() : numComponents(0)
,	initialPositions(NULL)
,	initialControlPoint(NULL)
{ 
	// The constructor must not call createChildren for user-defined
	// manipulators.
}


componentScaleManip::~componentScaleManip() 
{
	// initialPositions should always be either NULL or an allocated array
	// of MPoint.
	//
	delete [] initialPositions;
	delete [] initialControlPoint;
}


void *componentScaleManip::creator()
{
	 return new componentScaleManip();
}


MStatus componentScaleManip::initialize()
{ 
	MStatus stat;
	stat = MPxManipContainer::initialize();
	return stat;
}


MStatus componentScaleManip::createChildren()
{
	MStatus stat = MStatus::kSuccess;

	fScaleManip = addScaleManip("scaleManip", "scale");

	return stat;
}


MStatus componentScaleManip::connectToDependNode(const MObject &node)
{
	MStatus stat;
	
	MFnComponent componentFn(component);
	if (componentFn.elementCount() > numComponents)
	{
		delete [] initialPositions;
		delete [] initialControlPoint;
		numComponents = componentFn.elementCount();
		initialPositions = new MPoint[numComponents];
		initialControlPoint = new MPoint[numComponents];
	}

	// Iterate through the components, storing initial positions and find the
	// centroid.  Add manipToPlug callbacks for each component.
	//
	int i = 0;
	for (MItSurfaceCV iter(surfaceDagPath, component); !iter.isDone(); iter.next(), i++)
	{
		if (i >= numComponents)
		{
			MGlobal::displayWarning("More components found than expected.");
			break;
		}
		initialPositions[i] = iter.position(MSpace::kWorld);
		centroid += initialPositions[i];

		//
		// Create a manipToPlug callback that is invoked whenever the manipVal
		// changes.  The callback is added for every selected CV.
		//

		MFnDependencyNode nodeFn(node);
		MPlug controlPointArrayPlug = nodeFn.findPlug("controlPoints",  true,  &stat);
		if (controlPointArrayPlug.isNull())
		{
			MGlobal::displayError("Control point plug not found on node.");
			return MS::kFailure;
		}

		// The logical index from the component iterator is the same as the
		// logical index into the control points array.
		//
		MPlug controlPointPlug = 
			controlPointArrayPlug.elementByLogicalIndex(iter.index(), &stat);
		if (controlPointPlug.isNull())
		{
			MGlobal::displayError("Control point element not found.");
			return MS::kFailure;
		}

		// Store the initial value of the control point.
		//
		initialControlPoint[i] = vectorPlugValue(controlPointPlug);

		unsigned plugIndex = addManipToPlugConversion(controlPointPlug);

		// Plug indices should be allocated sequentially, starting at 0.  Each
		// manip container will have its own set of plug indices.  This code 
		// relies on the sequential allocation of indices, so trigger an error
		// if the indices are found to be different.
		//
		if (plugIndex != (unsigned)i)
		{
			MGlobal::displayError("Unexpected plug index returned.");
			return MS::kFailure;
		}
	}
	centroid = centroid / numComponents;

	MFnScaleManip scaleManip(fScaleManip);

	// Create a plugToManip callback that places the manipulator at the 
	// centroid of the CVs.
	//
	addPlugToManipConversion(scaleManip.scaleCenterIndex());

	finishAddingManips();
	MPxManipContainer::connectToDependNode(node);
	return stat;
}


void componentScaleManip::draw(M3dView & view, 
					 const MDagPath & path, 
					 M3dView::DisplayStyle style,
					 M3dView::DisplayStatus status)
{
	MPxManipContainer::draw(view, path, style, status);
}

MManipData componentScaleManip::plugToManipConversion(unsigned index) {
	MObject obj = MObject::kNullObj;

	// If we entered the callback with an invalid index, print an error and
	// return.  Since we registered the callback only for one index, all 
	// invocations of the callback should be for that index.
	//
	MFnScaleManip scaleManip(fScaleManip);
	if (index == scaleManip.scaleCenterIndex())
	{
		// Set the center point for scaling to the centroid of the CV's.
		//
		MFnNumericData numericData;
		obj = numericData.create( MFnNumericData::k3Double );
		numericData.setData(centroid.x,centroid.y,centroid.z);

		return MManipData(obj);
	}

	MGlobal::displayError("Invalid index in plugToManipConversion()!");

	// For invalid indices, return vector of 0's
	MFnNumericData numericData;
	obj = numericData.create( MFnNumericData::k3Double );
	numericData.setData(0.0,0.0,0.0);

	return obj;
}

MManipData componentScaleManip::manipToPlugConversion(unsigned index) {
	MObject obj = MObject::kNullObj;

	MFnScaleManip scaleManip(fScaleManip);
	if (index < (unsigned)numComponents)
	{
		//
		// Now we need to determine the scaled position of the CV specified by 
		// index.
		//

		MVector scaleVal;
		getConverterManipValue(scaleManip.scaleIndex(), scaleVal);

		// Determine the vector from the centroid to the CV
		//
		MVector positionVec = initialPositions[index] - centroid;

		// Scale the vector
		//
		MVector newPosition(positionVec.x*scaleVal.x, 
			positionVec.y*scaleVal.y, positionVec.z*scaleVal.z);

		// Form the vector from the initial position to the new position.
		//
		newPosition = newPosition - positionVec;

		// Move the control point from the initial control point position along
		// the vector.  Control point positions are always measured relative to
		// the initial position of the control point, which is why a separate 
		// array of control point positions is required.
		//
		newPosition += initialControlPoint[index];

		MFnNumericData numericData;
		obj = numericData.create( MFnNumericData::k3Double );
		numericData.setData(newPosition.x,newPosition.y,newPosition.z);

		return MManipData(obj);
	}

	// If we entered the handler with an invalid index, print an error and
	// return.  The callback should only be called for indices from 0 to
	// numComponents-1.
	//

	MGlobal::displayError("Invalid index in scale changed callback!");

	// For invalid indices, return vector of 0's
	MFnNumericData numericData;
	obj = numericData.create( MFnNumericData::k3Double );
	numericData.setData(0.0,0.0,0.0);

	return obj;
	
}

/////////////////////////////////////////////////////////////
//
// ComponentScaleContext
//
// This class is a simple context for supporting a scale manipulator.
//
/////////////////////////////////////////////////////////////

class ComponentScaleContext : public MPxSelectionContext
{
public:
	ComponentScaleContext();
	void	toolOnSetup(MEvent &event) override;
	void	toolOffCleanup() override;

	// Callback issued when selection list changes
	static void updateManipulators(void * data);

private:
	MCallbackId id1;
};

ComponentScaleContext::ComponentScaleContext()
{
	MString str("Plugin component scaling manipulator");
	setTitleString(str);
}


void ComponentScaleContext::toolOnSetup(MEvent &)
{
	MString str("Scale the selected components");
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


void ComponentScaleContext::toolOffCleanup()
{
	MStatus status;
	status = MModelMessage::removeCallback(id1);
	if (!status) {
		MGlobal::displayError("Model remove callback failed");
	}
	MPxContext::toolOffCleanup();
}


void ComponentScaleContext::updateManipulators(void * data)
{
	MStatus stat = MStatus::kSuccess;
	
	ComponentScaleContext * ctxPtr = (ComponentScaleContext *) data;
	ctxPtr->deleteManipulators(); 

	MSelectionList list;
	stat = MGlobal::getActiveSelectionList(list);
	MItSelectionList iter(list, MFn::kInvalid, &stat);

	if (MS::kSuccess == stat) {
		for (; !iter.isDone(); iter.next()) {

			// Make sure the selection list item is a dag path with components
			// before attaching the manipulator to it.
			//
			MDagPath dagPath;
			MObject components;
			iter.getDagPath(dagPath, components);

			if (components.isNull() || !components.hasFn(MFn::kComponent))
			{
				MGlobal::displayWarning("Object in selection list is not "
					"a component.");
				continue;
			}

			// Add manipulator to the selected object
			//
			MString manipName ("componentScaleManip");
			MObject manipObject;
			componentScaleManip* manipulator =
				(componentScaleManip *) componentScaleManip::newManipulator(manipName, manipObject);

			if (NULL != manipulator) {
				// Add the manipulator
				//
				ctxPtr->addManipulator(manipObject);

				// Connect the manipulator to the object in the selection list.
				//
				manipulator->setSurfaceDagPath(dagPath);
				manipulator->setComponentObject(components);
				if (!manipulator->connectToDependNode(dagPath.node()))
				{
					MGlobal::displayWarning("Error connecting manipulator to"
						" object");
				}
			} 
		}
	}
}


/////////////////////////////////////////////////////////////
//
// componentScaleContext
//
// This is the command that will be used to create instances
// of our context.
//
/////////////////////////////////////////////////////////////

class componentScaleContext : public MPxContextCommand
{
public:
	componentScaleContext() {};
	MPxContext * makeObj() override;

public:
	static void* creator();
};

MPxContext *componentScaleContext::makeObj()
{
	return new ComponentScaleContext();
}

void *componentScaleContext::creator()
{
	return new componentScaleContext;
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

	status = plugin.registerContextCommand("componentScaleContext",
										   &componentScaleContext::creator);
	if (!status) {
		MGlobal::displayError("Error registering componentScaleContext command");
		return status;
	}

	status = plugin.registerNode("componentScaleManip", componentScaleManip::id, 
								 &componentScaleManip::creator, &componentScaleManip::initialize,
								 MPxNode::kManipContainer);
	if (!status) {
		MGlobal::displayError("Error registering componentScaleManip node");
		return status;
	}

	return status;
}


MStatus uninitializePlugin(MObject obj)
{
	MStatus status;
	MFnPlugin plugin(obj);

	status = plugin.deregisterContextCommand("componentScaleContext");
	if (!status) {
		MGlobal::displayError("Error deregistering componentScaleContext command");
		return status;
	}

	status = plugin.deregisterNode(componentScaleManip::id);
	if (!status) {
		MGlobal::displayError("Error deregistering componentScaleManip node");
		return status;
	}

	return status;
}
