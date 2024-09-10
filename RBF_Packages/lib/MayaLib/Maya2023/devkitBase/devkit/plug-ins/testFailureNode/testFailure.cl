/*
	testFailure kernel
*/

__kernel void testFailure(
    __global float* finalPos,							//float3
    __global const float* initialPos,					//float3
    __global const float* weights,
    __global const unsigned int* affectMap,
    const float envelope,
    const float3 offset,
    const uint affectCount)
{
    unsigned int aid = get_global_id(0);
    if ( aid >= affectCount )
        return;

    const unsigned int vid = (affectMap ? affectMap[aid] : aid);
    const float e = envelope * (weights ? weights[ aid ] : 1.0f);

    const float3 op = vload3(vid, initialPos);
    float3 pos = op + e*offset;
    vstore3(pos, vid, finalPos);
}