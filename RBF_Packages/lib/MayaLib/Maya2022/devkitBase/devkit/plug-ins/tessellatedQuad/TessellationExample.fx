//**************************************************************************/
// Copyright (c) 2018 Autodesk, Inc.
// All rights reserved.
//
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.
//**************************************************************************/


#define PI 3.14

//------------------------------------
// Per Frame parameters
//------------------------------------
cbuffer UpdatePerFrame : register(b0)
{
	float4x4 gViewInvXf  : ViewInverse 		< string UIWidget = "None"; >;
	float4x4 gViewXf	 : View				< string UIWidget = "None"; >;
	float4x4 gProjXf	 : Projection		< string UIWidget = "None"; >;
	float4x4 gViewProjXf : ViewProjection	< string UIWidget = "None"; >;

	float2 gScreenSize	 : ViewportPixelSize < string UIWidget = "None"; >;
}

//------------------------------------
// Per Object parameters
//------------------------------------
cbuffer UpdatePerObject : register(b1)
{
	float4x4 gWXf : World < string UIWidget = "None"; > ;
}

// Maya HW Selection
uniform int gHWSPrimitiveBase : HWS_PrimitiveBase;

uniform float FlatTessellationBlend = 0.0;
uniform float DisplacementOffset = 0.5;
uniform float DisplacementMultiplier = 5.0;
uniform float ClippingBiasAdd = 10.0;

uniform float numInnerLevelsPer100Pix
<
	string UIName = "Inner levels/100 *100 pix";
> = 40.0f;

uniform float numOuterLevelsPer30PixSeg
<
	string UIName = "Outer levels/30 pix";
> = 12.0f;

uniform float TransparencyStd
<
	string UIName = "Transparency";
> = 0.0f;

uniform float texScale
<
	string UIName = "Height map scale factor";
> = 1.0f;

Texture2D gHeightMap
<
	string ResourceName = "moonHeightMapSharp.jpg";
	string ResourceType = "2D";
	string UIName = "Height Map";
	int mipmaplevels = 0;
>;

SamplerState gHeightMapSamp
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};


//------------------------------------
// Lighting
//------------------------------------
uniform float rimFresnelMin
<
	string UIGroup = "Lighting";
	string UIWidget = "Slider";
	float UIMin = 0.0;
	float UIMax = 1.0;
	float UIStep = 0.001;
	string UIName = "Rim Light Min";
	int UIOrder = 60;
> = 0.8;

uniform float rimFresnelMax
<
	string UIGroup = "Lighting";
	string UIWidget = "Slider";
	float UIMin = 0.0;
	float UIMax = 1.0;
	float UIStep = 0.001;
	string UIName = "Rim Light Max";
	int UIOrder = 61;
> = 1.0;

uniform float rimBrightness
<
	string UIGroup = "Lighting";
	string UIWidget = "Slider";
	float UIMin = 0.0;
	float UISoftMax = 10.0;
	float UIMax = 100000.0;
	float UIStep = 0.001;
	string UIName = "Rim Light Brightness";
	int UIOrder = 62;
> = 0.0;

uniform float3 SpecularColor : Specular
<
	string UIGroup = "Specular";
	string UIName = "Specular Color";
	string UIWidget = "ColorPicker";
	int UIOrder = 404;
> = {1.0f, 1.0f, 1.0f };

uniform float SpecPower
<
	string UIGroup = "Specular";
	string UIWidget = "Slider";
	float UIMin = 0.0;	// 0 for anisotropic
	float UISoftMax = 100.0;
	float UIMax = 1000000;
	float UIStep = 0.01;
	string UIName = "Specular Power";
	int UIOrder = 405;
> = 20.0;


