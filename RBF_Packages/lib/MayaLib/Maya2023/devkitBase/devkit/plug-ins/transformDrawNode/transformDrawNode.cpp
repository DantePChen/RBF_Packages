//-
// ==========================================================================
// Copyright 2015 Autodesk, Inc.  All rights reserved.
// Use of this software is subject to the terms of the Autodesk license agreement
// provided at the time of installation or download, or which otherwise
// accompanies this software in either electronic or hard copy form.
// ==========================================================================
//+

/*
 transformDrawNode uses MPxDrawOverride to draw texts about transformation of
 all mesh shapes in the viewport via MPxDrawOverride::addUIDrawables. By setting
 isAlwaysDirty to false in MPxDrawOverride constructor, the draw override will
 be updated (via prepareForDraw()) only when the node is marked dirty via DG
 evaluation or dirty propagation. Additional callbacks are also added to
 explicitly mark the node as being dirty (via MRenderer::setGeometryDrawDirty())
 for certain circumstances. Note that the draw callback in MPxDrawOverride
 constructor is set to NULL in order to achieve better performance.
*/


#include <maya/MCallbackIdArray.h>
#include <maya/MDagMessage.h>
#include <maya/MDrawRegistry.h>
#include <maya/MFnDagNode.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnPlugin.h>
#include <maya/MFrameContext.h>
#include <maya/MItDag.h>
#include <maya/MPxDrawOverride.h>
#include <maya/MPxLocatorNode.h>
#include <maya/MVectorArray.h>

#include <stdio.h>

using namespace MHWRender;


class transformDrawNode : public MPxLocatorNode
{
public:

	enum ETransformType
	{
		kTranslate,
		kRotate,
		kScale,
		kShear
	};

	static void* creator() { return new transformDrawNode(); }

	static MStatus initialize();

	// Registration
	static constexpr const char   className[] = "transformDrawNode";
	static MTypeId id;
	static MString drawDbClassification;
	static MString drawRegistrantId;

	// Attributes
	static MObject aTextColor;
	static MObject aTransformType;

private:

	transformDrawNode();
	~transformDrawNode() override;

	static void AllDagChangesCallback(
		MDagMessage::DagMessage msgType,
		MDagPath &child,
		MDagPath &parent,
		void *clientData);

	static void WorldMatrixModifiedCallback(
		MObject &transformNode,
		MDagMessage::MatrixModifiedFlags &modified,
		void *clientData);

	void processDagMessage(bool refreshWorldMatrixCbIds);

	MCallbackId fAllDagChangesCbId;
	MCallbackIdArray fWorldMatrixModifiedCbIds;
};

// Registration
constexpr const char transformDrawNode::className[];
MTypeId transformDrawNode::id(0x80034);
MString transformDrawNode::drawDbClassification("drawdb/geometry/transformDrawNode");
MString transformDrawNode::drawRegistrantId(transformDrawNode::className);

// Attributes
MObject transformDrawNode::aTextColor;
MObject transformDrawNode::aTransformType;

MStatus transformDrawNode::initialize()
{
	MFnNumericAttribute nAttr;
	aTextColor = nAttr.create("textColor", "tc", MFnNumericData::k3Float);
	nAttr.setDefault(1.0f, 1.0f, 1.0f);
	nAttr.setUsedAsColor(true);
	MPxNode::addAttribute(aTextColor);

	MFnEnumAttribute eAttr;
	aTransformType = eAttr.create("transformType", "tt", kTranslate);
	eAttr.addField("Translate", kTranslate);
	eAttr.addField("Rotate", kRotate);
	eAttr.addField("Scale", kScale);
	eAttr.addField("Shear", kShear);
	MPxNode::addAttribute(aTransformType);

	return MS::kSuccess;
}

transformDrawNode::transformDrawNode() : MPxLocatorNode()
{
	fAllDagChangesCbId = MDagMessage::addAllDagChangesCallback(
		AllDagChangesCallback, this);
}

transformDrawNode::~transformDrawNode()
{
	if (fWorldMatrixModifiedCbIds.length() > 0)
	{
		MMessage::removeCallbacks(fWorldMatrixModifiedCbIds);
	}

	if (fAllDagChangesCbId != 0)
	{
		MMessage::removeCallback(fAllDagChangesCbId);
	}
}

