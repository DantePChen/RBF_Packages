/*
	offset kernels
*/

__kernel void offset(
	__global float* finalPos,							//float3
	__global const float* initialPos,					//float3
	__global const float* weights,
	__global const unsigned int* affectMap,
	__global const float4* matrices,					//first matrix is offset matrix, second matrix is offset matrix inverse
	__global const float* randomOffsetTable,
	const uint randomOffsetTableSize,
    const float chunkWeight,
	const uint affectCount)
{
	unsigned int id = get_global_id(0);
	if ( id >= affectCount ) return;

	// If there is an affectMap, use it to get the actual vert id
	const unsigned int positionId = (affectMap ? affectMap[id] : id);

	unsigned int positionOffset = positionId * 3;				// Base positions are float3 when they come in here!
	float4 initialPosition;
	initialPosition.x = initialPos[positionOffset];
	initialPosition.y = initialPos[positionOffset+1];
	initialPosition.z = initialPos[positionOffset+2];
	initialPosition.w = 1.0f;

	float4 finalPosition;
	finalPosition.x = 0.0f;
	finalPosition.y = 0.0f;
	finalPosition.z = 0.0f;
	finalPosition.w = 1.0f;

	__global const float4* matrixInverse = &(matrices[4]);
	__global const float4* matrix = matrices;

	// point *= matrix inverse
	finalPosition.x = dot(initialPosition, matrixInverse[0]);
	finalPosition.y = dot(initialPosition, matrixInverse[1]);
	finalPosition.z = dot(initialPosition, matrixInverse[2]);

    float weight = weights[positionId];
    if (randomOffsetTableSize > 0) {
        weight *= (1.0 + chunkWeight*randomOffsetTable[positionId%randomOffsetTableSize]);
    }

	// pt.y += weight
	finalPosition.y += weight;

	// point *= matrix
	// can't write back into finalPosition here, we need to use the same value to calculate xyz
	// instead write into global memory
	finalPos[positionOffset] = dot(finalPosition, matrix[0]);
	finalPos[positionOffset+1] = dot(finalPosition, matrix[1]);
	finalPos[positionOffset+2] = dot(finalPosition, matrix[2]);
}