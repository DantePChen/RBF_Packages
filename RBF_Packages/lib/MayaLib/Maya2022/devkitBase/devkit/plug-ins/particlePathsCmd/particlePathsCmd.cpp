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
// This example plug-in produces the MEL command "particlePaths" that
// demonstrates how particle ID information can be used to trace out curves
// from particle positions over time. All particles in the given particle system
// are followed from the start time to the finish time, and their paths are traced
// out using the MNurbsCurve class. The particle ID is used to identify particles
// at different times.
//
// The following sequence of commands will create a set of curves for a particle explosion: 
//
//	emitter -type omni -r 15 -sro 0 -nuv 0 -cye none -cyi 1 -spd 1 -srn 0 -nsp 1 -tsp 0 -mxd 0 -mnd 0 -dx 1 -dy 0 -dz 0 -sp 0;
//	particle ;
//	connectDynamic -em emitter1 particle1;
//	select -r particleShape1;
//	gravity -pos 0 0 0 -m 0.5 -att 0 -dx 0 -dy -1 -dz 0 -mxd -1 -vsh none -vex 0 -vof 0 0 0 -vsw 360 -tsr 0.5;
//	connectDynamic -f gravityField1 particle1;
//	particlePaths -s 0 -f 4 -i 0.5 particleShape1;
//
// The command uses the function set MFnParticleSystem to sample particle positions
// and identifiers at discrete points in time.
//
// The command "particlePaths" supports the following options:
//
//	-s/-start double : Indicates the starting time (in seconds) for 
//		tracing curves.
//
//	-f/-finish double : Indicates the finish time (in seconds) for 
//		tracing curves.
//
//	-i/-increment double : Indicates the amount of time (in seconds) 
//		between sampling the particle system.
//
//	In addition, a particleShape object must be specified either through the
//	active selection or by passing the node name to the command.
//
//	Example:
//		particlePaths -s 0.0 -f 3.0 -i 0.5 particleShape1
//
// The particle positions will be sampled starting from the start time through
// to the finish time in increments of the increment time. The accumulated particle positions
// will be passed to the MFnNurbsCurve function set to create curves from the accumulated data. 
//	
////////////////////////////////////////////////////////////////////////

#include <maya/MGlobal.h>
#include <maya/MString.h>
#include <maya/MPointArray.h>
#include <maya/MVectorArray.h>
#include <maya/MDoubleArray.h>
#include <maya/MIntArray.h>
#include <maya/MArgList.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MSelectionList.h>
#include <maya/MPxCommand.h>
#include <maya/MFnParticleSystem.h>
#include <maya/MFnPlugin.h>
#include <maya/MFnNurbsCurve.h>
#include <maya/MSyntax.h>
#include <maya/MArgDatabase.h>
#include <maya/MTime.h>
#include <maya/MAnimControl.h>

#include "particleIdHash.h"

// Syntax string definitions
static const char *startFlag = "-s";
static const char *startLongFlag = "-start";
static const char *finishFlag = "-f";
static const char *finishLongFlag = "-finish";
static const char *incrementFlag = "-i";
static const char *incrementLongFlag = "-increment";

static const double TOLERANCE = 1e-10;

//
// particlePaths command class
//

class particlePathsCmd : public MPxCommand
{
public:
				 particlePathsCmd();
    		~particlePathsCmd() override;

    MStatus doIt ( const MArgList& args ) override;

    static void* creator();
	static MSyntax newSyntax();

private:
	MStatus parseArgs ( const MArgList& args );

private:
	MObject particleNode;
	double start,finish,increment;
};

particlePathsCmd::particlePathsCmd() : start(0.0),finish(0.0),increment(0.0)
{
}

particlePathsCmd::~particlePathsCmd() 
{
}


MSyntax particlePathsCmd::newSyntax() {
	MSyntax syntax;
	syntax.addFlag( startFlag, startLongFlag, MSyntax::kDouble );
	syntax.addFlag( finishFlag, finishLongFlag, MSyntax::kDouble );
	syntax.addFlag( incrementFlag, incrementLongFlag, MSyntax::kDouble );
	syntax.setObjectType(MSyntax::kSelectionList,1,1);
	syntax.useSelectionAsDefault();
	return syntax;
}


MStatus particlePathsCmd::parseArgs( const MArgList& args )
{
	MArgDatabase argData(syntax(), args);

	//
	// Parse the time flags
	//

	if (argData.isFlagSet(startFlag))
	{
		argData.getFlagArgument(startFlag, 0, start);
	}

	if (argData.isFlagSet(finishFlag))
	{
		argData.getFlagArgument(finishFlag, 0, finish);
	}

	if (argData.isFlagSet(incrementFlag))
	{
		argData.getFlagArgument(incrementFlag, 0, increment);
	}

	if (finish <= start || increment <= 0.0)
	{
		MGlobal::displayError( "Invalid time arguments." );
		return MS::kFailure;
	}

	//
	// Get the particle system object
	//

	MSelectionList selectList;
	argData.getObjects(selectList);
	
	if( selectList.length() < 1 )
	{
		MGlobal::displayError( "Missing particle node name argument." );
		return MS::kFailure;
	}
	else if( selectList.length() > 1 )
	{
		MGlobal::displayError( "Too many particle nodes given." );
		return MS::kFailure;
	}

	selectList.getDependNode(0,particleNode);

	if (particleNode.isNull() || !particleNode.hasFn(MFn::kParticle))
	{
		MGlobal::displayError( "Invalid node argument." );
		return MS::kFailure;
	}

	return MS::kSuccess;
}

