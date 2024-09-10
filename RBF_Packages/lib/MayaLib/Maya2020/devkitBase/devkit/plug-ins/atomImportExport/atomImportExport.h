/** Copyright 2012 Autodesk, Inc.  All rights reserved.
Use of this software is subject to the terms of the Autodesk license 
agreement provided at the time of installation or download,  or 
which otherwise accompanies this software in either electronic 
or hard copy form.*/ 

//
//	File Name:	atomImportExport.h
//
//
//		Import and exports .atom files.
//

#ifndef _atomImport
#define _atomImport

#include <map>
#include <maya/MPxFileTranslator.h>
#include <maya/MSelectionList.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MTime.h>
#include <maya/MAnimCurveClipboardItemArray.h>
#include "atomFileUtils.h"
#include "atomCachedPlugs.h"
#include "atomAnimLayers.h"

#include <iosfwd>

class atomNodeNameReplacer;

class atomEditsHelper {
public:
	atomEditsHelper(atomNodeNameReplacer* replacer, atomTemplateReader* templateReader): fReplacer(replacer), fTemplateReader(templateReader) {}
	virtual ~atomEditsHelper() {}

	atomNodeNameReplacer* 	fReplacer;
	atomTemplateReader* 	fTemplateReader;
};




// The atomImport command object
//
class atomImport : public MPxFileTranslator {
public:
	atomImport ();
	~atomImport () override; 

	MStatus				doIt (const MArgList &argList);
	static void *		creator ();

	MStatus		reader(	const MFileObject& file,
						const MString& optionsString,
						FileAccessMode mode) override;

	bool		haveReadMethod() const override;
	bool		haveWriteMethod() const override;
	bool		canBeOpened() const override;
	MString 	defaultExtension() const override;
	MFileKind	identifyFile(	const MFileObject& fileName,
										const char* buffer,
										short size) const override;

    static void 		connectionFailedCallback(MPlug& srcPlug,
												 MPlug& dstPlug,
												 const MString& srcName,
												 const MString& dstName,
												 void* clientData);

private:
	MStatus				importAnim(MSelectionList &list,std::ifstream&, const MString&, atomNodeNameReplacer &, MString&,
									atomTemplateReader &, bool replaceLayers);
	static bool 		replaceNameAndFindPlug(const MString&,
											   atomNodeNameReplacer&,
											   MPlug&);
	static bool 		checkPlugAgainstTemplate(const MString&,
												 const MPlug&,
												 atomTemplateReader*);

private:

	atomReader			fReader;
	

};

class atomWriter;

// The atomImport command object
//
class atomExport : public MPxFileTranslator {
public:
	atomExport ();
	~atomExport () override; 

	static void *		creator ();

	MStatus 	writer(	const MFileObject& file,
						const MString& optionsString,
						FileAccessMode mode ) override;

	bool		haveReadMethod() const override;
	bool		haveWriteMethod() const override;
	MString 	defaultExtension() const override;
	MFileKind	identifyFile(	const MFileObject& fileName,
										const char* buffer,
										short size) const override;
private:
	MStatus				exportSelected(std::ofstream&, MString &,
										std::set<std::string> &attrStrings,bool includeChildren, 
										bool useSpecifiedTimes, MTime &startTime,
										MTime &endTime,
										bool statics, bool cached, bool sdk, bool constraint, bool layers,
										const MString& exportEditsFile,
										atomTemplateReader &reader);


	void				writeStaticAndCached (MPlugArray &animtablePlugs, atomCachedPlugs *cachedPlugs,bool statics, bool cached, std::ofstream &animFile,
									std::set<std::string> &attrStrings,
									MString &name, unsigned int depth,
									unsigned int childCount,bool &hasAnimatable,
									atomTemplateReader &reader);

	MStatus				writeAnimCurves(std::ofstream &animFile,MString &nodeName,atomCachedPlugs *cachedPlugs,atomNodeWithAnimLayers *layerPlugs,
									MString &command, bool &haveAnimatedCurves,	atomTemplateReader &reader);

	bool				setUpCache(MSelectionList &sList,  std::vector<atomCachedPlugs *> &cachedPlugs,atomAnimLayers &animLayers,
									bool sdk, bool constraint, bool layers,
									std::set<std::string> &attrStrings, atomTemplateReader &templateReader,
									MTime &startTime, MTime &endTime,MAngle::Unit angularUnit,
									MDistance::Unit	linearUnit);

	bool				setUpAnimLayers(MSelectionList &sList,atomAnimLayers &animLayers, 
									std::vector<atomNodeWithAnimLayers *> &nodesWithAnimLayers,
									std::set<std::string> &attrStrings, atomTemplateReader &templateReader);


	//MStatus				writeSetDrivenKeys(ofstream &animFile, MFnDependencyNode &fnNode,MString &name, bool &hasSetDrivenKey);
	atomWriter			fWriter;
};
#endif

