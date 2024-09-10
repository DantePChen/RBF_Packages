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

//  Description
//	The torusField node implements an attraction-and-repel field.
//
//	The field repels all objects between itself and repelDistance attribute
//  and attracts objects greater than attractDistance attribute from itself.  
//  This will eventually result in the objects clustering
//	in a torus shape around the field.
//

#include <maya/MIOStream.h>
#include <maya/MVector.h>
#include <maya/MObject.h>
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MFnPlugin.h>
#include <maya/MPxFieldNode.h>

#include <maya/MGL.h>

#define McheckErr(stat, msg)		\
	if ( MS::kSuccess != stat )		\
	{								\
		cerr << msg;				\
		return MS::kFailure;		\
	}

class torusField: public MPxFieldNode
{
public:
	torusField() {};
	~torusField() override {};

	static void		*creator();
	static MStatus	initialize();

	// will compute output force.
	//
	MStatus	compute( const MPlug& plug, MDataBlock& block ) override;

	void 	draw (  M3dView  & view, const  MDagPath  & path,  M3dView::DisplayStyle  style, M3dView:: DisplayStatus ) override;

    ///
    MStatus getForceAtPoint(const MVectorArray& point,
                            const MVectorArray& velocity,
                            const MDoubleArray& mass,
                            MVectorArray& force,
                            double deltaTime) override;

	///
	MStatus iconSizeAndOrigin(	GLuint& width,
						GLuint& height,
						GLuint& xbo,
						GLuint& ybo   ) override;
	///
	MStatus iconBitmap(GLubyte* bitmap) override;

	//
	// attributes.
	//

	// minimum distance from field at which repel is applied
	//
	static MObject	aMinDistance;

	// min distance from field at which the force attracts
	//
	static MObject	aAttractDistance;

	// max distance from field at which the force repels.
	//
	static MObject	aRepelDistance;

	// drag exerted on the attractRepel force.
	//
	static MObject	aDrag;

	// amplitude/magnitude of the swarm force.
	//
	static MObject	aSwarmAmplitude;

	// frequency of the swarm force.
	//
	static MObject	aSwarmFrequency;

	// phase of the swarm force.
	//
	static MObject	aSwarmPhase;

	// Other data members
	//
	static MTypeId	id;

private:

	// methods to compute output force.
	//
	void	applyNoMaxDist( MDataBlock& block,
							const MVectorArray &points,
							const MVectorArray &velocities,
							const MDoubleArray &masses,
							MVectorArray &outputForce );

	void	applyMaxDist( MDataBlock& block,
							const MVectorArray &points,
							const MVectorArray &velocities,
							const MDoubleArray &masses,
							MVectorArray &outputForce );

	void	ownerPosition( MDataBlock& block, MVectorArray &vArray );
	MStatus	getWorldPosition( MVector &vector );
	MStatus	getWorldPosition( MDataBlock& block, MVector &vector );
	void	noiseFunction( double *inputNoise, double *out );

	// methods to get attribute value.
	//
	double	magnitudeValue( MDataBlock& block );
	double	attenuationValue( MDataBlock& block );
	double	maxDistanceValue( MDataBlock& block );
	bool	useMaxDistanceValue( MDataBlock& block );
	bool	applyPerVertexValue( MDataBlock& block );

	// methods to get attribute value of local attributes.
	//
	double	minDistanceValue( MDataBlock& block );
	double	attractDistanceValue( MDataBlock& block );
	double	repelDistanceValue( MDataBlock& block );
	double	dragValue( MDataBlock& block );
	double	swarmAmplitudeValue( MDataBlock& block );
	double	swarmFrequencyValue( MDataBlock& block );
	double	swarmPhaseValue( MDataBlock& block );

	MStatus	ownerCentroidValue( MDataBlock& block, MVector &vector );
};