//
// This routine creates the curves for the given particle system
//

MStatus particlePathsCmd::doIt( const MArgList& args )
{
	MStatus stat = parseArgs( args );
	if( stat != MS::kSuccess ) 
	{
		return stat;
	}

	MFnParticleSystem cloud( particleNode );

	if( ! cloud.isValid() )
	{
		MGlobal::displayError( "The function set is invalid!" );
		return MS::kFailure;
	}

	//
	// Create curves from the particle system in two stages.  First, sample 
	// all particle positions from the start time to the end time.  Then,
	// use the data that was collected to create curves.
	//

	// Create the particle hash table at a fixed size.  This should work fine
	// for small particle systems, but may become inefficient for larger ones.
	// If the plugin is running very slow, increase the size.  The value should
	// be roughly the number of particles that are expected to be emitted
	// within the time period.
	//
	ParticleIdHash hash(1024);
	MIntArray idList;

	//
	// Stage 1
	//

	MVectorArray positions;
	MIntArray ids;
	int i = 0;
	for (double time = start; time <= finish + TOLERANCE; time += increment)
	{
		MTime timeSeconds(time,MTime::kSeconds);

		// It is necessary to query the worldPosition attribute to force the 
		// particle positions to update.
		//
		cloud.evaluateDynamics(timeSeconds,false);
//		MGlobal::executeCommand(MString("getAttr ") + cloud.name() + 
//			MString(".worldPosition"));

		if (!cloud.isValid())
		{
			MGlobal::displayError( "Particle system has become invalid." );
			return MS::kFailure;
		}

		MGlobal::displayInfo( MString("Received ") + (int)(cloud.count()) + 
			" particles, at time " + time);

		// Request position and ID data for particles
		//
		cloud.position( positions );
		cloud.particleIds( ids );

		if (ids.length() != cloud.count() || positions.length() != cloud.count())
		{
			MGlobal::displayError( "Invalid array sizes." );
			return MS::kFailure;
		}

		for (int j = 0; j < (int)cloud.count(); j++)
		{
			// Uncomment to show particle positions as the plugin accumulates
			// samples.
			/*
			MGlobal::displayInfo(MString("(") + (positions[j])[0] + MString(",") + 
				(positions[j])[1] + MString(",") + (positions[j])[2] + MString(")"));
			*/

			MPoint pt(positions[j]);
			if (hash.getPoints(ids[j]).length() == 0)
			{
				idList.append(ids[j]);
			}
			hash.insert(ids[j],pt);
		}

		i++;
	}
	
	//
	// Stage 2
	//

	for (i = 0; i < (int)(idList.length()); i++)
	{
		MPointArray points = hash.getPoints(idList[i]);

		// Don't bother with single samples
		if (points.length() <= 1)
		{
			continue;
		}

		// Add two additional points, so that the curve covers all sampled
		// values.
		//
		MPoint p1 = points[0]*2 - points[1];
		MPoint p2 = points[points.length()-1]*2 - points[points.length()-2];
		points.insert(p1,0);
		points.append(p2);

		// Uncomment to show information about the generated curves
		/*
		MGlobal::displayInfo( MString("ID ") + (int)(idList[i]) + " has " + (int)(points.length()) + " curve points.");
		for (int j = 0; j < (int)(points.length()); j++)
		{
			MGlobal::displayInfo(MString("(") + points[j][0] + MString(",") + points[j][1] + MString(",") + points[j][2] + MString(")"));
		}
		*/

		MDoubleArray knots;
		knots.insert(0.0,0);
		for (int j = 0; j < (int)(points.length()); j++)
		{
			knots.append((double)j);
		}
		knots.append(points.length()-1);

		MStatus status;
		MObject dummy;
		MFnNurbsCurve curve;
		curve.create(points,knots,3,MFnNurbsCurve::kOpen,false,false,dummy,&status);
		if (!status)
		{
			MGlobal::displayError("Failed to create nurbs curve.");
			return MS::kFailure;
		}
	}

	return MS::kSuccess;
}


//
// Bookkeeping
//

void * particlePathsCmd::creator() { return new particlePathsCmd(); }


MStatus initializePlugin( MObject obj )
{
    MFnPlugin plugin(obj, PLUGIN_COMPANY, "6.0", "Any");

    MStatus status = plugin.registerCommand("particlePaths",
											particlePathsCmd::creator,
											particlePathsCmd::newSyntax);
	if (!status) 
		status.perror("registerCommand");

    return status;
}


MStatus uninitializePlugin( MObject obj )
{
    MFnPlugin plugin(obj);

    MStatus status = plugin.deregisterCommand("particlePaths");
	if (!status) 
		status.perror("deregisterCommand");

    return status;
}