// ---------------------------------------------
// Light 0 GROUP
// ---------------------------------------------
// This value is controlled by Maya to tell us if a light should be calculated
// For example the artist may disable a light in the scene, or choose to see only the selected light
// This flag allows Maya to tell our shader not to contribute this light into the lighting
uniform bool light0Enable : LIGHTENABLE
<
	string Object = "Light 0";	// UI Group for lights, auto-closed
	string UIName = "Enable Light 0";
	int UIOrder = 20;
> = false;	// maya manages lights itself and defaults to no lights


// follows LightParameterInfo::ELightType
// spot = 2, point = 3, directional = 4, ambient = 5,
uniform int light0Type : LIGHTTYPE
<
	string Object = "Light 0";
	string UIName = "Light 0 Type";
	string UIFieldNames ="None:Default:Spot:Point:Directional:Ambient";
	int UIOrder = 21;
	float UIMin = 0;
	float UIMax = 5;
	float UIStep = 1;
> = 2;	// default to spot so the cone angle etc work when "Use Shader Settings" option is used

uniform float3 light0Pos : POSITION
<
	string Object = "Light 0";
	string UIName = "Light 0 Position";
	string Space = "World";
	int UIOrder = 22;
> = {100.0f, 100.0f, 100.0f};

uniform float3 light0Color : LIGHTCOLOR
<
	string Object = "Light 0";
	string UIName = "Light 0 Color";
	string UIWidget = "Color";
	int UIOrder = 23;
> = { 1.0f, 1.0f, 1.0f};

uniform float light0Intensity : LIGHTINTENSITY
<
	string Object = "Light 0";
	string UIName = "Light 0 Intensity";
	float UIMin = 0.0;
	float UIMax = 10;
	float UIStep = 0.01;
	int UIOrder = 24;
> = 1.0f;

uniform float3 light0Dir : DIRECTION
<
	string Object = "Light 0";
	string UIName = "Light 0 Direction";
	string Space = "World";
	int UIOrder = 25;
> = {100.0f, 100.0f, 100.0f};


uniform float light0ConeAngle : HOTSPOT // In radians
<
	string Object = "Light 0";
	string UIName = "Light 0 Cone Angle";
	float UIMin = 0;
	float UIMax = PI/2;
	int UIOrder = 26;
> = 0.46f;

uniform float light0FallOff : FALLOFF // In radians. Should be HIGHER then cone angle or lighted area will invert
<
	string Object = "Light 0";
	string UIName = "Light 0 Penumbra Angle";
	float UIMin = 0;
	float UIMax = PI/2;
	int UIOrder = 27;
> = 0.7f;

uniform float light0AttenScale : DECAYRATE
<
	string Object = "Light 0";
	string UIName = "Light 0 Decay";
	float UIMin = 0.0;
	float UIMax = 20.0;
	float UIStep = 0.01;
	int UIOrder = 28;
> = 0.0f;

uniform float4x4 light0Matrix : SHADOWMAPMATRIX
<
	string Object = "Light 0";
	string UIWidget = "None";
>;

uniform int DiffuseModel
<
	string UIGroup = "Diffuse";
	string UIName = "Diffuse Model";
	string UIFieldNames ="Lambert:Soften Diffuse (Hair)";
	float UIMin = 0;
	float UIMax = 1;
	float UIStep = 1;
	int UIOrder = 198;
> = 0;

// Calculate a light:
struct lightOut
{
	float Specular;
	float3 Color;
};


// Vertex shader input structure.
struct APPDATA
{
	float3 Position	: POSITION;
	float3 Normal	: NORMAL;
	float2 map1		: TEXCOORD0;
};

struct SHADERDATA
{
	float4 Position	: SV_Position;
	float3 Normal	: NORMAL;
	float4 Pw		: TEXCOORD0;
	float2 map1		: TEXCOORD1;
};

struct SHADERDATA2
{
	float4 Position	: SV_POSITION;
	float3 Normal	: NORMAL;
	float4 Pw		: TEXCOORD0;
	float2 map1		: TEXCOORD1;

	float4 colorIN	: COLOR0;

