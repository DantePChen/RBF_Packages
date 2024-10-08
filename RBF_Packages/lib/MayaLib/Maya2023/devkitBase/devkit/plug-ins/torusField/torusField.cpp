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

////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
// 
// Produces the dependency graph node "torusField".
//
// This node is an example of a dynamics field that creates a attract-repel field
// between itself and a distance.
//
// The example MEL script "torusField.mel" shows how to create the node
// and appropriate connections to correctly establish a user defined field. 
//
////////////////////////////////////////////////////////////////////////

#include <maya/MIOStream.h>
#include <math.h>

#include "torusField.h"

#include <maya/MTime.h>
#include <maya/MVectorArray.h>
#include <maya/MDoubleArray.h>
#include <maya/MMatrix.h>
#include <maya/MArrayDataBuilder.h>

#include <maya/MFnDependencyNode.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnVectorArrayData.h>
#include <maya/MFnDoubleArrayData.h>
#include <maya/MFnMatrixData.h>


MObject torusField::aMinDistance;
MObject torusField::aAttractDistance;
MObject torusField::aRepelDistance;
MObject torusField::aDrag;
MObject torusField::aSwarmAmplitude;
MObject torusField::aSwarmFrequency;
MObject torusField::aSwarmPhase;

MTypeId torusField::id( 0x80018 );


void *torusField::creator()
{
    return new torusField;
}

MStatus torusField::initialize()
//
//	Descriptions:
//		Initialize the node, attributes.
//
{
	MStatus status;

	MFnNumericAttribute numAttr;

	// create the field basic attributes.
	//
	aMinDistance = numAttr.create("minDistance","mnd",MFnNumericData::kDouble);
	numAttr.setDefault( 0.0 );
	numAttr.setKeyable( true );
	status = addAttribute( aMinDistance );
	McheckErr(status, "ERROR adding aMinDistance attribute.\n");

	aAttractDistance = numAttr.create("attractDistance","ad",
										MFnNumericData::kDouble);
	numAttr.setDefault( 20.0 );
	numAttr.setKeyable( true );
	status = addAttribute( aAttractDistance );
	McheckErr(status, "ERROR adding aAttractDistance attribute.\n");

	aRepelDistance = numAttr.create("repelDistance","rd",
										MFnNumericData::kDouble);
	numAttr.setDefault( 10.0 );
	numAttr.setKeyable( true );
	status = addAttribute( aRepelDistance );
	McheckErr(status, "ERROR adding aRepelDistance attribute.\n");

	aDrag = numAttr.create("drag", "d", MFnNumericData::kDouble);
	numAttr.setDefault( 0.0 );
	numAttr.setKeyable( true );
	status = addAttribute( aDrag );
	McheckErr(status, "ERROR adding aDrag attribute.\n");

	aSwarmAmplitude = numAttr.create("swarmAmplitude", "samp",
										MFnNumericData::kDouble);
	numAttr.setDefault( 0.0 );
	numAttr.setKeyable( true );
	status = addAttribute( aSwarmAmplitude );
	McheckErr(status, "ERROR adding aSwarmAmplitude attribute.\n");

	aSwarmFrequency = numAttr.create("swarmFrequency", "sfrq",
										MFnNumericData::kDouble);
	numAttr.setDefault( 1.0 );
	numAttr.setKeyable( true );
	status = addAttribute( aSwarmFrequency );
	McheckErr(status, "ERROR adding aSwarmFrequency attribute.\n");

	aSwarmPhase = numAttr.create("swarmPhase", "sa", MFnNumericData::kDouble);
	numAttr.setDefault( 0.0 );
	numAttr.setKeyable( true );
	status = addAttribute( aSwarmPhase );
	McheckErr(status, "ERROR adding aSwarmPhase attribute.\n");

	// the new attribute will affect output force.
	//
	//status = attributeAffects( aMinDistance, outputForce );
	//McheckErr(status, "ERROR in attributeAffects(aMinDistance,outputForce).\n");
	//status = attributeAffects( aAttractDistance, outputForce );
	//McheckErr(status, "ERROR in attributeAffects(aAttractDistance,outputForce).\n");
	//status = attributeAffects( aRepelDistance, outputForce );
	//McheckErr(status, "ERROR in attributeAffects(aRepelDistance,outputForce).\n");
	//status = attributeAffects( aDrag, outputForce );
	//McheckErr(status, "ERROR in attributeAffects(aDrag,outputForce).\n");

	return( MS::kSuccess );
}


