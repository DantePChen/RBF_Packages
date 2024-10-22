///////////////////////////////////////////////////////
//
// DESCRIPTION:  
// Produces dependency graph node Lava

// This node is an example of a solid texture that uses turbulence.

// The output attributes of this node are called "outColor" and "outAlpha".

// To use this shader, create a Lava node and connect the output to an input of a surface/shader node such as Color. 
//
///////////////////////////////////////////////////////


#include <math.h>
#include <stdlib.h>

#include <maya/MPxNode.h>
#include <maya/MIOStream.h>
#include <maya/MString.h>
#include <maya/MTypeId.h>
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h> 
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFloatVector.h>
#include <maya/MFloatPoint.h>
#include <maya/MFnPlugin.h>

// Local functions
float Noise(float, float, float);
void  Noise_init();
static float Omega(int i, int j, int k, float t[3]);
static float omega(float);
static double turbulence(double u,double v,double w,int octaves);

#define PI                  3.14159265358979323846

#ifdef FLOOR
#undef FLOOR
#endif
#define FLOOR(x)            ((int)floorf(x))
#define TABLELEN            512
#define TLD2                256    // TABLELEN 

// Local variables
static int                  Phi[TABLELEN];
static char                 fPhi[TABLELEN];
static float                G[TABLELEN][3];


class Lava3D : public MPxNode
{
	public:
                    Lava3D();
            ~Lava3D() override;

    MStatus compute( const MPlug&, MDataBlock& ) override;
    SchedulingType schedulingType() const override { return SchedulingType::kParallel; }

    static  void *  creator();
    static  MStatus initialize();

	//  Id tag for use with binary file format
    static  MTypeId id;

	private:

	// Input attributes
    static MObject aColorBase;
    static MObject aColorFlame;
    static MObject aDeform;
    static MObject aWarp;
    static MObject aSpeed;
    static MObject aTurbulence;
    static MObject aPower;
    static MObject aFrame;
    static MObject aPointWorld;
    static MObject aPlaceMat;

	// Output attributes
    static MObject aOutColor;
    static MObject aOutAlpha;
};

// Static data
MTypeId Lava3D::id(0x81015);

// Attributes

MObject  Lava3D::aColorBase;
MObject  Lava3D::aColorFlame;
MObject  Lava3D::aDeform;
MObject  Lava3D::aWarp;
MObject  Lava3D::aSpeed;
MObject  Lava3D::aTurbulence;
MObject  Lava3D::aPower;
MObject  Lava3D::aFrame;
MObject  Lava3D::aPointWorld;
MObject  Lava3D::aPlaceMat;

MObject  Lava3D::aOutColor;
MObject  Lava3D::aOutAlpha;

#define MAKE_INPUT(attr)								\
    CHECK_MSTATUS ( attr.setKeyable(true) );    		\
	CHECK_MSTATUS ( attr.setStorable(true) );			\
    CHECK_MSTATUS ( attr.setReadable(true) ) ;  		\
	CHECK_MSTATUS ( attr.setWritable(true) );

#define MAKE_OUTPUT(attr)								\
    CHECK_MSTATUS ( attr.setKeyable(false) );  			\
	CHECK_MSTATUS ( attr.setStorable(false) ) ;			\
    CHECK_MSTATUS ( attr.setReadable(true) );  			\
	CHECK_MSTATUS ( attr.setWritable(false) );

//
// DESCRIPTION:
///////////////////////////////////////////////////////
Lava3D::Lava3D()
{
}

//
// DESCRIPTION:
///////////////////////////////////////////////////////
Lava3D::~Lava3D()
{
}

//
// DESCRIPTION:
///////////////////////////////////////////////////////
void * Lava3D::creator()
{
    return new Lava3D();
}

