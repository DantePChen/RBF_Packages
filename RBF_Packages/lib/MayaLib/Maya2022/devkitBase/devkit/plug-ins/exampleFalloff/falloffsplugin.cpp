#include <maya/MFnFalloffData.h>
#include "smoothFalloffNode.h"
#include <maya/MFnPlugin.h>

MStatus initializePlugin(MObject obj)
{

    MFnPlugin plugin( obj, PLUGIN_COMPANY, "1.0", "Any");
 
    MStatus status = plugin.registerNode( SmoothFalloffNode::typeName, SmoothFalloffNode::id, SmoothFalloffNode::creator, SmoothFalloffNode::initialize );

    return status;
}

MStatus uninitializePlugin(MObject obj)
{ 
    return MS::kSuccess;
}