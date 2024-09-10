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
// filteredAsciiFile.cpp
// 
// This plug-in is an example of a user-defined MPxMayaAsciiFilter. It
// allows users to segment scenes into files containing various types
// of nodes. Users can specify which node types are exported by 
// using the "includeNodeType" and "excludeNodeType" options in the file command.
// For example, the following would save only renderLayer nodes:
//
// mel: file -rename "myLayerData";
// mel: file -type "filteredAsciiFile";
// mel: file -save -options ";includeNodeType kRenderLayer;";
//
// In addition, the type of connectAttrs written in the ASCII file are
// determined by the nodes types which are exported. If the source node
// in the connectAttr is to be output, then the connectAttr is written.
//
// In addition to allowing scenes to be segmented, this plugin also
// allows ASCII files to write a line which will source other ASCII
// files. This can be used to allow a single master file source in 
// all of the different files containing scene segments. Again, this is
// controlled via the file command's options. The following illustrates
// how to source in the render layer segment saved in the above example:
//
// mel: file -rename "myMasterFile";
// mel: file -save -options ";excludeNodeType kRenderLayer; sourceFile myLayerData.faf"
// 
// Here, the excludeType argument is used to ensure no render layer 
// information is output in myMasterFile. The sourceFile option outputs
// the following line in myMasterFile.faf:
//
// customSourceFile -fileName "myLayerData.faf";
//
// When this line is parsed as myMasterFile.faf is loaded, the 
// customSourceFile command, also contained in this plugin, is executed.
// It simply sources myLayerData.faf, first prepending the 
// myMasterFile.faf's path.
//
////////////////////////////////////////////////////////////////////////

#include <maya/MPxMayaAsciiFilter.h>
#include <maya/MFnPlugin.h>
#include <maya/MFileIO.h>
#include <maya/MFileObject.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MSyntax.h>
#include <maya/MArgDatabase.h>
#include <maya/MPxCommand.h>
#include <maya/MGlobal.h>
#include <maya/MString.h>
#include <maya/MStringArray.h>

#include <vector>

#define kFileNameFlag		"-fn"
#define kFileNameFlagLong	"-fileName"

#define commandName			"customSourceFile"


class filteredAsciiFile : public MPxMayaAsciiFilter
{
public:
	static void*	creator();
	static void		setPluginName(const MString& name);
	static MString	translatorName();
	MString			defaultExtension() const override;

					filteredAsciiFile();

	static MString	fExtension;
	static MString	fPluginName;
	static MString	fTranslatorName;

protected:

	MStatus			processReadOptions(const MString& optionsString) override;
	MStatus			processWriteOptions(const MString& optionsString) override;
	
	bool			writesRequirements() const override;
	bool			writesCreateNode( const MObject& node ) const override;
	bool			writesSelectNode( const MObject& node ) const override;
	bool			writesFileReference( const MFileObject& referenceFile ) const override;
	bool			writesConnectAttr( const MPlug& srcPlug,
									   const MPlug& destPlug ) const override;
	bool			writesSetAttr( const MPlug& srcPlug ) const override;

	MStatus			writePostRequires( MPxMayaAsciiFilterOutput& fileIO ) override;
	MStatus			writePostCreateNodesBlock( MPxMayaAsciiFilterOutput& fileIO ) override;
		
private:
	bool					isNodeTypeIncluded(const MString& type) const;
	bool					isNodeNameExcluded(const MObject& node) const;

	MStringArray			includedNodeTypesArray;
	MStringArray			excludedNodeTypesArray;
	MStringArray			sourcedFilesArray;
	MStringArray			excludedNamesArray;
	
	bool					outputRequirements;
	bool					outputReferences;
};

MString filteredAsciiFile::fExtension = "faf";
MString filteredAsciiFile::fPluginName = "";
MString filteredAsciiFile::fTranslatorName = "filteredAsciiFile";

inline void filteredAsciiFile::setPluginName(const MString& name)
{	fPluginName = name;		}


inline MString filteredAsciiFile::translatorName()
{	return fTranslatorName;	}

inline MString filteredAsciiFile::defaultExtension() const
{	return fExtension;		}

filteredAsciiFile::filteredAsciiFile() :
	includedNodeTypesArray()
,	outputRequirements(false)
,	outputReferences(true)
{
}

MStatus	filteredAsciiFile::processReadOptions(const MString& optionsString)
{
	return MStatus::kSuccess;
}

MStatus	filteredAsciiFile::processWriteOptions(const MString& optionsString)
{
	MStringArray optionsArray;

	includedNodeTypesArray.clear();
	excludedNodeTypesArray.clear();
	sourcedFilesArray.clear();
	excludedNamesArray.clear();
	
	outputRequirements = true;
	outputReferences = true;
	
	optionsString.split(';', optionsArray);

	for (unsigned i = 0; i < optionsArray.length(); i++)
	{
		const MString& option = optionsArray[i];
		MStringArray optionArray;

		option.split(' ', optionArray);

		if (optionArray[0] == "includeNodeType" && optionArray.length() > 1)
		{
			includedNodeTypesArray.append(optionArray[1]);
		}
		else if (optionArray[0] == "excludeNodeType" && optionArray.length() > 1)
		{
			excludedNodeTypesArray.append(optionArray[1]);
		}
		else if (optionArray[0] == "sourceFile" && optionArray.length() > 1)
		{
			sourcedFilesArray.append(optionArray[1]);
		}
	}

	return MStatus::kSuccess;
}
	
void* filteredAsciiFile::creator()
{
	return new filteredAsciiFile();
}

bool filteredAsciiFile::writesRequirements() const
{
	return outputRequirements;
}