//
// DESCRIPTION:
///////////////////////////////////////////////////////
MStatus Lava3D::initialize()
{
    MFnMatrixAttribute mAttr;
    MFnNumericAttribute nAttr; 

	// Input attributes

    aColorBase = nAttr.createColor("ColorBase", "cb");
    MAKE_INPUT(nAttr);

    aColorFlame = nAttr.createColor("ColorFlame", "cf");
    MAKE_INPUT(nAttr);
    CHECK_MSTATUS ( nAttr.setDefault(1., 1., 1.) );

    aDeform = nAttr.create( "Deformation", "d", MFnNumericData::kLong);
    MAKE_INPUT(nAttr);
    CHECK_MSTATUS ( nAttr.setDefault(1) );
    CHECK_MSTATUS (nAttr.setMin(1) );
    CHECK_MSTATUS ( nAttr.setMax(10) );

    aWarp = nAttr.create( "Warp", "w", MFnNumericData::kFloat);
    MAKE_INPUT(nAttr);
    CHECK_MSTATUS ( nAttr.setDefault(0.1f) );
    CHECK_MSTATUS ( nAttr.setMin(0.0f) );
    CHECK_MSTATUS ( nAttr.setMax(10.0f) );

    aSpeed = nAttr.create( "Speed", "ws", MFnNumericData::kFloat);
    MAKE_INPUT(nAttr);
    CHECK_MSTATUS ( nAttr.setDefault(0.1f) );
    CHECK_MSTATUS (nAttr.setMin(0.0f) );
    CHECK_MSTATUS (nAttr.setMax(1.0f) );

    aTurbulence = nAttr.create( "Turbulence", "t", MFnNumericData::kLong);
    MAKE_INPUT(nAttr);
    CHECK_MSTATUS ( nAttr.setDefault(2) );
    CHECK_MSTATUS ( nAttr.setMin(1) );
    CHECK_MSTATUS ( nAttr.setMax(10) );

    aPower = nAttr.create( "Power", "pow", MFnNumericData::kFloat);
    MAKE_INPUT(nAttr);
    CHECK_MSTATUS ( nAttr.setDefault(1.0f) );
    CHECK_MSTATUS ( nAttr.setMin(0.0f) );
    CHECK_MSTATUS ( nAttr.setMax(100.0f) );

    aFrame = nAttr.create( "Frame", "f", MFnNumericData::kFloat);
    MAKE_INPUT(nAttr);
    CHECK_MSTATUS ( nAttr.setDefault(1.0f) );
    CHECK_MSTATUS ( nAttr.setMin(0.0f) );
    CHECK_MSTATUS ( nAttr.setMax(1000.0f) );

    aPlaceMat = mAttr.create("placementMatrix", "pm", 
							 MFnMatrixAttribute::kFloat);
    MAKE_INPUT(mAttr);

	// Implicit shading network attributes

    aPointWorld = nAttr.createPoint("pointWorld", "pw");
	MAKE_INPUT(nAttr);
    CHECK_MSTATUS ( nAttr.setHidden(true) );

	// Create output attributes

    aOutColor = nAttr.createColor("outColor", "oc");
	MAKE_OUTPUT(nAttr);

    aOutAlpha = nAttr.create( "outAlpha", "oa", MFnNumericData::kFloat);
	MAKE_OUTPUT(nAttr);

	// Add attributes to the node database.

    CHECK_MSTATUS ( addAttribute(aColorBase) );
    CHECK_MSTATUS ( addAttribute(aColorFlame) );
    CHECK_MSTATUS ( addAttribute(aDeform) );
    CHECK_MSTATUS ( addAttribute(aWarp) );
    CHECK_MSTATUS ( addAttribute(aSpeed) );
    CHECK_MSTATUS ( addAttribute(aTurbulence) );
    CHECK_MSTATUS ( addAttribute(aPower) );
    CHECK_MSTATUS ( addAttribute(aFrame) );
    CHECK_MSTATUS ( addAttribute(aPointWorld) );
    CHECK_MSTATUS ( addAttribute(aPlaceMat) );

    CHECK_MSTATUS ( addAttribute(aOutAlpha) );
    CHECK_MSTATUS ( addAttribute(aOutColor) );

    // All input affect the output color and alpha
    CHECK_MSTATUS ( attributeAffects( aColorBase, aOutColor) );
    CHECK_MSTATUS ( attributeAffects( aColorFlame, aOutColor) );
    CHECK_MSTATUS ( attributeAffects( aDeform, aOutColor) );
    CHECK_MSTATUS ( attributeAffects( aWarp, aOutColor) );
    CHECK_MSTATUS ( attributeAffects( aSpeed, aOutColor) );
    CHECK_MSTATUS ( attributeAffects( aTurbulence, aOutColor) );
    CHECK_MSTATUS ( attributeAffects( aPower, aOutColor) );
    CHECK_MSTATUS ( attributeAffects( aFrame, aOutColor) );
    CHECK_MSTATUS ( attributeAffects( aPointWorld, aOutColor) );
    CHECK_MSTATUS ( attributeAffects( aPlaceMat, aOutColor) );

    CHECK_MSTATUS ( attributeAffects (aColorBase, aOutAlpha) );
    CHECK_MSTATUS ( attributeAffects (aColorFlame, aOutAlpha) );
    CHECK_MSTATUS ( attributeAffects (aDeform, aOutAlpha) );
    CHECK_MSTATUS ( attributeAffects (aWarp, aOutAlpha) );
    CHECK_MSTATUS ( attributeAffects (aSpeed, aOutAlpha) );
    CHECK_MSTATUS ( attributeAffects (aTurbulence, aOutAlpha) );
    CHECK_MSTATUS ( attributeAffects (aPower, aOutAlpha) );
    CHECK_MSTATUS ( attributeAffects (aFrame, aOutAlpha) );
    CHECK_MSTATUS ( attributeAffects (aPointWorld, aOutAlpha) );
    CHECK_MSTATUS ( attributeAffects (aPlaceMat, aOutAlpha) );

    return MS::kSuccess;
}


