//-
// ==========================================================================
// Copyright 2020 Autodesk, Inc. All rights reserved.
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
// This plug-in produces the dependency graph node "customComponentTag".
//
// It is an example node that adds new componentTags in a procedural
// fashion.
//
// It handles mesh, nurbsSurface, nurbsCurve and lattice geometry and
// simply creates "leftSide" and "rightSide" tags by looking at whether
// the verts are on the positive or negative side of the x-axis.
// Note; left and right are defined assuming it is a character model
// facing into positive z, thereby making positive x the left side
// of the character.
// This is just a simple example node to demonstrate how componentTags
// can be created procedurally using a plugin node and probably not
// very useful in itself.
////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <maya/MIOStream.h>
#include <math.h>

#include <maya/MPxNode.h>

#include <maya/MFnGenericAttribute.h>
#include <maya/MFnPlugin.h>
#include <maya/MFnGeometryData.h>
#include <maya/MFnMesh.h>
#include <maya/MFnNurbsSurface.h>
#include <maya/MFnNurbsCurve.h>
#include <maya/MFnLattice.h>

#include <maya/MString.h>
#include <maya/MTypeId.h>
#include <maya/MPlug.h>
#include <maya/MMatrix.h>
#include <maya/MVector.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MItGeometry.h>
#include <maya/MStringArray.h>
#include <maya/MFnSingleIndexedComponent.h>
#include <maya/MFnDoubleIndexedComponent.h>
#include <maya/MFnTripleIndexedComponent.h>

// Macros
//
#define MCheckStatus(status,message)            \
        if( MStatus::kSuccess != status ) {     \
                cerr << message << "\n";        \
                return status;                  \
        }


class customComponentTag : public MPxNode
{
public:
    customComponentTag();
    ~customComponentTag() override;

    MStatus compute( const MPlug& iPlug, MDataBlock& iDataBlock ) override;

    // Note: it is important to define the pass-through otherwise the lookup
    // of the history of component tags through the deformation chain will be
    // incorrect.
    MPlug passThroughToOne(const MPlug&	iPlug) const override;

    static void* creator();
    static MStatus initialize();

public:
    static MObject inputGeometry;         // The inputGeometry value.
    static MObject outputGeometry;        // The outputGeometry value.
    static MTypeId id;
};

MTypeId     customComponentTag::id( 0x00081161 );
MObject     customComponentTag::inputGeometry;
MObject     customComponentTag::outputGeometry;

customComponentTag::customComponentTag() {}
customComponentTag::~customComponentTag() {}

MPlug customComponentTag::passThroughToOne(const MPlug&	iPlug) const
{
    // Maya will rewire the connections just like modeling nodes.
    if (iPlug == inputGeometry)
    {
        return MPlug(thisMObject(), outputGeometry);
    }
    else if (iPlug == outputGeometry)
    {
        return MPlug(thisMObject(), inputGeometry);
    }

    return MPxNode::passThroughToOne(iPlug);
}

