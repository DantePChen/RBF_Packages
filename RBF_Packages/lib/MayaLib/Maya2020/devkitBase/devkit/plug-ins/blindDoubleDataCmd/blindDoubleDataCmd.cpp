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
// This plug-in produces the MEL command "blindDoubleData" and user defined data type "blindDoubleData".
// 
// It demonstrates how to create blind data (dynamic attributes) based on user defined data types.
// The plug-in uses a simple double value as the user data type.
// The use of the "MPlug" class to set and retrieve the value of the attribute is demonstrated.
// Moreover, it demonstrates the read and write routines that implement the storage and retrieval of the data in both Maya ASCII and Maya Binary file formats. 
//
// To use this plug-in:
//	Select a dependency node, and then issue the command "blindDoubleData".
//	A dynamic attribute containing the double value "3.2" will be attached to each selected dependency node.
//	If the scene is saved in Maya ASCII format, you will see the MEL commands that save the value of the dynamic attribute.
//	If the scene is reloaded, the dynamic attribute will be reattached to the applicable nodes.
//
////////////////////////////////////////////////////////////////////////

#include <maya/MIOStream.h>

#include <maya/MFnPlugin.h>
#include <maya/MString.h>
#include <maya/MArgList.h>

#include <maya/MPxCommand.h>

#include <maya/MGlobal.h>
#include <maya/MItSelectionList.h>

#include <maya/MFnDependencyNode.h>
#include <maya/MFnTypedAttribute.h>

#include <maya/MPxData.h>
#include <maya/MTypeId.h>
#include <maya/MPlug.h>  
#include <maya/MFnPluginData.h>

///////////////////////////////////////
//
// Proxy data class declaration
//
///////////////////////////////////////
class blindDoubleData : public MPxData
{
public:
							blindDoubleData();
    					~blindDoubleData() override;

    MStatus         readASCII( const MArgList&, unsigned& lastElement ) override;
    MStatus         readBinary( istream& in, unsigned length ) override;
    MStatus         writeASCII( ostream& out ) override;
    MStatus         writeBinary( ostream& out ) override;

	void			copy( const MPxData& ) override;
	MTypeId                 typeId() const override; 
	MString                 name() const override;

    double                  value() const;
    void                    setValue( double );

	static const MString	typeName;
    static const MTypeId    id;
	static void*            creator();

private:
    double                  fValue; 
};

class blindDoubleDataCmd : public MPxCommand
{
public:
						blindDoubleDataCmd();
					~blindDoubleDataCmd() override; 

	MStatus				doIt( const MArgList& args ) override;
	MStatus				redoIt() override;
	MStatus				undoIt() override;
	bool				isUndoable() const override;
	
public:
	static void*		creator();
	MItSelectionList*	iter;
};



//////////////////////////////////////////////////////////////////////////////
//
// Proxy data class implementation
//
//////////////////////////////////////////////////////////////////////////////
void* blindDoubleData::creator()
{
    return new blindDoubleData;
}

blindDoubleData::blindDoubleData()
    :fValue( 0 )
    , MPxData()
{
}

blindDoubleData::~blindDoubleData()
{}

double blindDoubleData::value() const
{
    return fValue;
}

void blindDoubleData::setValue( double newValue  )
{
    fValue = newValue;
}

void blindDoubleData::copy ( const MPxData& other )
{
	fValue = ((const blindDoubleData&)other).fValue;
}

MTypeId blindDoubleData::typeId() const
{
	return blindDoubleData::id;
}

MString blindDoubleData::name() const
{ 
	return blindDoubleData::typeName;
}

MStatus blindDoubleData::readASCII(  const MArgList& args,
                                unsigned& lastParsedElement )
{
    MStatus status;
    if( args.length() > 0 ) {
        fValue = args.asDouble( lastParsedElement++, &status  );
        return status; 
    } else {
        return MS::kFailure;
    }
}

MStatus blindDoubleData::writeASCII( ostream& out )
{
    out << fValue << " ";
    return MS::kSuccess;
}

MStatus blindDoubleData::readBinary( istream& in, unsigned )
{
    in.read( (char*) &fValue, sizeof( fValue ));
    return in.fail() ? MS::kFailure : MS::kSuccess;
}

MStatus blindDoubleData::writeBinary( ostream& out )
{
    out.write( (char*) &fValue, sizeof( fValue));
    return out.fail() ? MS::kFailure : MS::kSuccess;
}

//
// this is the unique type id used to identify the new user-defined type.
//
const MTypeId blindDoubleData::id( 0x80003 );
const MString blindDoubleData::typeName( "blindDoubleData" );


