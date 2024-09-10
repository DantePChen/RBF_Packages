//-
// ==========================================================================
// Copyright 2018 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
// ==========================================================================
//+

///////////////////////////////////////////////////////////////////////////////
//
// This plug-in shows how to use MPxGeometryOverride to render geometry with Maya shader network.
// It will register a new type of node "geometryOverrideExample2_shape" that will generate 
// the geometry and connect it to a MPxGeometryOverride.
// 
// The GeometryOverrideExample2_shape class derives from MPxSurfaceShape and it is responsible 
// to generate the geometry data (Cube) and also enable the shape to be selectable in
// Viewport 2.0.
//
// The GeometryOverrideExample2 class derives from MPxGeometryOverride and shows how to
// create / update different render items and fill vertex / index buffers to render the geometry 
// of geometryOverrideExample2_shape.
//
//
// To use this plugin run the following mel or python commands once the plugin is loaded
//
//		Mel script:
//			
//			$shapeNodeName = `createNode geometryOverrideExample2_shape`;
//			sets -add initialShadingGroup $shapeNodeName;
//	
//		Python script:
//
//			shapeNodeName = maya.cmds.createNode('geometryOverrideExample2_shape')
//			maya.cmds.sets(shapeNodeName, add = 'initialShadingGroup')
//
//
// Once the mel/python commands are executed, a lambert shader will be attached to the shading group
// of the shape. Of course, you can change the lambert shader to a different one.
//
////////////////////////////////////////////////////////////////////////////////
#include <maya/MFnPlugin.h>
#include <maya/MDrawRegistry.h>
#include "geometryOverrideExample2.h"
#include <maya/MShaderManager.h>
#include <maya/MSelectionMask.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFloatVector.h>
#include <maya/MGlobal.h>
#include <iostream>

static void instancingChangedCallback(MDagPath& child, MDagPath& parent, void* clientData)
{
	MGlobal::displayWarning("GeometryOverrideExample2 does't support instancing !");
}



//===========================================================================
//
//				geometryOverrideExample2_shape implementation
//
//===========================================================================

MTypeId	GeometryOverrideExample2_shape::id = MTypeId(0x8003D);



