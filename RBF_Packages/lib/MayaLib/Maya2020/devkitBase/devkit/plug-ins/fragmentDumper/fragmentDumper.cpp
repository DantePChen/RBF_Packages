//-
// ==========================================================================
// Copyright 2013 Autodesk, Inc.  All rights reserved.
// Use of this software is subject to the terms of the Autodesk license agreement
// provided at the time of installation or download, or which otherwise
// accompanies this software in either electronic or hard copy form.
// ==========================================================================
//+

#include <maya/MFnPlugin.h>
#include <maya/MFragmentManager.h>
#include <maya/MSelectionList.h>
#include <maya/MItSelectionList.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MViewport2Renderer.h>
#include <maya/MPxCommand.h>
#include <maya/MArgList.h>
#include <maya/MGlobal.h>
#include <iostream>

//
// dumpFragment
//	Simple command to output the fragment XML code used to render a given
//	shading node or fragment name in Viewport 2.0. On success, code will 
//	be written to stderr. This demonstrates the usage of MFragmentManager::getFragmentXML().
//
//	Flags:
//		-iu/-includeUpstream
//			If specified, dump the XML for the entire shading graph rooted
//			at the given node, rather than just the graph for the given node
//		-oc/-objectContext
//			If specified, use the current selection as "object context" for
//			when retrieving the XML
//		-fn/-fragmentName
//			If specified, dump the XML for the fragment with the given name
//		-fl/-fragmentList
//			If specified, will provide a list of fragment names
//	Examples:
//		dumpFragment -iu lambert1;
//
//		dumpFragment checker1;
//
//		polySphere -r 1 -sx 20 -sy 20 -ax 0 1 0 -cuv 2 -ch 1 pSphereShape1
//		select pSphereShape1;
//		dumpFragment -iu -oc lambert1
//
//		dumpFragment -fn mayaIrradiance;
//		dumpFragment -fn mayaPointLight;
//		
//		string $a[] = `dumpFragment -fl`;
//		for ($i=0; $i<size($a); $i++)
//		{
//			print($a[$i] + "\n");
//		}
class dumpFragment : public MPxCommand
{
public:
	static void* creator()
	{
		return new dumpFragment();
	}

	dumpFragment() : MPxCommand() {}
	~dumpFragment() override {}

	MStatus doIt(const MArgList& args) override
	{
		// Get VP2 renderer
		MHWRender::MRenderer* renderer = MHWRender::MRenderer::theRenderer();
		if (!renderer) return MS::kFailure;
		MHWRender::MFragmentManager* fragmentMgr =
			renderer->getFragmentManager();
		if (!fragmentMgr) return MS::kFailure;

		// Parse args
		bool includeUpstream = false;
		bool useContext = false;
		bool fragmentList = false;
		bool fragmentName = false;
		MString shaderName;
		for (unsigned int i=0; i<args.length(); i++)
		{
			MString argStr = args.asString(i);
			if (argStr == "-fl" || argStr == "-fragmentList")
			{
				fragmentList = true;
			}
			else if (argStr == "-fn" || argStr == "-fragmentName")
			{
				fragmentName = true;
			}
			else if (argStr == "-iu" || argStr == "-includeUpstream")
			{
				includeUpstream = true;
			}
			else if (argStr == "-oc" || argStr == "-objectContext")
			{
				useContext = true;
			}
			else
			{
				shaderName = argStr;
				break;
			}
		}

		// Get shader if not asking for a fragment or fragment list
		MObject shaderObj;
		if (!fragmentName && !fragmentList)
		{
			if (shaderName.length() > 0)
			{
				MSelectionList list;
				if (!list.add(shaderName))
				{
					displayError(shaderName + ": no such object");
					return MS::kFailure;
				}
				MItSelectionList iter(list);
				iter.getDependNode(shaderObj);
			}
			else
			{
				displayError("No shader specified");
				return MS::kFailure;
			}
		}

		// Get object context if not asking for a fragment or fragment list
		MDagPath path;
		if (!fragmentName && !fragmentList)
		{
			if (useContext)
			{
				MSelectionList activeList;
				MGlobal::getActiveSelectionList(activeList);
				MItSelectionList iter(activeList);
				if (iter.getDagPath(path) != MS::kSuccess ||
					!path.isValid())
				{
					displayError(
						"Object context requested but no DAG object selected");
					return MS::kFailure;
				}
				path.extendToShape();
			}
		}

		// Dump XML
		MStatus status;
		
		if (!fragmentName && !fragmentList)
		{	
			MFnDependencyNode node(shaderObj, &status);
			if (status == MS::kSuccess)
			{
				MString buffer;
				if (fragmentMgr->getFragmentXML(shaderObj, buffer, includeUpstream, useContext ? &path : NULL))
				{
					std::cerr << "##############################################################################" << std::endl;
					std::cerr << "Fragment graph for shading network rooted at " << node.name().asChar();
					std::cerr << "(type: " << node.typeName().asChar() << ")" << std::endl;
					if (useContext)
					{
						std::cerr << "\tUsing object context: " << path.fullPathName().asChar() << std::endl;
					}
					std::cerr << "##############################################################################" << std::endl;
					std::cerr << buffer.asChar() << std::endl;
					std::cerr << "##############################################################################" << std::endl;
					return MS::kSuccess;
				}
				else
				{
					displayError(
						"Failed to get fragment graph XML for " + shaderName);
				}
			}
			else
			{
				displayError(shaderName + " is not a dependency node");
			}
		}
		else
		{
			if (fragmentList)
			{
				MStringArray fragmentNames;
				fragmentMgr->fragmentList(fragmentNames);
				setResult( fragmentNames );
				return MS::kSuccess;
			}
			else if (shaderName.length())
			{
				MString buffer;
				if (fragmentMgr->getFragmentXML(shaderName, buffer))
				{
					setResult( buffer );
					return MS::kSuccess;
				}
				else
				{
					displayError("Failed to retrieve fragment " + shaderName);
				}
			}
		}
		return MS::kFailure;
	}
};

MStatus initializePlugin(MObject obj)
{
	MFnPlugin plugin(obj, PLUGIN_COMPANY, "1.0", "Any");

	MStatus status = plugin.registerCommand(
		"dumpFragment", dumpFragment::creator);
	if (!status)
	{
		status.perror("registerCommand");
		return status;
	}

	return status;
}

MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin plugin(obj);

	MStatus status = plugin.deregisterCommand("dumpFragment");
	if (!status)
	{
		status.perror("deregisterCommand");
		return status;
	}

	return status;
}
