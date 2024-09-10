/*
	Utility functions
*/
float3 mul( float16 matrix , float3 point );

float3 mul( float16 matrix , float3 point )
{
    const float4 homogeneousPoint = (float4)( point.xyz , 1.0f );
    const float4 transformedPoint = (float4)(
        dot( homogeneousPoint , matrix.s048c ) ,
        dot( homogeneousPoint , matrix.s159d ) ,
        dot( homogeneousPoint , matrix.s26ae ) ,
        dot( homogeneousPoint , matrix.s37bf )
        );
    return transformedPoint.xyz / transformedPoint.w;
}

/*
	simpleDeformer kernels
*/

__kernel void simpleDeformer(
    __global float* finalPos,							//float3
    __global const float* initialPos,					//float3
    __global const float* vertWeights,
    __global const unsigned int* affectMap,
    const float16 locatorMatrix,					//first matrix is offset matrix, second matrix is offset matrix inverse
    const float deformerEnvelope,
    const uint affectCount)
{
    unsigned int aid = get_global_id(0);
    if ( aid >= affectCount ) return;

    // If there is an affectMap, use it to get the actual vert id
    const unsigned int vid = (affectMap ? affectMap[aid] : aid);

    float3 pt = vload3( vid , initialPos );
    float3 newPt = mul( locatorMatrix , pt );
    float weight = vertWeights ? (vertWeights[aid]*deformerEnvelope) : deformerEnvelope;
    pt += weight * (newPt - pt);

    vstore3( pt , vid , finalPos );
}