GeometryOverrideExample2_shape::GeometryOverrideExample2_shape()
{
	const float cubeWidth = 1.0f;
	const float cubeHeight= 1.0f;
	const float cubeDepth = 1.0f;

	//
	// Define the cube's vertices data
	//
	fPositions.emplace_back(-cubeWidth, -cubeHeight, -cubeDepth);
	fPositions.emplace_back(-cubeWidth,  cubeHeight, -cubeDepth);
	fPositions.emplace_back( cubeWidth,  cubeHeight, -cubeDepth);
	fPositions.emplace_back( cubeWidth, -cubeHeight, -cubeDepth);
	fPositions.emplace_back(-cubeWidth, -cubeHeight,  cubeDepth);
	fPositions.emplace_back( cubeWidth, -cubeHeight,  cubeDepth);
	fPositions.emplace_back( cubeWidth,  cubeHeight,  cubeDepth);
	fPositions.emplace_back(-cubeWidth,  cubeHeight,  cubeDepth);
	fPositions.emplace_back(-cubeWidth,  cubeHeight, -cubeDepth);
	fPositions.emplace_back(-cubeWidth,  cubeHeight,  cubeDepth);
	fPositions.emplace_back( cubeWidth,  cubeHeight,  cubeDepth);
	fPositions.emplace_back( cubeWidth,  cubeHeight, -cubeDepth);
	fPositions.emplace_back(-cubeWidth, -cubeHeight, -cubeDepth);
	fPositions.emplace_back( cubeWidth, -cubeHeight, -cubeDepth);
	fPositions.emplace_back( cubeWidth, -cubeHeight,  cubeDepth);
	fPositions.emplace_back(-cubeWidth, -cubeHeight,  cubeDepth);
	fPositions.emplace_back(-cubeWidth, -cubeHeight,  cubeDepth);
	fPositions.emplace_back(-cubeWidth,  cubeHeight,  cubeDepth);
	fPositions.emplace_back(-cubeWidth,  cubeHeight, -cubeDepth);
	fPositions.emplace_back(-cubeWidth, -cubeHeight, -cubeDepth);
	fPositions.emplace_back( cubeWidth, -cubeHeight, -cubeDepth);
	fPositions.emplace_back( cubeWidth,  cubeHeight, -cubeDepth);
	fPositions.emplace_back( cubeWidth,  cubeHeight,  cubeDepth);
	fPositions.emplace_back( cubeWidth, -cubeHeight,  cubeDepth);

	fNormals.emplace_back( 0.0f,  0.0f, -1.0f);
	fNormals.emplace_back( 0.0f,  0.0f, -1.0f);
	fNormals.emplace_back( 0.0f,  0.0f, -1.0f);
	fNormals.emplace_back( 0.0f,  0.0f, -1.0f);
	fNormals.emplace_back( 0.0f,  0.0f,  1.0f);
	fNormals.emplace_back( 0.0f,  0.0f,  1.0f);
	fNormals.emplace_back( 0.0f,  0.0f,  1.0f);
	fNormals.emplace_back( 0.0f,  0.0f,  1.0f);
	fNormals.emplace_back( 0.0f,  1.0f,  0.0f);
	fNormals.emplace_back( 0.0f,  1.0f,  0.0f);
	fNormals.emplace_back( 0.0f,  1.0f,  0.0f);
	fNormals.emplace_back( 0.0f,  1.0f,  0.0f);
	fNormals.emplace_back( 0.0f, -1.0f,  0.0f);
	fNormals.emplace_back( 0.0f, -1.0f,  0.0f);
	fNormals.emplace_back( 0.0f, -1.0f,  0.0f);
	fNormals.emplace_back( 0.0f, -1.0f,  0.0f);
	fNormals.emplace_back(-1.0f,  0.0f,  0.0f);
	fNormals.emplace_back(-1.0f,  0.0f,  0.0f);
	fNormals.emplace_back(-1.0f,  0.0f,  0.0f);
	fNormals.emplace_back(-1.0f,  0.0f,  0.0f);
	fNormals.emplace_back( 1.0f,  0.0f,  0.0f);
	fNormals.emplace_back( 1.0f,  0.0f,  0.0f);
	fNormals.emplace_back( 1.0f,  0.0f,  0.0f);
	fNormals.emplace_back( 1.0f,  0.0f,  0.0f);

	fTangents.emplace_back( 1.0f, 0.0f,  0.0f);
	fTangents.emplace_back( 1.0f, 0.0f,  0.0f);
	fTangents.emplace_back( 1.0f, 0.0f,  0.0f);
	fTangents.emplace_back( 1.0f, 0.0f,  0.0f);
	fTangents.emplace_back(-1.0f, 0.0f,  0.0f);
	fTangents.emplace_back(-1.0f, 0.0f,  0.0f);
	fTangents.emplace_back(-1.0f, 0.0f,  0.0f);
	fTangents.emplace_back(-1.0f, 0.0f,  0.0f);
	fTangents.emplace_back( 1.0f, 0.0f,  0.0f);
	fTangents.emplace_back( 1.0f, 0.0f,  0.0f);
	fTangents.emplace_back( 1.0f, 0.0f,  0.0f);
	fTangents.emplace_back( 1.0f, 0.0f,  0.0f);
	fTangents.emplace_back(-1.0f, 0.0f,  0.0f);
	fTangents.emplace_back(-1.0f, 0.0f,  0.0f);
	fTangents.emplace_back(-1.0f, 0.0f,  0.0f);
	fTangents.emplace_back(-1.0f, 0.0f,  0.0f);
	fTangents.emplace_back( 0.0f, 0.0f, -1.0f);
	fTangents.emplace_back( 0.0f, 0.0f, -1.0f);
	fTangents.emplace_back( 0.0f, 0.0f, -1.0f);
	fTangents.emplace_back( 0.0f, 0.0f, -1.0f);
	fTangents.emplace_back( 0.0f, 0.0f,  1.0f);
	fTangents.emplace_back( 0.0f, 0.0f,  1.0f);
	fTangents.emplace_back( 0.0f, 0.0f,  1.0f);
	fTangents.emplace_back( 0.0f, 0.0f,  1.0f);

	fTextureCoords.emplace_back(0.0f, 1.0f);
	fTextureCoords.emplace_back(0.0f, 0.0f);
	fTextureCoords.emplace_back(1.0f, 0.0f);
	fTextureCoords.emplace_back(1.0f, 1.0f);
	fTextureCoords.emplace_back(1.0f, 1.0f);
	fTextureCoords.emplace_back(0.0f, 1.0f);
	fTextureCoords.emplace_back(0.0f, 0.0f);
	fTextureCoords.emplace_back(1.0f, 0.0f);
	fTextureCoords.emplace_back(0.0f, 1.0f);
	fTextureCoords.emplace_back(0.0f, 0.0f);
	fTextureCoords.emplace_back(1.0f, 0.0f);
	fTextureCoords.emplace_back(1.0f, 1.0f);
	fTextureCoords.emplace_back(1.0f, 1.0f);
	fTextureCoords.emplace_back(0.0f, 1.0f);
	fTextureCoords.emplace_back(0.0f, 0.0f);
	fTextureCoords.emplace_back(1.0f, 0.0f);
	fTextureCoords.emplace_back(0.0f, 1.0f);
	fTextureCoords.emplace_back(0.0f, 0.0f);
	fTextureCoords.emplace_back(1.0f, 0.0f);
	fTextureCoords.emplace_back(1.0f, 1.0f);
	fTextureCoords.emplace_back(0.0f, 1.0f);
	fTextureCoords.emplace_back(0.0f, 0.0f);
	fTextureCoords.emplace_back(1.0f, 0.0f);
	fTextureCoords.emplace_back(1.0f, 1.0f);

	fBiTangents.resize(24);
	for (int i = 0; i < 24; i++) 
	{
		// cross product
		MFloatVector biTangents = MFloatVector(fNormals[i].x,  fNormals[i].y,  fNormals[i]. z) ^ 
						          MFloatVector(fTangents[i].x, fTangents[i].y, fTangents[i].z);
		fBiTangents[i].x = biTangents.x;
		fBiTangents[i].y = biTangents.y;
		fBiTangents[i].z = biTangents.z;
	}

	fShadedIndices =
	{
		12, 13, 14, 12, 14, 15, // bottom
		 8,  9, 10,  8, 10, 11, // top
		16, 17, 18, 16, 18, 19, // left
		20, 21, 22, 20, 22, 23, // right
		 4,  5,  6,  4,  6,  7, // back
		 0,  1,  2,  0,  2,  3, // front
	};

	fWireFrameIndices =
	{
		0, 1, 1, 2, 2, 3, 3, 0, // bottom
		4, 5, 5, 6, 6, 7, 7, 4, // top
		0, 4, 3, 5, 1, 7, 2, 6, // left / right
	};
}