void transformDrawNode::processDagMessage(bool refreshWorldMatrixModifiedCbIds)
{
	// Explicitly mark the node as being dirty on certain DAG message callbacks
	// so that the draw override can be updated.
	MRenderer::setGeometryDrawDirty(thisMObject());

	if (refreshWorldMatrixModifiedCbIds)
	{
		MStatus status;
		MItDag dagIt(MItDag::kDepthFirst, MFn::kMesh, &status);
		if (status)
		{
			// Remove existing callbacks
			if (fWorldMatrixModifiedCbIds.length() > 0)
			{
				MMessage::removeCallbacks(fWorldMatrixModifiedCbIds);
				fWorldMatrixModifiedCbIds.clear();
			}

			// Add new callbacks
			for ( ;!dagIt.isDone(); dagIt.next() )
			{
				MDagPath dagPath;
				status = dagIt.getPath(dagPath);
				if (status)
				{
					MCallbackId id = MDagMessage::addWorldMatrixModifiedCallback(
						dagPath, WorldMatrixModifiedCallback, this);
					fWorldMatrixModifiedCbIds.append(id);
				}
			}
		}
	}
}

void transformDrawNode::AllDagChangesCallback(
	MDagMessage::DagMessage msgType,
	MDagPath &child,
	MDagPath &parent,
	void *clientData)
{
	// We need to refresh the world matrix modified callbacks because the DAG
	// is just changed.
	transformDrawNode *node = static_cast<transformDrawNode*>(clientData);
	if (node) node->processDagMessage(true);
}

void transformDrawNode::WorldMatrixModifiedCallback(
	MObject &transformNode,
	MDagMessage::MatrixModifiedFlags &modified,
	void *clientData)
{
	// We don't need to refresh the world matrix modified callbacks because the
	// DAG is not changed.
	transformDrawNode *node = static_cast<transformDrawNode*>(clientData);
	if (node) node->processDagMessage(false);
}


class transformDrawData : public MUserData
{
public:
	MColor                            fTextColor{1.0f, 1.0f, 1.0f, 1.0f};
	transformDrawNode::ETransformType fTransformType{transformDrawNode::kTranslate};
	MVectorArray                      fPositions;
	MVectorArray                      fVectors;
};

class transformDrawOverride : public MPxDrawOverride
{
public:

	static MPxDrawOverride* creator(const MObject& obj)
	{
		return new transformDrawOverride(obj);
	}

	DrawAPI supportedDrawAPIs() const override { return kAllDevices; }

	bool hasUIDrawables() const override { return true; }

	MUserData* prepareForDraw(
		const MDagPath& objPath,
		const MDagPath& cameraPath,
		const MFrameContext& frameContext,
		MUserData* oldData) override;

	void addUIDrawables(
		const MDagPath& objPath,
		MUIDrawManager& drawManager,
		const MFrameContext& frameContext,
		const MUserData* data) override;

private:

	// By setting isAlwaysDirty to false in MPxDrawOverride constructor, the
	// draw override will be updated (via prepareForDraw()) only when the node
	// is marked dirty via DG evaluation or dirty propagation. Additional
	// callbacks are also added to explicitly mark the node as being dirty (via
	// MRenderer::setGeometryDrawDirty()) for certain circumstances. Note that
	// the draw callback in MPxDrawOverride constructor is set to NULL in order
	// to achieve better performance.
	transformDrawOverride(const MObject& obj) : MPxDrawOverride(obj, NULL, false) {}
	~transformDrawOverride() override {}
};