///////////////////////////////////////////////////////
// DESCRIPTION:
// This function gets called by Maya to evaluate the texture.
//
///////////////////////////////////////////////////////

MStatus Lava3D::compute(const MPlug& plug, MDataBlock& block)
{
    if ( (plug != aOutColor) && (plug.parent() != aOutColor) &&
         (plug != aOutAlpha) )
       return MS::kUnknownParameter;

    const float3& worldPos = block.inputValue(aPointWorld).asFloat3();
    const MFloatMatrix& m = block.inputValue(aPlaceMat).asFloatMatrix();
    const MFloatVector& cBase = block.inputValue(aColorBase).asFloatVector();
    const MFloatVector& cFlame =block.inputValue(aColorFlame).asFloatVector();

    const int   deform = block.inputValue( aDeform ).asInt();
    const float warp   = block.inputValue( aWarp ).asFloat();
    const float speed  = block.inputValue( aSpeed ).asFloat();
    const int   turbValue   = block.inputValue( aTurbulence ).asInt();
    const float power  = block.inputValue( aPower ).asFloat();
    const float frame  = block.inputValue( aFrame ).asFloat();

	MFloatPoint q(worldPos[0], worldPos[1], worldPos[2]);
    q *= m;									// Convert into solid space

    float u, v, w;
    u = q.x; v = q.y; w = q.z;

    float dist = speed * frame;
    float au, av, aw;
    au = u + dist;
    av = v + dist;
    aw = w + dist;

	// Calculate 3 noise values
    float ascale = (float) turbulence( au, av, aw,(int)deform);
    float bscale = (float) turbulence( au,-av, aw,(int)deform);
    float cscale = (float) turbulence(-au, av,-aw,(int)deform);
    float dscale = warp;

	// Add this noise as a vector to the texture coordinates
	// (since we are only calculating one value, the
	// displacement will be alont the 1 1 1 vector ... this
	// displacement generates the "flicker" movement as the
	// value moves around the texture coordinate

    u += ascale * dscale;
    v += bscale * dscale;
    w += cscale * dscale;

	// Calculate a turbulence value for this point

    float scalar = (float) (turbulence(u,v,w,(int)turbValue) + 0.5);

	// convert scalar into a point on the color curve

    if (power != 1) scalar = powf (scalar, power);

    MDataHandle outHandle = block.outputValue( aOutColor );
    MFloatVector & outColor = outHandle.asFloatVector();
    outColor = ((cFlame-cBase)*scalar) + cBase;
    outHandle.setClean();

    outHandle = block.outputValue(aOutAlpha);
    outHandle.asFloat() = scalar;
    outHandle.setClean();

    return MS::kSuccess;
}