	float clipped : CLIPPED;

	// Geometry generated control points:
	// .Pw is CP0, so we don't need to store it again
	float3 CP1		: TEXCOORD2;
	float3 CP2		: TEXCOORD3;

	float3 DominantNormalE0		: TEXCOORD4;
	float3 DominantNormalE1		: TEXCOORD5;
	float3 DominantNormalCorner	: TEXCOORD6;
};

struct HSCONSTANTDATA
{
	float TessFactor[3]		: SV_TessFactor;
	float InsideTessFactor	: SV_InsideTessFactor;
	float3 CPCenter			: CENTER;
};


//
// Vertex shader
//
SHADERDATA VS(APPDATA IN)
{
	float4 Normal = mul(float4(IN.Normal, 0.0f), gWXf);
	float4 Pw = mul(float4(IN.Position, 1.0f), gWXf);

	SHADERDATA OUT;
	OUT.Position = Pw;
	OUT.Pw = Pw;
	OUT.Normal = normalize(Normal.xyz);
	OUT.map1 = IN.map1 * texScale;
	return OUT;
}

//
// Hull shader
//
float3 ComputeCP(float3 posA, float3 posB, float3 normA)
{
	return (2.0 * posA + posB - (dot((posB - posA), normA) * normA)) / 3.0;
}

float IsClipped(float4 clipPos, float clipBias)
{
	float W = clipPos.w + clipBias;
	return (-W <= clipPos.x && clipPos.x <= W &&
		-W <= clipPos.y && clipPos.y <= W &&
		-W <= clipPos.z && clipPos.z <= W) ? 0.0 : 1.0;
}

float ComputeClipping(
	float3 cpA, float3 cpB, float3 cpC,
	float clipBias,
	float4x4 vprj)
{
	float4 projPosA = mul(float4(cpA,1), vprj);
	float4 projPosB = mul(float4(cpB,1), vprj);
	float4 projPosC = mul(float4(cpC,1), vprj);
	return min(min(
		IsClipped(projPosA, clipBias),
		IsClipped(projPosB, clipBias)),
		IsClipped(projPosC, clipBias));
}

