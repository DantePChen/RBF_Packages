/** Copyright 2012 Autodesk, Inc.  All rights reserved.
Use of this software is subject to the terms of the Autodesk license 
agreement provided at the time of installation or download,  or 
which otherwise accompanies this software in either electronic 
or hard copy form.*/ 

//
//	File Name:	atomFileUtils.h
//
//
//		Utility classes to read and write .atom files.
//

#ifndef _atomFileUtils
#define _atomFileUtils

#include <vector>
#include <set>
#include <string>
#include <map>
#include <maya/MFnAnimCurve.h>
#include <maya/MAngle.h>
#include <maya/MTime.h>
#include <maya/MDistance.h>
#include <maya/MAnimUtil.h>
#include <maya/MFnAttribute.h>
#include <maya/MFnNumericAttribute.h> 
#include <maya/MVector.h> 
#include <maya/MFnUnitAttribute.h>
#include <maya/MPlugArray.h>
#include <maya/MSelectionList.h>
#include <maya/MDGContext.h>
#include <maya/MPlug.h>
#include <maya/MFnAnimCurve.h>
#include "atomNodeNameReplacer.h"

#include <iosfwd>

class atomTemplateReader;
class atomLayerClipboard;

//class to handle getting selected objects in the proper breadth first order
//which let's us consistently 
class SelectionGetter
{
public:

	static void						getSelectedObjects(bool selectedChildren,MSelectionList &list, std::vector<unsigned int> &depths);
private:
	SelectionGetter(){}; //all static funcs
};

class atomCachedPlugs;
// The base class for the translators.
//
class atomBase : public streamIO  {
public:
	//statics @todo, if we make this OO need to find a better place for these guys
	static bool						isEquivalent(double, double);
	static bool						getPlug(const MString &nodeName, const MString &attributeName, MPlug &plug);
	static void						getAttrName(MPlug &plug, MString &attributeName);

	atomBase();
	~atomBase() override;

	enum AnimBaseType			{kAnimBaseUnitless, kAnimBaseTime,
									kAnimBaseLinear, kAnimBaseAngular};

	const char *				tangentTypeAsWord(MFnAnimCurve::TangentType);
	MFnAnimCurve::TangentType	wordAsTangentType(char *);
	const char *				infinityTypeAsWord(MFnAnimCurve::InfinityType);
	MFnAnimCurve::InfinityType 	wordAsInfinityType(const char *);
	const char *				outputTypeAsWord(MFnAnimCurve::AnimCurveType);
	MFnAnimCurve::AnimCurveType	typeAsAnimCurveType(AnimBaseType,
													AnimBaseType);

	AnimBaseType				wordAsOutputType(const char *);
	AnimBaseType				wordAsInputType(const char *);
	const char *				boolInputTypeAsWord(bool);


	MAngle::Unit				getAngularUnit(){return angularUnit;}
	MDistance::Unit				getLinearUnit(){return linearUnit;}

	MDistance::Unit				getOldDistanceUnit(){return mOldDistanceUnit;}
	MTime::Unit					getOldTimeUnit(){return mOldTimeUnit;}

	MTime						getStartTime(){return mStartTime;}
	MTime						getEndTime(){return mEndTime;}
	
	double						getStartUnitless(){return mStartUnitless;}
	double						getEndUnitless(){return mEndUnitless;}
protected:
	void						resetUnits();

	MTime						mStartTime;
	MTime						mEndTime;
	double						mStartUnitless;
	double						mEndUnitless;
	MDistance::Unit				mOldDistanceUnit;
	MTime::Unit					mOldTimeUnit;

	MTime::Unit					timeUnit;
	MAngle::Unit				angularUnit;
	MDistance::Unit				linearUnit;
};

class atomReader : public atomBase {
public:
	atomReader();
	~atomReader() override;

	//new
	MStatus	readAtom(std::ifstream &, atomLayerClipboard &atomClipboard, MSelectionList &mList,atomNodeNameReplacer & replacer, 
		MString& exportEditsFile,bool &removeExportEditsFile, atomTemplateReader &reader, bool replaceLayers);

	void setImportFrameRange( double pImportStartFrame, double pImportEndFrame ) 
	{ 
		mImportStartFrame		= pImportStartFrame; 
		mImportEndFrame			= pImportEndFrame; 
		mImportCustomFrameRange = true;
	}
    
	void clearImportFrameRange() 
	{  
		mImportCustomFrameRange = false;
	}

protected:
	bool	readAnimCurve(std::ifstream&, MAnimCurveClipboardItem&);
	void	convertAnglesAndWeights2To3(MFnAnimCurve::AnimCurveType, bool,
										MAngle &, double &);
	void	convertAnglesAndWeights3To2(MFnAnimCurve::AnimCurveType, bool,
										MAngle &, double &);