MStatus torusField::compute(const MPlug& plug, MDataBlock& block)
//
//	Descriptions:
//		compute output force.
//
{
	MStatus status;

	if( !(plug == mOutputForce) )
        return( MS::kUnknownParameter );

	// get the logical index of the element this plug refers to.
	//
	int multiIndex = plug.logicalIndex( &status );
	McheckErr(status, "ERROR in plug.logicalIndex.\n");

	// Get input data handle, use outputArrayValue since we do not
	// want to evaluate both inputs, only the one related to the
	// requested multiIndex. Evaluating both inputs at once would cause
	// a dependency graph loop.
	//
	MArrayDataHandle hInputArray = block.outputArrayValue( mInputData, &status );
	McheckErr(status,"ERROR in hInputArray = block.outputArrayValue().\n");

	status = hInputArray.jumpToElement( multiIndex );
	McheckErr(status, "ERROR: hInputArray.jumpToElement failed.\n");

	// get children of aInputData.
	//
	MDataHandle hCompond = hInputArray.inputValue( &status );
	McheckErr(status, "ERROR in hCompond=hInputArray.inputValue\n");

	MDataHandle hPosition = hCompond.child( mInputPositions );
	MObject dPosition = hPosition.data();
	MFnVectorArrayData fnPosition( dPosition );
	MVectorArray points = fnPosition.array( &status );
	McheckErr(status, "ERROR in fnPosition.array(), not find points.\n");

	MDataHandle hVelocity = hCompond.child( mInputVelocities );
	MObject dVelocity = hVelocity.data();
	MFnVectorArrayData fnVelocity( dVelocity );
	MVectorArray velocities = fnVelocity.array( &status );
	McheckErr(status, "ERROR in fnVelocity.array(), not find velocities.\n");

	MDataHandle hMass = hCompond.child( mInputMass );
	MObject dMass = hMass.data();
	MFnDoubleArrayData fnMass( dMass );
	MDoubleArray masses = fnMass.array( &status );
	McheckErr(status, "ERROR in fnMass.array(), not find masses.\n");

	// Compute the output force.
	//

	MVectorArray forceArray;
	bool useMaxDistSet = useMaxDistanceValue( block );
	if( useMaxDistSet )
	{
		applyMaxDist( block, points, velocities, masses, forceArray );
	}
	else
	{
		applyNoMaxDist( block, points, velocities, masses, forceArray );
	}

	// get output data handle
	//
	MArrayDataHandle hOutArray = block.outputArrayValue( mOutputForce, &status);
	McheckErr(status, "ERROR in hOutArray = block.outputArrayValue.\n");
	MArrayDataBuilder bOutArray = hOutArray.builder( &status );
	McheckErr(status, "ERROR in bOutArray = hOutArray.builder.\n");

	// get output force array from block.
	//
	MDataHandle hOut = bOutArray.addElement(multiIndex, &status);
	McheckErr(status, "ERROR in hOut = bOutArray.addElement.\n");

	MFnVectorArrayData fnOutputForce;
	MObject dOutputForce = fnOutputForce.create( forceArray, &status );
	McheckErr(status, "ERROR in dOutputForce = fnOutputForce.create\n");

	// update data block with new output force data.
	//
	hOut.set( dOutputForce );
	block.setClean( plug );

	return( MS::kSuccess );
}


void torusField::applyNoMaxDist
	(
		MDataBlock &block,				// get field param from this block
		const MVectorArray &points,		// current position of Object
		const MVectorArray &velocities,	// current velocity of Object
		const MDoubleArray &/*masses*/,		// mass of Object
		MVectorArray &outputForce		// output force
	)