HSCONSTANTDATA HS_Constant( const OutputPatch<SHADERDATA2, 3> IN,
	uint patchID : SV_PrimitiveID )
{
	HSCONSTANTDATA OUT = (HSCONSTANTDATA)0;
	
	// Now setup the PNTriangle control points...
	// Center control point
	float3 f3E = (IN[0].CP1 + IN[0].CP2 + IN[1].CP1 + IN[1].CP2 + IN[2].CP1 + IN[2].CP2) / 6.0f;
	float3 f3V = (IN[0].Pw.xyz + IN[1].Pw.xyz + IN[2].Pw.xyz) / 3.0f;
	OUT.CPCenter = f3E + ((f3E - f3V) / 2.0f);

	// Clipping:
	float4 centerViewPos = mul( float4(OUT.CPCenter, 1), gViewProjXf );
	bool centerClipped = IsClipped(centerViewPos, ClippingBiasAdd);

	if (IN[0].clipped && IN[1].clipped && IN[2].clipped && centerClipped)
	{
		// If all control points are clipped, the surface cannot possibly be visible.
		// Not entirely true, because displacement mapping can make them visible in the domain shader
		// so we provide the user with a bias factor to avoid clipping too early
		OUT.InsideTessFactor = 0.0f;

		OUT.TessFactor[0] = OUT.TessFactor[1] = OUT.TessFactor[2] = 0.0f;
	}
	else
	{
		//Calculate Inner Level
		float4 vViewPosA = mul(float4(IN[0].Pw.xyz,1), gViewXf);
		float4 vViewPosB = mul(float4(IN[1].Pw.xyz,1), gViewXf);
		float4 vViewPosC = mul(float4(IN[2].Pw.xyz,1), gViewXf);

		vViewPosA = vViewPosA / vViewPosA.w;
		vViewPosB = vViewPosB / vViewPosB.w;
		vViewPosC = vViewPosC / vViewPosC.w;

		float3 ABView = vViewPosB.xyz - vViewPosA.xyz;
		float3 ACView = vViewPosC.xyz - vViewPosA.xyz;

		float3 triCenter = (vViewPosA.xyz+vViewPosB.xyz+vViewPosC.xyz)/3;

		float mulLengths = length(ABView) * length(ACView);
		float cosTheta = dot(ABView,ACView)/(mulLengths);
		float areaTriView = 0.5 * mulLengths * sqrt(1-cosTheta*cosTheta);

		//hypothetical 3d circle that has same area and that faces the camera
		float hypoRadius = sqrt(areaTriView /PI);
		float3 hypoViewCirclePoint = triCenter + float3(hypoRadius,0.0,0.0);

		float4 vProjPosCenter = mul(float4(triCenter,1.0), gProjXf);
		float4 vProjPosRadPoint = mul(float4(hypoViewCirclePoint,1.0), gProjXf);

		vProjPosCenter = vProjPosCenter / vProjPosCenter.w;
		vProjPosRadPoint = vProjPosRadPoint / vProjPosRadPoint.w;

		float newRadius = abs(vProjPosCenter - vProjPosRadPoint).x*0.5;

		float newArea = PI * newRadius * newRadius * gScreenSize.x * gScreenSize.y;

		//Calculate Outer Levels
		float4 vProjPosA = mul(float4(IN[0].Pw.xyz,1),gViewProjXf);
		float4 vProjPosB = mul(float4(IN[1].Pw.xyz,1),gViewProjXf);
		float4 vProjPosC = mul(float4(IN[2].Pw.xyz,1),gViewProjXf);

		//make in range [0; 1]
		vProjPosA = vProjPosA * 0.5 / vProjPosA.w + 0.5;
		vProjPosB = vProjPosB * 0.5 / vProjPosB.w + 0.5;
		vProjPosC = vProjPosC * 0.5 / vProjPosC.w + 0.5;

		//convert to pixels
		vProjPosA.x = vProjPosA.x * gScreenSize.x;
		vProjPosB.x = vProjPosB.x * gScreenSize.x;
		vProjPosC.x = vProjPosC.x * gScreenSize.x;
		vProjPosA.y = vProjPosA.y * gScreenSize.y;
		vProjPosB.y = vProjPosB.y * gScreenSize.y;
		vProjPosC.y = vProjPosC.y * gScreenSize.y;

		OUT.InsideTessFactor = numInnerLevelsPer100Pix * sqrt(newArea / (100*100));

		OUT.TessFactor[1] = length( vProjPosA.xy - vProjPosB.xy ) * numOuterLevelsPer30PixSeg / 30;
		OUT.TessFactor[0] = length( vProjPosB.xy - vProjPosC.xy ) * numOuterLevelsPer30PixSeg / 30;
		OUT.TessFactor[2] = length( vProjPosC.xy - vProjPosA.xy ) * numOuterLevelsPer30PixSeg / 30;
	}

	return OUT;
}

