// ===========================================================================
// Copyright 2016 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

// DESCRIPTION: Planar reflection - render.
//**************************************************************************/

// Intensity type: how the visibility of the reflection is computed:
// 0 = full intensity
// 1 = constant intensity
// 2 = intensity varies by view angle, similar to Fresnel reflectance
// 3 = intensity varies by distance from the reflection plane
// #define INTENSITY_TYPE 0

// Blur type: how the blurring of the reflection is computed:
// 0 = no blur
// 1 = constant blur
// 2 = blur varies by view angle, similar to Fresnel reflectance
// 3 = blur varies by distance from the reflection plane
// #define BLUR_TYPE 0

// Shortcut macros for certain state.
#define NONE     0
#define CONSTANT 1
#define VIEW     2
#define DISTANCE 3
#define USE_BLUR     (BLUR_TYPE >= VIEW)
#define USE_VIEW     (INTENSITY_TYPE == VIEW     || BLUR_TYPE == VIEW)
#define USE_DISTANCE (INTENSITY_TYPE == DISTANCE || BLUR_TYPE == DISTANCE)

// World-view-projection transformation.
float4x4 gWVPXf : WorldViewProjection < string UIWidget = "None"; >;

// World transformation, needed for clipping.
float4x4 gWXf : World < string UIWidget = "None"; >;

// The color of the reflection.
float3 gColor = { 1.0f, 1.0f, 1.0f };

// The saturation of the reflection.
float gSaturation = 1.0f;

// The intensity range.
float2 gIntensity = { 1.0f, 0.0f };

// The blur amount range.
float2 gBlurAmount = { 0.0f, 9.0f };

// The blur amounts for each blurred reflection map, including 0.0 for the unblurred map.
static float4 gBlurMapAmounts = { 0.0f, 2.0f, 6.0f, 12.0f };

// Reflection map texture and sampler.
Texture2D gReflectionTex < string UIWidget = "None"; > = NULL;
SamplerState gReflectionSamp;

// Extra parameters when variable blur is needed.
#if USE_BLUR

    // The three blurred reflection maps.
    Texture2D gBlur1Tex < string UIWidget = "None"; > = NULL;
    SamplerState gBlur1Samp;
    Texture2D gBlur2Tex;
    SamplerState gBlur2Samp = sampler_state { texture = <gBlur2Tex>; };
    Texture2D gBlur3Tex;
    SamplerState gBlur3Samp = sampler_state { texture = <gBlur3Tex>; };

#endif

// Extra parameters when the distance map is needed.
#if USE_DISTANCE

    // Distance map texture and sampler.
    Texture2D gDistanceTex < string UIWidget = "None"; > = NULL;
    SamplerState gDistanceSamp;

#endif

// Extra parameters for computing view-angle-based falloff (Fresnel reflectance).
#ifdef USE_VIEW

    // World transformation, inverse transpose.
    float4x4 gWITXf : WorldInverseTranspose < string UIWidget = "None"; >;

    // The position of the view in world space.
    float3 gViewPosition : WorldCameraPosition;

#endif

// Depth priority, which shifts the model a bit forward in the z-buffer
float gDepthPriority : DepthPriority = 0.0f;

// Vertex shader input structure.
struct VS_INPUT_Render
{
    float3 Pos : POSITION;

    #ifdef USE_VIEW
        float3 Normal : NORMAL;
    #endif
};

// Vertex shader output structure.
struct VS_TO_PS_Render
{
    // The vertex position in clip space.
    float4 HPos : SV_Position;
    
    // The vertex position in clip space, for use by the pixel shader.
    float4 HPos2 : TEXCOORD0;

    #ifdef USE_VIEW
        // The vertex normal in world space.
        float3 Normal : TEXCOORD1;
        
        // The view direction in world space, not normalized.
        float3 View : TEXCOORD2;
    #endif
};

// A linear step function, which returns a value in [0.0, 1.0] indicating the given value lies in
// the given range.
static float linstep(float start, float end, float value)
{
    return saturate((value - start) / (end - start));
}

// An approximation of Fresnel reflectance, where normal angles (high NdotV) give less reflectance
// and grazing angles (low NdotV) give more reflectance.
static float reflectance(float NdotV)
{
    return 1.0f - smoothstep(0.2f, 0.7f, NdotV);
}

// Vertex shader.
VS_TO_PS_Render VS_PlanarReflection_Render(VS_INPUT_Render In)
{
    VS_TO_PS_Render Out;
    
    // Transform the vertex from object space to clip space.
    Out.HPos = mul(float4(In.Pos, 1.0f), gWVPXf);
    // modify the HPos a bit by biasing the Z a bit forward, based on depth priority
    Out.HPos.z -= Out.HPos.w*gDepthPriority;

    Out.HPos2 = Out.HPos;

    float4 HPw = mul(float4(In.Pos, 1.0f), gWXf);

    #ifdef USE_VIEW
        // Transform the vertex normal from object space to world space.
        Out.Normal = mul(In.Normal, (float3x3) gWITXf);
        
        // Compute the view direction, using the eye position and vertex position.  The eye
        // position is the translation vector of the inverse view transformation matrix.
        // NOTE: This is the direction from the vertex to the eye.
        Out.View = gViewPosition - HPw.xyz;
    #endif
        
    return Out;
}

