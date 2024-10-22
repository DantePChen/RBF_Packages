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
// 
// customAttrManip.cpp
// 
// This plug-in demonstrates how to create user-defined manipulators
// from a user-defined context.
//
// This is the script for running this plug-in:
// 
// loadPlugin "customAttrManip.so";
// source "customAttrManip.mel";
// sphere;
// move 5 0 0;
// cone;
// move -5 0 0;
// select -cl;
// 
// Now click on the customAttrManip on Shelf1!
// 
////////////////////////////////////////////////////////////////////////

#include <maya/MIOStream.h>
#include <stdio.h>
#include <stdlib.h>

#include <maya/MFn.h>
#include <maya/MPxNode.h>
#include <maya/MPxManipContainer.h> 
#include <maya/MFnDistanceManip.h> 
#include <maya/MFnFreePointTriadManip.h> 
#include <maya/MFnDiscManip.h> 
#include <maya/MPxContext.h>
#include <maya/MPxSelectionContext.h>
#include <maya/MModelMessage.h>

#include <maya/MPxToolCommand.h>
#include <maya/MFnPlugin.h>
#include <maya/MArgList.h>
#include <maya/MGlobal.h>
#include <maya/MItSelectionList.h>
#include <maya/MPoint.h>
#include <maya/MVector.h>
#include <maya/MDagPath.h>

#include <maya/MFnTransform.h>
#include <maya/MItCurveCV.h>
#include <maya/MItSurfaceCV.h>
#include <maya/MItMeshVertex.h>

#include <maya/MPxSelectionContext.h>
#include <maya/MPxContextCommand.h>
#include <maya/M3dView.h>
#include <maya/MFnCamera.h>

#include <maya/MFnUnitAttribute.h>
#include <maya/MDistance.h>

#include <maya/MManipData.h>
#include <maya/MEulerRotation.h>
#include <maya/MQuaternion.h>


#define 	customAttribute1String	"customManip1"
#define 	customAttribute2String	"customManip2"
#define 	customAttribute3String	"customManip3"

#define 	scaleAttributeString	"customScaleAttr"

#define CHECKRESULT(stat,msg)     \
    if (MS::kSuccess != stat) {   \
        cerr << msg << endl;      \
    }

#ifdef LATER
	#define ADDMANIPFUNC		addDiscManip
	#define CUSTOMMANIPTYPE		MFnDiscManip
	#define MANIPDIRECTIONFUNC		setNormal
	#define MANIPPLUGCONN		connectToAnglePlug
#else
	#define ADDMANIPFUNC		addDistanceManip
	#define CUSTOMMANIPTYPE		MFnDistanceManip
	#define MANIPDIRECTIONFUNC		setDirection
	#define MANIPPLUGCONN		connectToDistancePlug
#endif

class customAttrManip : public MPxManipContainer
{
public:
    customAttrManip();
    ~customAttrManip() override;
    
    static void * creator();
    static MStatus initialize();
    MStatus createChildren() override;
    MStatus connectToDependNode(const MObject &node) override;

	MStatus doPress() override;
	MStatus doDrag() override;
	MStatus doRelease() override;

	void draw(M3dView &view, 
						  const MDagPath &path, 
					  M3dView::DisplayStyle style,
					  M3dView::DisplayStatus status) override;

	MVector 	nodeTranslation() const;
	MQuaternion 	nodeRotation() const;

	void updateManipLocations();

    MDagPath fManip1;
    MDagPath fManip2;
    MDagPath fManip3;

	MDagPath fNodePath;

public:
    static MTypeId id;
};


MTypeId customAttrManip::id( 0x80020 );

customAttrManip::customAttrManip() 
{ 
    // Do not call createChildren from here - 
    // MayaPtr has not been set up yet.
}


customAttrManip::~customAttrManip() 
{
}


void *customAttrManip::creator()
{
     return new customAttrManip();
}


