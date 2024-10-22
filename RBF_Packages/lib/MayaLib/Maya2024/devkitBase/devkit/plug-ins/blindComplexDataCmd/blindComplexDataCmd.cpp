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
// This plug-in produces the MEL command "blindComplexData" and user defined data type "blindComplexData". 
//
// It demonstrates how to create blind data (dynamic attributes) based on user defined data types.
// The plug-in uses an array of structures in which each element contains both a double and an int as the user data type.
// 
// The use of the "MPlug" class to set and retrieve the value of the attribute is demonstrated.
// Moreover, it demonstrates the read and write routines that implement the storage and retrieval of the data in both Maya ASCII and Maya Binary file formats.
// 
// To use this plug-in, select a dependency node and then issue the command "blindComplexData".
// A dynamic attribute containing a five element array is attached to each selected dependency node.
// If the scene is saved in Maya ASCII format, you will see the MEL commands that save the value of the dynamic attribute.
// If the scene is reloaded, the dynamic attribute will be reattached to the applicable nodes.
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
#include <maya/MFnNurbsCurve.h>

#include <assert.h>

struct CVData {

	MStatus readASCII( const MArgList&, unsigned& );
	MStatus writeASCII( ostream& );

	MStatus readBinary( istream& in );
	MStatus writeBinary( ostream& out );

	double  _doubleData;
	int		_intData;

	static CVData empty;
};

CVData CVData::empty;

///////////////////////////////////////
//
// Proxy data class declaration
//
///////////////////////////////////////

class blindComplexData : public MPxData
{
public:
    blindComplexData();
    blindComplexData( const unsigned int );
    ~blindComplexData() override;

	//
	// Override methods in MPxData.
    MStatus         readASCII( const MArgList&, unsigned& lastElement ) override;
    MStatus         readBinary( istream& in, unsigned length ) override;
    MStatus         writeASCII( ostream& out ) override;
    MStatus         writeBinary( ostream& out ) override;

	//
	// Custom methods.
    void			copy( const MPxData& ) override; 

	MTypeId                 typeId() const override; 
	MString					name() const override;

	bool				setLength( const unsigned int numCVs,
									   const bool copyOldData = false );
	
	const CVData &operator [] ( const unsigned int index ) const;
	CVData &operator [] ( const unsigned int index );

	unsigned int 					length() const;

	//
	// static methods and data.
	static const MString    typeName;
    static const MTypeId    id;
	static void*            creator();

private:

	bool indexOk( const unsigned int ) const;

	// 
	// any customized data can be declared here, in this case, an array of 
	// CVData that is associated with the CVs. 
	
	CVData*					_CVDataArrayPtr;	
	unsigned int					_length;
};

const MTypeId blindComplexData::id( 0x80002 );
const MString blindComplexData::typeName( "blindComplexData" );