//////////////////////////////////////////////////////////////////////////////
//
// Command class implementation
//
//////////////////////////////////////////////////////////////////////////////

void* blindDoubleDataCmd::creator()
{
	return new blindDoubleDataCmd;
}

blindDoubleDataCmd::~blindDoubleDataCmd()
{
}

blindDoubleDataCmd::blindDoubleDataCmd()
	: MPxCommand()
{
}

MStatus blindDoubleDataCmd::doIt( const MArgList& )
{
	MStatus stat;

	// Create a selection list iterator
	//
    MSelectionList list;
    MGlobal::getActiveSelectionList( list );
	iter = new MItSelectionList( list,	MFn::kInvalid, &stat	);
	if ( MS::kSuccess == stat )
		stat = redoIt();

	return stat;
}

MStatus blindDoubleDataCmd::redoIt()
{
	MStatus     stat;				// Status code
	MObject 	dependNode;		// Selected dependency node

	// Iterate over all selected dependency nodes
	//
	for ( ; !iter->isDone(); iter->next() ) 
	{
		// Get the selected dependency node and create
		// a function set for it
		//
		if ( MS::kSuccess != iter->getDependNode( dependNode ) ) {
			cerr << "Error getting the dependency node" << endl;
			continue;
		}
		MFnDependencyNode fnDN( dependNode, &stat );
		if ( MS::kSuccess != stat ) {
			cerr << "Error creating MFnDependencyNode" << endl;
			continue;
		}

		const MString fullName( "blindDoubleData" );
		if( !fnDN.findPlug( fullName, true ).isNull() ) {
			// already have the attribute
			continue;
		}

		// Create a new attribute for our blind data
		//
		// cerr << "Creating attr..." << endl;
        
		MFnTypedAttribute fnAttr;
		const MString briefName( "BDD" );
		MObject newAttr = fnAttr.create( fullName, briefName,
										 blindDoubleData::id );

		// Now add the new attribute to the current dependency node
		//
		// cerr << "Adding attr..." << endl;
		fnDN.addAttribute( newAttr);
		
		// Create a plug to set and retrive value off the node.
		//
        MPlug plug( dependNode, newAttr );  

		// Instantiate blindDoubleData and set its value.
		//
 		blindDoubleData * newData = new blindDoubleData;
		// cerr << "Setting value of attribute to 3.2" << endl;
 		newData->setValue( 3.2 ); 

		// Set the value for the plug.
		//
        stat = plug.setValue( newData );

		// Now try to retrieve the value off the plug as an MObject.
		//
        MObject sData;
        stat = plug.getValue( sData );

        if ( stat != MS::kSuccess ) {
            cerr << "error getting value off plug" << endl;
            continue;
        }

		// Convert the data back to MPxData.
		//
        MFnPluginData pdFn( sData ); 
        blindDoubleData* data = ( blindDoubleData* ) pdFn.constData( &stat );

		// Get the value.
		//
		if ( NULL == data ) {
			cerr << "error: failed to retrieve data." << endl;  
		}
	}
	return MS::kSuccess;
}

MStatus blindDoubleDataCmd::undoIt()
{
	return MS::kSuccess;
}

bool blindDoubleDataCmd::isUndoable() const
{
	return true;
}


//////////////////////////////////////////////////////////////////////////////
//
// The following routines are used to register/unregister
// the command we are creating within Maya
//
//////////////////////////////////////////////////////////////////////////////
MStatus initializePlugin( MObject obj )
{ 
	MStatus	  status;
	MFnPlugin plugin( obj, PLUGIN_COMPANY, "3.0", "Any" );

	//
	// Registers the new data type.
	status = plugin.registerData("blindDoubleData", blindDoubleData::id,
										   blindDoubleData::creator);
	if (!status) {
		status.perror("registerData");
		return status;
	}
	
	//
	// Register the command.
	status = plugin.registerCommand("blindDoubleData",
									blindDoubleDataCmd::creator);
	if (!status) {
		plugin.deregisterData( blindDoubleData::id );
		status.perror("registerCommand");
		return status;
	}

	return MS::kSuccess;
}

MStatus uninitializePlugin( MObject obj)
{
	MStatus   status;
	MFnPlugin plugin( obj );

	status = plugin.deregisterCommand( "blindDoubleData" );
	if (!status) {
		status.perror("deregisterCommand");
		return status;
	}

	status = plugin.deregisterData( blindDoubleData::id );
	if (!status) {
		status.perror("deregisterData");
		return status;
	}

	return status;
}