GeometryOverrideExample2_shape::~GeometryOverrideExample2_shape()
{
	MMessage::removeCallback(mInstanceAddedCallbackId);
}



//  ============================================================
//
//  When instances of this node are created internally, the MObject associated
//  with the instance is not created until after the constructor of this class
//  is called. This means that no member functions of MPxSurfaceShape can
//  be called in the constructor.
//  The postConstructor solves this problem. Maya will call this function
//  after the internal object has been created.
//  As a general rule do all of your initialization in the postConstructor.
//
//  Note : For more information, see MPxSurfaceShape::postConstructor()
//
//  ============================================================
void GeometryOverrideExample2_shape::postConstructor()
{
	// This call allows the shape to have shading groups assigned
	setRenderable(true);


	//
	// Add a callback that will be called when instance are added.
	//
	MDagPath path;
	MDagPath::getAPathTo(thisMObject(), path);
	mInstanceAddedCallbackId = MDagMessage::addInstanceAddedDagPathCallback(const_cast<MDagPath&>(path), &instancingChangedCallback);
}


//  ============================================================
//
//  This method is overriden to support interactive object selection in Viewport 2.0
//
//	Returns The selection mask of the shaper.
//
//  Note : For more information, see MPxSurfaceShape::getShapeSelectionMask()
//
//  ============================================================
MSelectionMask	GeometryOverrideExample2_shape::getShapeSelectionMask() const
{
	MSelectionMask::SelectionType selType = MSelectionMask::kSelectMeshes;
	return MSelectionMask(selType);
}


//  ============================================================
//
//  Returns the bounding box for this object.
//  It is a good idea not to recompute here as this funcion is called often.
//
//  Note : This function is only called if MPxSurfaceShape::isBounded() return true.
//         For more information, see MPxSurfaceShape::boundingBox()
//
//  ============================================================
MBoundingBox GeometryOverrideExample2_shape::boundingBox() const
{
	MPoint corner1Point(fPositions[0].x, fPositions[0].y, fPositions[0].z); // back-left-bottom vertex position
	MPoint corner2Point(fPositions[6].x, fPositions[6].y, fPositions[6].z); // front-right-top  vertex position

	return MBoundingBox(corner1Point, corner2Point);
}




