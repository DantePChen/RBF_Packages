#ifndef _smoothfalloffnode_h_
#define _smoothfalloffnode_h_


#include <maya/MPxNode.h>
#include <maya/MTypeId.h>

class SmoothFalloffNode : public MPxNode
{
public:
    static void* creator();
    static MStatus initialize();
    static MStatus uninitialize();

    static const MTypeId id;
    static const MString typeName;

    /*----- member functions -----*/

    SmoothFalloffNode();
    ~SmoothFalloffNode() override;
    MStatus compute( const MPlug & plug, MDataBlock & block ) override;
private:
    void doSmooth(MFloatArray& inOut, MFnMesh& mesh, const MIndexMapper& mapper);

    static MObject aSmoothFactor;
    static MObject aFalloffInput;
    static MObject aFalloffOutput;
    static MObject aIteration;
    static MObject aSyncId;
};

#endif
