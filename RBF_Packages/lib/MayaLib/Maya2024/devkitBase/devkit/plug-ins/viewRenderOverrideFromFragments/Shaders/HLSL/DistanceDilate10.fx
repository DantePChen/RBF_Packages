// ===========================================================================
// Copyright 2016 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

// DESCRIPTION: Distance dilate effect.
//**************************************************************************/

#include "Common10.fxh"

// The dilate amount (number of samples in each direction, or the "radius" of the filter).  Use this
// to determine the number of samples per pixel, including the center sample.
int gAmount = 1;
static int gNumSamples = gAmount * 2 + 1;

// The source buffer and sampler.
Texture2D gSourceTex < string UIWidget = "None"; > = NULL;
SamplerState gSourceSamp;

// Pixel shader.
// NOTE: This expects screen quad vertex shader output.
float4 PS_Dilate(VS_TO_PS_ScreenQuad In,
    uniform Texture2D sourceTex, uniform SamplerState sourceSamp,
    uniform float2 direction) : SV_Target
{
    // Return immediately if the center sample does not have the maximum depth.  Dilation will only
    // happen where the depth is 1.0.
    float current = sourceTex.SampleLevel(sourceSamp, In.UV, 0).r;
    if (current != 1.0f) return current;

    // Compute the per-sample offset, based on the texel size and dilate direction.  Then compute
    // the location of the starting sample, using the number of taps.
    float2 offset = direction * gTexelSize;
    float2 UV = In.UV - offset * (gNumSamples - 1) * 0.5f;
    
    // Sum each of the samples (box filter).
    float sum = 0.0f;
    int count = 0;
    for (int i = 0; i < gNumSamples; i++)
    {
        // Add the value from the source texture if it does not have the maximum depth; 1.0 depth
        // values are completely ignored.  That is what makes this a dilation filter.
        float value = sourceTex.SampleLevel(sourceSamp, UV, 0).r;
        if (value != 1.0f)
        {
            sum += value;
            count++;
        }

        // Increment the texture coordinates by the offset.
        UV += offset;
    }
    
    // Return the average color and alpha.
    return count == 0 ? 1.0f : sum / count;
}

// Horizontal dilate technique.
technique10 DilateHoriz
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_4_0, VS_ScreenQuad()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, PS_Dilate(gSourceTex, gSourceSamp, float2(1.0f, 0.0f))));
        // A horizontal dilate direction is specified as an argument above.
    }
}

// Vertical dilate technique.
technique10 DilateVert
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_4_0, VS_ScreenQuad()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, PS_Dilate(gSourceTex, gSourceSamp, float2(0.0f, 1.0f))));
        // A vertical dilate direction is specified as an argument above.
    }
}