MUserData* transformDrawOverride::prepareForDraw(
	const MDagPath& objPath,
	const MDagPath& cameraPath,
	const MFrameContext& frameContext,
	MUserData* oldData)
{
	MStatus status;
	MObject obj = objPath.node(&status);
	if (!status) return NULL;

	transformDrawData* tdData = dynamic_cast<transformDrawData*>(oldData);
	if (!tdData)
	{
		tdData = new transformDrawData();
	}

	// Text color
	{
		MPlug plug(obj, transformDrawNode::aTextColor);
		MObject o = plug.asMObject();
		MFnNumericData nData(o);
		nData.getData(tdData->fTextColor.r, tdData->fTextColor.g, tdData->fTextColor.b);
	}

	// Transform type
	{
		MPlug plug(obj, transformDrawNode::aTransformType);
		tdData->fTransformType = (transformDrawNode::ETransformType)plug.asInt();
	}

	tdData->fPositions.clear();
	tdData->fVectors.clear();

	MItDag dagIt(MItDag::kDepthFirst, MFn::kMesh, &status);
	if (status)
	{
		for ( ; !dagIt.isDone(); dagIt.next() )
		{
			MDagPath dagPath;
			status = dagIt.getPath(dagPath);
			if (!status)
			{
				status.perror("MItDag::getPath");
				continue;
			}

			MObject transformNode = dagPath.transform(&status);
			if (!status)
			{
				status.perror("MDagPath::transform");
				continue;
			}

			MFnDagNode transform(transformNode, &status);
			if (!status)
			{
				status.perror("MFnDagNode constructor");
				continue;
			}

			MTransformationMatrix matrix(transform.transformationMatrix());
			MVector vec = matrix.getTranslation(MSpace::kWorld);
			tdData->fPositions.append(vec);

			double tmp[3];
			MTransformationMatrix::RotationOrder order;
			switch (tdData->fTransformType)
			{
			case transformDrawNode::kRotate:
				matrix.getRotation(tmp, order);
				vec = MVector(tmp);
				break;
			case transformDrawNode::kScale:
				matrix.getScale(tmp, MSpace::kWorld);
				vec = MVector(tmp);
				break;
			case transformDrawNode::kShear:
				matrix.getShear(tmp, MSpace::kWorld);
				vec = MVector(tmp);
				break;
			default:
				// Don't reset vec so that translation is drawn by default.
				break;
			}

			tdData->fVectors.append(vec);
		}
	}

	return tdData;
}

void transformDrawOverride::addUIDrawables(
	const MDagPath& objPath,
	MUIDrawManager& drawManager,
	const MFrameContext& frameContext,
	const MUserData* data)
{
	const transformDrawData* tdData = dynamic_cast<const transformDrawData*>(data);
	if (!tdData) return;

	drawManager.beginDrawable();

	drawManager.setColor(tdData->fTextColor);

	for (unsigned int i = 0; i < tdData->fVectors.length(); i++)
	{
		MPoint pos(tdData->fPositions[i]);
		MVector vec(tdData->fVectors[i]);
		char tmpStr[128] = {0};
		sprintf(tmpStr, "(%.3f, %.3f, %.3f)", vec.x, vec.y, vec.z);
		MString text(tmpStr);
		drawManager.text(pos, text, MUIDrawManager::kCenter);
	}

	drawManager.endDrawable();
}


MStatus initializePlugin( MObject obj )
{
	MFnPlugin plugin( obj, PLUGIN_COMPANY, "1.0", "Any" );

	MStatus status = plugin.registerNode(
		transformDrawNode::className,
		transformDrawNode::id,
		transformDrawNode::creator,
		transformDrawNode::initialize,
		MPxNode::kLocatorNode,
		&transformDrawNode::drawDbClassification);
	if (!status)
	{
		status.perror("registerNode");
		return status;
	}

	status = MDrawRegistry::registerDrawOverrideCreator(
		transformDrawNode::drawDbClassification,
		transformDrawNode::drawRegistrantId,
		transformDrawOverride::creator);
	if (!status)
	{
		status.perror("registerDrawOverrideCreator");
		return status;
	}

	return status;
}

MStatus uninitializePlugin( MObject obj )
{
	MFnPlugin plugin( obj );

	MStatus status = MDrawRegistry::deregisterDrawOverrideCreator(
		transformDrawNode::drawDbClassification,
		transformDrawNode::drawRegistrantId);
	if (!status)
	{
		status.perror("deregisterDrawOverrideCreator");
		return status;
	}

	status = plugin.deregisterNode(transformDrawNode::id);
	if (!status)
	{
		status.perror("deregisterNode");
		return status;
	}

	return status;
}