[domain("tri")]
[partitioning("fractional_odd")]
[outputtopology("triangle_cw")]
[patchconstantfunc("HS_Constant")]
[outputcontrolpoints(3)]
[maxtessfactor(64.0)]
SHADERDATA2 HS(InputPatch<SHADERDATA, 18> IN,
	uint index : SV_OutputControlPointID,
	uint patchID : SV_PrimitiveID )
{
	SHADERDATA2 OUT = (SHADERDATA2)0;

	OUT.Position = IN[index].Position;
	OUT.Normal = IN[index].Normal;
	OUT.Pw = IN[index].Pw;
	OUT.map1 = IN[index].map1;

	// Make sure that PNAEN18 is chosen in technique annotation
	const uint nextIndex = index < 2 ? index + 1 : 0;
	const uint neighborIndex = 3 + 2 * index;
	const uint neighborNextIndex = neighborIndex + 1;

	float3 myCP = ComputeCP(IN[index].Pw.xyz,
		IN[nextIndex].Pw.xyz,
		IN[index].Normal);

	float3 neighborCP = ComputeCP(IN[neighborIndex].Pw.xyz,
		IN[neighborNextIndex].Pw.xyz,
		IN[neighborIndex].Normal);

	OUT.CP1 = (myCP + neighborCP) / 2;

	myCP = ComputeCP(IN[nextIndex].Pw.xyz,
		IN[index].Pw.xyz,
		IN[nextIndex].Normal);

	neighborCP = ComputeCP(IN[neighborNextIndex].Pw.xyz,
		IN[neighborIndex].Pw.xyz,
		IN[neighborNextIndex].Normal);

	OUT.CP2 = (myCP + neighborCP) / 2;

	const uint dominantEdgeIndex = 9 + 2 * index;
	const uint dominantEdgeNextIndex = dominantEdgeIndex + 1;
	const uint dominantVertexIndex = 15 + index;

	OUT.DominantNormalE0 = IN[dominantEdgeNextIndex].Normal;
	OUT.DominantNormalE1 = IN[dominantEdgeIndex].Normal;
	OUT.DominantNormalCorner = IN[dominantVertexIndex].Normal;
	OUT.Normal = IN[index].Normal;
	OUT.Pw = IN[index].Pw;
	OUT.map1 = IN[index].map1;

	return OUT;
}

//
// Domain shader
//
float3 PickDominant( float3 vec,
	float U, float V, float W,
	float3 DE0A, float3 DE0B,
	float3 DE1A, float3 DE1B,
	float3 DE2A, float3 DE2B,
	float3 DV0, float3 DV1, float3 DV2 )
{
	float3 dominantVector = vec;

	float edgeThreshold = 0.0001f;
	int edgeU = (U == 0) ? 1 : 0;
	int edgeV = (V == 0) ? 1 : 0;
	int edgeW = (W == 0) ? 1 : 0;

	int corner = ((edgeU + edgeV + edgeW) == 2) ? 1 : 0;
	int innerarea = ((edgeU + edgeV + edgeW) == 0) ? 1 : 0;

	if (innerarea != 1)
	{
		if (corner == 1)
		{
			if (U > 1.0 - edgeThreshold)
				dominantVector = DV1;
			else if (V > 1.0 - edgeThreshold)
				dominantVector = DV2;
			else if (W > 1.0 - edgeThreshold)
				dominantVector = DV0;
		}
		else
		{
			if (edgeU == 1)
				dominantVector = lerp(DE2A, DE2B, W);
			else if (edgeV == 1)
				dominantVector = lerp(DE0A, DE0B, U);
			else
				dominantVector = lerp(DE1A, DE1B, V);
		}
	}

	return dominantVector;
}