class blindComplexDataCmd : public MPxCommand
{
public:
						blindComplexDataCmd();
					~blindComplexDataCmd() override; 

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

void* blindComplexData::creator()
{
    return new blindComplexData;
}

blindComplexData::blindComplexData()
    :_CVDataArrayPtr( NULL ),
	 _length(0)
{
}

blindComplexData::~blindComplexData()
{
	delete [] _CVDataArrayPtr;
	_length = 0;
}

blindComplexData::blindComplexData( const unsigned int cvNum )
	: _length(0),
	  _CVDataArrayPtr( NULL )
{
	setLength( cvNum );
}

//
// Set the length of the array  
bool blindComplexData::setLength ( const unsigned int cvNum, 
										 const bool copyOldData ) 
{
	bool ret = true;
	if ( _length != cvNum || _CVDataArrayPtr == NULL ) {
		CVData* ptr;
		if ( _length != 0 && ! copyOldData ) {
			cerr << "warning: might be erasing previous data." << endl;
		}
		ptr = new CVData[cvNum];
		if ( ptr == NULL ) {
			cerr << "out of memory, setLength() failed" << endl;
			ret = false;
		} else {
			if ( copyOldData ) {
				if ( cvNum < _length ) {
					cerr << "warning: new size not big enough for old data." 
						 << endl;
				}
				for ( unsigned int i = 0; i < cvNum; i++ ) {
					ptr[i] = _CVDataArrayPtr[i];  
				}
			}
			delete [] _CVDataArrayPtr;
			_CVDataArrayPtr = ptr;
			_length = cvNum;
		}
	} else {
		cerr << "warning: setLength() o.k., same length, no change." << endl;
	}
	return ret;
}

unsigned int blindComplexData::length ( ) const 
{
	return _length;
}

inline 
bool blindComplexData::indexOk( const unsigned int index ) const {
	return ( index < _length );
}

const CVData& blindComplexData::operator [] ( const unsigned int index ) const 
{
	if ( indexOk( index  ) ) {
		return _CVDataArrayPtr[index];
	} else {
		cerr << "indexing error.  operator [] failed, returning invalid object"
		 	 << endl;
		return CVData::empty;
	}
}

CVData &blindComplexData::operator [] ( const unsigned int index ) 
{
	if ( indexOk( index  ) ) {
		return _CVDataArrayPtr[index];
	} else {
		cerr << "indexing error.  operator [] failed, returning invalid object"
		 	 << endl;
		// 
		// to get away a compiler warning.
		return CVData::empty; 
	}
}

void blindComplexData::copy( const MPxData& other )
//
//  Deescription:
//      Perform a copy or a conversion
//
{ 
    if( other.typeId() == blindComplexData::id ) {
        const blindComplexData* otherData =
										(const blindComplexData*)&other;
		if ( _length != otherData->_length ) {
			if ( ! setLength( otherData->_length ) ) {
				return;
			}
		} 
		for ( unsigned int i = 0; i < _length; i++ ) {
			_CVDataArrayPtr[i] = (*otherData)[i];
		}

    } else {
        //  we need to convert to the other type based on its iff Tag
		cerr << "wrong data format!" << endl;
    }
    return;
}

MTypeId blindComplexData::typeId() const
{
	return blindComplexData::id;
}

MString blindComplexData::name() const
{ 
	return blindComplexData::typeName; 
}

MStatus blindComplexData::readASCII(  const MArgList& args,
        		                        unsigned& lastParsedElement )
{
    MStatus status;
	int argLength = args.length();
    if( argLength > 0 ) {
		int numDataRecord = (argLength - lastParsedElement); 
		//
		// Note: a better solution to determine the number of records is to
		// write out the number of records in the writeASCII() routine.
		//
		if ( numDataRecord % 2 != 0 ) {
			cerr << "warning: corrupted data for blindComplexData" << endl;
		}
		// 
		// 2 numbers per record.
		numDataRecord /= 2;
		setLength(numDataRecord); 
		for ( unsigned int i=0; i < _length; i++ ) {
        	status = _CVDataArrayPtr[i].readASCII(args, lastParsedElement);
			if ( status != MS::kSuccess ) {
				return status;
			}
		}
		return MS::kSuccess;
    } 
	return MS::kFailure;
}

MStatus CVData::readASCII ( const MArgList& args,
							unsigned& lastParsedElement ) 
{
	MStatus status;
	_intData = args.asInt( lastParsedElement++, &status );
	if ( status == MS::kSuccess ) {
	    _doubleData = args.asDouble( lastParsedElement++, &status );
	} 
	return status;
}
				
MStatus CVData::writeASCII ( ostream& out )  
{
	out << _intData << " " << _doubleData << " ";
	return out.fail() ? MS::kFailure : MS::kSuccess;
}

MStatus blindComplexData::writeASCII( ostream& out )
{
	MStatus status;
	for ( unsigned int i=0; i < _length; i++ ) {
		status = _CVDataArrayPtr[i].writeASCII(out);
		if ( status != MS::kSuccess ) {
			return MS::kFailure;
		}
	}
    return MS::kSuccess;
}

MStatus blindComplexData::readBinary( istream& in, unsigned length )
{
	MStatus status;
	if ( length > 0 ) {
		unsigned int recNum;
		in.read( (char*) &recNum, sizeof(recNum) );
		if ( ! in.fail() && recNum > 0 ) {
			setLength(recNum); 
			for ( unsigned int i=0; i < _length; i++ ) {
				status = _CVDataArrayPtr[i].readBinary(in);
				if ( status != MS::kSuccess ) {
					return status;
				}
			}
		}
	} else {
		return MS::kFailure;
	}
	return MS::kSuccess;
}

MStatus blindComplexData::writeBinary( ostream& out )
{
	MStatus status;
	out.write( (char*) &_length, sizeof(_length) );
	if ( ! out.fail() ) {
		for ( unsigned int i=0; i < _length; i++ ) {
			status = _CVDataArrayPtr[i].writeBinary(out);
			if ( status != MS::kSuccess ) {
				return status; 
			}
		}
	} else {
		return MS::kFailure;
	}
    return MS::kSuccess;
}

MStatus CVData::readBinary ( istream& in )
{
	in.read( (char*) &_intData, sizeof(_intData) );
	if ( !in.fail() ) {
		in.read( (char*) &_doubleData, sizeof(_doubleData) );
	} else {
		return MS::kFailure;
	}
	return in.fail() ? MS::kFailure : MS::kSuccess;
}
				
MStatus CVData::writeBinary ( ostream& out )  
{
	out.write( (char*) &_intData, sizeof(_intData) );
	if ( !out.fail() ) {
		out.write( (char*) &_doubleData, sizeof(_doubleData) );
	} else {
		return MS::kFailure;
	}
	return out.fail() ? MS::kFailure : MS::kSuccess;
}


//////////////////////////////////////////////////////////////////////////////
//
// Command class implementation
//
//////////////////////////////////////////////////////////////////////////////

void* blindComplexDataCmd::creator()
{
	return new blindComplexDataCmd;
}

blindComplexDataCmd::~blindComplexDataCmd()
{
}

blindComplexDataCmd::blindComplexDataCmd()
	: MPxCommand()
{
}

MStatus blindComplexDataCmd::doIt( const MArgList& )
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

MStatus blindComplexDataCmd::redoIt()
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