//===========================================================================
//
//				GeometryOverrideExample2 implementation
//
//===========================================================================

const char* GeometryOverrideExample2::sActiveWireframeRenderItemName = "GeometryOverrideExample2_ActiveWireframe";
const char* GeometryOverrideExample2::sDormantWireframeRenderItemName = "GeometryOverrideExample2_DormantWireframe";
const char* GeometryOverrideExample2::sShadedRenderItemName = "GeometryOverrideExample2_shaded";


GeometryOverrideExample2::GeometryOverrideExample2(const MObject& obj)
	: MHWRender::MPxGeometryOverride(obj)
{
	// get the real mesh object from the MObject
	MStatus status;
	MFnDependencyNode node(obj, &status);
	if (status)
	{
		fMesh = dynamic_cast<GeometryOverrideExample2_shape*>(node.userNode());
	}
}



GeometryOverrideExample2::~GeometryOverrideExample2()
{

}


//  ============================================================
//
//  This function return draw API that is supported by this plugin.
//
//  Note : For more information, see MPxGeometryOverride::supportedDrawAPIs()
// 
//  ============================================================
MHWRender::DrawAPI GeometryOverrideExample2::supportedDrawAPIs() const
{
	return  (MHWRender::kOpenGL | MHWRender::kDirectX11 | MHWRender::kOpenGLCoreProfile);
}



//  ============================================================
//
//  Perform any work required to translate the geometry data that needs to get
//  information from the dependency graph.This should be the only place that
//  dependency graph evaluation occurs.Any data retrieved should be cached for
//  later stages.
//
//  Note : For more information, see MPxGeometryOverride::updateDG()
//
//  ============================================================
void GeometryOverrideExample2::updateDG()
{
	// In this example, there is nothing to do here
}



//  ============================================================
//
//  This method is called once during each draw-preparation phase.  If this method returns true 
//  then the associated DAG object will have a chance to update its render geometry this frame.
//  (Maya will call populateGeometry())
//
//  Note : For more information, see MPxGeometryOverride::requiresGeometryUpdate()
//
//  ============================================================
bool GeometryOverrideExample2::requiresGeometryUpdate() const
{
	// In this example, we always return false because the node doesn't have 
	// attributes that affect the geometry.
	return false;
}

//  ============================================================
//
//  If this method returns true then the MPxGeometryOverride will be considered
//  for Evaluation Manager Parallel Update.
//
//  Note : For more information, see MPxGeometryOverride::supportsEvaluationManagerParallelUpdate()
//
//  ============================================================
bool GeometryOverrideExample2::supportsEvaluationManagerParallelUpdate() const
{
	return true;
}



//  ============================================================
//
//  This method is called for each instance of the associated DAG object whenever 
//  the object changes and receive the path to the instance and the current list 
//  of render items associated with that instance. Implementations of this method 
//  may add, remove or modify items in the list. As an alternative this method 
//  can enable or disable items that must be used or not based on some properties.
//  
//  A render item represents a single renderable entity and contain many properties 
//  to let the Viewport 2.0 to know how to render the entity. By example, A render 
//  item contain a name, a type, the geometry primitive type, a set of geometry 
//  buffers and a shader instance. 
//  
//  In this example, this functions will create 2 render items to render the object's 
//  geometry in wireframe mode. The first render item will be use to display the object 
//  in wireframe with a single color only when the Viewport 2.0 is in wireframe mode. 
//  The second render item will be use to render the object with a single color in 
//  wireframe only when the object is selected independing of the Viewport 2.0 display 
//  mode. Both wireframe render item will be enable or disable depending of the object 
//  selection state. 
//  
//  This example will not create a render item to display the geometry in shaded or 
//  textured mode because Maya will do it when a material / shader will be assigned 
//  to the object. This render item will be added in the list and can be edited 
//  if desired. If no material is assigned, the object will not be visible in 
//  Viewport 2.0 except if the node is selected in the outliner. In this situation,
//  only the edges of the object will be displayed because we are creating
//  a wireframe render item that will be used when the object is selected.
//
//  Note : For more information, see MPxGeometryOverride::updateRenderItems()
//
//  ============================================================
void GeometryOverrideExample2::updateRenderItems(const MDagPath& path, MHWRender::MRenderItemList& renderItems)
{
	if (!path.isValid())
		return;


	MRenderer* renderer = MRenderer::theRenderer();
	if (!renderer)
		return;

	const MShaderManager* shaderManager = renderer->getShaderManager();
	if (!shaderManager)
		return;


	// Get the inherited DAG display properties.
	auto wireframeColor = MHWRender::MGeometryUtilities::wireframeColor(path);
	auto displayStatus         = MHWRender::MGeometryUtilities::displayStatus(path);

	// Update the wireframe render item used when the object will be selected
	bool isWireFrameRenderItemEnabled = displayStatus == MHWRender::kLead || displayStatus == MHWRender::kActive;
	updateWireframeItems(sActiveWireframeRenderItemName,
		MHWRender::MGeometry::kAll,
		MHWRender::MRenderItem::sSelectionDepthPriority,
		wireframeColor,
		isWireFrameRenderItemEnabled,
		renderItems,
		*shaderManager);


	// Update the wireframe render item used when the object will not be selected
	isWireFrameRenderItemEnabled = displayStatus == MHWRender::kDormant;
	updateWireframeItems(sDormantWireframeRenderItemName,
		MHWRender::MGeometry::kWireframe,
		MHWRender::MRenderItem::sDormantWireDepthPriority,
		wireframeColor,
		isWireFrameRenderItemEnabled,
		renderItems,
		*shaderManager);
}



