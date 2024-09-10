// ===========================================================================
// Copyright 2020 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

__kernel void basicMorph(
	__global float* finalPos,							//float3
	__global const float* initialPos,					//float3
	__global const float* targetPos,					//float3
	const float envelope,
	__global const float* weights,
	__global const unsigned int* affectMap,
	const uint affectCount)
{
	unsigned int id = get_global_id(0);
	if (id >= affectCount) return;

	// If there is an affectMap, use it to get the actual vert id
	const unsigned int positionId = (affectMap ? affectMap[id] : id);

	float3 initialPosition = vload3(positionId, initialPos);
	float3 targetPosition = vload3(positionId, targetPos);

	float weight = weights[positionId] * envelope;

	float3 newPosition = initialPosition + (targetPosition - initialPosition) * weight;

	vstore3(newPosition, positionId, finalPos);
}