#include <maya/MFnPlugin.h>

#include <maya/MArrayDataBuilder.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnVectorArrayData.h>
#include <maya/MGlobal.h>
#include <maya/MPxDeformerNode.h>
#include <maya/MVectorArray.h>

struct bssIdCluster : MPxDeformerNode {
    static void* creator() { return new bssIdCluster(); };
    static MStatus initialize();
    MStatus deform(MDataBlock& data, MItGeometry& iter, const MMatrix& matrix,
        unsigned int multiIndex) override;

    static MTypeId id_;
    static MObject deformCounterAttr_;
    static MObject parameterizedVectorsAttr_;
    static MObject compoundParameterizationAttr_;
};

MTypeId bssIdCluster::id_(0x00101f00, 71);
MObject bssIdCluster::deformCounterAttr_;
MObject bssIdCluster::parameterizedVectorsAttr_;
MObject bssIdCluster::compoundParameterizationAttr_;

MStatus bssIdCluster::initialize()
{
    MStatus stat;
    MFnTypedAttribute typedAttr;
    MFnNumericAttribute numericAttr;
    MFnCompoundAttribute compAttr;

    // count how many times deform is called:
    deformCounterAttr_ = numericAttr.create("deformCounter", "dct", MFnNumericData::kInt, 0, &stat);
    CHECK_MSTATUS(stat);
    stat = addAttribute(deformCounterAttr_);
    CHECK_MSTATUS(stat);

    // this is from the original plugin:
    MFnVectorArrayData vectorArrayData;
    MObject parameterizedVectors = vectorArrayData.create();
    parameterizedVectorsAttr_ = typedAttr.create("parameterizedVectors", "pvec",
        MFnData::kVectorArray, parameterizedVectors, &stat);
    typedAttr.setStorable(true);
    CHECK_MSTATUS(stat);
    compoundParameterizationAttr_ = compAttr.create("compoundParameterization", "cprm", &stat);
    CHECK_MSTATUS(stat);
    stat = compAttr.addChild(parameterizedVectorsAttr_);
    CHECK_MSTATUS(stat);
    compAttr.setUsesArrayDataBuilder(true);
    compAttr.setConnectable(false);
    compAttr.setCached(false);
    compAttr.setStorable(true);
    compAttr.setArray(true);
    stat = addAttribute(compoundParameterizationAttr_);
    CHECK_MSTATUS(stat);
    stat = attributeAffects(compoundParameterizationAttr_, outputGeom);
    CHECK_MSTATUS(stat);
    stat = attributeAffects(parameterizedVectorsAttr_, outputGeom);
    CHECK_MSTATUS(stat);

    return MStatus::kSuccess;
}

MStatus bssIdCluster::deform(MDataBlock& data, MItGeometry&,
    const MMatrix&, unsigned int)
{
    MStatus stat;

    // count how many times deform is called:
    int deformCounter = data.inputValue(deformCounterAttr_, &stat).asInt();
    CHECK_MSTATUS(stat);
    data.outputValue(deformCounterAttr_, &stat).asInt() = deformCounter + 1;
    CHECK_MSTATUS(stat);

    // create parameterizedVectors only the first time:
    if (deformCounter == 0) {
        MGlobal::displayInfo(name() + " building data");

        // this is from the original plugin:
        MArrayDataHandle arrayHandle = data.outputArrayValue(compoundParameterizationAttr_, &stat);
        CHECK_MSTATUS(stat);
        MArrayDataBuilder arrayBuilder = arrayHandle.builder(&stat);
        CHECK_MSTATUS(stat);
        arrayBuilder.addElement(0);
        stat = arrayHandle.set(arrayBuilder);
        CHECK_MSTATUS(stat);
        stat = arrayHandle.jumpToArrayElement(0);
        CHECK_MSTATUS(stat);
        MDataHandle handle = arrayHandle.outputValue(&stat);
        CHECK_MSTATUS(stat);
        #if 1
        // When BlueSky provided their plugin, that was the code that was
        // triggering the bug (even though it shouldn't have, this code should
        // be ok).
        MObject obj = handle.child(parameterizedVectorsAttr_).data();
        MFnVectorArrayData vectorArrayData(obj);
        // resize parameterizedVectors:
        vectorArrayData.set(MVectorArray(40));
        #else
        // This is the workaround we provided to them to avoid the problem.
        MDataHandle childHandle = handle.child(parameterizedVectorsAttr_);
        MFnVectorArrayData vectorArrayData;
        MObject obj = vectorArrayData.create(MVectorArray(40));
        childHandle.set(obj);
        #endif
    }

    // check parameterizedVectors:
    MArrayDataHandle compoundParamAH = data.outputArrayValue(bssIdCluster::compoundParameterizationAttr_, &stat);
    CHECK_MSTATUS(stat);
    const unsigned elementCount = compoundParamAH.elementCount();
    MGlobal::displayInfo(name() + " element count: " + elementCount);
    for (unsigned i = 0; i < elementCount; ++i) {
        MDataHandle compoundHandle = compoundParamAH.outputValue(&stat);
        CHECK_MSTATUS(stat);
        MDataHandle handle = compoundHandle.child(parameterizedVectorsAttr_);
        MFnVectorArrayData vectorData(handle.data());
        MGlobal::displayInfo(name() + " vector length: " + vectorData.length());
    }

    data.setClean (outputGeom);
    return MStatus::kSuccess;
}

MStatus initializePlugin(MObject obj)
{
    MFnPlugin plugin(obj, "Blue Sky Studios", "", "Any");

    MStatus status = plugin.registerNode(
        "bssIdCluster", bssIdCluster::id_, bssIdCluster::creator,
        bssIdCluster::initialize, MPxNode::kDeformerNode);
    CHECK_MSTATUS(status);

    return MStatus::kSuccess;
}

MStatus uninitializePlugin(MObject obj)
{
    MFnPlugin plugin(obj);

    MStatus status = plugin.deregisterNode(bssIdCluster::id_);
    CHECK_MSTATUS(status);

    return MStatus::kSuccess;
}