//  ============================================================
//
//	Fill in data and index streams based on the requirements passed in.
//	Associate indexing with the render items passed in.
//
//  Note : For more information, see MPxGeometryOverride::populateGeometry()
//
//  ============================================================
void GeometryOverrideExample2::populateGeometry(const MHWRender::MGeometryRequirements& requirements, const MHWRender::MRenderItemList& renderItems, MHWRender::MGeometry& data)
{
	if (!fMesh)
		return;

	const MVertexBufferDescriptorList&  vertexBufferDescriptorList = requirements.vertexRequirements();
	for (int i = 0; i < vertexBufferDescriptorList.length(); i++)
	{
		MVertexBufferDescriptor desc{};
		if (!vertexBufferDescriptorList.getDescriptor(i, desc))
			continue;

		std::cout << desc.semanticName().asChar() << std::endl;

		switch (desc.semantic())
		{
		case MGeometry::kPosition:
		{
			//
			// Create and fill the vertex position buffer
			//
			MHWRender::MVertexBuffer* positionBuffer = data.createVertexBuffer(desc);

			if (positionBuffer)
			{
				GeometryOverrideExample2_shape::Float3Array positions = fMesh->getPositions();

				void* buffer = positionBuffer->acquire(positions.size(), true /*writeOnly */);
				if (buffer)
				{
					const std::size_t bufferSizeInByte =
						sizeof(GeometryOverrideExample2_shape::Float3Array::value_type) * positions.size();

					memcpy(buffer, positions.data(), bufferSizeInByte);

					// Transfer from CPU to GPU memory.
					positionBuffer->commit(buffer);
				}
			}
		}
		break;
		case MGeometry::kNormal:
		{
			//
			// Create and fill the vertex normal buffer
			//
			MHWRender::MVertexBuffer* normalsBuffer = data.createVertexBuffer(desc);

			if (normalsBuffer)
			{
				GeometryOverrideExample2_shape::Float3Array normals = fMesh->getNormals();

				void* buffer = normalsBuffer->acquire(normals.size(), true /*writeOnly*/);
				if (buffer)
				{
					const std::size_t bufferSizeInByte =
						sizeof(GeometryOverrideExample2_shape::Float3Array::value_type) * normals.size();

					memcpy(buffer, normals.data(), bufferSizeInByte);

					// Transfer from CPU to GPU memory.
					normalsBuffer->commit(buffer);
				}
			}
		}
		break;
		case MGeometry::kTangent:
		{
			MHWRender::MVertexBuffer* tangentBuffer = data.createVertexBuffer(desc);

			if (tangentBuffer)
			{
				GeometryOverrideExample2_shape::Float3Array tangents = fMesh->getTangents();

				void* buffer = tangentBuffer->acquire(tangents.size(), true /*writeOnly*/);
				if (buffer)
				{
					const std::size_t bufferSizeInByte =
						sizeof(GeometryOverrideExample2_shape::Float3Array::value_type) * tangents.size();

					memcpy(buffer, tangents.data(), bufferSizeInByte);

					// Transfer from CPU to GPU memory.
					tangentBuffer->commit(buffer);
				}
			}
		}
			break;
		case MGeometry::kBitangent:
		{
			MHWRender::MVertexBuffer* tangentBuffer = data.createVertexBuffer(desc);

			if (tangentBuffer)
			{
				GeometryOverrideExample2_shape::Float3Array tangents = fMesh->getBiTangents();

				void* buffer = tangentBuffer->acquire(tangents.size(), true /*writeOnly*/);
				if (buffer)
				{
					const std::size_t bufferSizeInByte =
						sizeof(GeometryOverrideExample2_shape::Float3Array::value_type) * tangents.size();

					memcpy(buffer, tangents.data(), bufferSizeInByte);

					// Transfer from CPU to GPU memory.
					tangentBuffer->commit(buffer);
				}
			}
		}
			break;
		case MGeometry::kTexture:
		{
			//
			// Create and fill the vertex texture coords buffer
			//
			MHWRender::MVertexBuffer* texCoordsBuffer = data.createVertexBuffer(desc);

			if (texCoordsBuffer)
			{
				GeometryOverrideExample2_shape::Float2Array texCoords = fMesh->getTexCoords();

				void* buffer = texCoordsBuffer->acquire(texCoords.size(), true /*writeOnly*/);
				if (buffer)
				{
					const std::size_t bufferSizeInByte =
						sizeof(GeometryOverrideExample2_shape::Float2Array::value_type) * texCoords.size();

					memcpy(buffer, texCoords.data(), bufferSizeInByte);

					// Transfer from CPU to GPU memory.
					texCoordsBuffer->commit(buffer);
				}
			}
		}
		break;
		case MGeometry::kColor:
		case MGeometry::kTangentWithSign:
		case MGeometry::kInvalidSemantic:	// avoid compiling error
			//
			// In this example, we don't need to used those vertex informantions.
			//
			break;
		}
	}


	/////////////////////////////////////////////////////////////////////
	//	 Update indexing data for all appropriate render items
	/////////////////////////////////////////////////////////////////////
	const int numItems = renderItems.length();
	for (int i = 0; i < numItems; i++)
	{
		const MHWRender::MRenderItem* item = renderItems.itemAt(i);
		if (!item)
			continue;

		if (item->primitive() == MHWRender::MGeometry::kTriangles)
		{
			//
			// Create and fill the index buffer used to render triangles
			//
			MHWRender::MIndexBuffer* indexBuffer = data.createIndexBuffer(MHWRender::MGeometry::kUnsignedInt32);
			if (indexBuffer)
			{
				GeometryOverrideExample2_shape::IndexList indices = fMesh->getShadedIndices();
				void* buffer = indexBuffer->acquire(indices.size(), true /*writeOnly*/);
				if (buffer)
				{
					const std::size_t bufferSizeInByte =
						sizeof(GeometryOverrideExample2_shape::IndexList::value_type) * indices.size();

					memcpy(buffer, indices.data(), bufferSizeInByte);

					// Transfer from CPU to GPU memory.
					indexBuffer->commit(buffer);

					// Associate index buffer with render item
					item->associateWithIndexBuffer(indexBuffer);
				}
			}
		}
		else if (item->primitive() == MHWRender::MGeometry::kLines)
		{
			//
			// Create and fill the index buffer used to render lines (Wireframe)
			//
			MHWRender::MIndexBuffer* indexBuffer = data.createIndexBuffer(MHWRender::MGeometry::kUnsignedInt32);
			if (indexBuffer)
			{
				GeometryOverrideExample2_shape::IndexList indices = fMesh->getWireFrameIndices();

				void* buffer = indexBuffer->acquire(indices.size(), true /*writeOnly*/);
				if (buffer)
				{
					const std::size_t bufferSizeInByte =
						sizeof(GeometryOverrideExample2_shape::IndexList::value_type) * indices.size();

					memcpy(buffer, indices.data(), bufferSizeInByte);

					// Transfer from CPU to GPU memory.
					indexBuffer->commit(buffer);

					// Associate index buffer with render item
					item->associateWithIndexBuffer(indexBuffer);
				}
			}
		}
	}
}