//
//	Descriptions:
//		Compute output force in the case that the useMaxDistance is not set.
//
{
	// points and velocities should have the same length. If not return.
	//
	if( points.length() != velocities.length() )
		return;

	// clear the output force array.
	//
	outputForce.clear();

	// get field parameters.
	//
	double magValue = magnitudeValue( block );
	// double attenValue = attenuationValue( block );
	double minDist = minDistanceValue( block );
	double attractDist = attractDistanceValue( block );
	double repelDist = repelDistanceValue( block );
	double dragMag = dragValue( block );
	double swarmAmp = swarmAmplitudeValue( block );

	// get owner's data. posArray may have only one point which is the centroid
	// (if this has owner) or field position(if without owner). Or it may have
	// a list of points if with owner and applyPerVertex.
	//
	MVectorArray posArray;
	posArray.clear();
	ownerPosition( block, posArray );

	int fieldPosCount = posArray.length();
	int receptorSize = points.length();

	// With this model,if max distance isn't set then we
	// also don't attenuate, because 1 - dist/maxDist isn't
	// meaningful. No max distance and no attenuation.
	//
	if (dragMag <= 0)
	{
		if (swarmAmp <= 0)
		{
			// No max dist, no attenuation, no drag, no swarm
			//
			for (int ptIndex = 0; ptIndex < receptorSize; ptIndex ++ )
			{
				MVector forceV(0.0,0.0,0.0);
				const MVector &receptorPoint = points[ptIndex];

				// Apply from every field position to every receptor position.
				//
				for(int i = fieldPosCount; --i>=0; )
				{
					MVector difference = (receptorPoint-posArray[i]);

					double distance = difference.length();
					if (distance < minDist) continue;

					if (distance <= repelDist)
						forceV += difference * magValue;
					else if (distance >= attractDist)
						forceV += -difference * magValue;
				}
				outputForce.append( forceV );
			}
		}
		else
		{
			// No max dist, no attenuation, no drag, yes swarm
			//
			for (int ptIndex = 0; ptIndex < receptorSize; ptIndex ++ )
			{
				MVector forceV(0.0,0.0,0.0);
				const MVector &receptorPoint = points[ptIndex];

				// Apply from every field position to every receptor position.
				//
				double distance = 0.0;
				int i;
				for(i = fieldPosCount; --i>=0; )
				{
					MVector difference = (receptorPoint-posArray[i]);
					distance = difference.length();
					if (distance < minDist) continue;

					if (distance <= repelDist)
						forceV += difference * magValue;
					else if (distance >= attractDist)
						forceV += -difference * magValue;
				}

				// Apply swarm only if the object is inside the zone
				// the repulsion-attraction is pushing the object to.
				//
				if ( distance >= repelDist && distance <= attractDist)
				{
					double frequency = swarmFrequencyValue( block );
					MVector phase( 0.0, 0.0, swarmPhaseValue(block) );

					// Add swarm in here
					//
					for(i = fieldPosCount; --i >= 0;)
					{
						MVector difference = receptorPoint - posArray[i];
						difference = (difference + phase) * frequency;

						double *noiseEffect = &difference.x;
						if( (noiseEffect[0] < -2147483647.0) ||
							(noiseEffect[0] >  2147483647.0) ||
							(noiseEffect[1] < -2147483647.0) ||
							(noiseEffect[1] >  2147483647.0) ||
							(noiseEffect[2] < -2147483647.0) ||
							(noiseEffect[2] >  2147483647.0) )
							continue;

						double noiseOut[4];
						noiseFunction( noiseEffect, noiseOut );
			 			MVector swarmForce( noiseOut[0] * swarmAmp,
											noiseOut[1] * swarmAmp,
											noiseOut[2] * swarmAmp );
						forceV += swarmForce;
					}
				}
				outputForce.append( forceV );
			}
		}
	}
	else
	{
		if (swarmAmp <= 0)
		{
			// Yes drag, no swarm
			//
			for (int ptIndex = 0; ptIndex < receptorSize; ptIndex ++ )
			{
				const MVector& receptorPoint = points[ptIndex];

				// Apply from every field position to every receptor position.
				//
				MVector forceV(0,0,0);
				double distance = 0.0;
				for(int i = fieldPosCount; --i>=0; )
				{
					MVector difference = (receptorPoint-posArray[i]);
					distance = difference.length();
					if (distance < minDist) continue;

					if (distance <= repelDist)
						forceV += difference * magValue;
					else if (distance >= attractDist)
						forceV += -difference * magValue;
				}

				// Apply drag only if the object is inside the zone
				// the repulsion-attraction is pushing the object to.
				//
				if ( distance >= repelDist && distance <= attractDist)
				{
					if (fieldPosCount > 0)
					{
						MVector dragForceV;
						dragForceV = velocities[ptIndex] *
												(-dragMag) * fieldPosCount;
						forceV += dragForceV;
					}
				}

				outputForce.append( forceV );
			}
		}
		else
		{
			// Yes drag, yes swarm
			//
			for (int ptIndex = 0; ptIndex < receptorSize; ptIndex ++ )
			{
				const MVector &receptorPoint = points[ptIndex];

				// Apply from every field position to every receptor position.
				//
				MVector forceV(0,0,0);
				double distance = 0.0;
				int i;
				for(i = fieldPosCount; --i>=0; )
				{
					MVector difference = (receptorPoint-posArray[i]);
					distance = difference.length();
					if (distance < minDist) continue;

					if (distance <= repelDist)
						forceV += difference * magValue;
					else if (distance >= attractDist)
						forceV += -difference * magValue;
				}

				// Apply drag and swarm only if the object is inside
				// the zone the repulsion-attraction is pushing the object to.
				//
				if ( distance >= repelDist && distance <= attractDist)
				{
					if (fieldPosCount > 0)
					{
						MVector dragForceV;
						dragForceV = velocities[ptIndex] *
												(-dragMag) * fieldPosCount;
						forceV += dragForceV;
					}

					// Add swarm in here
					//
					double frequency = swarmFrequencyValue( block );
					MVector phase( 0.0, 0.0, swarmPhaseValue(block) );

					for(i = fieldPosCount; --i>=0; )
					{
						MVector difference = receptorPoint - posArray[i];
						difference = (difference + phase) * frequency;

						double *noiseEffect = &difference.x;
						if( (noiseEffect[0] < -2147483647.0) ||
							(noiseEffect[0] >  2147483647.0) ||
							(noiseEffect[1] < -2147483647.0) ||
							(noiseEffect[1] >  2147483647.0) ||
							(noiseEffect[2] < -2147483647.0) ||
							(noiseEffect[2] >  2147483647.0) )
							continue;

						double noiseOut[4];
						noiseFunction( noiseEffect, noiseOut );
						MVector swarmForce( noiseOut[0] * swarmAmp,
											noiseOut[1] * swarmAmp,
											noiseOut[2] * swarmAmp );
						forceV += swarmForce;
					}
				}
				outputForce.append( forceV );
			}
		}
	}
}