[domain("tri")]
SHADERDATA2 DS(
	HSCONSTANTDATA HSIN,
	OutputPatch<SHADERDATA2, 3> IN,
	float3 BarycentricCoords : SV_DomainLocation )
{
	SHADERDATA2 OUT = (SHADERDATA2)0;

	float U = BarycentricCoords.x;
	float V = BarycentricCoords.y;
	float W = BarycentricCoords.z;

	float UU = U * U;
	float VV = V * V;
	float WW = W * W;
	float UU3 = UU * 3.0;
	float VV3 = VV * 3.0;
	float WW3 = WW * 3.0;

	float3 position =
		IN[0].Pw.xyz * WW * W +
		IN[1].Pw.xyz * UU * U +
		IN[2].Pw.xyz * VV * V +
		IN[0].CP1 * WW3 * U +
		IN[0].CP2 * W * UU3 +
		IN[2].CP2 * WW3 * V +
		IN[1].CP1 * UU3 * V +
		IN[2].CP1 * W * VV3 +
		IN[1].CP2 * U * VV3 +
		HSIN.CPCenter * 6.0 * W * U * V;

	float3 flatPosition =
		IN[0].Pw.xyz * W +
		IN[1].Pw.xyz * U +
		IN[2].Pw.xyz * V;

	position = lerp(position, flatPosition, FlatTessellationBlend);

	OUT.Normal = IN[0].Normal * W + IN[1].Normal * U + IN[2].Normal * V;
	OUT.Normal = normalize(OUT.Normal);
	OUT.map1 = IN[0].map1 * W + IN[1].map1 * U + IN[2].map1 * V;

	float4 Sampler = gHeightMap.SampleLevel(gHeightMapSamp, OUT.map1.xy, 0);

	float3 PNCrackFixNORMAL = normalize( PickDominant(
		OUT.Normal, U, V, W,
		IN[0].DominantNormalE0,
		IN[0].DominantNormalE1,
		IN[1].DominantNormalE0,
		IN[1].DominantNormalE1,
		IN[2].DominantNormalE0,
		IN[2].DominantNormalE1,
		IN[0].DominantNormalCorner,
		IN[1].DominantNormalCorner,
		IN[2].DominantNormalCorner) );

	float3 GrayScale = (PNCrackFixNORMAL * ((Sampler.x+Sampler.y+Sampler.z+Sampler.w)/4 - DisplacementOffset));
	float3 DisplaceAmount = (DisplacementMultiplier * GrayScale);

	position.xyz += DisplaceAmount;

	OUT.Pw = float4(position.xyz, 1);

	OUT.colorIN = float4(abs(Sampler.xyz), 1.0f);

	OUT.Position = mul(OUT.Pw, gViewProjXf);

	return OUT;
}


//
// Pixel shader
//

// Spot light cone
float lightConeangle(float coneAngle, float coneFalloff, float3 lightVec, float3 lightDir)
{
	// the cone falloff should be equal or bigger then the coneAngle or the light inverts
	// this is added to make manually tweaking the spot settings easier.
	if (coneFalloff < coneAngle)
		coneFalloff = coneAngle;

	float LdotDir = dot(normalize(lightVec), lightDir);

	// cheaper cone, no fall-off control would be:
	// float cone = pow(saturate(LdotDir), 1 / coneAngle);

	// higher quality cone (more expensive):
	float cone = smoothstep( cos(coneFalloff), cos(coneAngle), LdotDir);

	return cone;
}

