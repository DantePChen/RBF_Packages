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
// This plug-in shows how to use MPxGeometryOverride to render geometry with a stock shader.
// It will register a new type of node "geometryOverrideExample1_shape" that will generate 
// the geometry and connect it to a MPxGeometryOverride.
// 
// The GeometryOverrideExample1_shape class derives from MPxSurfaceShape and it is responsible 
// to generate the geometry data (Cube) and also enable the shape to be selectable in
// Viewport 2.0.
//
// The GeometryOverrideExample1 class derives from MPxGeometryOverride and shows how to
// create / update different render items and fill vertex / index buffers to render the geometry 
// of geometryOverrideExample1_shape.
//
//
// To use this plugin run the following mel commands once the plugin is loaded:
//
//	createNode geometryOverrideExample1_shape;
//
////////////////////////////////////////////////////////////////////////////////
#include <maya/MFnPlugin.h>
#include <maya/MDrawRegistry.h>
#include "geometryOverrideExample1.h"
#include <maya/MShaderManager.h>
#include <maya/MSelectionMask.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MGlobal.h>
#include <iostream>

static void instancingChangedCallback(MDagPath& child, MDagPath& parent, void* clientData)
{
	MGlobal::displayWarning("GeometryOverrideExample1 does't support instancing !");
}



//===========================================================================
//
//				geometryOverrideExample1_shape implementation
//
//===========================================================================

MTypeId	GeometryOverrideExample1_shape::id = MTypeId(0x8003B);



GeometryOverrideExample1_shape::GeometryOverrideExample1_shape()
{
	// unit cube vertices
	fPositions.push_back(MFloatVector(-1, -1, -1));
	fPositions.push_back(MFloatVector( 1, -1, -1));
	fPositions.push_back(MFloatVector( 1, -1,  1));
	fPositions.push_back(MFloatVector(-1, -1,  1));
	fPositions.push_back(MFloatVector(-1,  1, -1));
	fPositions.push_back(MFloatVector(-1,  1,  1));
	fPositions.push_back(MFloatVector( 1,  1,  1));
	fPositions.push_back(MFloatVector( 1,  1, -1));

	const float normal_value = 0.5775f;
	fNormals.push_back(MFloatVector(-normal_value, -normal_value, -normal_value));
	fNormals.push_back(MFloatVector( normal_value, -normal_value, -normal_value));
	fNormals.push_back(MFloatVector( normal_value, -normal_value,  normal_value));
	fNormals.push_back(MFloatVector(-normal_value, -normal_value,  normal_value));
	fNormals.push_back(MFloatVector(-normal_value,  normal_value, -normal_value));
	fNormals.push_back(MFloatVector(-normal_value,  normal_value,  normal_value));
	fNormals.push_back(MFloatVector( normal_value,  normal_value,  normal_value));
	fNormals.push_back(MFloatVector( normal_value,  normal_value, -normal_value));

	fShadedIndices =
	{
		0, 1, 2, 2, 3, 0, // bottom
		4, 5, 6, 6, 7, 4, // top
		0, 3, 5, 5, 4, 0, // left
		1, 7, 6, 6, 2, 1, // right
		4, 1, 0, 4, 7, 1, // back
		5, 3, 2, 2, 6, 5, // front
	};

	fWireFrameIndices =
	{
		0, 1, 1, 2, 2, 3, 3, 0, // bottom
		4, 5, 5, 6, 6, 7, 7, 4, // top
		0, 4, 3, 5, 1, 7, 2, 6, // left / right
	};
}



GeometryOverrideExample1_shape::~GeometryOverrideExample1_shape()
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
void GeometryOverrideExample1_shape::postConstructor()
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
MSelectionMask	GeometryOverrideExample1_shape::getShapeSelectionMask() const
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
MBoundingBox GeometryOverrideExample1_shape::boundingBox() const
{
	MPoint corner1Point(fPositions[0].x, fPositions[0].y, fPositions[0].z); // back-left-bottom vertex position
	MPoint corner2Point(fPositions[6].x, fPositions[6].y, fPositions[6].z); // front-right-top  vertex position

	return MBoundingBox(corner1Point, corner2Point);
}




//===========================================================================
//
//				GeometryOverrideExample1 implementation
//
//===========================================================================

const char* GeometryOverrideExample1::sActiveWireframeRenderItemName  = "GeometryOverrideExample1_ActiveWireframe";
const char* GeometryOverrideExample1::sDormantWireframeRenderItemName = "GeometryOverrideExample1_DormantWireframe";
const char* GeometryOverrideExample1::sShadedRenderItemName           = "GeometryOverrideExample1_shaded";