MStatus customAttrManip::initialize()
{ 
    MStatus stat;
    stat = MPxManipContainer::initialize();
    return stat;
}

// Do nothing special for now
MStatus  customAttrManip::doPress()
{
	// This is the safe definition as the base class, so it could be removed
	// It's simply here to show what return value is needed to force Maya
	// to also handle the event
	return MS::kUnknownParameter;
}

// Do nothing special for now
MStatus  customAttrManip::doDrag()
{
	return MS::kUnknownParameter;
}

//
// Snap the manip to the object's new location, since the custom Attrs
// may have somehow moved it
//
MStatus  customAttrManip::doRelease()
{
	// Custom release handling
	updateManipLocations();

	// Let Maya do it's work too
	return MS::kUnknownParameter;
}

MStatus customAttrManip::createChildren()
{
    MStatus stat = MStatus::kSuccess;

    MString pointManipName1("customtManip1");
    MString pointName1("customPoint1");
    fManip1 = ADDMANIPFUNC(pointManipName1, pointName1);

    MString pointManipName2("customtManip2");
    MString pointName2("customPoint2");
    fManip2 = ADDMANIPFUNC(pointManipName2, pointName2);

    MString pointManipName3("customtManip3");
    MString pointName3("customPoint3");
    fManip3 = ADDMANIPFUNC(pointManipName3, pointName3);

    return stat;
}

MQuaternion customAttrManip::nodeRotation() const
{
	//
	//  Since this is a manip, we're working directly on the transform
	//  no need to move up the dag path, which is done for locators
	//
	
	MFnDagNode dagFn(fNodePath);
	MDagPath path;
	dagFn.getPath(path);
	MFnTransform transformFn(path);
	MQuaternion q;
	transformFn.getRotation( q, MSpace::kWorld );
	return q;
}

MVector customAttrManip::nodeTranslation() const
{
	//
	//  Since this is a manip, we're working directly on the transform
	//  no need to move up the dag path, which is done for locators
	//
	
	MFnDagNode dagFn(fNodePath);
	MDagPath path;
	dagFn.getPath(path);
	MFnTransform transformFn(path);
	return transformFn.getTranslation(MSpace::kWorld);
}

void customAttrManip::updateManipLocations()
{
	MVector  trans = nodeTranslation();
	MQuaternion  q = nodeRotation();

	CUSTOMMANIPTYPE freePointManipFn1(fManip1);
	CUSTOMMANIPTYPE freePointManipFn2(fManip2);
	CUSTOMMANIPTYPE freePointManipFn3(fManip3);

	MVector vecX(1.0, 0.0, 0.0);
	MVector vecY(0.0, 1.0, 0.0);
	MVector vecZ(0.0, 0.0, 1.0);

	freePointManipFn1.MANIPDIRECTIONFUNC(vecX);
	freePointManipFn1.rotateBy(q);
	freePointManipFn1.setTranslation(trans, MSpace::kWorld);

	freePointManipFn2.MANIPDIRECTIONFUNC(vecY);
	freePointManipFn2.rotateBy(q);
	freePointManipFn2.setTranslation(trans, MSpace::kWorld);

	freePointManipFn3.MANIPDIRECTIONFUNC(vecZ);
	freePointManipFn3.rotateBy(q);
	freePointManipFn3.setTranslation(trans, MSpace::kWorld);
}

