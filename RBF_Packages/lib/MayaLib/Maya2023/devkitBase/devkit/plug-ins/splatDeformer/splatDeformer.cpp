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

//
//  File: splatDeformer.cpp
//
//  Description:
// 		Example implementation of a threaded deformer. This node
//		deforms one mesh using another.
//

#include <maya/MIOStream.h>

#include <maya/MPxGeometryFilter.h>
#include <maya/MItGeometry.h>
#include <maya/MFnPlugin.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MPoint.h>
#include <maya/MTimer.h>
#include <maya/MFnMesh.h>
#include <maya/MPointArray.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnMeshData.h>
#include <maya/MMeshIntersector.h>

#include <maya/MThreadUtils.h>
#include <tbb/blocked_range.h>
#include <tbb/parallel_for.h>

// Macros
//
#define MCheckStatus(status,message)	\
	if( MStatus::kSuccess != status ) {	\
		cerr << message << "\n";		\
		return status;					\
	}

//======================================================================

class splatDeformer : public MPxGeometryFilter
{
public:
						splatDeformer();
					~splatDeformer() override;

	static  void*		creator();
	static  MStatus		initialize();

	// deformation function
	//
	MStatus compute(const MPlug& plug, MDataBlock& dataBlock) override;

public:
	// local node attributes
	static	MObject	deformingMesh;		// Reference mesh for splat deforming
	static  MObject parallelEnabled;	// Boolean indicating whether the parallel compute is to be used

	static	MTypeId	id;	// Plug-in ID

private:
	// Helper method to make it easier to handle both the case of evaluating
	// one child (as in DG evaluation) and the case of evaluating all children
	// (as in EM evaluation).
	MStatus computeOneOutput(unsigned int index, MDataBlock& data, MDataHandle& hInput);
};

//======================================================================

MTypeId	splatDeformer::id( 0x8104D );
MObject	splatDeformer::deformingMesh;
MObject	splatDeformer::parallelEnabled;

splatDeformer::splatDeformer() {}
splatDeformer::~splatDeformer() {}

//======================================================================

void* splatDeformer::creator()
{
	return new splatDeformer();
}

//======================================================================

MStatus splatDeformer::initialize()
{
	// local attribute initialization
	MStatus status;
	MFnTypedAttribute mAttr;
 	deformingMesh=mAttr.create( "deformingMesh", "dm", MFnMeshData::kMesh);
	mAttr.setStorable(true);

	MFnNumericAttribute mParallelAttr;
 	parallelEnabled = mParallelAttr.create( "enableParallel", "pll", MFnNumericData::kBoolean, 0, &status);
	mParallelAttr.setStorable(true);

 	//  deformation attributes
 	status = addAttribute( deformingMesh );
	MCheckStatus(status, "ERROR in addAttribute(deformingMesh)\n");
 	status = addAttribute( parallelEnabled );
	MCheckStatus(status, "ERROR in addAttribute(parallelEnabled)\n");

 	status = attributeAffects( deformingMesh, outputGeom );
	MCheckStatus(status, "ERROR in attributeAffects(deformingMesh)\n");
 	status = attributeAffects( parallelEnabled, outputGeom );
	MCheckStatus(status, "ERROR in attributeAffects(parallelEnabled)\n");

	return MStatus::kSuccess;
}