MStatus customComponentTag::compute( const MPlug& iPlug, MDataBlock& iDataBlock )
{

    MStatus status = MStatus::kSuccess;

    MDataHandle stateData = iDataBlock.outputValue( state, &status );
    MCheckStatus( status, "ERROR getting state" );

    // Check for the HasNoEffect/PassThrough flag on the node.
    //
    // (stateData is an enumeration standard in all depend nodes)
    //
    // (0 = Normal)
    // (1 = HasNoEffect/PassThrough)
    // (2 = Blocking)
    if( stateData.asShort() == 1 ) {
        MDataHandle inputDataHandle = iDataBlock.inputValue( inputGeometry, &status );
        MCheckStatus(status,"ERROR getting inputGeometry");

        MDataHandle outputDataHandle = iDataBlock.outputValue( outputGeometry, &status );
        MCheckStatus(status,"ERROR getting outputGeometry");

        // Simply redirect the inputGeometry to the outputGeometry for the PassThrough effect
        outputDataHandle.copy(inputDataHandle);
        return status;
    }

    if( iPlug == outputGeometry )
    {
        MDataHandle inputDataHandle = iDataBlock.inputValue( inputGeometry, &status );
        MCheckStatus(status,"ERROR getting inputGeometry");

        MDataHandle outputDataHandle = iDataBlock.outputValue( outputGeometry, &status );
        MCheckStatus(status,"ERROR getting outputGeometry");

        if( status != MStatus::kSuccess )
            cerr << "ERROR getting dataBlock" << endl;
        else {

            MString leftTag("leftSide");
            MString rightTag("rightSide");

            outputDataHandle.copy(inputDataHandle);

            MObject geomOb = outputDataHandle.data();

            MFnGeometryData geomDataFn(geomOb, &status);

            geomDataFn.addComponentTag(leftTag);
            geomDataFn.addComponentTag(rightTag);

            if (geomOb.hasFn(MFn::kMesh)) {
                // Handle the mesh geometry
                MFnMesh meshFn(geomOb, &status);
                MCheckStatus( status, "ERROR getting mesh" );

                if (status == MStatus::kSuccess) {
                    MFnSingleIndexedComponent leftFacesFn;
                    MFnSingleIndexedComponent rightFacesFn;
                    MObject leftFacesComp = leftFacesFn.create(MFn::kMeshPolygonComponent);
                    MObject rightFacesComp = rightFacesFn.create(MFn::kMeshPolygonComponent);

                    const int polyCount = meshFn.numPolygons( &status );

                    for (int polygonId = 0; polygonId < polyCount; ++polygonId) {
                        bool allLeft = true;
                        bool allRight = true;
                        MIntArray vertexList;
                        status = meshFn.getPolygonVertices(polygonId, vertexList);
                        if (status) {
                            const unsigned int numVerts = vertexList.length();
                            MPoint pos;
                            for (unsigned int i = 0; i < numVerts && allLeft; ++i) {
                                meshFn.getPoint(vertexList[i], pos);
                                if (pos.x < 0.0) {
                                    allLeft = false;
                                } else if (pos.x > 0.0) {
                                    allRight = false;
                                }
                            }

                            if (allLeft)
                                leftFacesFn.addElement(polygonId);
                            else if (allRight)
                                rightFacesFn.addElement(polygonId);
                        }
                    }

                    geomDataFn.setComponentTagContents(leftTag, leftFacesComp);
                    geomDataFn.setComponentTagContents(rightTag, rightFacesComp);
                }

            } else if ( geomOb.hasFn(MFn::kNurbsSurface)) {
                // Handle the nurbsSurface geometry
                MFnNurbsSurface geomFn(geomOb, &status);
                MCheckStatus( status, "ERROR getting nurbsSurface" );
                if (status == MStatus::kSuccess) {
                    MFnDoubleIndexedComponent leftCvsFn;
                    MFnDoubleIndexedComponent rightCvsFn;

                    MObject leftCvsComp = leftCvsFn.create(MFn::kSurfaceCVComponent);
                    MObject rightCvsComp = rightCvsFn.create(MFn::kSurfaceCVComponent);

                    int nu = geomFn.numCVsInU();
                    int nv = geomFn.numCVsInV();
                    MPoint pos;

                    for (int u = 0; u < nu; ++u) {
                        for (int v = 0; v < nv; ++v) {
                            geomFn.getCV(u, v, pos);
                            if (pos.x >= 0.0) {
                               leftCvsFn.addElement(u, v);
                            }
                            if (pos.x <= 0.0) {
                               rightCvsFn.addElement(u, v);
                            }
                        }
                    }

                    geomDataFn.setComponentTagContents(leftTag, leftCvsComp);
                    geomDataFn.setComponentTagContents(rightTag, rightCvsComp);
                }

            } else if ( geomOb.hasFn(MFn::kNurbsCurve)) {
                // Handle the nurbsCurve geometry
                MFnNurbsCurve geomFn(geomOb, &status);
                MCheckStatus( status, "ERROR getting nurbsCurve" );
                if (status == MStatus::kSuccess) {
                    MFnSingleIndexedComponent leftCvsFn;
                    MFnSingleIndexedComponent rightCvsFn;

                    MObject leftCvsComp = leftCvsFn.create(MFn::kCurveCVComponent);
                    MObject rightCvsComp = rightCvsFn.create(MFn::kCurveCVComponent);

                    MPoint pos;
                    int nu = geomFn.numCVs();
                    for (int u = 0; u < nu; ++u) {
                        geomFn.getCV (u, pos);
                        if (pos.x >= 0.0) {
                            leftCvsFn.addElement(u);
                        }
                        if (pos.x <= 0.0) {
                            rightCvsFn.addElement(u);
                        }
                    }

                    geomDataFn.setComponentTagContents(leftTag, leftCvsComp);
                    geomDataFn.setComponentTagContents(rightTag, rightCvsComp);
                }

            } else if ( geomOb.hasFn(MFn::kLattice)) {
                // Handle the lattice geometry
                MFnLattice geomFn(geomOb, &status);
                MCheckStatus( status, "ERROR getting lattice" );

                if (status == MStatus::kSuccess) {
                    MFnTripleIndexedComponent leftPtsFn;
                    MFnTripleIndexedComponent rightPtsFn;

                    MObject leftPtsComp = leftPtsFn.create(MFn::kLatticeComponent);
                    MObject rightPtsComp = rightPtsFn.create(MFn::kLatticeComponent);

                    unsigned int ns, nt, nu;
                    geomFn.getDivisions(ns, nt, nu);

                    for (unsigned int s = 0; s < ns; ++s) {
                        for (unsigned int t = 0; t < nt; ++t) {
                            for (unsigned int u = 0; u < nu; ++u) {
                                MPoint &pos = geomFn.point (s, t, u);
                                if (pos.x >= 0.0) {
                                    leftPtsFn.addElement(s, t, u);
                                }
                                if (pos.x <= 0.0) {
                                    rightPtsFn.addElement(s, t, u);
                                }
                            }
                        }
                    }

                    geomDataFn.setComponentTagContents(leftTag, leftPtsComp);
                    geomDataFn.setComponentTagContents(rightTag, rightPtsComp);
                }
            }

            iDataBlock.setClean(iPlug);
        }
    } else {
        return MStatus::kUnknownParameter;
    }

    return MStatus::kSuccess;
}