void torusField::applyMaxDist
	(
		MDataBlock& block,				// get field param from this block
		const MVectorArray &points,		// current position of Object
		const MVectorArray &velocities,	// current velocity of Object
		const MDoubleArray &/*masses*/,		// mass of Object
		MVectorArray &outputForce		// output force
	)
//
//	Descriptions:
//		Compute output force in the case that the useMaxDistance is set.
//
{
	// points and velocities should have the same length. If not return.
	//
	if( points.length() != velocities.length() )
		return;

	// clear the output force array.
	//
	outputForce.clear();

	// get field parameters.
	//
	double magValue = magnitudeValue( block );
	double attenValue = attenuationValue( block );
	double maxDist = maxDistanceValue( block );
	double minDist = minDistanceValue( block );
	double attractDist = attractDistanceValue( block );
	double repelDist = repelDistanceValue( block );
	double dragMag = dragValue( block );
	double swarmAmp = swarmAmplitudeValue( block );

	// get owner's data. posArray may have only one point which is the centroid
	// (if this has owner) or field position(if without owner). Or it may have
	// a list of points if with owner and applyPerVertex.
	//
	MVectorArray posArray;
	posArray.clear();
	ownerPosition( block, posArray );

	int fieldPosCount = posArray.length();
	int receptorSize = points.length();

	if (attenValue > 0.0)
	{
		// Max distance applies and so does attenuation.
		//
		for (int ptIndex = 0; ptIndex < receptorSize; ptIndex ++ )
		{
			const MVector &receptorPoint = points[ptIndex];

			// Apply from every field position to every receptor position.
			//
			MVector forceV(0,0,0);
			MVector sumForceV(0,0,0);
			for(int i = fieldPosCount; --i>=0; )
			{
				MVector difference = receptorPoint-posArray[i];
				double distance  = difference.length();
				if (distance <= maxDist && distance >= minDist )
				{
					double force = magValue *
									(pow((1.0-(distance/maxDist)),attenValue));
					forceV = difference * force;

					// Apply drag and swarm if the object is inside
					// the zone the repulsion-attraction is pushing the
					// object to, and if they are set.
					//
					if ( distance >= repelDist && distance <= attractDist)
					{
						if (fieldPosCount > 0 && dragMag > 0)
						{
							MVector dragForceV;
							dragForceV = velocities[ptIndex] *
											(-dragMag) * fieldPosCount;
							forceV += dragForceV;
						}

						// Add swarm if swarm aplitude is set.
						//
						if (swarmAmp > 0)
						{
							double frequency = swarmFrequencyValue( block );
							MVector phase( 0.0, 0.0, swarmPhaseValue(block) );

							difference = receptorPoint - posArray[i];
							difference = (difference + phase) * frequency;
							double *noiseEffect = &difference.x;
							if( (noiseEffect[0] < -2147483647.0) ||
								(noiseEffect[0] >  2147483647.0) ||
								(noiseEffect[1] < -2147483647.0) ||
								(noiseEffect[1] >  2147483647.0) ||
								(noiseEffect[2] < -2147483647.0) ||
								(noiseEffect[2] >  2147483647.0) )
								continue;

							double noiseOut[4];
							noiseFunction( noiseEffect, noiseOut );
							MVector swarmForce( noiseOut[0] * swarmAmp,
												noiseOut[1] * swarmAmp,
												noiseOut[2] * swarmAmp );
							forceV += swarmForce;
						}
					}
				}
				if (maxDist > 0.0) forceV *= falloffCurve(distance/maxDist);
				sumForceV += forceV;					
			}
			outputForce.append( sumForceV );
		}
	}
	else
	{
		// Max dist applies, but not attenuation.
		//
		for (int ptIndex = 0; ptIndex < receptorSize; ptIndex ++ )
		{
			const MVector & receptorPoint = points[ptIndex];

			// Apply from every field position to every receptor position.
			//
			MVector forceV(0,0,0);
			MVector sumForceV(0,0,0);
			int i;
			for(i = fieldPosCount; --i>=0; )
			{
				MVector difference = (receptorPoint-posArray[i]);
				double distance = difference.length();
				if (distance < minDist || distance > maxDist) continue;

				if (distance <= repelDist)
					forceV = difference * magValue;
				else if (distance >= attractDist)
					forceV = -difference * magValue;

				// Apply drag and swarm if the object is inside
				// the zone the repulsion-attraction is pushing the
				// object to, and if they are set.
				//
				if ( distance >= repelDist && distance <= attractDist)
				{
					if (fieldPosCount > 0 && dragMag > 0)
					{
						MVector dragForceV;
						dragForceV = velocities[ptIndex] *
											(-dragMag) * fieldPosCount;
						forceV += dragForceV;
					}

					// Add swarm if swarm aplitude is set.
					//
					if (swarmAmp > 0)
					{
						double frequency = swarmFrequencyValue( block );
						MVector phase( 0.0, 0.0, swarmPhaseValue(block) );

						for(i = fieldPosCount; --i >= 0;)
						{
							difference = receptorPoint - posArray[i];
							difference = (difference + phase) * frequency;
							double *noiseEffect = &difference.x;
							if( (noiseEffect[0] < -2147483647.0) ||
								(noiseEffect[0] >  2147483647.0) ||
								(noiseEffect[1] < -2147483647.0) ||
								(noiseEffect[1] >  2147483647.0) ||
								(noiseEffect[2] < -2147483647.0) ||
								(noiseEffect[2] >  2147483647.0) )
								continue;

							double noiseOut[4];
							noiseFunction( noiseEffect, noiseOut );
							MVector swarmForce( noiseOut[0] * swarmAmp,
												noiseOut[1] * swarmAmp,
												noiseOut[2] * swarmAmp );

							forceV += swarmForce;
						}
					}
				}
				if (maxDist > 0.0) forceV *= falloffCurve(distance/maxDist);
				sumForceV += forceV;
			}
			outputForce.append( sumForceV );
		}
	}
}


