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


#ifndef _apiMeshShape
#define _apiMeshShape

///////////////////////////////////////////////////////////////////////////////
//
// apiMeshShape.h
//
// Implements a new type of shape node in maya called apiMesh.
//
// INPUTS
//     inputSurface    - input apiMeshData
//     outputSurface   - output apiMeshData
//     worldSurface    - array of world space apiMeshData, each element
//                       represents an istance of the shape
// OUTPUTS
//     mControlPoints  - inherited control vertices for the mesh. These values
//                       are tweaks (offsets) that will be applied to the
//                       vertices of the input shape.
//     bboxCorner1     - bounding box upper left corner
//     bboxCorner2     - bounding box lower right corner
//
////////////////////////////////////////////////////////////////////////////////

#include <map>
#include <string>

#include <maya/MPxSurfaceShape.h>
#include <maya/MPxGeometryIterator.h>
#include <maya/MTypeId.h>
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MVectorArray.h>
#include <maya/MDoubleArray.h>
#include <maya/MSelectionList.h>
#include <maya/MSelectionMask.h>
#include <maya/MNodeMessage.h>
#include "apiMeshGeom.h"
#include "apiMeshData.h"
#include "apiMeshIterator.h"


class apiMesh : public MPxSurfaceShape
{
public:
	apiMesh();
	~apiMesh() override;

    //////////////////////////////////////////////////////////
	//
	// Overrides
	//
    //////////////////////////////////////////////////////////

	// From MPxNode
	//
	void			postConstructor() override;
	MStatus			preEvaluation(const MDGContext& context, const MEvaluationNode& evaluationNode) override;
	MStatus			postEvaluation(const MDGContext& context, const MEvaluationNode& evaluationNode, PostEvaluationType evalType) override;
	MStatus			compute( const MPlug& plug, MDataBlock& data ) override;
	MStatus			setDependentsDirty( const MPlug& plug, MPlugArray& plugArray) override;
	bool			getInternalValue( const MPlug&, MDataHandle&) override;
	bool			setInternalValue( const MPlug&, const MDataHandle&) override;
	MStatus			connectionMade( const MPlug& plug,
											 const MPlug& otherPlug,
											 bool asSrc ) override;
	MStatus			connectionBroken( const MPlug& plug,
											 const MPlug& otherPlug,
											 bool asSrc ) override;
	MStatus			shouldSave( const MPlug& plug, bool& result ) override;
	void			getCacheSetup( const MEvaluationNode& evalNode,
											 MNodeCacheDisablingInfo& disablingInfo,
											 MNodeCacheSetupInfo& cacheSetupInfo,
											 MObjectArray& monitoredAttributes) const override;

	// Attribute to component (components)
	//
	void			componentToPlugs( MObject &,
											  MSelectionList & ) const override;
	MatchResult		matchComponent( const MSelectionList& item,
											const MAttributeSpecArray& spec,
											MSelectionList& list ) override;
	bool			match(	const MSelectionMask & mask,
									const MObjectArray& componentList ) const override;

	MSelectionMask	getShapeSelectionMask() const override;
	MSelectionMask	getComponentSelectionMask() const override;

	// Support deformers (components)
	//
	MObject			createFullVertexGroup() const override;
	MObject 		localShapeInAttr() const override;
 	MObject 		localShapeOutAttr() const override;
 	MObject 		worldShapeOutAttr() const override;
 	MObject 		cachedShapeAttr() const override;

	MObject			geometryData() const override;
	virtual void			closestPoint ( const MPoint & toThisPoint,
									MPoint & theClosestPoint,
									double tolerance ) const;

	// Support the translate/rotate/scale tool (components)
	//
	void		    transformUsing( const MMatrix & mat,
						    			const MObjectArray & componentList ) override;
	void		    transformUsing( const MMatrix & mat,
											const MObjectArray & componentList,
											MPxSurfaceShape::MVertexCachingMode cachingMode,
											MPointArray* pointCache) override;
	void			tweakUsing( const MMatrix & mat,
										const MObjectArray & componentList,
										MVertexCachingMode cachingMode,
										MPointArray* pointCache,
										MArrayDataHandle& handle ) override;