MStatus customAttrManip::connectToDependNode(const MObject &node)
{
    MStatus stat;

	// Get the DAG path
	//
	MFnDagNode dagNodeFn(node);
	dagNodeFn.getPath(fNodePath);

    // Connect the plugs
    //    

    MFnDependencyNode nodeFn(node);    

	CUSTOMMANIPTYPE freePointManipFn1(fManip1);
	CUSTOMMANIPTYPE freePointManipFn2(fManip2);
	CUSTOMMANIPTYPE freePointManipFn3(fManip3);

    MPlug cPlug1 = nodeFn.findPlug(customAttribute1String,  true,  &stat);
	if( stat != MStatus::kSuccess )
	{
		cerr << "Can't find dynamic attr -- " << customAttribute1String << endl;
	}
	else
	{
		freePointManipFn1.MANIPPLUGCONN(cPlug1);
	}

    MPlug cPlug2 = nodeFn.findPlug(customAttribute2String,  true,  &stat);
	if( stat != MStatus::kSuccess )
	{
		cerr << "Can't find dynamic attr -- " << customAttribute1String << endl;
	}
	else
	{
		freePointManipFn2.MANIPPLUGCONN(cPlug2);
	}

    MPlug cPlug3 = nodeFn.findPlug(customAttribute3String,  true,  &stat);
	if( stat != MStatus::kSuccess )
	{
		cerr << "Can't find dynamic attr -- " << customAttribute1String << endl;
	}
	else
	{
		freePointManipFn3.MANIPPLUGCONN(cPlug3);
	}
    finishAddingManips();


    MPlug cPlug4 = nodeFn.findPlug(scaleAttributeString,  true,  &stat);
	if( stat != MStatus::kSuccess )
	{
		cerr << "Can't find dynamic attr -- " << scaleAttributeString << endl;
	}
	else
	{
		double factor;
		cPlug4.getValue( factor );

		freePointManipFn1.setScalingFactor( factor );
		freePointManipFn2.setScalingFactor( factor );
		freePointManipFn3.setScalingFactor( factor );
	}

	updateManipLocations();

    MPxManipContainer::connectToDependNode(node);        

    return stat;
}


void customAttrManip::draw(M3dView & view, 
					 const MDagPath & path, 
					 M3dView::DisplayStyle style,
					 M3dView::DisplayStatus status)
{ 
    MPxManipContainer::draw(view, path, style, status);
/*	
    view.beginGL(); 

    MPoint textPos(0, 0, 0);
    char str[100];
    sprintf(str, "Stretch Me!"); 
    MString distanceText(str);
    view.drawText(distanceText, textPos, M3dView::kLeft);

    view.endGL();
	*/
}



/////////////////////////////////////////////////////////////
//
// The customAttrManip Context
//
// - tool contexts are custom event handlers. The selection
//   context class defaults to maya's selection mode and
//   allows you to override press/drag/release events.
//
/////////////////////////////////////////////////////////////
#define MOVEHELPSTR		"Drag rings to change values on custom attributes"
#define MOVETITLESTR	"customAttrManip"


class customAttrCtx : public MPxSelectionContext
{
public:
    customAttrCtx();

    void    toolOnSetup(MEvent &event) override;
    void    toolOffCleanup() override;
    MStatus doEnterRegion(MEvent &event) override;

    customAttrManip * caManip;
};


void updateManipulators(void * data);
MCallbackId id1;


customAttrCtx::customAttrCtx()
{
    MString str(MOVETITLESTR);
    setTitleString(str);
}


void customAttrCtx::toolOnSetup(MEvent &)
{
    MString str(MOVEHELPSTR);
    setHelpString(str);

	updateManipulators(this);
	MStatus status;
	id1 = MModelMessage::addCallback(MModelMessage::kActiveListModified,
									 updateManipulators, 
									 this, &status);
	if (!status) {
		cerr << "Model addCallback failed\n";
	}
}


void customAttrCtx::toolOffCleanup()
{
    MStatus status;
	status = MModelMessage::removeCallback(id1);
	if (!status) {
		cerr << "Model remove callback failed\n";
	}
	MPxContext::toolOffCleanup();
}