void torusField::ownerPosition
	(
		MDataBlock& block,
		MVectorArray &ownerPosArray
	)
//
//	Descriptions:
//		If this field has an owner, get the owner's position array or
//		centroid, then assign it to the ownerPosArray.
//		If it does not have owner, get the field position in the world
//		space, and assign it to the given array, ownerPosArray.
//
{
	MStatus status;

	if( applyPerVertexValue(block) )
	{
		MDataHandle hOwnerPos = block.inputValue( mOwnerPosData, &status );
		if( status == MS::kSuccess )
		{
			MObject dOwnerPos = hOwnerPos.data();
			MFnVectorArrayData fnOwnerPos( dOwnerPos );
			MVectorArray posArray = fnOwnerPos.array( &status );
			if( status == MS::kSuccess )
			{
				// assign vectors from block to ownerPosArray.
				//
				for( unsigned int i = 0; i < posArray.length(); i ++ )
					ownerPosArray.append( posArray[i] );
			}
			else
			{
				MVector worldPos(0.0, 0.0, 0.0);
				//status = getWorldPosition( block, worldPos );
				status = getWorldPosition( worldPos );
				ownerPosArray.append( worldPos );
			}
		}
		else
		{
			// get the field position in the world space
			// and add it into ownerPosArray.
			//
			MVector worldPos(0.0, 0.0, 0.0);
			//status = getWorldPosition( block, worldPos );
			status = getWorldPosition( worldPos );
			ownerPosArray.append( worldPos );
		}
	}
	else
	{
		MVector centroidV(0.0, 0.0, 0.0);
		status = ownerCentroidValue( block, centroidV );
		if( status == MS::kSuccess )
		{
			// assign centroid vector to ownerPosArray.
			//
			ownerPosArray.append( centroidV );
		}
		else
		{
			// get the field position in the world space.
			//
			MVector worldPos(0.0, 0.0, 0.0);
			//status = getWorldPosition( block, worldPos );
			status = getWorldPosition( worldPos );
			ownerPosArray.append( worldPos );
		}
	}
}