//////////////////////////////////////////////////////////////////
MStatus initializePlugin( MObject obj )
{
    const MString UserClassify( "texture/3d" );

    MFnPlugin plugin( obj, PLUGIN_COMPANY, "4.5", "Any" );
    CHECK_MSTATUS ( plugin.registerNode( "lava", Lava3D::id, 
	    Lava3D::creator, Lava3D::initialize,
	    MPxNode::kDependNode, &UserClassify) );

    Noise_init();
    
    return MS::kSuccess;
}

// DESCRIPTION:
///////////////////////////////////////////////////////
MStatus uninitializePlugin( MObject obj )
{
    MFnPlugin plugin( obj );
    CHECK_MSTATUS ( plugin.deregisterNode( Lava3D::id ) );

    return MS::kSuccess;
}

//
//  REFERENCES:
//      Perlin, K. An Image Synthesizer, Computer Graphics, 
//      Vol. 19, No. 3, July 1985.
//
//      Perlin, K., Hoffert, E.M., Hypertexture, Computer Graphics, 
//      Vol. 23, No. 3, July 1989.
//

float Noise(float u, float v, float w)
{
    int         i;
    int         j;
    int         k;
    int         ul;
    int         vl;
    int         wl;
    float       ans;
    float       t[3];

    ans = 0.0;
    ul  = FLOOR(u);
    vl  = FLOOR(v);
    wl  = FLOOR(w);

    for(i = ul + 1; i >= ul; i--)
    {
		t[0] = u - i;
		for(j = vl + 1; j >= vl; j--)
        {   
			t[1] = v - j;
			for(k = wl + 1; k >= wl; k--)
            {   
				t[2] = w - k;
				ans += Omega(i, j, k, t);
            }
        }
    }

    return ans;
}

static float Omega(int i, int j, int k, float t[3])
{
    int ct;

    ct = Phi[((i + 
         Phi[((j + 
         Phi[(k%TLD2)+TLD2]) % TLD2) + TLD2]) % TLD2) + TLD2];

    return omega(t[0]) * omega(t[1]) * omega(t[2]) *
		( G[ct][0]*t[0] + G[ct][1]*t[1] + G[ct][2]*t[2] );
}

static float omega(float t)
{
    t  = fabsf(t);
    return (t * (t * (t * (float)2.0 - (float)3.0))) + (float)1.0;
}

void Noise_init()
{
    int i;
    float u, v, w, s, len;
    static int first_time = 1;

    if (first_time)
        first_time = 0;
    else
        return;

    (void)srand48(0l);

    for(i = 0; i < TABLELEN; i++)
        fPhi[i] = 0;

    for(i = 0; i < TABLELEN; i++) {
		Phi[i] = lrand48() % TABLELEN;

        if (fPhi[Phi[i]])
            i--;
        else
            fPhi[Phi[i]] = 1;
    }
    for(i = 0; i < TABLELEN; i++) {
		u = (float) (2.0 * drand48() - 1.0);
		v = (float) (2.0 * drand48() - 1.0);
		w = (float) (2.0 * drand48() - 1.0);
		if((s = u*u + v*v + w*w) > 1.0) 
        {  
			i--;
			continue;
		}
        else if (s == 0.0)
		{
			i--;
			continue;
		}

		len = 1.0f / sqrtf(s);
		G[i][0] = u * len;
		G[i][1] = v * len;
		G[i][2] = w * len;
    }
}

static double turbulence(double u,double v,double w,int octaves)
{
	double s,t;
	s = 1.0;
	t = 0.0;

	while (octaves--) {
		t += Noise((float)u, (float)v, (float)w)*s;
		s *= 0.5;
		u*=2.0; v*=2.0; w*=2.0;
	}
	return t;
}
// =====================================================================
// Copyright 2018 Autodesk, Inc.  All rights reserved.
//
// This computer source code  and related  instructions and comments are
// the unpublished confidential and proprietary information of Autodesk,
// Inc. and are  protected  under applicable  copyright and trade secret
// law. They may not  be disclosed to, copied or used by any third party
// without the prior written consent of Autodesk, Inc.
// =====================================================================
