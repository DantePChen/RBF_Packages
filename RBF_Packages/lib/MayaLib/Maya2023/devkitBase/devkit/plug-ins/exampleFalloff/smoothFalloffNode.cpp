#include "smoothFalloffNode.h"

#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnFalloffData.h>
#include <maya/MFnMesh.h>
#include <maya/MIndexMapper.h>
#include <vector>
const MTypeId SmoothFalloffNode::id(0x0008006F);

const MString SmoothFalloffNode::typeName("exampleSmoothFalloff");

MObject SmoothFalloffNode::aSmoothFactor;
MObject SmoothFalloffNode::aFalloffInput;
MObject SmoothFalloffNode::aFalloffOutput;
MObject SmoothFalloffNode::aIteration;
MObject SmoothFalloffNode::aSyncId;


SmoothFalloffNode::SmoothFalloffNode(){}
SmoothFalloffNode::~SmoothFalloffNode(){}

void* SmoothFalloffNode::creator()
{
    return new SmoothFalloffNode{};
}



/*
Smooth Falloff takes a weighfunction in and provides an output weight function.
The outputweight function will smooth the weight of its input
*/


MStatus SmoothFalloffNode::initialize()
{
    MStatus status;

    MFnNumericAttribute numAttrFn;
    MFnTypedAttribute typedAttrFn;

    aIteration = numAttrFn.create("iteration", "itr", MFnNumericData::kInt, 1 );
    numAttrFn.setMin(0);
    numAttrFn.setMax(20);
    status = MPxNode::addAttribute(aIteration);
    CHECK_MSTATUS_AND_RETURN_IT(status);

    aSyncId = numAttrFn.create("syncid", "sid", MFnNumericData::kInt, -1 );
    numAttrFn.setHidden(true);
    status = MPxNode::addAttribute(aSyncId);
    CHECK_MSTATUS_AND_RETURN_IT(status);

    aSmoothFactor = numAttrFn.create("smoothFactor", "smf", MFnNumericData::kDouble, 0.1 );
    numAttrFn.setMin(0.0);
    numAttrFn.setMax(1.0);
    status = MPxNode::addAttribute(aSmoothFactor);
    CHECK_MSTATUS_AND_RETURN_IT(status);

    //example ho to declare a falloff function
    aFalloffInput = typedAttrFn.create("falloffInput", "foi", MFnData::kFalloffFunction );
    status = MPxNode::addAttribute(aFalloffInput);
    CHECK_MSTATUS_AND_RETURN_IT(status);

    aFalloffOutput = typedAttrFn.create("falloffOutput", "foo", MFnData::kFalloffFunction );
    typedAttrFn.setWritable(false);
    typedAttrFn.setStorable(false);
    status = MPxNode::addAttribute(aFalloffOutput);
    CHECK_MSTATUS_AND_RETURN_IT(status);

    status = attributeAffects( aFalloffInput, aFalloffOutput );
    CHECK_MSTATUS_AND_RETURN_IT(status);
    status = attributeAffects( aSmoothFactor, aFalloffOutput );
    CHECK_MSTATUS_AND_RETURN_IT(status);
    status = attributeAffects( aIteration, aFalloffOutput );
    CHECK_MSTATUS_AND_RETURN_IT(status);

    return MStatus::kSuccess;
}



MStatus SmoothFalloffNode::uninitialize()
{
    return MStatus::kSuccess;
}

class SmoothFalloff: public MFalloffFunction
{
    public:

    struct VertInfo{
            unsigned int vertId;
            double weight;
    };

    SmoothFalloff(MPxNode* node, MObject inputFalloff, double smoothFactor, int iter, int syncId)
        :mNode(node)
        ,mInput(inputFalloff)
        ,mSmoothFactor(smoothFactor)
        ,mIter(iter)
        ,mSyncId(syncId)
        {}


    //The Actual smooth algorithm
    //It takes an inOutBuffer initalized to the input weights.
    //mesh provide the connectivity information to do a proper smooth.
    //Since weight function can provide weight on a subset of the total number of vert in a mesh;
    //the mapper will inform us how to map a weight buffer index in to the mesh vertex buffer (full Id to affect Id "fullToAffect")
    