//======================================================================
//
// Standard compute method. Notice how it handles both the case of
// evaluation of the child and the parent array plugs.
//
// Normal deformer plug-ins only override the deform() method, which is
// the simpler way to do things. In those cases where you have other
// reasons for wanting to override the compute() method directly use this
// as a template for how to do that.
//
MStatus splatDeformer::compute(const MPlug& plug, MDataBlock& data)
{
	MStatus status = MStatus::kUnknownParameter;
	MObject thisNode = this->thisMObject();
 	if (plug.attribute() != outputGeom)
	{
		printf("Ignoring requested plug\n");
		return status;
	}

	// The evaluation manager always evaluates root attributes so it is
	// necessary in the compute() method to handle that case, as well as
	// the case of only evaluating a single child attribute.
	//
	if( plug.isArray() )
	{
		// In the EM evaluation the input array will already have been
		// computed so inputArrayReference() could also be used, but
		// this way the same code works for EM and DG modes.
		//
		MPlug inPlug(thisNode,input);
		MArrayDataHandle hInput = data.inputArrayValue(inPlug, &status);
		MCheckStatus(status, "ERROR getting input mesh\n");
	
		if( MStatus::kSuccess == hInput.jumpToArrayElement(0) )
		{
			do
			{
				MDataHandle hInputElement = hInput.inputValue( &status );
				MCheckStatus(status, "ERROR getting input mesh element\n");
				unsigned int inputIndex = hInput.elementIndex(&status);
				MCheckStatus(status, "ERROR getting input mesh element index\n");
				computeOneOutput( inputIndex, data, hInputElement );
			} while( MStatus::kSuccess == hInput.next() );
		}
	}
	else
	{
		// Children are still computed when you are doing some types of manipulation,
		// when the EM is disabled, when the EM mode is set to "off", or in those rare
		// cases where an internal evaluation has triggered a non-root plug request.
		//
		MPlug inPlug(thisNode,input);
		inPlug.selectAncestorLogicalIndex(plug.logicalIndex(),input);
		MDataHandle hInput = data.inputValue(inPlug, &status);
		MCheckStatus(status, "ERROR getting input mesh\n");
	
		computeOneOutput( plug.logicalIndex(), data, hInput );
	}
	return status;
}

//======================================================================
// Cancelable range implementation, so that a parallel_for can end early
// if a failure (or for that matter, a success) is found.
//
// From https://software.intel.com/en-us/blogs/2007/11/08/have-a-fish-how-break-from-a-parallel-loop-in-tbb
//
//! Like a blocked_range, but becomes immediately empty if "stop" flag is true.
template<typename Value>
class cancelable_range {
    tbb::blocked_range<Value> my_range;
    volatile bool& my_stop;
public:
    // Constructor for client code
    /** Range becomes empty if stop==true. */
    cancelable_range( int begin, int end, int grainsize, volatile bool& stop ) :
        my_range(begin,end,grainsize),
        my_stop(stop)
    {}

    //! Splitting constructor used by parallel_for
    cancelable_range( cancelable_range& r, tbb::split ) :
        my_range(r.my_range,tbb::split()),
        my_stop(r.my_stop)
    {}

    //! Forced undefined assign operator to avoid compiler warning.
    cancelable_range & operator=( const cancelable_range & );

    //! Cancel the range.
    void cancel() const {my_stop=true;}

    //! True if range is empty.
    /** Range is empty if there is request to cancel the range. */
    bool empty() const {return my_stop || my_range.empty();}

    //! True if range is divisible
    /** Range becomes indivisible if there is request to cancel the range. */
    bool is_divisible() const {return !my_stop && my_range.is_divisible();}

    //! Initial value in range.
    Value begin() const {return my_range.begin();}

    //! One past last value in range
    /** Note that end()==begin() if there is request to cancel the range.
        The value of end() may change asynchronously if another thread cancels the range. **/
    Value end() const {return my_stop ? my_range.begin() : my_range.end();}
};