GeometryOverrideExample1::GeometryOverrideExample1(const MObject& obj) 
	: MHWRender::MPxGeometryOverride(obj)
{
	// get the real mesh object from the MObject
	MStatus status;
	MFnDependencyNode node(obj, &status);
	if (status)
	{
		fMesh = dynamic_cast<GeometryOverrideExample1_shape*>(node.userNode());
	}
}



GeometryOverrideExample1::~GeometryOverrideExample1()
{

}


//  ============================================================
//
//  This function return draw API that is supported by this plugin.
//
//  Note : For more information, see MPxGeometryOverride::supportedDrawAPIs()
// 
//  ============================================================
MHWRender::DrawAPI GeometryOverrideExample1::supportedDrawAPIs() const
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
void GeometryOverrideExample1::updateDG()
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
bool GeometryOverrideExample1::requiresGeometryUpdate() const
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
bool GeometryOverrideExample1::supportsEvaluationManagerParallelUpdate() const
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
//  item contain a name, a type, the geometry primitive type, a set of geometry buffers
//  and a shader instance. 
//  
//  In this example, this functions will create 3 render items to render the object's 
//  geometry. The first render item will be use to display the oject with a single color 
//  only when the Viewport 2.0 is in shaded or textured mode. The second render item 
//  will be use to display the object in wireframe with a single color only when the 
//  Viewport 2.0 is in wireframe mode. The last render item will be use to render the 
//  object with a single color in wireframe only when the object is selected independing 
//  of the Viewport 2.0 display mode. Both wireframe render item will be enable or disable 
//  depending of the object selection state.
//
//  Note : For more information, see MPxGeometryOverride::updateRenderItems()
//
//  ============================================================
void GeometryOverrideExample1::updateRenderItems(const MDagPath& path, MHWRender::MRenderItemList& renderItems)
{
	if (!path.isValid())
		return;

	MRenderer* renderer = MRenderer::theRenderer();
	if (!renderer)
		return;
	
	const MShaderManager* shaderManager = renderer->getShaderManager();
	if (!shaderManager)
		return;


	//
	// Create a render item that will render geometry with a single color 
	// if it didn't already exist.
	//
	// This render item will be used only in shaded and textured mode.
	//
	auto renderItemIndex = renderItems.indexOf(sShadedRenderItemName);
	if (renderItemIndex < 0)
	{
		// Create the new render item with the given name.
		// We designate this item as a UI Material.
		// The "topology" for the render item is a triangle list.
		MHWRender::MRenderItem* shadedRenderItem = 
			MHWRender::MRenderItem::Create(sShadedRenderItemName,
										   MHWRender::MRenderItem::MaterialSceneItem,
										   MHWRender::MGeometry::kTriangles);

		// We want this render item to only show up when in shaded or textured mode
		shadedRenderItem->setDrawMode((MHWRender::MGeometry::DrawMode)(MHWRender::MGeometry::kShaded | 
			                                                          MHWRender::MGeometry::kTextured));
		
		// The the depth priority of this render item to sDormantFilledDepthPriority (The default value).
		// Depth priority is usefull when an object is displayed with many render items.
		// Viewport 2.0 will use the depth priority to know which render item must be draw on top
		// of others.
		shadedRenderItem->depthPriority(MHWRender::MRenderItem::sDormantFilledDepthPriority);

		// Enable the render item so it will be use for rendering
		shadedRenderItem->enable(true);

		// Get an instance of a 3dSolidShader from the shader manager.
		// The shader tells the graphics hardware how to draw the geometry. 
		// The MShaderInstance is a reference to a shader along with the values for the shader parameters.
		MShaderInstance* shader = shaderManager->getStockShader(MShaderManager::k3dSolidShader);
		if (shader)
		{
			// Set the shader color parameter
			const float blueColor[] = { 0.0f, 0.0f, 1.0f, 1.0f };
			shader->setParameter("solidColor", blueColor);

			// Assign the shader to the render item. This adds a reference to that
			// shader.
			shadedRenderItem->setShader(shader);

			// Once assigned, no need to hold on to shader instance
			shaderManager->releaseShader(shader);
		}

		// The item must be added to the persistent list to be considered
		// for update / rendering
		renderItems.append(shadedRenderItem);
	}


	// Get the inherited DAG display properties.
	auto wireframeColor = MHWRender::MGeometryUtilities::wireframeColor(path);
	auto displayStatus = MHWRender::MGeometryUtilities::displayStatus(path);

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
void GeometryOverrideExample1::populateGeometry(const MHWRender::MGeometryRequirements& requirements, const MHWRender::MRenderItemList& renderItems, MHWRender::MGeometry& data)
{
	if (!fMesh)
		return;
	
	const MVertexBufferDescriptorList&  vertexBufferDescriptorList = requirements.vertexRequirements();
	for (int i = 0; i < vertexBufferDescriptorList.length(); i++)
	{
		MVertexBufferDescriptor desc{};
		if (!vertexBufferDescriptorList.getDescriptor(i, desc))
			continue;

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
				GeometryOverrideExample1_shape::PositionList positions = fMesh->getPositions();
				
				void* buffer = positionBuffer->acquire(positions.size(), true /*writeOnly */);
				if(buffer)
				{
					const std::size_t bufferSizeInByte = 
						sizeof(GeometryOverrideExample1_shape::PositionList::value_type) * positions.size();

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
				GeometryOverrideExample1_shape::NormalList normals = fMesh->getNormals();

				void* buffer = normalsBuffer->acquire(normals.size(), true /*writeOnly*/);
				if(buffer)
				{
					const std::size_t bufferSizeInByte =
						sizeof(GeometryOverrideExample1_shape::NormalList::value_type) * normals.size();

					memcpy(buffer, normals.data(), bufferSizeInByte);

					// Transfer from CPU to GPU memory.
					normalsBuffer->commit(buffer);
				}
			}
		}
				break;
		case MGeometry::kTexture:
		case MGeometry::kColor:
		case MGeometry::kTangent:
		case MGeometry::kBitangent:
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
				GeometryOverrideExample1_shape::IndexList indices = fMesh->getShadedIndices();
				void* buffer = indexBuffer->acquire(indices.size(), true /*writeOnly*/);
				if (buffer)
				{
					const std::size_t bufferSizeInByte =
						sizeof(GeometryOverrideExample1_shape::IndexList::value_type) * indices.size();

					memcpy(buffer, indices.data(), bufferSizeInByte);

					// Transfer from CPU to GPU memory.
					indexBuffer->commit(buffer);

					// Associate index buffer with render item
					item->associateWithIndexBuffer(indexBuffer);
				}
			}
		}
		else if(item->primitive() == MHWRender::MGeometry::kLines)
		{			
			//
			// Create and fill the index buffer used to render lines (Wireframe)
			//
			MHWRender::MIndexBuffer* indexBuffer = data.createIndexBuffer(MHWRender::MGeometry::kUnsignedInt32);
			if (indexBuffer)
			{
				GeometryOverrideExample1_shape::IndexList indices = fMesh->getWireFrameIndices();

				void* buffer = indexBuffer->acquire(indices.size(), true /*writeOnly*/);
				if (buffer)
				{
					const std::size_t bufferSizeInByte =
						sizeof(GeometryOverrideExample1_shape::IndexList::value_type) * indices.size();

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
void GeometryOverrideExample1::cleanUp()
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
void GeometryOverrideExample1::updateWireframeItems(const char* renderItemName, MGeometry::DrawMode drawMode, 
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
		// We designate this item as a UI "decoration" and will not be
		// involved in rendering aspects such as casting shadows
		// The "topology" for the render item is a line list.
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

	if(renderItem)
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
	MString sDrawDbClassification("drawdb/geometry/geometryOverrideExample1");
	MString sDrawRegistrantId("geometryOverrideExample1Plugin");
}




MStatus initializePlugin(MObject obj)
{
	MFnPlugin plugin(obj, PLUGIN_COMPANY, "3.0", "Any");
	MStatus status;

	status = plugin.registerShape("geometryOverrideExample1_shape",				// the name of the new type of user defined shape node
								GeometryOverrideExample1_shape::id,				// a unique id that identifies this node
								&GeometryOverrideExample1_shape::creator,       // function that will return a pointer to a new instance of the class 
																				// (derived from MPxSurfaceNode) that implements the new shape node type
								&GeometryOverrideExample1_shape::initialize,    // function that will initialize all the attributes of the new shape node type
								&sDrawDbClassification);
	if (!status)
	{
        std::cerr << "Failed to register geometryOverrideExample1_shape." << std::endl;
		return status;
	}


	status = MHWRender::MDrawRegistry::registerGeometryOverrideCreator(sDrawDbClassification,
																		sDrawRegistrantId,
																		GeometryOverrideExample1::Creator);
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

	status = plugin.deregisterNode(GeometryOverrideExample1_shape::id);
	if (!status) 
	{
        std::cerr << "Failed to deregister GeometryOverrideExample1_shape." << std::endl;
	}

	return status;
}