		// Create a new attribute for our blind data
		//
		// cout << "Creating attr..." << endl;
		
		MFnTypedAttribute fnAttr;
		const MString fullName( "blindComplexData" );
		const MString briefName( "BCD" );
		MObject newAttr = fnAttr.create( fullName, briefName,
										 blindComplexData::id );

		// Now add the new attribute to the current dependency node
		//
		// cout << "Adding attr..." << endl;
		fnDN.addAttribute( newAttr);
		
		//
		// now we will demonstrate setting the value by using a plug.
		MPlug plug( dependNode, newAttr );  

		//
		// create an instance of the blind data with an initial array size of
		// 5.
		blindComplexData * newData = new blindComplexData( 5 );

		//
		// initialized 
		// cout << "setting data values..." << endl;
		unsigned int i;
		for ( i= 0; i < newData->length(); i++ ) {
			(*newData)[i]._intData = 10 + i;
			(*newData)[i]._doubleData = 20.02 + i;
		}

		// 
		// setting the value for the plug.
		stat = plug.setValue( newData );

		//
		// The following code demonstrates the retrieving of data from the 
		// plug.
		MObject sData;
		stat = plug.getValue( sData );

		if ( stat != MS::kSuccess ) {
			cerr << "error getting value off plug" << endl;
			continue;
		}

		// 
		// Convert the data from an MObject back to a pointer to MPxData, then
		// cast it back to a pointer to blindComplexData.
		MFnPluginData pdFn( sData ); 
		blindComplexData* data =
							( blindComplexData* ) pdFn.constData( &stat );
		//
		// read the data, and set the result to the values set.
		
		clearResult();

		if ( NULL != data ) {
			// cout << "retrieving data values..." << endl;
			for ( i = 0; i < data->length(); i++ ) {
				// cout << "rec #" << i << ": " << (*data)[i]._intData << ", "; 
				// cout << (*data)[i]._doubleData << endl;
				appendToResult((double) ((*data)[i]._intData));
				appendToResult((*data)[i]._doubleData);
			}
		} else {
			// cout << "Null data" << endl;
		}

	}
	return MS::kSuccess;
}

MStatus blindComplexDataCmd::undoIt()
{
	return MS::kSuccess;
}

bool blindComplexDataCmd::isUndoable() const
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

	status = plugin.registerData( "blindComplexData",
						  blindComplexData::id,
						  blindComplexData::creator );
	if (!status) {
		status.perror("registerData");
		return status;
	}

	status = plugin.registerCommand(  "blindComplexData",
							 blindComplexDataCmd::creator );
	if (!status) {
		plugin.deregisterData( blindComplexData::id );
		status.perror("registerCommand");
		return status;
	}

	return status;
}

MStatus uninitializePlugin( MObject obj)
{
	MStatus	  status1, status2;
	MFnPlugin plugin( obj );

	status1 = plugin.deregisterCommand( "blindComplexData" );
	if (!status1) {
		status1.perror("deregisterCommand");
	}
	status2 = plugin.deregisterData( blindComplexData::id );
	if (!status2) {
		status2.perror("deregisterData");
	}

	if ( !status1 )
		return status1;
	return status2;
}