	// Support the soft-select translate/rotate/scale tool (components)
	//
	void			weightedTransformUsing(	const MTransformationMatrix& mat,
													const MMatrix* space,
													const MObjectArray& componentList,
													MVertexCachingMode cachingMode,
													MPointArray* pointCache,
													const MPlane* freezePlane ) override;
    void            weightedTweakUsing(     const MTransformationMatrix& xform,
                                                    const MMatrix* space,
                                                    const MObjectArray& componentList,
                                                    MVertexCachingMode cachingMode,
                                                    MPointArray* pointCache,
                                                    const MPlane* freezePlane,
                                                    MArrayDataHandle& handle ) override;

	// Support the move tools normal/u/v mode (components)
	//
	bool			vertexOffsetDirection( MObject & component,
                                                   MVectorArray & direction,
                                                   MVertexOffsetMode mode,
												   bool normalize ) override;

	// Bounding box methods
	//
	bool            isBounded() const override;
	MBoundingBox    boundingBox() const override;

	// Associates a user defined iterator with the shape (components)
	//
		MPxGeometryIterator*
							geometryIteratorSetup( MObjectArray&, MObject&,
												   bool forReadOnly = false ) override;
	bool			acceptsGeometryIterator( bool  writeable=true ) override;
	bool			acceptsGeometryIterator( MObject&,
													 bool writeable=true,
													 bool forReadOnly = false) override;

    //////////////////////////////////////////////////////////
	//
	// Helper methods
	//
    //////////////////////////////////////////////////////////

	bool					hasHistory();

	bool					shapeDirty();
	void					resetShapeDirty();

	bool					materialDirty() const;
	void					setMaterialDirty(bool dirty);

    MStatus 		  		computeInputSurface( const MPlug&, MDataBlock& );
    MStatus 		  		computeOutputSurface( const MPlug&, MDataBlock& );
    MStatus 		  		computeWorldSurface( const MPlug&, MDataBlock& );
	MStatus 		  		computeBoundingBox( MDataBlock& );

	MStatus					applyTweaks( MDataBlock&, apiMeshGeom* );

	apiMeshGeom*			meshGeomToUse( );

	MStatus					buildControlPoints( MDataBlock&, int count );
	void					verticesUpdated();

	static  void *          creator();
	static  MStatus         initialize();

public:
    //////////////////////////////////////////////////////////
    //
    // Attributes
    //
    //////////////////////////////////////////////////////////
    static  MObject         inputSurface;
    static  MObject         outputSurface;
    static  MObject         worldSurface;

    static  MObject         useWeightedTransformUsingFunction;
    static  MObject         useWeightedTweakUsingFunction;

	// used to support tweaking of points, the inputSurface attribute data is
	// transferred into the cached surface when it is dirty. The control points
	// tweaks are added into it there.
	//
    static  MObject         cachedSurface;

    static  MObject         bboxCorner1;
    static  MObject         bboxCorner2;
    static	MObject 		enableNumericDisplay;

	static	MTypeId		id;

private:
	void 					updateCachedSurface( const apiMeshGeom* geomPtr, const MObjectArray & componentList );

	bool					value( MDataBlock&, int pntInd, int vlInd, double & val ) const;
	bool					value( MDataBlock&, int pntInd, MPoint & val ) const;
	bool					setValue( MDataBlock&, int pntInd, int vlInd, double val );
	bool					setValue( MDataBlock&, int pntInd, const MPoint & val );

	MObject					meshDataRef( MDataBlock& );
	apiMeshGeom*			meshGeom( MDataBlock& );

	MObject					cachedDataRef( MDataBlock& );
	apiMeshGeom*			cachedGeom( MDataBlock& );

	void					setShapeDirty();
	void					notifyViewport();
	void					signalDirtyToViewport();
	MObject					convertToVertexComponent(const MObject& components);

	bool fHasHistoryOnCreate;
	bool fShapeDirty;
	bool fMaterialDirty;
	std::map<std::string, MCallbackId> fMaterialDirtyCbIds;
};

#endif /* _apiMeshShape */