bool filteredAsciiFile::writesCreateNode( const MObject& node ) const
{
	bool result = false;

	MFnDependencyNode depNode(node);

	if (depNode.isFromReferencedFile() && !outputReferences)
	{
		return false;
	}

	if (isNodeNameExcluded(node))
	{
		return false;
	}

	if (!result)
	{
		result = isNodeTypeIncluded(node.apiTypeStr());
	}

	return result;
}

bool filteredAsciiFile::writesConnectAttr( const MPlug& srcPlug,
									 const MPlug& destPlug ) const
{
	return (writesCreateNode(srcPlug.node()) && !isNodeNameExcluded(destPlug.node()));
}

bool filteredAsciiFile::writesSelectNode( const MObject& node ) const
{
	return writesCreateNode(node);
}

bool filteredAsciiFile::writesFileReference( const MFileObject& referenceFile ) const
{
	return outputReferences;
}

bool filteredAsciiFile::writesSetAttr( const MPlug& srcPlug ) const
{
	return writesCreateNode(srcPlug.node());
}

MStatus filteredAsciiFile::writePostRequires( MPxMayaAsciiFilterOutput& fileIO )
{
	fileIO << "requires " << fPluginName.asChar() << " \"1.0\";\n";
	return MS::kSuccess;
}

MStatus	filteredAsciiFile::writePostCreateNodesBlock( MPxMayaAsciiFilterOutput& fileIO )
{
	for (unsigned int i = 0; i < sourcedFilesArray.length(); i++)
	{
		fileIO << "eval (\"customSourceFile -fileName \\\"";
		fileIO << sourcedFilesArray[i].asChar();
		fileIO << "\\\";\");\n";
	}

	return MS::kSuccess;
}

bool filteredAsciiFile::isNodeTypeIncluded(const MString& nodeType) const
{
	if ( (excludedNodeTypesArray.length() == 0 ) &&
		 (includedNodeTypesArray.length() == 0 ) )
	{
		// there are no types specifically included or excluded,
		// so we assume everything is included.

		return true;
	}

	// if we aren't excluding any nodes at this point, then we're
	// only interested in nodes which are explicitly included.

	bool result = (excludedNodeTypesArray.length() > 0);
	
	for (unsigned int i = 0; i < includedNodeTypesArray.length(); i++)
	{
		if (nodeType == includedNodeTypesArray[i])
		{
			result = true;
			break;
		}
	}

	for (unsigned int j = 0; j < excludedNodeTypesArray.length(); j++)
	{
		if (nodeType == excludedNodeTypesArray[j])
		{
			result = false;
			break;
		}
	}

	return result;
}

bool filteredAsciiFile::isNodeNameExcluded(const MObject& node) const
{
	for (unsigned int i = 0; i < excludedNamesArray.length(); i++)
	{
		MFnDependencyNode depNode(node);

		if (excludedNamesArray[i] == depNode.name())
		{
			return true;
		}
	}

	return false;
}

// ****************************************

class sourceFileCmd : public MPxCommand
{
public:
	sourceFileCmd();
				~sourceFileCmd() override; 

	MStatus			doIt( const MArgList& args ) override;

	static MSyntax	newSyntax();
	static void*	creator();

private:
	
};

sourceFileCmd::sourceFileCmd()
{
}

sourceFileCmd::~sourceFileCmd()
{
}

MStatus
sourceFileCmd::doIt (const MArgList& args )
{
	MString fileName;
	MStatus status = MS::kSuccess;

	MArgDatabase	argData(syntax(), args);

	if (argData.isFlagSet(kFileNameFlag))
	{
		status = argData.getFlagArgument(kFileNameFlag, 0, fileName);

		if (status == MS::kSuccess)
		{
			MString currFile = MFileIO::fileCurrentlyLoading();

			MStringArray pathDirectories;

			currFile.split ('/', pathDirectories);

			if (pathDirectories.length() > 0)
			{
				MString expandedFileName;

				for (unsigned int i = 0; i < pathDirectories.length()-1; i++)
				{
					expandedFileName += pathDirectories[i];
					expandedFileName += "/";
				}

				expandedFileName += fileName;

				MGlobal::sourceFile(expandedFileName);
			}
		}
	}	

	return status;
}

void*
sourceFileCmd::creator()
{
	return (void*) new sourceFileCmd();
}

MSyntax sourceFileCmd::newSyntax()
{
	 MSyntax syntax;
	 syntax.addFlag(kFileNameFlag, kFileNameFlagLong, MSyntax::kString);
	 return syntax;
}

// ****************************************

MStatus initializePlugin(MObject obj)
{
	MFnPlugin plugin(obj, PLUGIN_COMPANY, "1.0", "Any");

	filteredAsciiFile::setPluginName(plugin.name());

	plugin.registerFileTranslator(
		filteredAsciiFile::translatorName(),
		NULL,
		filteredAsciiFile::creator,
		NULL,
		NULL,
		false
	);

	MStatus status = plugin.registerCommand( commandName,
									 sourceFileCmd::creator,
									 sourceFileCmd::newSyntax);

	return status;
}


MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin plugin( obj );

	MStatus status;
	status = plugin.deregisterFileTranslator(filteredAsciiFile::translatorName());
	if (!status) 
	{
		status.perror("deregisterFileTranslator");
		MGlobal::displayInfo(MString("Failed to unregister file translator ") + MString(filteredAsciiFile::translatorName()));
		return status;
	}
	
	status = plugin.deregisterCommand(commandName);
	if (!status) 
	{
		status.perror("deregisterCommand");
		MGlobal::displayInfo(MString("Failed to unregister cmd  ") + MString(commandName));
		return status;
	}

	return status;
}