//  ============================================================
//
//  Clean up any cached data stored from the updateDG() phase.
//
//  Note : For more information, see MPxGeometryOverride::cleanUp()
//
//  ============================================================
void GeometryOverrideExample2::cleanUp()
{
	// Nothing here because the plugin doesn't keep any temporary data.
}



//  ============================================================
//
//	Update the wireframe render item named 'renderItemName' or create it
//  if it doesn't exists.
//
//  Note : Arguments drawMode and depthPriority are only used for creation of 
//         the render item.
//
//  ============================================================
void GeometryOverrideExample2::updateWireframeItems(const char* renderItemName, MGeometry::DrawMode drawMode,
	unsigned int depthPriority, MColor color, bool isEnable,
	MHWRender::MRenderItemList& renderItemList,
	const MHWRender::MShaderManager& shaderManager)
{

	MHWRender::MRenderItem* renderItem = nullptr;

	// Try to find the active wireframe render item.
	// If the returning index is smaller than 0, that means 
	// the render item does't exists yet. So, create it.
	auto renderItemIndex = renderItemList.indexOf(renderItemName);
	if (renderItemIndex < 0)
	{
		// Create the new render item with the given name.
		// We designate this item as a UI “decoration” and will not be
		// involved in rendering aspects such as casting shadows
		// The “topology” for the render item is a line list.
		renderItem = MHWRender::MRenderItem::Create(renderItemName,
			MHWRender::MRenderItem::DecorationItem,
			MHWRender::MGeometry::kLines);

		// We want this render item to show up when in all mode ( Wireframe, Shaded, Textured and BoundingBox)
		renderItem->setDrawMode(drawMode);

		// Set selection priority: on top of everything
		renderItem->depthPriority(depthPriority);

		// Get an instance of a 3dSolidShader from the shader manager.
		// The shader tells the graphics hardware how to draw the geometry. 
		// The MShaderInstance is a reference to a shader along with the values for the shader parameters.
		MShaderInstance* shader = shaderManager.getStockShader(MShaderManager::k3dSolidShader);
		if (shader)
		{
			// Assign the shader to the render item. This adds a reference to that
			// shader.
			renderItem->setShader(shader);

			// Once assigned, no need to hold on to shader instance
			shaderManager.releaseShader(shader);
		}

		// The item must be added to the persistent list to be considered
		// for update / rendering
		renderItemList.append(renderItem);
	}
	else
	{
		renderItem = renderItemList.itemAt(renderItemIndex);
	}

	if (renderItem)
	{
		MHWRender::MShaderInstance* shader = renderItem->getShader();
		if (shader)
		{
			// Set the shader color parameter
			shader->setParameter("solidColor", &color.r);
		}

		renderItem->enable(isEnable);
	}
}