void updateManipulators(void * data)
{
    //cerr << "\n\n -- Active List Changed callback -- " << endl;

    MStatus stat = MStatus::kSuccess;
	
	customAttrCtx * ctxPtr = (customAttrCtx *) data;
	ctxPtr->deleteManipulators(); 

    // iterate through the selected objects:
    // 
    MSelectionList list;
    stat = MGlobal::getActiveSelectionList(list);
    MItSelectionList iter(list, MFn::kInvalid, &stat);

    if (MS::kSuccess == stat) {
        for (; !iter.isDone(); iter.next()) {
            // create the customAttrManip for each object selected:
            //
            MString manipName ("customAttrManip");
            MObject manipObject;
            ctxPtr->caManip = (customAttrManip *) customAttrManip::newManipulator(manipName, manipObject);

            if (NULL != ctxPtr->caManip) {
			    ctxPtr->addManipulator(manipObject);

                MObject dependNode;
                iter.getDependNode(dependNode);
                MFnDependencyNode dependNodeFn(dependNode);

				//cerr << "Iterating on node " << dependNodeFn.name().asChar() << endl;

                dependNodeFn.findPlug(customAttribute1String,  true,  &stat);
                if (MStatus::kSuccess != stat) {
                    cerr << "dependNodeFn.findPlug failed\n";
					ctxPtr->deleteManipulators(); 
					return;
                }
                dependNodeFn.findPlug(customAttribute2String,  true,  &stat);
                if (MStatus::kSuccess != stat) {
                    cerr << "dependNodeFn.findPlug failed\n";
					ctxPtr->deleteManipulators(); 
					return;
                }
                dependNodeFn.findPlug(customAttribute3String,  true,  &stat);
                if (MStatus::kSuccess != stat) {
                    cerr << "dependNodeFn.findPlug failed\n";
					ctxPtr->deleteManipulators(); 
					return;
                }
                else {
                    //cerr << "dependNodeFn.findPlug succeed\n";
                }
                ctxPtr->caManip->connectToDependNode(dependNode);
            } 
        }
    }
}


MStatus customAttrCtx::doEnterRegion(MEvent &event)
//
// Print the tool description in the help line.
//
{
    MString str(MOVEHELPSTR);
    return setHelpString(str);
}


/////////////////////////////////////////////////////////////
//
// Context creation command
//
//  This is the command that will be used to create instances
//  of our context.
//
/////////////////////////////////////////////////////////////
#define CREATE_CTX_NAME "customAttrManipContext"


class customAttrCtxCommand : public MPxContextCommand
{
public:
    customAttrCtxCommand() {};
    MPxContext * makeObj() override;

public:
    static void* creator();
};


MPxContext *customAttrCtxCommand::makeObj()
{
    customAttrCtx *newC = new customAttrCtx();
	return newC;
}


void *customAttrCtxCommand::creator()
{
    return new customAttrCtxCommand;
}


///////////////////////////////////////////////////////////////////////
//
// The following routines are used to register/unregister
// the commands we are creating within Maya
//
///////////////////////////////////////////////////////////////////////
MStatus initializePlugin(MObject obj)
{
    MStatus status;
    MFnPlugin plugin(obj, PLUGIN_COMPANY, "3.0", "Any");

    status = plugin.registerContextCommand(CREATE_CTX_NAME,
										   &customAttrCtxCommand::creator);

    if (!status) 
	{
        status.perror("registerContextCommand");
        return status;
    }

    status = plugin.registerNode("customAttrManip", customAttrManip::id, 
								 &customAttrManip::creator, &customAttrManip::initialize,
								 MPxNode::kManipContainer);
    if (!status) 
	{
        status.perror("registerNode");
        return status;
    }
    return status;
}


MStatus uninitializePlugin(MObject obj)
{
    MStatus status;
    MFnPlugin plugin(obj);

    status = plugin.deregisterContextCommand(CREATE_CTX_NAME);
    if (!status) {
        status.perror("deregisterContextCommand");
        return status;
    }

    status = plugin.deregisterNode(customAttrManip::id);
    if (!status) {
        status.perror("deregisterNode");
        return status;
    }
    return status;
}