// Pixel shader.
float4 PS_PlanarReflection_Render(VS_TO_PS_Render In,
     uniform Texture2D mapTex, uniform SamplerState mapSamp) : SV_Target
{
    // Divide the clip-space point by w to form the NDC-space point.
    float3 Pndc = In.HPos2.xyz / In.HPos2.w;

    // Compute the texture coordinates for reflection map sampling based on the clip-space point.
    // They must be mapped from the range [-1.0, 1.0] to [0.0, 1.0].  Since this is used to sample
    // a reflection map rendered with a reflected camera, the v coordinate is not flipped, so that
    // v = 0.0 remains at the top.
    float2 UV = Pndc.xy * 0.5f + 0.5f;
    
    // Compute dot(N, V), if needed; it will be used below.
    #if USE_VIEW
        float NdotV = dot(normalize(In.Normal), normalize(In.View));
    #endif

    // Sample the distance map, if needed.  This is expected to be a normalized value [0.0, 1.0],
    // relative to the desired range of distances.
    #if USE_DISTANCE
        float distance = gDistanceTex.Sample(gDistanceSamp, UV).r;
    #endif

    // Compute reflection intensity, optionally with Fresnel-style reflectance.
    #if INTENSITY_TYPE == NONE
        float intensity = 1.0;
    #elif INTENSITY_TYPE == CONSTANT
        float intensity = gIntensity.x;
    #elif INTENSITY_TYPE == VIEW
        // Use the normal and view direction to compute the reflection intensity.
        float intensity = lerp(gIntensity.y, gIntensity.x, reflectance(NdotV));
    #elif INTENSITY_TYPE == DISTANCE
        // Interpolate between the start and end intensity based on the distance.
        float intensity = lerp(gIntensity.x, gIntensity.y, distance);
    #endif

    // Blur the reflection map if needed.  This is done by interpolating between a set of
    // pre-blurred versions of the reflection map.
    #if USE_BLUR
        // Compute the desired blur amount based on the blur type.
        #if BLUR_TYPE == VIEW
            // Use the normal and view direction to compute the reflection intensity.
            float blur = lerp(gBlurAmount.y, gBlurAmount.x, reflectance(NdotV));
        #elif BLUR_TYPE == DISTANCE
            // Interpolate between the start and end blur based on the distance.
            float blur = lerp(gBlurAmount.x, gBlurAmount.y, distance);
        #endif

        // Compute the index of the blur amount of the blurred reflection maps that is greater than
        // the *desired* blur amount.  This will be in the range (1, 3).  This index and index-1
        // represent the blur amounts and maps that will be interpolated to get the desired blur.
        float4 blurCompare = blur > gBlurMapAmounts;
        int index = (int) dot(1.0f, blurCompare);
        index = clamp(index, 1, 3);
        
        // Determine where the desired blur amount lies in the range between the blur map amounts
        // with the selected indices.
        float factor = linstep(gBlurMapAmounts[index-1], gBlurMapAmounts[index], blur);
        
        // Sample the two blur maps, and interpolate between them using the factor.
        // NOTE: The sampler references must be literals, hence the numerous sample calls.
        float4 color1 = 0.0f, color2 = 0.0f;
        switch (index)
        {
            case 1: color1 = mapTex.Sample(mapSamp, UV);       color2 = gBlur1Tex.Sample(gBlur1Samp, UV); break;
            case 2: color1 = gBlur1Tex.Sample(gBlur1Samp, UV); color2 = gBlur2Tex.Sample(gBlur2Samp, UV); break;
            case 3: color1 = gBlur2Tex.Sample(gBlur2Samp, UV); color2 = gBlur3Tex.Sample(gBlur3Samp, UV); break;
        }
        float4 reflColor = lerp(color1, color2, factor);
    #else
        // Sample the reflection map directly (no interpolated blurring), including alpha.
        float4 reflColor = mapTex.Sample(mapSamp, UV);
    #endif

    // Apply the tint color and saturation.  The gray value is computed with a standard color
    // sensitivity transform, and the saturation value is used to interpolate between that gray
    // and the full color.
    float gray = dot(reflColor.rgb, float3(0.212671f, 0.715160f, 0.072169f));
    float3 resultColor = lerp(gray, reflColor.rgb, gSaturation);
        
    // Combine the reflection map color with the specified color and computed intensity.
    return float4(gColor * resultColor, reflColor.a * intensity);
}

#ifndef FX_COMPOSER

// Technique.
technique10 PlanarReflection_Render_Main
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_4_0, VS_PlanarReflection_Render()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, PS_PlanarReflection_Render(gReflectionTex, gReflectionSamp)));
    }
}

#endif