lightOut CalculateLight	(	bool lightEnable, int lightType, float lightAtten, float3 lightPos, float3 vertWorldPos,
						float3 lightColor, float lightIntensity, float3 lightDir, float lightConeAngle, float lightFallOff, float4x4 lightViewPrjMatrix,
						float3 vertexNormal, float3 normal, float3 diffuseColorIn,
						float3 eyeVec, float roughness,	float3 specularColorIn, float3 thickness, float softenMask,
						float gammaCorrection, float rim, float glossiness, float opacity, float3 ambientOcclusion, float4 anisotropicDir )
{
	lightOut OUT;

	OUT.Specular = 0.0;
	OUT.Color = float3(0,0,0);

	if (lightEnable)
	{
		// For Maya, flip the lightDir:
		lightDir = -lightDir;

		// Ambient light does no diffuse, specular shading or shadow casting.
		// Because it does equal shading from all directions to the object, we will also not have it do any translucency.
		bool isAmbientLight = (lightType == 5);
		if (isAmbientLight)
		{
			OUT.Color = diffuseColorIn * lightColor * lightIntensity;
			// Ambient Occlusion (and sometimes Lightmap) should affect the contribution of the ambient light:
			OUT.Color.rgb *= ambientOcclusion;
			return OUT;
		}

		// directional light has no position, so we use lightDir instead
		bool isDirectionalLight = (lightType == 4);
		float3 lightVec = lerp(lightPos - vertWorldPos, lightDir, float(isDirectionalLight));

		float3 L = normalize(lightVec);

		// Diffuse:
		float3 diffuseColor = float3(0,0,0);
		if (DiffuseModel == 0)	// Lambert:
		{
			diffuseColor = clamp(dot(normal, L), 0.0, 1.0) * diffuseColorIn;
		}
		else if (DiffuseModel == 1) // (hair/fur):
		{
			/// lerp to shift the shadow boundary for a softer look:
			diffuseColor = clamp( lerp(0.25, 1.0, dot(normal, L) ), 0.0, 1.0 ) * diffuseColorIn;
		}

		// Rim Light:
		float rimUnit = rim * clamp(dot(normal, -L), 0.0, 1.0);
		float3 rimColor = float3(rimUnit, rimUnit, rimUnit);

		// Specular:
		float3 specularColor = float3(0,0,0);

		// BLINN
		{
			// Phong:
			// float3 R = -reflect(L, normal);
			// float RdotV = saturate(dot(R,eyeVec));
			// specularColor = (pow(RdotV, glossiness) * specularColorIn);

			float3 H = normalize(L + eyeVec); // half angle
			float NdotH = clamp(dot(normal, H),0.0,1.0);
			specularColor = specularColorIn * pow(NdotH, glossiness);
			specularColor *= clamp( dot(normal, L), 0.0, 1.0 );	// prevent spec leak on back side of model
		}


		// Light Attenuation:
		bool enableAttenuation = lightAtten > 0.0001f;
		float attenuation = 1.0f;
		if (!isDirectionalLight)	// directional lights do not support attenuation, skip calculation
		{
			attenuation = lerp(1.0, 1 / pow(length(lightVec), lightAtten), enableAttenuation);
		}

		// compensate diffuse and specular color with various light settings:
		specularColor *= (lightColor * lightIntensity * attenuation);
		diffuseColor *= (lightColor * lightIntensity * attenuation);

		// Spot light Cone Angle:
		if (lightType == 2)
		{
			float angle = lightConeangle(lightConeAngle, lightFallOff, lightVec, lightDir);
			diffuseColor *= angle;
			specularColor *= angle;
		}

		// Add specular and rim light on top of final output color
		// multiply OUT.Color with opacity since we are using a pre-multiplied alpha render state
		// if we don't do this, the rim may have halo's around it when the object is fully transparent
		OUT.Color += diffuseColor;
		OUT.Color *= opacity;

		OUT.Color += specularColor + rimColor;

		// Output specular and rim for opacity:
		OUT.Specular = dot(clamp(specularColor, 0.0, 1.0), float3(0.3f, 0.6f, 0.1f)) + rimColor.r;


	} // end if light enabled

	return OUT;
}

// Pixel Shader to show waves
float4 PS_Wave(SHADERDATA2 IN) : SV_Target
{
	float3 N = IN.Normal;
	float3 Nw = N;
	float3 V = normalize( gViewInvXf[3].xyz - IN.Pw.xyz );

	float roughness = 0;
	float3 specularColorIn = float3(0.0,0.0,0.0);
	float3 thickness = float3(0.0,0.0,0.0);
	float softenMask = 0;
	float gammaCorrection = 0;
	float rim = clamp((clamp(1.0f - dot(N, V),0.0, 1.0)-rimFresnelMin)/(max(rimFresnelMax, rimFresnelMin)  - rimFresnelMin),0.0, 1.0);
	rim *= rimBrightness;

	float glossiness = max(1.0, SpecPower);
	float opacity = 1.0f-TransparencyStd;
	float3 ambientOcclusion = float3(1,1,1);
	float4 anisotropicDir = float4(1.0,1.0,1.0,1.0);

	float3 effectiveDiffuseColor = IN.colorIN.xyz; //DiffuseColor;

	// light 0:
	lightOut light0 = CalculateLight(
		light0Enable, light0Type, light0AttenScale, light0Pos, IN.Pw.xyz,
		light0Color, light0Intensity, light0Dir, light0ConeAngle, light0FallOff, light0Matrix,
		Nw, N, effectiveDiffuseColor, V, roughness, SpecularColor,
		thickness, softenMask, gammaCorrection, rim, glossiness, opacity, ambientOcclusion,
		anisotropicDir );

	float3 lightTotal = light0.Color;

	lightTotal += effectiveDiffuseColor;
	float specTotal = light0.Specular;

	float transparency = (specTotal) + opacity;
	transparency = clamp(transparency, 0.0, 1.0);	// keep 0-1 range

	return float4(lightTotal, transparency);
}