void* customComponentTag::creator()
{
    return new customComponentTag();
}

MStatus customComponentTag::initialize()
{
    MStatus				stat;

 	MFnGenericAttribute geoAttr;
    inputGeometry = geoAttr.create("inputGeometry", "ig");
 	geoAttr.addDataAccept(MFnData::kMesh);
 	geoAttr.addDataAccept(MFnData::kNurbsSurface);
 	geoAttr.addDataAccept(MFnData::kNurbsCurve);
 	geoAttr.addDataAccept(MFnData::kLattice);

    geoAttr.setStorable(true);
    geoAttr.setKeyable(false);
    geoAttr.setReadable(true);
    geoAttr.setWritable(true);
    geoAttr.setCached(false);

    stat = addAttribute( inputGeometry );
    if (!stat) {
        stat.perror("addAttribute");
        return stat;
    }

    outputGeometry = geoAttr.create("outputGeometry", "og");
 	geoAttr.addDataAccept(MFnData::kMesh);
 	geoAttr.addDataAccept(MFnData::kNurbsSurface);
 	geoAttr.addDataAccept(MFnData::kNurbsCurve);
 	geoAttr.addDataAccept(MFnData::kLattice);
    geoAttr.setWritable(false);
    geoAttr.setStorable(false);

    stat = addAttribute( outputGeometry );
    if (!stat) {
        stat.perror("addAttribute");
        return stat;
    }

    stat = attributeAffects( inputGeometry, outputGeometry );
    if (!stat) {
        stat.perror("attributeAffects");
        return stat;
    }

    return MStatus::kSuccess;
}

MStatus initializePlugin( MObject obj )
{
    MStatus   status;
    MFnPlugin plugin( obj, PLUGIN_COMPANY, "3.0", "Any");

    status = plugin.registerNode( "customComponentTag", customComponentTag::id, customComponentTag::creator,
                                    customComponentTag::initialize );
    if (!status) {
        status.perror("registerNode");
        return status;
    }

    return status;
}

MStatus uninitializePlugin( MObject obj)
{
    MStatus   status;
    MFnPlugin plugin( obj );

    status = plugin.deregisterNode( customComponentTag::id );
    if (!status) {
        status.perror("deregisterNode");
        return status;
    }

    return status;
}