MStatus torusField::getWorldPosition( MVector &vector )
//
//	Descriptions:
//		get the field position in the world space.
//		The position value is from inherited attribute, aWorldMatrix.
//
{
	MStatus status;

	MObject thisNode = thisMObject();
	MFnDependencyNode fnThisNode( thisNode );

	// get worldMatrix attribute.
	//
	MObject worldMatrixAttr = fnThisNode.attribute( "worldMatrix" );

	// build worldMatrix plug, and specify which element the plug refers to.
	// We use the first element(the first dagPath of this field).
	//
	MPlug matrixPlug( thisNode, worldMatrixAttr );
	matrixPlug = matrixPlug.elementByLogicalIndex( 0 );

	// Get the value of the 'worldMatrix' attribute
	//
	MObject matrixObject;
	status = matrixPlug.getValue( matrixObject );
	if( !status )
	{
		status.perror("torusField::getWorldPosition: get matrixObject");
		return( status );
	}

	MFnMatrixData worldMatrixData( matrixObject, &status );
	if( !status )
	{
		status.perror("torusField::getWorldPosition: get worldMatrixData");
		return( status );
	}

	MMatrix worldMatrix = worldMatrixData.matrix( &status );
	if( !status )
	{
		status.perror("torusField::getWorldPosition: get worldMatrix");
		return( status );
	}

	// assign the translate to the given vector.
	//
	vector[0] = worldMatrix( 3, 0 );
	vector[1] = worldMatrix( 3, 1 );
	vector[2] = worldMatrix( 3, 2 );

    return( status );
}