    void doSmooth(MFloatArray& inOut, MFnMesh& mesh, const MIndexMapper& mapper)
    {
        const auto numEdges = mesh.numEdges();

        std::vector<std::vector<VertInfo>> vertConnections(mapper.affectCount());

        //Gather Vertices Connection
        //for each vertex there is a list of VertInfo, wich hold connecting ver and invert distance to it
        for(int i = 0; i < numEdges; ++i){
            int2 vs;
            auto status = mesh.getEdgeVertices(i,vs);
            auto v0 = mapper.fullToAffect(vs[0]); //takes the vertexId and return the index in the weight buffer
            auto v1 = mapper.fullToAffect(vs[1]);

            if(v0 != MIndexMapper::InvalidIndex && v1 !=  MIndexMapper::InvalidIndex){ //if vertex are not art of the desired subset don't consider them

                MPoint p0;
                MPoint p1;
                mesh.getPoint(v0, p0);
                mesh.getPoint(v1, p1);
                auto invert_d = pow(1.0/p1.distanceTo(p0), 4.0);
                vertConnections[v0].emplace_back(VertInfo{v1, invert_d});
                vertConnections[v1].emplace_back(VertInfo{v0, invert_d});
            }
        }


        //normalize inverted distance to have proper weight per vertex
        for(auto &v : vertConnections){
            double sumWeight = 0;
            for(auto& info: v){
                sumWeight += info.weight;
            }
            for(auto& info: v){
                info.weight /= sumWeight;
            }
        }


        const auto OneMinusFactor = 1.0 - mSmoothFactor;
        if (mIter < 0) mIter = 0;
        if (mIter > 20) mIter = 20;

        //simple smooth algorithm
        MFloatArray initial;
        for(int iteration = 0; iteration < mIter; ++iteration){
            initial = inOut;

            for(unsigned i = 0; i < initial.length(); ++i){
                double averageNeighbours = 0.0;
                for(const auto& vertInfo : vertConnections[i]){
                    averageNeighbours += initial[vertInfo.vertId] * vertInfo.weight;
                }
                if(vertConnections[i].size() > 0)
                    inOut[i] = (initial[i] * OneMinusFactor) + (mSmoothFactor * averageNeighbours);

            }
        }

    }
    //This is the function that will be called when the weight buffer are needed
    //MFalloffContext would provide the neede information to correctly compute the weight for the targeted geometry
    ReturnValue operator()(MFalloffContext& accessor) override{

        //create an identifier for this node within the context
        //Since this node only require 1 buffer by context, we can have layerId = 0 as second parameter.
        //In case where we would have multiple function on same node (ex: using multi) we could use the multiIndex as second param.
        MBufferIdentifier bufferId{mNode,0};

        //This is an example how to call a falloff input function with the same context
        MFnFalloffData inputFoff(mInput);
        auto rv = inputFoff.call(accessor);
        //rv now store infromation to get to the input buffer we need.

        //if the topology is dirty we always need to recompute no matter everythiong else
        //if the input is the same (Cached) then only recompute if this buffer is out of sync (input on this node has changes since last evaluation)
        if( !accessor.isTopologyDirty() &&
            rv.getCachedIndicator() == MFalloffFunction::ReturnValue::Cached &&
            accessor.isSync(mSyncId,bufferId))
        {
                return  ReturnValue{bufferId, rv.getRequirement(), ReturnValue::Cached};
        }
        //get the actual input buffer
        MFloatArray initial = accessor.getValues(rv.getBufferId());
        //get the targeted geometry
        MObject geom = accessor.getGeometry();
        MIndexMapper mapper = accessor.getIndexMapper();

        MStatus status;
        MFnMesh mesh(geom, &status);

        //this smooth falloff won't work on geometry other than mesh
        if(status != MStatus::kSuccess){
            return rv; //simply return the input if it is not a mesh
        }

        if(mIter > 0 && mSmoothFactor > 0.0)
            doSmooth(initial, mesh, mapper); 

        //set the computed buffer.
        accessor.setValue(initial,bufferId);

        //make sure it is sync with our captured syncId
        accessor.sync(mSyncId, bufferId);

        //This node does not have any different requirement, returning input requirment as is.
        //Some falloff could require current geometry instead of the original geometry.
        //Current geometry requirement would cause the falloff to be computed everytime the input geometry of a deformer changes.
        //OriginalGeometry requirement would only trigger weight computation when original geometry of a deformer changes.
        return ReturnValue{bufferId, rv.getRequirement(), ReturnValue::NotCached};
    }
    MPxNode* mNode;
    MObject mInput;
    double mSmoothFactor;
    int mIter;
    int mSyncId;
};

MStatus SmoothFalloffNode::compute( const MPlug & plug, MDataBlock & block )
{
    if(plug.attribute() == aFalloffOutput){

        //When outputing a falloff function we must pull on all needed data at the compute stage.
        auto input = block.inputValue(aFalloffInput);
        auto sf = block.inputValue(aSmoothFactor).asDouble();
        auto iter = block.inputValue(aIteration).asInt();
        auto syncId = block.inputValue(aSyncId).asInt();

        //increment sync id so that we remember that this computation id and can compare if the buffer is stale or not
        block.outputValue(aSyncId).set(++syncId);

        //It is important to have a syncId because of the computation paradigm of the weight function.
        //The weight function is not evaluated a this compute stage but rather at the time where weights are needed.
        //For example weight function are consumed by deformer (geometryFilter)
        //at the compute time of the geometryfilter, the function will be invoked with a context parameter MFalloffContext.
        //The same function can be evaluated multiples time by different context and
        //could potentially be evaluated multiple times within the same context (see smoothFalloff1 example below).

        //Take this graph example:
        //
        //  WeightFunctionProvider1   --> smoothFallOff1--> firstDeformer
        //                                            \ --> blendfallof1.layer1
        //                                            \ --> blendfallof1.layer2
        //                             someOtherFalloff1--> blendfallof1.layer3
        //                                                              .outFunction --->  secondDeformer
        //                                                                          \--->  thirdDeformer

        //smoothFallOff1 outputFunction would be called exaclty once by firstDeformer at its compute time.
        //It would be called exaclty twice by secondDeformer and the twice as well for thirdDeformer

        //For the case of the second & third deformer they are call twice each since the smoothFallOff1
        //is connected to 2 layer of the blendfalloff.
        //The blendfalloff to blend each layer would ask for each input buffer.
        //The first time it get asked it probably need to compute if (syncId is not in sync with buffer)
        //the second time it will be sync then it can return Cached and skip the computation.

        //In the case where input parameters of someOtherFalloff1 change then it goes out of sync.
        //It provokes a dirty propagation for the entire weight function graph
        //In order to avoid to recompute all the layers of the blendFalloff1,
        //each falloff function can bail out as soon as possible by checking the syncId stored with buffer syncId.
        //That way only someOtherFalloff1 & the blendFalloff1 would need to recompute but not WeightFunctionProvider1 and smoothFallOff1


        MFnFalloffData foffData;
        //set a MSharedPtr of a callable to output falloff data
        auto data = foffData.create(MSharedPtr<SmoothFalloff>(new SmoothFalloff{this,input.asFalloffFunction(), sf, iter, syncId}));
        block.outputValue(plug).set(data);

    }
    return MS::kSuccess;
}