//======================================================================
// Given the index of a single input, compute the corresponding deformed output
// and put it back into the datablock.
//
MStatus splatDeformer::computeOneOutput(unsigned int index, MDataBlock& data, MDataHandle& hInput)
{
	MStatus status = MStatus::kUnknownParameter;
	MObject thisNode = this->thisMObject();

	// Construct the proper output plug that matches the input by index
	MPlug outPlug(thisNode, outputGeom);
	outPlug.selectAncestorLogicalIndex(index, outputGeom);

	// get the input geometry
	MDataHandle inputData = hInput.child(inputGeom);
	if (inputData.type() != MFnData::kMesh)
	{
 		printf("Incorrect input geometry type\n");
		return MStatus::kFailure;
 	}

	// get the input groupId
	MDataHandle hGroup = inputData.child(groupId);
	unsigned int lGroupId = hGroup.asInt();

	// get deforming mesh
	MDataHandle deformData = data.inputValue(deformingMesh, &status);
	MCheckStatus(status, "ERROR getting deforming mesh\n");
    if (deformData.type() != MFnData::kMesh)
	{
		printf("Incorrect deformer geometry type %d\n", deformData.type());
		return MStatus::kFailure;
	}

  	MObject dSurf = deformData.asMeshTransformed();
 	MFnMesh fnDeformingMesh;
 	fnDeformingMesh.setObject( dSurf ) ;

	// Create a unique copy of the output data from the input data
	MDataHandle outputData = data.outputValue(outPlug);
	outputData.copyWritable(inputData);
 	if (outputData.type() != MFnData::kMesh)
	{
		printf("Incorrect output mesh type\n");
		return MStatus::kFailure;
	}
	
  	MObject oSurf = outputData.asMesh() ;
 	if(oSurf.isNull())
	{
		printf("Input surface is NULL\n");
		return MStatus::kFailure;
	}

  	MFnMesh outMesh;
  	outMesh.setObject( oSurf ) ;
	MCheckStatus(status, "ERROR setting points\n");

	MItGeometry iter(outputData, lGroupId, false);

	// create fast intersector structure
	MMeshIntersector intersector;
	intersector.create(dSurf);

	// get all points at once. Faster to query, and also better for
	// threading than using iterator
	MPointArray verts;
	iter.allPositions(verts);
	unsigned int nPoints = verts.length();

	// use bool variable as lightweight object for failure check in loop below
	volatile bool failed = false;

	MDataHandle parallelEnabledData = data.inputValue(parallelEnabled, &status);
	bool lParallelEnabled = (bool) parallelEnabledData.asBool();	

 	MTimer timer; timer.beginTimer();
	if( lParallelEnabled )
	{
		bool stop = false;
		tbb::parallel_for( cancelable_range<unsigned int>(0,nPoints,nPoints/1000,stop),
						   [&](const cancelable_range<unsigned int>& r)
		{
			// Iterate over subrange.  It is important that "<" be used for comparison,
			// because the value of r.end() changes to r.begin() if r is cancelled.
			for(unsigned int i = r.begin(); i < r.end(); ++i)
			{
				// mesh point object must be in loop-local scope to avoid race conditions
				MPointOnMesh meshPoint;

				// Do intersection. Need to use per-thread status value as
				// MStatus has internal state and may trigger race conditions
				// if set from multiple threads. Probably benign in this case,
				// but worth being careful.
				MStatus localStatus = intersector.getClosestPoint(verts[i], meshPoint);
				if(localStatus != MStatus::kSuccess)
				{
					failed = true;
					r.cancel();
				}
				else
				{
					// default scheduling breaks traversal into large
					// chunks, so low risk of false sharing here in array write.
					verts[i] = meshPoint.getPoint();
				}
			}
		});
	}
	else
	{
		MPointOnMesh meshPoint;
		for(unsigned int i=0; i<nPoints; ++i )
		{
			// Do intersection.
			status = intersector.getClosestPoint(verts[i], meshPoint);
			if(status != MStatus::kSuccess)
			{
				failed = true;
				break;
			}
			verts[i] = meshPoint.getPoint();
		}
	}

 	timer.endTimer(); printf("Runtime for %s loop %f\n", lParallelEnabled ? "parallel" : "serial", timer.elapsedTime() );

	// write values back onto output using fast set method on iterator
	iter.setAllPositions(verts);

	// NOTE: This is a crucial step in EM evaluation, especially in
	//       the case where you have multiple outgoing connections.
	//       It keeps the EM state up to date and prevents multiple
	//       evaluation requests that at best will slow things down
	//       and at worst could crash the system.
	//
	// Tell the EM that data has been set and is now valid.
	//
	data.setClean( outPlug );
	outputData.setMObject( outputData.asMesh() );

	if(failed)
	{
		printf("Closest point failed\n");
		return MStatus::kFailure;
	}

	return status;
}

//======================================================================

// standard initialization procedures
//
MStatus initializePlugin( MObject obj )
{
	MStatus result;
	MFnPlugin plugin( obj, PLUGIN_COMPANY, "1.0", "Any");
	result = plugin.registerNode( "splatDeformer", splatDeformer::id, splatDeformer::creator,
								  splatDeformer::initialize, MPxNode::kDeformerNode );

	return result;
}

//======================================================================

MStatus uninitializePlugin( MObject obj)
{
	MStatus result;
	MFnPlugin plugin( obj );
	result = plugin.deregisterNode( splatDeformer::id );
	return result;
}