MStatus torusField::getWorldPosition( MDataBlock& block, MVector &vector )
//
//	Descriptions:
//		Find the field position in the world space.
//
{
    MStatus status;

	MObject thisNode = thisMObject();
	MFnDependencyNode fnThisNode( thisNode );

	// get worldMatrix attribute.
	//
	MObject worldMatrixAttr = fnThisNode.attribute( "worldMatrix" );

	// build worldMatrix plug, and specify which element the plug refers to.
	// We use the first element(the first dagPath of this field).
	//
	MPlug matrixPlug( thisNode, worldMatrixAttr );
	matrixPlug = matrixPlug.elementByLogicalIndex( 0 );

    //MDataHandle hWMatrix = block.inputValue( worldMatrix, &status );
    MDataHandle hWMatrix = block.inputValue( matrixPlug, &status );

	McheckErr(status, "ERROR getting hWMatrix from dataBlock.\n");

    if( status == MS::kSuccess )
    {
        MMatrix wMatrix = hWMatrix.asMatrix();
        vector[0] = wMatrix(3, 0);
        vector[1] = wMatrix(3, 1);
        vector[2] = wMatrix(3, 2);
    }
    return( status );
}

MStatus torusField::getForceAtPoint(const MVectorArray&	points,
                                  const MVectorArray&	velocities,
                                  const MDoubleArray&	masses,
                                  MVectorArray&	forceArray,
                                  double	/*deltaTime*/)
//
//    This method is not required to be overridden, it is only necessary
//    for compatibility with the MFnField function set.
//
{
	MDataBlock block = forceCache();

	bool useMaxDistSet = useMaxDistanceValue( block );
	if( useMaxDistSet )
	{
		applyMaxDist( block, points, velocities, masses, forceArray );
	}
	else
	{
		applyNoMaxDist( block, points, velocities, masses, forceArray );
	}

    return MS::kSuccess;
}

MStatus torusField::iconSizeAndOrigin(	GLuint& width,
					GLuint& height,
					GLuint& xbo,
					GLuint& ybo   )
//
//	This method is not required to be overridden.  It should be overridden
//	if the plug-in has custom icon.
//
//	The width and height have to be a multiple of 32 on Windows and 16 on 
//	other platform.
//
//	Define an 8x8 icon at the lower left corner of the 32x32 grid. 
//	(xbo, ybo) of (4,4) makes sure the icon is center at origin.
//
{
	width = 32;
	height = 32;
	xbo = 4;
	ybo = 4;
	return MS::kSuccess;
}

MStatus torusField::iconBitmap(GLubyte* bitmap)
//
//	This method is not required to be overridden.  It should be overridden
//	if the plug-in has custom icon.
//
//	Define an 8x8 icon at the lower left corner of the 32x32 grid. 
//	(xbo, ybo) of (4,4) makes sure the icon is center at origin.
{
	bitmap[0] = 0x18;
	bitmap[4] = 0x66;
	bitmap[8] = 0xC3;
	bitmap[12] = 0x81;
	bitmap[16] = 0x81;
	bitmap[20] = 0xC3;
	bitmap[24] = 0x66;
	bitmap[28] = 0x18;

	return MS::kSuccess;
}

#define rand3a(x,y,z)	frand(67*(x)+59*(y)+71*(z))
#define rand3b(x,y,z)	frand(73*(x)+79*(y)+83*(z))
#define rand3c(x,y,z)	frand(89*(x)+97*(y)+101*(z))
#define rand3d(x,y,z)	frand(103*(x)+107*(y)+109*(z))

int		xlim[3][2];		// integer bound for point
double	xarg[3];		// fractional part

double frand( int s )   // get random number from seed
{
	s = s << 13^s;
	return(1. - ((s*(s*s*15731+789221)+1376312589)&0x7fffffff)/1073741824.);
}

double hermite( double p0, double p1, double r0, double r1, double t )
{
	double t2, t3, _3t2, _2t3 ;
	t2 = t * t;
	t3 = t2 * t;
	_3t2 = 3. * t2;
	_2t3 = 2. * t3 ;

	return(p0*(_2t3-_3t2+1) + p1*(-_2t3+_3t2) + r0*(t3-2.*t2+t) + r1*(t3-t2));
}