// Geometry shader to show wireframe
[maxvertexcount(6)]
void GS_Wireframe(triangle SHADERDATA2 IN[3], inout LineStream<SHADERDATA2> stream)
{
	SHADERDATA2 OUT = IN[0];
	stream.Append( OUT );

	OUT = IN[1];
	stream.Append( OUT );

	stream.RestartStrip();

	OUT = IN[1];
	stream.Append( OUT );

	OUT = IN[2];
	stream.Append( OUT );

	stream.RestartStrip();

	OUT = IN[2];
	stream.Append( OUT );

	OUT = IN[0];
	stream.Append( OUT );

	stream.RestartStrip();
}

// Pixel shader to show wireframe
float4 PS_Wireframe() : SV_Target
{
	return float4(1.0, 0.0, 0.0, 1.0);
}

// Pixel shader used for Viewport 2.0 hardware selection
float4 PS_HWSelection() : SV_Target
{
	int colorID = gHWSPrimitiveBase;

	float4 color;
	color.x = float(colorID & 0x000000FF) / 255.0f;
	color.y = float((colorID & 0x0000FF00) >> 8) / 255.0f;
	color.z = float((colorID & 0x00FF0000) >> 16) / 255.0f;
	color.w = float((colorID & 0xFF000000) >> 24) / 255.0f;
	return color;
}

technique11 Main
<
	// We do not need transparency for this technique
	int isTransparent = 0;

	string index_buffer_type = "PNAEN18";
>
{
	pass p0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetHullShader(CompileShader(hs_5_0, HS()));
		SetDomainShader(CompileShader(ds_5_0, DS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_Wave()));
	}
}

technique11 Wireframe
<
	// We do not need transparency for this technique
	int isTransparent = 0;

	string index_buffer_type = "PNAEN18";
>
{
	pass p0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetHullShader(CompileShader(hs_5_0, HS()));
		SetDomainShader(CompileShader(ds_5_0, DS()));
		SetGeometryShader(CompileShader(gs_5_0, GS_Wireframe()));
		SetPixelShader(CompileShader(ps_5_0, PS_Wireframe()));
	}
}

technique11 Main_Selection
<
	// We do not need transparency for this technique
	int isTransparent = 0;

	string index_buffer_type = "PNAEN18";

	// The technique is used for Maya HW selection.
	string keyword = "mayaHWSelection";
>
{
	pass p0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetHullShader(CompileShader(hs_5_0, HS()));
		SetDomainShader(CompileShader(ds_5_0, DS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_HWSelection()));
	}
}

technique11 Wireframe_Selection
<
	// We do not need transparency for this technique
	int isTransparent = 0;

	string index_buffer_type = "PNAEN18";

	// The technique is used for Maya HW selection.
	string keyword = "mayaHWSelection";
>
{
	pass p0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetHullShader(CompileShader(hs_5_0, HS()));
		SetDomainShader(CompileShader(ds_5_0, DS()));
		SetGeometryShader(CompileShader(gs_5_0, GS_Wireframe()));
		SetPixelShader(CompileShader(ps_5_0, PS_HWSelection()));
	}
}