// inlines
//
inline double torusField::magnitudeValue( MDataBlock& block )
{
	MStatus status;

	MDataHandle hValue = block.inputValue( mMagnitude, &status );

	double value = 0.0;
	if( status == MS::kSuccess )
		value = hValue.asDouble();

	return( value );
}

inline double torusField::attenuationValue( MDataBlock& block )
{
	MStatus status;

	MDataHandle hValue = block.inputValue( mAttenuation, &status );

	double value = 0.0;
	if( status == MS::kSuccess )
		value = hValue.asDouble();

	return( value );
}

inline double torusField::maxDistanceValue( MDataBlock& block )
{
	MStatus status;

	MDataHandle hValue = block.inputValue( mMaxDistance, &status );

	double value = 0.0;
	if( status == MS::kSuccess )
		value = hValue.asDouble();

	return( value );
}

inline bool torusField::useMaxDistanceValue( MDataBlock& block )
{
	MStatus status;

	MDataHandle hValue = block.inputValue( mUseMaxDistance, &status );

	bool value = false;
	if( status == MS::kSuccess )
		value = hValue.asBool();

	return( value );
}

inline bool torusField::applyPerVertexValue( MDataBlock& block )
{
	MStatus status;

	MDataHandle hValue = block.inputValue( mApplyPerVertex, &status );

	bool value = false;
	if( status == MS::kSuccess )
		value = hValue.asBool();

	return( value );
}

inline double torusField::minDistanceValue( MDataBlock& block )
{
	MStatus status;

	MDataHandle hValue = block.inputValue( aMinDistance, &status );

	double value = 0.0;
	if( status == MS::kSuccess )
		value = hValue.asDouble();

	return( value );
}

inline double torusField::attractDistanceValue( MDataBlock& block )
{
	MStatus status;

	MDataHandle hValue = block.inputValue( aAttractDistance, &status );

	double value = 0.0;
	if( status == MS::kSuccess )
		value = hValue.asDouble();

	return( value );
}

inline double torusField::repelDistanceValue( MDataBlock& block )
{
	MStatus status;

	MDataHandle hValue = block.inputValue( aRepelDistance, &status );

	double value = 0.0;
	if( status == MS::kSuccess )
		value = hValue.asDouble();

	return( value );
}

inline double torusField::dragValue( MDataBlock& block )
{
	MStatus status;

	MDataHandle hValue = block.inputValue( aDrag, &status );

	double value = 0.0;
	if( status == MS::kSuccess )
		value = hValue.asDouble();

	return( value );
}

inline double torusField::swarmAmplitudeValue( MDataBlock& block )
{
	MStatus status;

	MDataHandle hValue = block.inputValue( aSwarmAmplitude, &status );

	double value = 0.0;
	if( status == MS::kSuccess )
		value = hValue.asDouble();

	return( value );
}

inline double torusField::swarmFrequencyValue( MDataBlock& block )
{
	MStatus status;

	MDataHandle hValue = block.inputValue( aSwarmFrequency, &status );

	double value = 0.0;
	if( status == MS::kSuccess )
		value = hValue.asDouble();

	return( value );
}

inline double torusField::swarmPhaseValue( MDataBlock& block )
{
	MStatus status;

	MDataHandle hValue = block.inputValue( aSwarmPhase, &status );

	double value = 0.0;
	if( status == MS::kSuccess )
		value = hValue.asDouble();

	return( value );
}

inline MStatus torusField::ownerCentroidValue(MDataBlock& block,MVector &vector)
{
	MStatus status;

	MDataHandle hValueX = block.inputValue( mOwnerCentroidX, &status );
	MDataHandle hValueY = block.inputValue( mOwnerCentroidY, &status );
	MDataHandle hValueZ = block.inputValue( mOwnerCentroidZ, &status );

	if( status == MS::kSuccess )
	{
		vector[0] = hValueX.asDouble();
		vector[1] = hValueY.asDouble();
		vector[2] = hValueZ.asDouble();
	}

	return( status );
}