//===========================================================================
//
//				initialize / uninitialize plugin
//
//===========================================================================



namespace
{
	MString sDrawDbClassification("drawdb/geometry/geometryOverrideExample2");
	MString sDrawRegistrantId("geometryOverrideExample2Plugin");
}




MStatus initializePlugin(MObject obj)
{
	MFnPlugin plugin(obj, PLUGIN_COMPANY, "3.0", "Any");
	MStatus status;

	status = plugin.registerShape("geometryOverrideExample2_shape",				// the name of the new type of user defined shape node
		GeometryOverrideExample2_shape::id,				// a unique id that identifies this node
		&GeometryOverrideExample2_shape::creator,       // function that will return a pointer to a new instance of the class 
														// (derived from MPxSurfaceNode) that implements the new shape node type
		&GeometryOverrideExample2_shape::initialize,    // function that will initialize all the attributes of the new shape node type
		nullptr,
		&sDrawDbClassification);
	if (!status)
	{
        std::cerr << "Failed to register geometryOverrideExample2_shape." << std::endl;
		return status;
	}


	status = MHWRender::MDrawRegistry::registerGeometryOverrideCreator(sDrawDbClassification,
		sDrawRegistrantId,
		GeometryOverrideExample2::Creator);
	if (!status)
	{
		std::cerr << "Failed to register Viewport 2.0 geometry override." << std::endl;
		return status;
	}

	return status;

}

MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin plugin(obj);
	MStatus status;

	status = MHWRender::MDrawRegistry::deregisterGeometryOverrideCreator(sDrawDbClassification, sDrawRegistrantId);
	if (!status)
	{
		std::cerr << "Failed to deregister geometry override." << std::endl;
	}

	status = plugin.deregisterNode(GeometryOverrideExample2_shape::id);
	if (!status)
	{
        std::cerr << "Failed to deregister GeometryOverrideExample2_shape." << std::endl;
	}

	return status;
}