	char *	readStaticValue(MString &nodeName,char *dataType, 
			unsigned int depth, unsigned int childCount,std::ifstream &readAnim,
			atomLayerClipboard &cb, atomTemplateReader &reader);
	char * readHeader(std::ifstream &readAnim, bool &hasVersionString,double &startTime, double &endTime, double &startUnitless, double &endUnitless);
	char *	readExportEditsFile(char*, std::ifstream&, MString& filename,bool &removeExportEditsFile);
	char *	readExportEditsFilePresent(char*, std::ifstream&, bool &present, MString &filename);
	
	MStatus readAnim(std::ifstream&, MAnimCurveClipboardItem&);

	MStatus readNodes(char *dataType,std::ifstream &readAnim, atomLayerClipboard &cb,
		MSelectionList &mList, atomNodeNameReplacer & replacer,atomTemplateReader &reader,
		bool replaceLayers, MString& exportEditsFile,bool &removeExportEditsFile);

	char *skipToNextParenth(char *dataType, std::ifstream &readAnim,int parenthCount = 1);


	char * readCachedValues(MString &nodeName,char *dataType, 
			unsigned int depth, unsigned int childCount,std::ifstream &readAnim,
			atomLayerClipboard &cb,atomTemplateReader &reader);
	char * putCachedOnAnimCurve(MString &nodeName,MString &attributeName,char *dataType, std::ifstream &readAnim, 
			MPlug &plug,MFnAnimCurve &curve);
	void	addDynamicAttributeIfNeeded(MString& nodeName,
										MString& attributeName);

	bool  	convertAnglesFromV2To3;
	bool  	convertAnglesFromV3To2;
	
	double mImportStartFrame;
	double mImportEndFrame;
	bool   mImportCustomFrameRange;

	double	animVersion;
};

class atomWriter : public atomBase {
public:
	atomWriter();
	~atomWriter() override;
	
	//note that this also sets the start and endtime for the write
	bool	writeHeader(std::ofstream&,bool useSpedifiedTimes,MTime &startTime, MTime &endTime);
	bool	writeExportEditsFile(std::ofstream&,const MString& filename);
	bool	writeExportEditsFilePresent(std::ofstream&);

	MStatus	writeClipboard(	std::ofstream&, const MAnimCurveClipboard&, atomCachedPlugs *cachedPlugs,const MString &layerName, const MString &nodeName);
	
	void				writeStaticValues (
							std::ofstream &animFile,
							const MPlugArray &animatablePlugs,
							std::set<std::string> &attrStrings, //if this is empty then write out all statics, otherwise only write out these attributes
							const MString &nodeName,
							unsigned int depth,
							unsigned int childCount,
							atomTemplateReader &templateReader
						);

	void				writeCachedValues (
							std::ofstream &animFile,
							atomCachedPlugs *cachedPlugs,
							std::set<std::string> &attrStrings, //if this is empty then write out all statics, otherwise only write out these attributes
							const MString &nodeName,
							unsigned int depth,
							unsigned int childCount,
							atomTemplateReader &templateReader
						);


	void				writeNodeStart(std::ofstream&,atomNodeNameReplacer::NodeType nodeType, MString &name, unsigned int depth =0, unsigned int childCount =0);
	void				writeNodeEnd(std::ofstream&);

protected:

	bool	writeAnim(	std::ofstream&, const MAnimCurveClipboardItem&, const MString &layerName, const MString &nodeName);
	bool 	writeAnimCurve(	std::ofstream&, const MObject *, 
							MFnAnimCurve::AnimCurveType,
							bool = false);
	void	writeValue(std::ofstream & clip,MPlug &plug);
};

class atomUnitNames {
public:
	atomUnitNames();
	virtual ~atomUnitNames();

	static void setToLongName(const MAngle::Unit&, MString&);
	static void setToShortName(const MAngle::Unit&, MString&);

	static void setToLongName(const MDistance::Unit&, MString&);
	static void setToShortName(const MDistance::Unit&, MString&);

	static void setToLongName(const MTime::Unit&, MString&);
	static void setToShortName(const MTime::Unit&, MString&);

	static bool setFromName(const MString&, MAngle::Unit&);
	static bool setFromName(const MString&, MDistance::Unit&);
	static bool setFromName(const MString&, MTime::Unit&);
};

class atomTemplateReader
{
public:
	atomTemplateReader():fTemplateSet(false){}
	~atomTemplateReader();
	void setTemplate(const MString &templateName,MString &viewName);
	bool findNode(const MString &name);
	bool findNodeAndAttr(const MString &name,const MString &attr);
	MString attributesForNode(const MString  &name);
	void selectNodes();
	bool isTemplateSet(){return fTemplateSet;}
protected:
	bool fTemplateSet; //if false not set so no checks.

	struct Attrs
	{
		~Attrs() {fAttrStrings.clear();}
		std::set<std::string> fAttrStrings;
	};
	typedef std::map< std::string, Attrs  > AttrMap;
	AttrMap fNodeAttrs;

};


#endif /* _atomFileUtils */
