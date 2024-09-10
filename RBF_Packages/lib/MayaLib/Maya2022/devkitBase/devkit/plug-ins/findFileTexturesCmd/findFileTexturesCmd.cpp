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
// Produces the MEL command "findFileTextures".
//
// This example demonstrates how to navigate the dependency graph both manually and using the DG iterator.
// The command searches the dependency graph for file texture nodes that are attached to the shading engine.
// This example also illustrates the use of filters when navigating the DG.
// As file texture nodes are found, information about their attributes is extracted and printed on standard error.
//
// To use this plug-in:
//	(1) Load a scene that contains some texture information, and find a node (or nodes) that are being shaded by a file texture.
//	(2) Execute "findFileTextures nodeName1 nodeName2 ..." to find the file textures. 
//
////////////////////////////////////////////////////////////////////////

#include <maya/MSimple.h>
#include <maya/MObject.h>
#include <maya/MGlobal.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MSelectionList.h>
#include <maya/MString.h>
#include <maya/MObjectArray.h>
#include <maya/MPlug.h>
#include <maya/MItSelectionList.h>
#include <maya/MItDependencyGraph.h>
#include <maya/MFnStringData.h>

#include <maya/MIOStream.h>

//
// This small example illustrates that a node of a particular type can be found
// by walking through the DG graph.  In this example, the nodes of type 
// fileTexture has to be found.  The plugin will start from the shape node that
// the user selected, or the ones passing in as parameters.  It then look for a
// connection to the shadingEngine.  Once found, it will use a dependency graph
// iterator with a filter that matches the type desire to iterate through the
// connections to the shadingEngine. 
//

void dumpInfo( MObject fileNode, 
               MFnDependencyNode& nodeFn, 
			   MObjectArray& nodePath )
{
    MObject currentNode;
	MObject	fileAttr = nodeFn.attribute("fileTextureName");
	MPlug	plugToFile( fileNode, fileAttr ); 
    MFnDependencyNode  dgFn;
	MStatus stat;

    cerr << "Name:    " << nodeFn.name() << endl;

	MObject	fnameValue;
	stat = plugToFile.getValue( fnameValue );
	if ( !stat ) {
		stat.perror("error getting value from plug");
	} else {
		MFnStringData stringFn( fnameValue );
		cerr << "Texture: " << stringFn.string() << endl;
	}

    cerr << "Path:    ";
    for ( int i = nodePath.length()-1; i >= 0; i-- ) {
        currentNode = nodePath[i];
        dgFn.setObject( currentNode );
        cerr << dgFn.name() << "(" << dgFn.typeName() << ")";
		if ( i > 0)
			cerr << " ->\n         ";
    }
    cerr << endl;
}
                
DeclareSimpleCommand( findFileTextures, PLUGIN_COMPANY, "3.0");

MStatus findFileTextures::doIt( const MArgList& args )
{

	MSelectionList list;
    MStatus        status;

	if ( args.length() > 0 ) {
		// Arg list is > 0 so use objects that were passes in
		//
		MString argStr;

		unsigned last = args.length();
		for ( unsigned i = 0; i < last; i++ ) {
			// Attempt to find all of the objects matched
			// by the string and add them to the list
			//
			args.get( i, argStr );  
			list.add( argStr ); 
		}
	} else {
		// Get arguments from Maya's selection list.
		MGlobal::getActiveSelectionList( list );
    }

	MObject             node;
    MFnDependencyNode   nodeFn,dgNodeFnSet;
    MItDependencyGraph* dgIt; 
    MObject             currentNode;
	MObject 			thisNode;
    MObjectArray        nodePath;

	for ( MItSelectionList iter( list ); !iter.isDone(); iter.next() ) {

		iter.getDependNode( node );

        //
        // The following code shows how to navigate the DG manually without
        // using an iterator.  First, find the attribute that you are 
        // interested.  Then connect a plug to it and see where the plug 
        // connected to.  Once you get all the connections, you can choose 
        // which route you want to go.
        //
        // In here, we wanted to get to the nodes that instObjGroups connected
        // to since we know that the shadingEngine connects to the instObjGroup
        // attribute.
        //

        nodeFn.setObject( node );
        MObject iogAttr = nodeFn.attribute( "instObjGroups", &status);
		if ( !status ) {
			cerr << nodeFn.name() << ": is not a renderable object, skipping\n";
			continue;
		}

        MPlug iogPlug( node, iogAttr );
        MPlugArray iogConnections;

        //
        // instObjGroups is a multi attribute.  In this example, just the
        // first connection will be tried.
        //
        iogPlug.elementByLogicalIndex(0).connectedTo( iogConnections, false, true, &status );

		if ( !status ) {
			cerr << nodeFn.name() << ": is not in a shading group, skipping\n";
			continue;
		}

        //
        // Now we would like to traverse the DG starting from the shadingEngine
        // since most likely all file texture nodes will be found.  Note the 
        // filter used to initialize the DG iterator.  There are lots of filter
        // type available in MF::Type that you can choose to suite your needs.
        //
		bool foundATexture = false;
        for ( unsigned int i=0; i<iogConnections.length(); i++ ) {

            currentNode = iogConnections[i].node();

            // 
            // Note that upon initilization, the current pointer of the 
            // iterator already points to the first valid node.
            //
            dgIt = new MItDependencyGraph( currentNode, 
                               MFn::kFileTexture,
                               MItDependencyGraph::kUpstream, 
                               MItDependencyGraph::kBreadthFirst,
                               MItDependencyGraph::kNodeLevel, 
                               &status );
			if ( !status ) {
				delete dgIt;
				continue;
			}
            dgIt->disablePruningOnFilter();

            for ( ; ! dgIt->isDone(); dgIt->next() ) {
              
			   	thisNode = dgIt->currentItem();
                dgNodeFnSet.setObject( thisNode ); 
                status = dgIt->getNodePath( nodePath );

                if ( !status ) {
					status.perror("getNodePath");
					continue;
                }

                //
                // append the starting node.
                //
                nodePath.append(node);
                dumpInfo( thisNode, dgNodeFnSet, nodePath );
				foundATexture = true;
            }
            delete dgIt;
        }
		
		if ( !foundATexture ) {
			cerr << nodeFn.name() << ": is not connected to a file texture\n";
		}
    }
    return MS::kSuccess; 
}
