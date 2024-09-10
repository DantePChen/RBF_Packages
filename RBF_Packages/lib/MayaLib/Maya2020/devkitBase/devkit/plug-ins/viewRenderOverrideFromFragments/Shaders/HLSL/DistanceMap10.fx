// ===========================================================================
// Copyright 2016 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

// DESCRIPTION: Distance map effect.
//**************************************************************************/

// World transformation.
float4x4 gWXf : World < string UIWidget = "None"; >;

// World-view-projection transformation.
float4x4 gWVPXf : WorldViewProjection < string UIWidget = "None"; >;

// The plane from which distances are computed.
float4 gPlane = float4(0.0f, 0.0f, 1.0f, 0.0f);

// The range of distances used for normalizing the map values.
float2 gRange = float2(0.0f, 1.0f);

// Vertex shader input structure.
struct VS_INPUT
{
    float3 Pos : POSITION;
};

// Vertex shader output structure.
struct VS_TO_PS
{
    float4 HPos : SV_Position;

    // Vertex position in world space.
    float Distance : TEXCOORD0;
};

// Vertex shader.
VS_TO_PS VS_DistanceMap(VS_INPUT In)
{
    VS_TO_PS Out;
    
    // Transform the vertex from object space to clip space.
    Out.HPos = mul(float4(In.Pos, 1.0f), gWVPXf);
    
    float4 HPw = mul(float4(In.Pos, 1.0f), gWXf);

    // Transform the vertex from object space to world space, and compute the absolute distance from
    // the plane with a dot product.
    Out.Distance = dot(gPlane, HPw);

    return Out;
}

// Pixel shader.
float4 PS_DistanceMap(VS_TO_PS In) : SV_Target
{
    // Normalize the input distance in the distance range.
    float distance = saturate((abs(In.Distance) - gRange.x) / (gRange.y - gRange.x));

    // Use the (monochrome) distance as the output color.
    return float4(distance.xxx, 1.0f);
}

// The main technique.
technique10 Main
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_4_0, VS_DistanceMap()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, PS_DistanceMap()));
    }
}