void interpolate( double f[4], int i, int n )
//
//	f[] returned tangent and value *
//	i   location ?
//	n   order
//
{
	double f0[4], f1[4] ;  //results for first and second halves

	if( n == 0 )	// at 0, return lattice value
	{
		f[0] = rand3a( xlim[0][i&1], xlim[1][i>>1&1], xlim[2][i>>2] );
		f[1] = rand3b( xlim[0][i&1], xlim[1][i>>1&1], xlim[2][i>>2] );
		f[2] = rand3c( xlim[0][i&1], xlim[1][i>>1&1], xlim[2][i>>2] );
		f[3] = rand3d( xlim[0][i&1], xlim[1][i>>1&1], xlim[2][i>>2] );
		return;
	}

	n--;
	interpolate( f0, i, n );		// compute first half
	interpolate( f1, i| 1<<n, n );	// compute second half

	// use linear interpolation for slopes
	//
	f[0] = (1. - xarg[n]) * f0[0] + xarg[n] * f1[0];
	f[1] = (1. - xarg[n]) * f0[1] + xarg[n] * f1[1];
	f[2] = (1. - xarg[n]) * f0[2] + xarg[n] * f1[2];

	// use hermite interpolation for values
	//
	f[3] = hermite( f0[3], f1[3], f0[n], f1[n], xarg[n] );
}

void torusField::noiseFunction( double *inNoise, double *out )
//
//	Descriptions:
//		A noise function.
//
{
	xlim[0][0] = (int)floor( inNoise[0] );
	xlim[0][1] = xlim[0][0] + 1;
	xlim[1][0] = (int)floor( inNoise[1] );
	xlim[1][1] = xlim[1][0] + 1;
	xlim[2][0] = (int)floor( inNoise[2] );
	xlim[2][1] = xlim[2][0] + 1;

	xarg[0] = inNoise[0] - xlim[0][0];
	xarg[1] = inNoise[1] - xlim[1][0];
	xarg[2] = inNoise[2] - xlim[2][0];

	interpolate( out, 0, 3 ) ;
}

#define TORUS_PI 3.14159265
#define TORUS_2PI 2*TORUS_PI
#define EDGES 30
#define SEGMENTS 20

//
//	Descriptions:
//		Draw a set of rings to symbolie the field. This does not override default icon, you can do that by implementing the iconBitmap() function
//
void torusField::draw( M3dView& view, const MDagPath& path, M3dView::DisplayStyle style, M3dView:: DisplayStatus )
{
	_OPENMAYA_DEPRECATION_PUSH_AND_DISABLE_WARNING
	 view.beginGL();
	 _OPENMAYA_POP_WARNING
	 for (int j = 0; j < SEGMENTS; j++ )
	 {
		glPushMatrix();
		glRotatef( GLfloat(360 * j / SEGMENTS), 0.0, 1.0, 0.0 );
		glTranslatef( 1.5, 0.0, 0.0 );

		 for (int i = 0; i < EDGES; i++ )
		 {
			glBegin(GL_LINE_STRIP);
			float p0 = float( TORUS_2PI * i / EDGES );
			float p1 = float( TORUS_2PI * (i+1) / EDGES );
			glVertex2f( cos(p0), sin(p0) );
			glVertex2f( cos(p1), sin(p1) );
			glEnd();
		 }
		glPopMatrix();
	 }
	 _OPENMAYA_DEPRECATION_PUSH_AND_DISABLE_WARNING
	 view.endGL ();
	 _OPENMAYA_POP_WARNING
}


MStatus initializePlugin(MObject obj)
{
	MStatus status;
	MFnPlugin plugin(obj, PLUGIN_COMPANY, "3.0", "Any");

	status = plugin.registerNode( "torusField", torusField::id,
							&torusField::creator, &torusField::initialize,
							MPxNode::kFieldNode );
	if (!status) {
		status.perror("registerNode");
		return status;
	}

	return status;
}

MStatus uninitializePlugin(MObject obj)
{
	MStatus status;
	MFnPlugin plugin(obj);

	status = plugin.deregisterNode( torusField::id );
	if (!status) {
		status.perror("deregisterNode");
		return status;
	}

	return status;
}

