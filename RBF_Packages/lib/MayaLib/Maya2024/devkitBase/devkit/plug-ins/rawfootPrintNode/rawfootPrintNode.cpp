//-
// ==========================================================================
// Copyright 2020 Autodesk, Inc.  All rights reserved.
// Use of this software is subject to the terms of the Autodesk license agreement
// provided at the time of installation or download, or which otherwise
// accompanies this software in either electronic or hard copy form.
// ==========================================================================
//+
#if _MSC_VER >= 1700
#pragma warning( disable: 4005 )
#endif

#include <maya/MPxLocatorNode.h>
#include <maya/MString.h>
#include <maya/MDagPath.h>
#include <maya/MTypeId.h>
#include <maya/MPlug.h>
#include <maya/MVector.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MColor.h>
#include <maya/M3dView.h>
#include <maya/MFnPlugin.h>
#include <maya/MDistance.h>
#include <maya/MMatrix.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFileObject.h>
#include <maya/MHWGeometry.h>
#include <maya/MEventMessage.h>
#include <maya/MEvaluationNode.h>
#include <maya/MPointArray.h>

// Viewport 2.0 includes
#include <maya/MDrawRegistry.h>
#include <maya/MPxDrawOverride.h>
#include <maya/MUserData.h>
#include <maya/MDrawContext.h>
#include <maya/MSelectionContext.h>
#include <maya/MGlobal.h>
#include <maya/MSelectionList.h>
#include <maya/MViewport2Renderer.h>
#include <maya/MHWGeometryUtilities.h>
#include <maya/MStateManager.h>
#include <maya/MShaderManager.h>
#include <maya/MRenderTargetManager.h>
#include <maya/MAnimControl.h>
#include <maya/MFnCamera.h>

#include <assert.h>
#include <stdlib.h>

// DX stuff
#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <d3d11.h>

    #include <dxgi.h>
    #include <DirectXMath.h>
    
    using namespace DirectX;

    #include <d3dcompiler.h>

    #ifndef D3DCOMPILE_ENABLE_STRICTNESS
        #define D3DCOMPILE_ENABLE_STRICTNESS D3D10_SHADER_ENABLE_STRICTNESS
        #define D3DCOMPILE_DEBUG D3D10_SHADER_DEBUG
    #endif
#endif

// GL stuff
#if defined (__APPLE__)
    #define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
    #include <OpenGL/gl3.h> // For gl*VertexArray(s) and APIENTRY
#elif defined (__linux__)
    #include <GL/glx.h> // For glXGetProcAddressARB
#endif

// Foot Data
//
static float sole[][3] = { {  0.00f, 0.0f, -0.70f },
                           {  0.04f, 0.0f, -0.69f },
                           {  0.09f, 0.0f, -0.65f },
                           {  0.13f, 0.0f, -0.61f },
                           {  0.16f, 0.0f, -0.54f },
                           {  0.17f, 0.0f, -0.46f },
                           {  0.17f, 0.0f, -0.35f },
                           {  0.16f, 0.0f, -0.25f },
                           {  0.15f, 0.0f, -0.14f },
                           {  0.13f, 0.0f,  0.00f },
                           {  0.00f, 0.0f,  0.00f },
                           { -0.13f, 0.0f,  0.00f },
                           { -0.15f, 0.0f, -0.14f },
                           { -0.16f, 0.0f, -0.25f },
                           { -0.17f, 0.0f, -0.35f },
                           { -0.17f, 0.0f, -0.46f },
                           { -0.16f, 0.0f, -0.54f },
                           { -0.13f, 0.0f, -0.61f },
                           { -0.09f, 0.0f, -0.65f },
                           { -0.04f, 0.0f, -0.69f },
                           { -0.00f, 0.0f, -0.70f } };
#ifdef _WIN32
static float soleNorm[][3] = { {  0.00f, 1.0f, 0.00f },
                           { 0.00f, 1.0f, 0.00f },
                           { 0.00f, 1.0f, 0.00f },
                           { 0.00f, 1.0f, 0.00f },
                           { 0.00f, 1.0f, 0.00f },
                           { 0.00f, 1.0f, 0.00f },
                           { 0.00f, 1.0f, 0.00f },
                           { 0.00f, 1.0f, 0.00f },
                           { 0.00f, 1.0f, 0.00f },
                           { 0.00f, 1.0f, 0.00f },
                           { 0.00f, 1.0f, 0.00f },
                           { 0.00f, 1.0f, 0.00f },
                           { 0.00f, 1.0f, 0.00f },
                           { 0.00f, 1.0f, 0.00f },
                           { 0.00f, 1.0f, 0.00f },
                           { 0.00f, 1.0f, 0.00f },
                           { 0.00f, 1.0f, 0.00f },
                           { 0.00f, 1.0f, 0.00f },
                           { 0.00f, 1.0f, 0.00f },
                           { 0.00f, 1.0f, 0.00f },
                           { 0.00f, 1.0f, 0.00f } };
#endif
static float heel[][3] = { {  0.00f, 0.0f,  0.06f },
                           {  0.13f, 0.0f,  0.06f },
                           {  0.14f, 0.0f,  0.15f },
                           {  0.14f, 0.0f,  0.21f },
                           {  0.13f, 0.0f,  0.25f },
                           {  0.11f, 0.0f,  0.28f },
                           {  0.09f, 0.0f,  0.29f },
                           {  0.04f, 0.0f,  0.30f },
                           {  0.00f, 0.0f,  0.30f },
                           { -0.04f, 0.0f,  0.30f },
                           { -0.09f, 0.0f,  0.29f },
                           { -0.11f, 0.0f,  0.28f },
                           { -0.13f, 0.0f,  0.25f },
                           { -0.14f, 0.0f,  0.21f },
                           { -0.14f, 0.0f,  0.15f },
                           { -0.13f, 0.0f,  0.06f },
                           { -0.00f, 0.0f,  0.06f } };
#ifdef _WIN32
static float heelNorm[][3] = { {  0.00f, 1.0f,  0.00f },
                           {  0.00f, 1.0f,  0.00f },
                           {  0.00f, 1.0f,  0.00f },
                           {  0.00f, 1.0f,  0.00f },
                           {  0.00f, 1.0f,  0.00f },
                           {  0.00f, 1.0f,  0.00f },
                           {  0.00f, 1.0f,  0.00f },
                           {  0.00f, 1.0f,  0.00f },
                           {  0.00f, 1.0f,  0.00f },
                           {  0.00f, 1.0f,  0.00f },
                           {  0.00f, 1.0f,  0.00f },
                           {  0.00f, 1.0f,  0.00f },
                           {  0.00f, 1.0f,  0.00f },
                           {  0.00f, 1.0f,  0.00f },
                           {  0.00f, 1.0f,  0.00f },
                           {  0.00f, 1.0f,  0.00f },
                           {  0.00f, 1.0f,  0.00f } };
#endif
static int soleCount = 21;
static int heelCount = 17;

// Index buffer data for drawing in Dx and Core Profile modes:
static unsigned short soleWireIndices[] =
        {
            0, 1,
            1, 2,
            2, 3,
            3, 4,
            4, 5,
            5, 6,
            6, 7,
            7, 8,
            8, 9,
            9, 10,
            10, 11,
            11, 12,
            12, 13,
            13, 14,
            14, 15,
            15, 16,
            16, 17,
            17, 18,
            18, 19,
            19, 20
        };
static unsigned short heelWireIndices[] =
        {
            0, 1,
            1, 2,
            2, 3,
            3, 4,
            4, 5,
            5, 6,
            6, 7,
            7, 8,
            8, 9,
            9, 10,
            10, 11,
            11, 12,
            12, 13,
            13, 14,
            14, 15,
            15, 16
        };
static unsigned short soleShadedIndices[] =
        {
            0, 1, 2,
            0, 2, 3,
            0, 3, 4,
            0, 4, 5,
            0, 5, 6,
            0, 6, 7,
            0, 7, 8,
            0, 8, 9,
            0, 9, 10,
            0, 10, 11,
            0, 11, 12,
            0, 12, 13,
            0, 13, 14,
            0, 14, 15,
            0, 15, 16,
            0, 16, 17,
            0, 17, 18,
            0, 18, 19,
            0, 19, 20
        };
static unsigned short heelShadedIndices[] =
        {
            0, 1, 2,
            0, 2, 3,
            0, 3, 4,
            0, 4, 5,
            0, 5, 6,
            0, 6, 7,
            0, 7, 8,
            0, 8, 9,
            0, 9, 10,
            0, 10, 11,
            0, 11, 12,
            0, 12, 13,
            0, 13, 14,
            0, 14, 15,
            0, 15, 16
        };

// 8 Vertices for drawing the bounding box in DX and Core Profile mode
// lower and upper values of the sole and heel vertices +/- 0.1
static float bbData[][3] = {
        { -0.18f, 0.f, -0.71f},
        {  0.18f, 0.f, -0.71f},
        {  0.18f, 0.f,  0.31f},
        { -0.18f, 0.f,  0.31f},
        { -0.18f, 0.f, -0.71f},
        {  0.18f, 0.f, -0.71f},
        {  0.18f, 0.f,  0.31f},
        { -0.18f, 0.f,  0.31f}};

// Index buffer for Wireframe drawing in Dx and Core Profile
static unsigned short bbWireIndices[] =
        {
            0,1,
            1,2,
            2,3,
            3,0,
            4,5,
            5,6,
            6,7,
            7,4,
            0,4,
            1,5,
            2,6,
            3,7
        };

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Node implementation with standard viewport draw
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

class rawfootPrint : public MPxLocatorNode
{
public:
    rawfootPrint();
    ~rawfootPrint() override;

    MStatus   		compute( const MPlug& plug, MDataBlock& data ) override;

    bool            isBounded() const override;
    MBoundingBox    boundingBox() const override;

    MStatus preEvaluation(const MDGContext& context, const MEvaluationNode& evaluationNode) override;
    void    getCacheSetup(const MEvaluationNode& evalNode, MNodeCacheDisablingInfo& disablingInfo, MNodeCacheSetupInfo& cacheSetupInfo, MObjectArray& monitoredAttributes) const override;

    static  void *          creator();
    static  MStatus         initialize();

    static  MObject         size;               // The size of the foot
    static  MObject         transparencySort;   // The transparent status of the foot
    static  MObject         transparency;       // The transparent value of the foot
    
public:
    static	MTypeId		id;
    static	MString		drawDbClassification;
    static	MString		drawRegistrantId;
};

MObject rawfootPrint::size;
MObject rawfootPrint::transparencySort;
MObject rawfootPrint::transparency;
MTypeId rawfootPrint::id( 0x0008002D );
MString	rawfootPrint::drawDbClassification("drawdb/geometry/rawfootPrint");
MString	rawfootPrint::drawRegistrantId("RawFootprintNodePlugin");

rawfootPrint::rawfootPrint() {}
rawfootPrint::~rawfootPrint() {}

MStatus rawfootPrint::compute( const MPlug& /*plug*/, MDataBlock& /*data*/ )
{
    return MS::kUnknownParameter;
}

bool rawfootPrint::isBounded() const
{
    return true;
}

MBoundingBox rawfootPrint::boundingBox() const
{
    // Get the size
    //
    MObject thisNode = thisMObject();
    MPlug plug( thisNode, size );
    MDistance sizeVal;
    plug.getValue( sizeVal );

    double multiplier = sizeVal.asCentimeters();

    MPoint corner1( -0.17, 0.0, -0.7 );
    MPoint corner2( 0.17, 0.0, 0.3 );

    corner1 = corner1 * multiplier;
    corner2 = corner2 * multiplier;

    return MBoundingBox( corner1, corner2 );
}

// Called before this node is evaluated by Evaluation Manager
MStatus rawfootPrint::preEvaluation(
    const MDGContext& context,
    const MEvaluationNode& evaluationNode)
{
    if (context.isNormal())
    {
        // The size attribute is set to affect the localScale attribute during
        // initialization, thus no need to be checked here to trigger geometry
        // change.
        MStatus status;
        if ((evaluationNode.dirtyPlugExists(transparencySort, &status) && status) ||
            (evaluationNode.dirtyPlugExists(transparency, &status) && status))
        {
            MHWRender::MRenderer::setGeometryDrawDirty(thisMObject());
        }
    }

    return MStatus::kSuccess;
}

void rawfootPrint::getCacheSetup(const MEvaluationNode& evalNode, MNodeCacheDisablingInfo& disablingInfo, MNodeCacheSetupInfo& cacheSetupInfo, MObjectArray& monitoredAttributes) const
{
    MPxLocatorNode::getCacheSetup(evalNode, disablingInfo, cacheSetupInfo, monitoredAttributes);
    assert(!disablingInfo.getCacheDisabled());
    cacheSetupInfo.setPreference(MNodeCacheSetupInfo::kWantToCacheByDefault, true);
}

void* rawfootPrint::creator()
{
    return new rawfootPrint();
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Viewport 2.0 override implementation
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

class RawFootPrintData : public MUserData
{
public:
    ~RawFootPrintData() override
    { 
        mBlendState = nullptr; 
    }

    float                         fMultiplier;
    float                         fColor[4];
    bool                          fCustomBoxDraw;
    MBoundingBox                  fCurrentBoundingBox;
    MDAGDrawOverrideInfo          fDrawOV;
    const MHWRender::MBlendState* mBlendState{nullptr};
    MUint64                       mLastFrameStamp{0};
};

// Helper class declaration for the object drawing
class RawFootPrintDrawAgent
{
public:
    RawFootPrintDrawAgent() : mDrawContext(NULL), mShaderOverride(NULL) {}
    virtual ~RawFootPrintDrawAgent(){}

    virtual void drawShaded() = 0;
    virtual void drawBoundingBox() = 0;
    virtual void drawWireframe() = 0;

    virtual void setMatrix(
        const MHWRender::MDrawContext& context,
        const MHWRender::MSelectionInfo* selectInfo) = 0;

    virtual void beginDraw( const MHWRender::MDrawContext& context, MHWRender::MShaderInstance* passShaderOverride ) {
        mDrawContext = &context;
        mShaderOverride = passShaderOverride;

        // Sample code to debug geometry streams required for the override
        // shader.
        const bool debugShader = false;
        MHWRender::MVertexBufferDescriptorList bufferList;
        if (mShaderOverride) 
        {
            if (debugShader)
            {
                mShaderOverride->requiredVertexBuffers(bufferList);
                for (int i=0; i<bufferList.length(); i++)
                {
                    MHWRender::MVertexBufferDescriptor desc;
                    bufferList.getDescriptor( i, desc );
                    printf("Buffer[%d][name=%s]\n", i,
                        desc.name().asChar());
                    printf("[semantic = %s\n", desc.semanticName().asChar());
                    printf("[dataType = %d\n", desc.dataType());
                    printf("[dataTyleSize = %d\n", desc.dataTypeSize());
                    printf("[dimension = %d\n", desc.dimension());
                    printf("[offset = %d\n", desc.offset());
                    printf("[stride = %d\n", desc.stride());
                }
            }			

            mShaderOverride->bind( *mDrawContext );
            mShaderOverride->updateParameters( *mDrawContext );
            mShaderOverride->activatePass( *mDrawContext, 0 );
        }
    }

    virtual void endDraw() {
        if (mShaderOverride) {
            mShaderOverride->unbind( *mDrawContext );
        }

        mDrawContext = NULL;
        mShaderOverride = NULL;
    }

    void setColor( const MColor& color){
        mColor = color;
    }

protected:
    const MHWRender::MDrawContext* mDrawContext;
    MHWRender::MShaderInstance* mShaderOverride;
    MColor  mColor;
};

// GL draw agent declaration
class RawFootPrintDrawAgentGL : public RawFootPrintDrawAgent
{
public:
    static RawFootPrintDrawAgentGL& getDrawAgent(){
        static RawFootPrintDrawAgentGL obj;
        return obj;
    }

    void drawShaded() override;
    void drawBoundingBox() override;
    void drawWireframe() override;
    void beginDraw( const MHWRender::MDrawContext& context, MHWRender::MShaderInstance* passShaderOverride ) override;
    void endDraw() override;

    void setMatrix(
        const MHWRender::MDrawContext& context,
        const MHWRender::MSelectionInfo* selectInfo) override
    {
        // Get  world view matrix
        MStatus status;
        mWorldViewMatrix =
            context.getMatrix(MHWRender::MFrameContext::kWorldViewMtx, &status);
        if (status != MStatus::kSuccess) return;

        // Get projection matrix
        mProjectionMatrix = context.getMatrix(MHWRender::MFrameContext::kProjectionMtx, &status);
        if (status != MStatus::kSuccess) return;

        // Compute a pick matrix that, when it is post-multiplied with the
        // projection matrix, will cause the picking region to fill the entire
        // viewport for OpenGL selection mode.
        if (selectInfo)
        {
            int view_x, view_y, view_w, view_h;
            context.getViewportDimensions(view_x, view_y, view_w, view_h);

            unsigned int sel_x, sel_y, sel_w, sel_h;
            selectInfo->selectRect(sel_x, sel_y, sel_w, sel_h);

            double center_x = sel_x + sel_w * 0.5;
            double center_y = sel_y + sel_h * 0.5;

            MMatrix pickMatrix;
            pickMatrix[0][0] = view_w / double(sel_w);
            pickMatrix[1][1] = view_h / double(sel_h);
            pickMatrix[3][0] = (view_w - 2.0 * (center_x - view_x)) / double(sel_w);
            pickMatrix[3][1] = (view_h - 2.0 * (center_y - view_y)) / double(sel_h);

            mProjectionMatrix *= pickMatrix;
        }
    }

private:
    RawFootPrintDrawAgentGL(){}
    ~RawFootPrintDrawAgentGL() override{}
    RawFootPrintDrawAgentGL( const RawFootPrintDrawAgentGL& v ){}
    RawFootPrintDrawAgentGL operator = (const RawFootPrintDrawAgentGL& v){ return *this; }

    MMatrix mWorldViewMatrix;
    MMatrix mProjectionMatrix;
};

// GL Core Profile functions:
class GLCP
{
public:
    // For more extensive OpenGL coding you will definitively want to
    // use a good extension loader framework. I recommend looking at:
    // https://www.opengl.org/wiki/OpenGL_Loading_Library 
    // to find the one that will fit your needs the best.
    // Also note that MGLFunctionTable stops at GL 2.1, so a lot
    // of functions will be missing.

#if !defined (__APPLE__)
    typedef char GLchar;
    typedef ptrdiff_t GLsizeiptr;
#endif

    typedef void (APIENTRY *Tp_glBindVertexArray) (GLuint array);
    static Tp_glBindVertexArray BindVertexArray;

    typedef void (APIENTRY *Tp_glGenVertexArrays) (GLsizei n, GLuint *arrays);
    static Tp_glGenVertexArrays GenVertexArrays;

    typedef void (APIENTRY *Tp_glDeleteVertexArrays ) (GLsizei n, const GLuint *arrays);
    static Tp_glDeleteVertexArrays DeleteVertexArrays;

    typedef void (APIENTRY *Tp_glUniform4f) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
    static Tp_glUniform4f Uniform4f;

    typedef void (APIENTRY *Tp_glUniformMatrix4fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
    static Tp_glUniformMatrix4fv UniformMatrix4fv;

    typedef GLint (APIENTRY *Tp_glGetUniformLocation) (GLuint program, const GLchar *name);
    static Tp_glGetUniformLocation GetUniformLocation;

    typedef GLint (APIENTRY *Tp_glGetAttribLocation) (GLuint program, const GLchar *name);
    static Tp_glGetAttribLocation GetAttribLocation;

    typedef GLuint (APIENTRY *Tp_glCreateShader ) (GLenum type);
    static Tp_glCreateShader CreateShader;

    typedef void (APIENTRY *Tp_glShaderSource ) (GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length);
    static Tp_glShaderSource ShaderSource;

    typedef void (APIENTRY *Tp_glCompileShader ) (GLuint shader);
    static Tp_glCompileShader CompileShader;

    typedef void (APIENTRY *Tp_glGetShaderiv ) (GLuint shader, GLenum pname, GLint *params);
    static Tp_glGetShaderiv GetShaderiv;

    typedef void (APIENTRY *Tp_glGetShaderInfoLog ) (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
    static Tp_glGetShaderInfoLog GetShaderInfoLog;

    typedef GLuint (APIENTRY *Tp_glCreateProgram ) ();
    static Tp_glCreateProgram CreateProgram;

    typedef void (APIENTRY *Tp_glAttachShader ) (GLuint program, GLuint shader);
    static Tp_glAttachShader AttachShader;

    typedef void (APIENTRY *Tp_glDeleteShader ) (GLuint shader);
    static Tp_glDeleteShader DeleteShader;

    typedef void (APIENTRY *Tp_glLinkProgram ) (GLuint program);
    static Tp_glLinkProgram LinkProgram;

    typedef void (APIENTRY *Tp_glGetProgramiv ) (GLuint program, GLenum pname, GLint *params);
    static Tp_glGetProgramiv GetProgramiv;

    typedef void (APIENTRY *Tp_glUseProgram ) (GLuint program);
    static Tp_glUseProgram UseProgram;

    typedef void (APIENTRY *Tp_glDeleteProgram ) (GLuint program);
    static Tp_glDeleteProgram DeleteProgram;

    typedef void (APIENTRY *Tp_glGetProgramInfoLog ) (GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
    static Tp_glGetProgramInfoLog GetProgramInfoLog;

    typedef void (APIENTRY *Tp_glGenBuffers ) (GLsizei n, GLuint *buffers);
    static Tp_glGenBuffers GenBuffers;

    typedef void (APIENTRY *Tp_glDeleteBuffers ) (GLsizei n, const GLuint *buffers);
    static Tp_glDeleteBuffers DeleteBuffers;

    typedef void (APIENTRY *Tp_glBindBuffer ) (GLenum target, GLuint buffer);
    static Tp_glBindBuffer BindBuffer;

    typedef void (APIENTRY *Tp_glBufferData ) (GLenum target, GLsizeiptr size, const void *data, GLenum usage);
    static Tp_glBufferData BufferData;

    typedef void (APIENTRY *Tp_glEnableVertexAttribArray ) (GLuint index);
    static Tp_glEnableVertexAttribArray EnableVertexAttribArray;
    
    typedef void (APIENTRY *Tp_glVertexAttribPointer ) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
    static Tp_glVertexAttribPointer VertexAttribPointer;

    static void initGLFunctionsCoreProfile();
};

GLCP::Tp_glBindVertexArray GLCP::BindVertexArray = 0;
GLCP::Tp_glGenVertexArrays GLCP::GenVertexArrays = 0;
GLCP::Tp_glDeleteVertexArrays GLCP::DeleteVertexArrays = 0;
GLCP::Tp_glUniform4f GLCP::Uniform4f = 0;
GLCP::Tp_glUniformMatrix4fv GLCP::UniformMatrix4fv = 0;
GLCP::Tp_glGetUniformLocation GLCP::GetUniformLocation = 0;
GLCP::Tp_glGetAttribLocation GLCP::GetAttribLocation = 0;
GLCP::Tp_glCreateShader GLCP::CreateShader = 0;
GLCP::Tp_glShaderSource GLCP::ShaderSource = 0;
GLCP::Tp_glCompileShader GLCP::CompileShader = 0;
GLCP::Tp_glGetShaderiv GLCP::GetShaderiv = 0;
GLCP::Tp_glGetShaderInfoLog GLCP::GetShaderInfoLog = 0;
GLCP::Tp_glCreateProgram GLCP::CreateProgram = 0;
GLCP::Tp_glAttachShader GLCP::AttachShader = 0;
GLCP::Tp_glDeleteShader GLCP::DeleteShader = 0;
GLCP::Tp_glLinkProgram GLCP::LinkProgram = 0;
GLCP::Tp_glGetProgramiv GLCP::GetProgramiv = 0;
GLCP::Tp_glUseProgram GLCP::UseProgram = 0;
GLCP::Tp_glDeleteProgram GLCP::DeleteProgram = 0;
GLCP::Tp_glGetProgramInfoLog GLCP::GetProgramInfoLog = 0;
GLCP::Tp_glGenBuffers GLCP::GenBuffers = 0;
GLCP::Tp_glDeleteBuffers GLCP::DeleteBuffers = 0;
GLCP::Tp_glBindBuffer GLCP::BindBuffer = 0;
GLCP::Tp_glBufferData GLCP::BufferData = 0;
GLCP::Tp_glEnableVertexAttribArray GLCP::EnableVertexAttribArray = 0;
GLCP::Tp_glVertexAttribPointer GLCP::VertexAttribPointer = 0;

void GLCP::initGLFunctionsCoreProfile()
{
#if defined (__APPLE__)
    #define INIT_GLPOINTER(funcName) funcName = gl##funcName
#elif defined (_WIN32)
    #define INIT_GLPOINTER(funcName) if (!funcName) funcName = (Tp_gl##funcName)wglGetProcAddress("gl" #funcName)
#else
    #define INIT_GLPOINTER(funcName) if (!funcName) funcName = (Tp_gl##funcName)::glXGetProcAddress((const GLubyte*)"gl" #funcName)
#endif

    INIT_GLPOINTER(BindVertexArray);
    INIT_GLPOINTER(GenVertexArrays);
    INIT_GLPOINTER(DeleteVertexArrays);
    INIT_GLPOINTER(Uniform4f);
    INIT_GLPOINTER(UniformMatrix4fv);
    INIT_GLPOINTER(GetUniformLocation);
    INIT_GLPOINTER(GetAttribLocation);
    INIT_GLPOINTER(CreateShader);
    INIT_GLPOINTER(ShaderSource);
    INIT_GLPOINTER(CompileShader);
    INIT_GLPOINTER(GetShaderiv);
    INIT_GLPOINTER(GetShaderInfoLog);
    INIT_GLPOINTER(CreateProgram);
    INIT_GLPOINTER(AttachShader);
    INIT_GLPOINTER(DeleteShader);
    INIT_GLPOINTER(LinkProgram);
    INIT_GLPOINTER(GetProgramiv);
    INIT_GLPOINTER(UseProgram);
    INIT_GLPOINTER(DeleteProgram);
    INIT_GLPOINTER(GetProgramInfoLog);
    INIT_GLPOINTER(GenBuffers);
    INIT_GLPOINTER(DeleteBuffers);
    INIT_GLPOINTER(BindBuffer);
    INIT_GLPOINTER(BufferData);
    INIT_GLPOINTER(EnableVertexAttribArray);
    INIT_GLPOINTER(VertexAttribPointer);

#undef INIT_GLPOINTER
}

#if !defined (__APPLE__)
#define GL_FRAGMENT_SHADER                0x8B30
#define GL_VERTEX_SHADER                  0x8B31
#define GL_ARRAY_BUFFER                   0x8892
#define GL_ELEMENT_ARRAY_BUFFER           0x8893
#define GL_STATIC_DRAW                    0x88E4
#define GL_COMPILE_STATUS                 0x8B81
#define GL_LINK_STATUS                    0x8B82
#define GL_INFO_LOG_LENGTH                0x8B84
#endif

// Core Profile draw agent declaration
class RawFootPrintDrawAgentCoreProfile : public RawFootPrintDrawAgent
{
public:
    static RawFootPrintDrawAgentCoreProfile& getDrawAgent(){
        static RawFootPrintDrawAgentCoreProfile obj;
        return obj;
    }

    void drawShaded() override;
    void drawBoundingBox() override;
    void drawWireframe() override;
    void beginDraw( const MHWRender::MDrawContext& context, MHWRender::MShaderInstance* passShaderOverride ) override;
    void endDraw() override;

    void setMatrix(
        const MHWRender::MDrawContext& context,
        const MHWRender::MSelectionInfo* selectInfo) override
    {
        // Get world view projection matrix
        MStatus status;
        mWorldViewProjMatrix =
            context.getMatrix(MHWRender::MFrameContext::kWorldViewProjMtx, &status);
    }

    bool releaseCoreProfileResources();
private:
    GLuint mShaderProgram;
    GLint mWVPIndex;
    GLint mColorIndex;
    GLint mVtxAttrib;

    GLuint mBBoxVAO;
    GLuint mSoleWireframeVAO;
    GLuint mHeelWireframeVAO;
    GLuint mSoleShadedVAO;
    GLuint mHeelShadedVAO;

    GLuint mBoundingboxVertexBuffer;
    GLuint mBoundingboxIndexBuffer;
    GLuint mSoleVertexBuffer;
    GLuint mSoleNormalBuffer;
    GLuint mHeelVertexBuffer;
    GLuint mHeelNormalBuffer;
    GLuint mSoleWireIndexBuffer;
    GLuint mSoleShadedIndexBuffer;
    GLuint mHeelWireIndexBuffer;
    GLuint mHeelShadedIndexBuffer;

    MMatrix mWorldViewProjMatrix;
    bool mInitialized;
    bool mValid;
    bool initShadersCoreProfile();
    bool initBuffersCoreProfile();
    void setupUniforms();
private:
    RawFootPrintDrawAgentCoreProfile()
        // Shader program data
        : mShaderProgram(0)
        , mWVPIndex(0)
        , mColorIndex(0)
        , mVtxAttrib(0)
        // VAOs
        , mBBoxVAO(0)
        , mSoleWireframeVAO(0)
        , mHeelWireframeVAO(0)
        , mSoleShadedVAO(0)
        , mHeelShadedVAO(0)
        // Draw buffers
        , mBoundingboxVertexBuffer(0)
        , mBoundingboxIndexBuffer(0)
        , mSoleVertexBuffer(0)
        , mSoleNormalBuffer(0)
        , mHeelVertexBuffer(0)
        , mHeelNormalBuffer(0)
        , mSoleWireIndexBuffer(0)
        , mSoleShadedIndexBuffer(0)
        , mHeelWireIndexBuffer(0)
        , mHeelShadedIndexBuffer(0)
        , mInitialized(false)
        , mValid(false)
        {}
    ~RawFootPrintDrawAgentCoreProfile() override{}
    RawFootPrintDrawAgentCoreProfile( const RawFootPrintDrawAgentCoreProfile& v ){}
    RawFootPrintDrawAgentCoreProfile operator = (const RawFootPrintDrawAgentCoreProfile& v){ return *this; }
};

// DX stuff
#ifdef _WIN32

// DX draw agent declaration
class RawFootPrintDrawAgentDX : public RawFootPrintDrawAgent
{
public:
    static RawFootPrintDrawAgentDX& getDrawAgent(){
        static RawFootPrintDrawAgentDX obj;
        return obj;
    }

    virtual void drawShaded();
    virtual void drawBoundingBox();
    virtual void drawWireframe();
    virtual void beginDraw( const MHWRender::MDrawContext& context, MHWRender::MShaderInstance* passShaderOverride );
    virtual void endDraw();

    void setMatrix(
        const MHWRender::MDrawContext& context,
        const MHWRender::MSelectionInfo* selectInfo) override
    {
        // Get world view projection matrix
        MStatus status;
        mWorldViewProjMatrix =
            context.getMatrix(MHWRender::MFrameContext::kWorldViewProjMtx, &status);
    }

    bool releaseDXResources();
private:
    ID3D11Device* mDevicePtr;
    ID3D11DeviceContext* mDeviceContextPtr;

    ID3D11Buffer* mBoundingboxVertexBufferPtr;
    ID3D11Buffer* mBoundingboxIndexBufferPtr;
    ID3D11Buffer* mSoleVertexBufferPtr[2];
    ID3D11Buffer* mHeelVertexBufferPtr[2];
    ID3D11Buffer* mSoleWireIndexBufferPtr;
    ID3D11Buffer* mSoleShadedIndexBufferPtr;
    ID3D11Buffer* mHeelWireIndexBufferPtr;
    ID3D11Buffer* mHeelShadedIndexBufferPtr;
    ID3D11Buffer* mConstantBufferPtr;
    ID3D11VertexShader* mVertexShaderPtr;
    ID3D11PixelShader* mPixelShaderPtr;
    ID3D11InputLayout* mVertexLayoutPtr;

    MString mEffectLocation;
    bool mEffectLoad;
    unsigned int mStride[2];
    unsigned int mOffset[2];
    MMatrix mWorldViewProjMatrix;
    struct ConstantBufferDef
    {
        XMMATRIX fWVP;
        XMFLOAT4 fMatColor;
    };
    bool initShadersDX();
    bool initBuffersDX();
    void setupConstantBuffer();
private:
    RawFootPrintDrawAgentDX():
        mDevicePtr(NULL), mDeviceContextPtr(NULL), mEffectLocation(""),
        mEffectLoad(false),
        mBoundingboxVertexBufferPtr(NULL),
        mBoundingboxIndexBufferPtr(NULL),
        mSoleWireIndexBufferPtr(NULL),
        mSoleShadedIndexBufferPtr(NULL),
        mHeelWireIndexBufferPtr(NULL),
        mHeelShadedIndexBufferPtr(NULL),
        mConstantBufferPtr(NULL),
        mVertexShaderPtr(NULL),
        mPixelShaderPtr(NULL),
        mVertexLayoutPtr(NULL)
    {
        mSoleVertexBufferPtr[0] = NULL;
        mSoleVertexBufferPtr[1] = NULL;
        mHeelVertexBufferPtr[0] = NULL;
        mHeelVertexBufferPtr[1] = NULL;

        mStride[0] = sizeof(float) * 3;
        mStride[1] = sizeof(float) * 3;

        mOffset[0] = 0;
        mOffset[1] = 0;
    }
    ~RawFootPrintDrawAgentDX(){}
    RawFootPrintDrawAgentDX( const RawFootPrintDrawAgentDX& v ){}
    RawFootPrintDrawAgentDX operator = (const RawFootPrintDrawAgentDX& v){ return *this; }
};

#endif // _WIN32

// GL draw agent definition
//
void RawFootPrintDrawAgentGL::beginDraw( const MHWRender::MDrawContext& context, MHWRender::MShaderInstance* passShaderOverride )
{
    RawFootPrintDrawAgent::beginDraw(context, passShaderOverride);

    if (!mShaderOverride)
    {
        // set world view matrix
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadMatrixd(mWorldViewMatrix.matrix[0]);

        // set projection matrix
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadMatrixd(mProjectionMatrix.matrix[0]);

        glPushAttrib( GL_CURRENT_BIT );
    }
}

void RawFootPrintDrawAgentGL::endDraw()
{
    if (!mShaderOverride)
    {
        glPopAttrib();
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
    }
    RawFootPrintDrawAgent::endDraw();
}

void RawFootPrintDrawAgentGL::drawShaded()
{
    // set color
    glColor4fv( &(mColor.r) );

    glBegin( GL_TRIANGLE_FAN );
    int i;
    int last = soleCount - 1;
    for ( i = 0; i < last; ++i ) {
        glNormal3f( 0.0f, 1.0f, 0.0f );
        glVertex3f( sole[i][0],
            sole[i][1],
            sole[i][2]);
    }
    glEnd();
    glBegin( GL_TRIANGLE_FAN );
    last = heelCount - 1;
    for ( i = 0; i < last; ++i ) {
        glNormal3f( 0.0f, 1.0f, 0.0f );
        glVertex3f( heel[i][0],
            heel[i][1],
            heel[i][2]);
    }
    glEnd();
}

void RawFootPrintDrawAgentGL::drawBoundingBox()
{
    // set color
    glColor4fv( &(mColor.r) );

    const float* bottomLeftFront = bbData[0];
    const float* topLeftFront = bbData[4];
    const float* bottomRightFront = bbData[1];
    const float* topRightFront = bbData[5];
    const float* bottomLeftBack = bbData[3];
    const float* topLeftBack = bbData[7];
    const float* bottomRightBack = bbData[2];
    const float* topRightBack = bbData[6];

    glBegin( GL_LINES );

    // 4 Bottom lines
    //
    glVertex3fv( bottomLeftFront );
    glVertex3fv( bottomRightFront );
    glVertex3fv( bottomRightFront );
    glVertex3fv( bottomRightBack );
    glVertex3fv( bottomRightBack );
    glVertex3fv( bottomLeftBack );
    glVertex3fv( bottomLeftBack );
    glVertex3fv( bottomLeftFront );

    // 4 Top lines
    //
    glVertex3fv( topLeftFront );
    glVertex3fv( topRightFront );
    glVertex3fv( topRightFront );
    glVertex3fv( topRightBack );
    glVertex3fv( topRightBack );
    glVertex3fv( topLeftBack );
    glVertex3fv( topLeftBack );
    glVertex3fv( topLeftFront );

    // 4 Side lines
    //
    glVertex3fv( bottomLeftFront );
    glVertex3fv( topLeftFront );
    glVertex3fv( bottomRightFront );
    glVertex3fv( topRightFront );
    glVertex3fv( bottomRightBack );
    glVertex3fv( topRightBack );
    glVertex3fv( bottomLeftBack );
    glVertex3fv( topLeftBack );

    glEnd();
}

void RawFootPrintDrawAgentGL::drawWireframe()
{
    // set color
    glColor4fv( &(mColor.r) );

    // draw wire
    glBegin( GL_LINES );
    int i;
    int last = soleCount - 1;
    for ( i = 0; i < last; ++i ) {
        glVertex3f( sole[i][0],
            sole[i][1],
            sole[i][2]);
        glVertex3f( sole[i+1][0],
            sole[i+1][1],
            sole[i+1][2]);
    }
    last = heelCount - 1;
    for ( i = 0; i < last; ++i ) {
        glVertex3f( heel[i][0],
            heel[i][1],
            heel[i][2]);
        glVertex3f( heel[i+1][0],
            heel[i+1][1],
            heel[i+1][2]);
    }
    glEnd();
}

// Core Profile draw agent definition
//
void RawFootPrintDrawAgentCoreProfile::beginDraw( const MHWRender::MDrawContext& context, MHWRender::MShaderInstance* passShaderOverride )
{
    // Init common data:
    if ( !mInitialized ){
        // Set up all the one-time data:
        GLCP::initGLFunctionsCoreProfile();
        mValid = initShadersCoreProfile() && initBuffersCoreProfile();
        mInitialized = true;
    }

    if (!mValid)
        return;

    RawFootPrintDrawAgent::beginDraw(context, passShaderOverride);
    if (!mShaderOverride) {
        GLCP::UseProgram(mShaderProgram);
    }
}

void RawFootPrintDrawAgentCoreProfile::endDraw()
{
    if (!mValid)
        return;

    if (!mShaderOverride) {
        GLCP::UseProgram(0);
    }
    GLCP::BindVertexArray(0);

    RawFootPrintDrawAgent::endDraw();
}

void RawFootPrintDrawAgentCoreProfile::setupUniforms()
{
    if( mShaderOverride == NULL )
    {
        float fMatrix[4][4];
        for (size_t i = 0; i < 4; ++i)
            for (size_t j = 0; j < 4; ++j)
                fMatrix[i][j] = (float)mWorldViewProjMatrix.matrix[i][j];

        GLCP::UniformMatrix4fv(mWVPIndex, 1, GL_FALSE, (float*)fMatrix);

        GLCP::Uniform4f(mColorIndex, mColor.r, mColor.g, mColor.b, mColor.a);
    }
}

void RawFootPrintDrawAgentCoreProfile::drawShaded()
{
    if (!mValid)
        return;

    // Set uniforms
    setupUniforms();

    GLCP::BindVertexArray(mSoleShadedVAO);
    glDrawElements(GL_TRIANGLES, 3 * (soleCount-2), GL_UNSIGNED_SHORT, 0);
    GLCP::BindVertexArray(mHeelShadedVAO);
    glDrawElements(GL_TRIANGLES, 3 * (heelCount-2), GL_UNSIGNED_SHORT, 0);
}

void RawFootPrintDrawAgentCoreProfile::drawBoundingBox()
{
    if (!mValid)
        return;

    // Set uniforms
    setupUniforms();

    GLCP::BindVertexArray(mBBoxVAO);
    glDrawElements(GL_LINES, 2 * 12, GL_UNSIGNED_SHORT, 0);
}

void RawFootPrintDrawAgentCoreProfile::drawWireframe()
{
    if (!mValid)
        return;

    // Set uniforms
    setupUniforms();

    GLCP::BindVertexArray(mSoleWireframeVAO);
    glDrawElements(GL_LINES, 2 * (soleCount-1), GL_UNSIGNED_SHORT, 0);
    GLCP::BindVertexArray(mHeelWireframeVAO);
    glDrawElements(GL_LINES, 2 * (heelCount-1), GL_UNSIGNED_SHORT, 0);
}

// DX stuff
#ifdef _WIN32
// DX draw agent definition
//
void RawFootPrintDrawAgentDX::beginDraw( const MHWRender::MDrawContext& context, MHWRender::MShaderInstance* passShaderOverride )
{
    if (!mEffectLoad)
    {
        // Please move file "rawfootprint.hlsl" to following location, or 
        // change following location as your local path.
        mEffectLocation = MString("$MAYA_PLUG_IN_PATH/rawfootprint.hlsl");

        MFileObject fileObj;
        fileObj.setRawFullName(mEffectLocation);
        fileObj.setResolveMethod(MFileObject::kInputFile);
        assert(fileObj.exists());
        if (!fileObj.exists())
        {
            MString msg = MString("Can not find file:") + mEffectLocation;
            MGlobal::displayWarning(msg);
            mEffectLoad = false;
            return;
        }
        mEffectLocation = fileObj.resolvedFullName();
        mEffectLoad = true;
    }

    // Initial device
    if( !mDevicePtr || !mDeviceContextPtr ){
        // get renderer
        MHWRender::MRenderer* theRenderer = MHWRender::MRenderer::theRenderer();
        if ( theRenderer ){
            mDevicePtr = (ID3D11Device*)theRenderer->GPUDeviceHandle();
            if ( mDevicePtr ){
                mDevicePtr->GetImmediateContext( &mDeviceContextPtr );
            }
        }
    }
    assert( mDevicePtr );
    assert( mDeviceContextPtr );

    if ( mDevicePtr && mDeviceContextPtr ){

        RawFootPrintDrawAgent::beginDraw(context, passShaderOverride);
        // Init shaders
        if ( initShadersDX() && passShaderOverride == NULL ){
            // Set up shader
            mDeviceContextPtr->VSSetShader(mVertexShaderPtr, NULL, 0);
            mDeviceContextPtr->PSSetShader(mPixelShaderPtr, NULL, 0);
        }

        // Init buffers
        initBuffersDX();
    }
}

void RawFootPrintDrawAgentDX::endDraw()
{
    RawFootPrintDrawAgent::endDraw();
}

void RawFootPrintDrawAgentDX::setupConstantBuffer()
{
    assert( mDeviceContextPtr );
    if ( !mDeviceContextPtr )
        return;

    if( mShaderOverride == NULL )
    {
        // Compute matrix
        XMMATRIX dxTransform = XMMATRIX(
            (float)mWorldViewProjMatrix.matrix[0][0], (float)mWorldViewProjMatrix.matrix[0][1], (float)mWorldViewProjMatrix.matrix[0][2], (float)mWorldViewProjMatrix.matrix[0][3],
            (float)mWorldViewProjMatrix.matrix[1][0], (float)mWorldViewProjMatrix.matrix[1][1], (float)mWorldViewProjMatrix.matrix[1][2], (float)mWorldViewProjMatrix.matrix[1][3],
            (float)mWorldViewProjMatrix.matrix[2][0], (float)mWorldViewProjMatrix.matrix[2][1], (float)mWorldViewProjMatrix.matrix[2][2], (float)mWorldViewProjMatrix.matrix[2][3],
            (float)mWorldViewProjMatrix.matrix[3][0], (float)mWorldViewProjMatrix.matrix[3][1], (float)mWorldViewProjMatrix.matrix[3][2], (float)mWorldViewProjMatrix.matrix[3][3]);

        // Set constant buffer
        ConstantBufferDef cb;
        cb.fWVP = XMMatrixTranspose(dxTransform);
        cb.fMatColor = XMFLOAT4( mColor.r, mColor.g, mColor.b, mColor.a);

        mDeviceContextPtr->UpdateSubresource(mConstantBufferPtr, 0, NULL, &cb, 0, 0);
        mDeviceContextPtr->VSSetConstantBuffers(0, 1, &mConstantBufferPtr);
        mDeviceContextPtr->PSSetConstantBuffers(0, 1, &mConstantBufferPtr);
    }
}

void RawFootPrintDrawAgentDX::drawShaded()
{
    assert( mDeviceContextPtr );
    if ( !(mDeviceContextPtr && mEffectLoad) )
        return;

    // Set constant buffer
    setupConstantBuffer();

    // Draw the sole. Position + normals
    mDeviceContextPtr->IASetVertexBuffers(0, 2, &mSoleVertexBufferPtr[0], &mStride[0], &mOffset[0]);
    mDeviceContextPtr->IASetIndexBuffer(mSoleShadedIndexBufferPtr, DXGI_FORMAT_R16_UINT, 0);
    mDeviceContextPtr->IASetInputLayout(mVertexLayoutPtr);
    mDeviceContextPtr->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    mDeviceContextPtr->DrawIndexed(3 * (soleCount-2), 0, 0);
    // Draw the heel. Position + normals
    mDeviceContextPtr->IASetVertexBuffers(0, 2, &mHeelVertexBufferPtr[0], &mStride[0], &mOffset[0]);
    mDeviceContextPtr->IASetIndexBuffer(mHeelShadedIndexBufferPtr, DXGI_FORMAT_R16_UINT, 0);
    mDeviceContextPtr->IASetInputLayout(mVertexLayoutPtr);
    mDeviceContextPtr->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    mDeviceContextPtr->DrawIndexed(3 * (heelCount-2), 0, 0);
}

void RawFootPrintDrawAgentDX::drawBoundingBox()
{
    assert( mDeviceContextPtr );
    if ( !(mDeviceContextPtr && mEffectLoad) )
        return;

    // Set constant buffer
    setupConstantBuffer();

    mDeviceContextPtr->IASetVertexBuffers(0, 1, &mBoundingboxVertexBufferPtr, &mStride[0], &mOffset[0]);
    mDeviceContextPtr->IASetIndexBuffer(mBoundingboxIndexBufferPtr, DXGI_FORMAT_R16_UINT, 0);
    mDeviceContextPtr->IASetInputLayout(mVertexLayoutPtr);
    mDeviceContextPtr->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
    mDeviceContextPtr->DrawIndexed(2 * 12, 0, 0);
}

void RawFootPrintDrawAgentDX::drawWireframe()
{
    assert( mDeviceContextPtr );
    if ( !(mDeviceContextPtr && mEffectLoad) )
        return;

    // Set constant buffer
    setupConstantBuffer();

    // Draw the sole. Only use position buffer
    mDeviceContextPtr->IASetVertexBuffers(0, 1, &mSoleVertexBufferPtr[0], &mStride[0], &mOffset[0]);
    mDeviceContextPtr->IASetIndexBuffer(mSoleWireIndexBufferPtr, DXGI_FORMAT_R16_UINT, 0);
    mDeviceContextPtr->IASetInputLayout(mVertexLayoutPtr);
    mDeviceContextPtr->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
    mDeviceContextPtr->DrawIndexed(2 * (soleCount-1), 0, 0);
    // Draw the heel. Only use position buffer
    mDeviceContextPtr->IASetVertexBuffers(0, 1, &mHeelVertexBufferPtr[0], &mStride[0], &mOffset[0]);
    mDeviceContextPtr->IASetIndexBuffer(mHeelWireIndexBufferPtr, DXGI_FORMAT_R16_UINT, 0);
    mDeviceContextPtr->IASetInputLayout(mVertexLayoutPtr);
    mDeviceContextPtr->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
    mDeviceContextPtr->DrawIndexed(2 * (heelCount-1), 0, 0);
}

#endif // _WIN32


class RawFootPrintDrawOverride : public MHWRender::MPxDrawOverride
{
public:
    static MHWRender::MPxDrawOverride* Creator(const MObject& obj)
    {
        return new RawFootPrintDrawOverride(obj);
    }

    ~RawFootPrintDrawOverride() override;

    MHWRender::DrawAPI supportedDrawAPIs() const override;

    bool isBounded(
        const MDagPath& objPath,
        const MDagPath& cameraPath) const override;

    MMatrix transform(
        const MDagPath& objPath,
        const MDagPath& cameraPath) const override;

    MBoundingBox boundingBox(
        const MDagPath& objPath,
        const MDagPath& cameraPath) const override;

    bool disableInternalBoundingBoxDraw() const override;
    bool excludedFromPostEffects() const override;
    bool isTransparent() const override;

    MUserData* prepareForDraw(
        const MDagPath& objPath,
        const MDagPath& cameraPath,
        const MHWRender::MFrameContext& frameContext,
        MUserData* oldData) override;

    bool hasUIDrawables() const override { return true; }

    void addUIDrawables(
        const MDagPath& objPath,
        MHWRender::MUIDrawManager& drawManager,
        const MHWRender::MFrameContext& frameContext,
        const MUserData* data) override;

    // The general trace sequence we will get is:
    //
    // At creation and during geometry chagnes (size attribute)
    //
    // rawFootPrintNode: Start draw override DG bounding box update for object:: |transform1|rawfootPrint1
    // rawFootPrintNode: End draw override DG bounding box update for object:: |transform1|rawfootPrint1
    //

    // At creation and during attribute or transform modification:
    //
    // rawFootPrintNode: Start draw override DG node transform update for object: |transform1|rawfootPrint1
    // rawFootPrintNode: End draw override DG node transform update for object: |transform1|rawfootPrint1
    //

    // Every refresh:
    //
    // rawFootPrintNode: Start draw override render item update for object: |transform1|rawfootPrint1
    // rawFootPrintNode:  - call draw override prepareForDraw()
    // rawFootPrintNode: End draw override render item update for object: |transform1|rawfootPrint1
    // rawFootPrintNode: Start draw override queuing of UI drawables for object: |transform1|rawfootPrint1
    // rawFootPrintNode:  - call draw override addUIDrawables()
    // rawFootPrintNode: End draw override queuing of UI drawables for object: |transform1|rawfootPrint1
    //

    // Every draw:
    //
    // rawFootPrintNode: Start draw override draw function: |transform1|rawfootPrint1
    // rawFootPrintNode: End draw override draw function: |transform1|rawfootPrint1
    //
    bool traceCallSequence() const override
    {
        // Return true if internal tracing is desired.
        return false;
    }
    void handleTraceMessage( const MString &message ) const override
    {
        // Some simple custom message formatting.
        fputs("rawFootPrintNode: ", stderr);
        fputs(message.asChar(), stderr);
        fputs("\n", stderr);
    }

    bool wantUserSelection() const override
    {
        // Perform user selection when the viewport renderer is using legacy
        // OpenGL API which supports OpenGL selection mode
        MHWRender::MRenderer* theRenderer = MHWRender::MRenderer::theRenderer();
        return theRenderer && theRenderer->drawAPI() == MHWRender::kOpenGL;
    }

    bool userSelect(
        const MHWRender::MSelectionInfo& selectInfo,
        const MHWRender::MDrawContext& context,
        const MDagPath& objPath,
        const MUserData* data,
        MSelectionList& selectionList,
        MPointArray& worldSpaceHitPts) override
    {
        // This is the user selection function that will be invoked only if
        // wantUserSelection() returns true. The OpenGL selection mode is used
        // here for simplicity reason, however, a plug-in can choose other
        // GPU-based approaches like occlusion query, or CPU-based approaches
        // which perform hit test on object shapes.

        // Set up selection buffer
        const GLsizei BUF_SIZE = 10;
        GLuint selectBuf[BUF_SIZE];
        glSelectBuffer(BUF_SIZE, selectBuf);

        // Enter OpenGL selection mode
        glRenderMode(GL_SELECT);

        // Call into draw code with a name pushed onto the name stack and the
        // select info passed into the method for adjusting projection matrix.
        glInitNames();
        glPushName(0);
        drawImpl(context, &selectInfo, data);
        glPopName();

        // Exit OpenGL selection mode.
        GLint hits = glRenderMode(GL_RENDER);

        // Return false if nothing is hit.
        if (hits <= 0) return false;

        // Otherwise, calculate the hit point in world space. The depth is the
        // average of minimum and maximum, so it isn't the accurate depth at
        // the cursor position.
        GLuint minZ = selectBuf[1];
        GLuint maxZ = selectBuf[2];
        double depth = (minZ*0.5 + maxZ*0.5) / 0xFFFFFFFF;

        MFnCamera camera(context.getCurrentCameraPath());
        if (!camera.isOrtho())
        {
            const double Zn = camera.nearClippingPlane();
            const double Zf = camera.farClippingPlane();
            depth *= Zn / (Zf - depth * (Zf - Zn));
        }

        unsigned int x, y, w, h;
        selectInfo.selectRect(x, y, w, h);

        MPoint nearPw, farPw;
        context.viewportToWorld(x + w*0.5, y + h*0.5, nearPw, farPw);

        const MPoint hitPoint = nearPw + depth * (farPw - nearPw);

        selectionList.add(objPath);
        worldSpaceHitPts.append(hitPoint);

        return true;
    }

    static void drawImpl(
        const MHWRender::MDrawContext& context,
        const MHWRender::MSelectionInfo* selectInfo,
        const MUserData* data);

    static void draw(
        const MHWRender::MDrawContext& context,
        const MUserData* data)
    {
        drawImpl(context, nullptr, data);
    }

protected:
    MBoundingBox mCurrentBoundingBox;
    bool		 mCustomBoxDraw;
    bool		 mExcludeFromPostEffects;
    bool		 mTransparencySort;
    MUint64		mLastFrameStamp;
private:
    RawFootPrintDrawOverride(const MObject& obj);

    float	getMultiplier(const MDagPath& objPath) const;
    bool	isTransparentSort(const MDagPath& objPath) const;
    float	getTransparency(const MDagPath& objPath) const;

    static void OnModelEditorChanged(void *clientData);

    MCallbackId fModelEditorChangedCbId;
    MObject fRawFootPrint;

    const MHWRender::MBlendState* mBlendState;
};

// By setting isAlwaysDirty to false in MPxDrawOverride constructor, the
// draw override will be updated (via prepareForDraw()) only when the node
// is marked dirty via DG evaluation or dirty propagation. Additional
// callback is also added to explicitly mark the node as being dirty (via
// MRenderer::setGeometryDrawDirty()) for certain circumstances.
RawFootPrintDrawOverride::RawFootPrintDrawOverride(const MObject& obj)
: MHWRender::MPxDrawOverride(obj, RawFootPrintDrawOverride::draw, false)
// We want to perform custom bounding box drawing
// so return true so that the internal rendering code
// will not draw it for us.
, mCustomBoxDraw(true)
, mExcludeFromPostEffects(false)
, mTransparencySort(false)
, mBlendState(NULL)
, mLastFrameStamp(0)
, fRawFootPrint(obj)
{
    fModelEditorChangedCbId = MEventMessage::addEventCallback(
        "modelEditorChanged", OnModelEditorChanged, this);
}

RawFootPrintDrawOverride::~RawFootPrintDrawOverride()
{
    if (mBlendState)
    {
        MHWRender::MStateManager::releaseBlendState(mBlendState);
        mBlendState = NULL;
    }

    if (fModelEditorChangedCbId != 0)
    {
        MMessage::removeCallback(fModelEditorChangedCbId);
        fModelEditorChangedCbId = 0;
    }
}

void RawFootPrintDrawOverride::OnModelEditorChanged(void *clientData)
{
    // Mark the node as being dirty so that it can update on display appearance
    // switch among wireframe and shaded.
    RawFootPrintDrawOverride *ovr = static_cast<RawFootPrintDrawOverride*>(clientData);
    if (ovr) MHWRender::MRenderer::setGeometryDrawDirty(ovr->fRawFootPrint);
}

MHWRender::DrawAPI RawFootPrintDrawOverride::supportedDrawAPIs() const
{
    // this plugin supports both GL and DX
    return (MHWRender::kOpenGL | MHWRender::kDirectX11 | MHWRender::kOpenGLCoreProfile);
}

float RawFootPrintDrawOverride::getMultiplier(const MDagPath& objPath) const
{
    // Retrieve value of the size attribute from the node
    MStatus status;
    MObject rawfootprintNode = objPath.node(&status);
    if (status)
    {
        MPlug plug(rawfootprintNode, rawfootPrint::size);
        if (!plug.isNull())
        {
            MDistance sizeVal;
            if (plug.getValue(sizeVal))
            {
                return (float)sizeVal.asCentimeters();
            }
        }
    }

    return 1.0f;
}

bool RawFootPrintDrawOverride::isTransparentSort(const MDagPath& objPath) const
{
    MStatus status;
    MObject rawfootprintNode = objPath.node(&status);
    if (status)
    {
        MPlug plug(rawfootprintNode, rawfootPrint::transparencySort);
        if (!plug.isNull())
        {
            bool isTransparent;
            if (plug.getValue(isTransparent))
            {
                return isTransparent;
            }
        }
    }

    return false;
}

float RawFootPrintDrawOverride::getTransparency(const MDagPath& objPath) const
{
    // Retrieve value of the transparency attribute from the node
    MStatus status;
    MObject rawfootprintNode = objPath.node(&status);
    if (status)
    {
        MPlug plug(rawfootprintNode, rawfootPrint::transparency);
        if (!plug.isNull())
        {
            float transparencyVal;
            if (plug.getValue(transparencyVal))
            {
                return transparencyVal;
            }
        }
    }

    return 1.0f;
}

bool RawFootPrintDrawOverride::isBounded(const MDagPath& /*objPath*/,
                                      const MDagPath& /*cameraPath*/) const
{
    return true;
}

MMatrix RawFootPrintDrawOverride::transform(
    const MDagPath& objPath,
    const MDagPath& cameraPath) const
{
    // Update the transform matrix with the multiplier value,
    // this way, the matrices of the draw context will already contain the multiplier factor.
    // That way the WorldViewProjection uniform of the selection shader - which we do not have access to -
    // will be set properly.

    MMatrix transform = objPath.inclusiveMatrix();
    float multiplier = getMultiplier(objPath);

    transform[0][0] *= multiplier;
    transform[1][1] *= multiplier;
    transform[2][2] *= multiplier;

    return transform;
}

MBoundingBox RawFootPrintDrawOverride::boundingBox(
    const MDagPath& objPath,
    const MDagPath& cameraPath) const
{
    MPoint corner1( -0.17, 0.0, -0.7 );
    MPoint corner2( 0.17, 0.0, 0.3 );

    float multiplier = getMultiplier(objPath);
    corner1 = corner1 * multiplier;
    corner2 = corner2 * multiplier;

    RawFootPrintDrawOverride *nonConstThis = (RawFootPrintDrawOverride *)this;
    nonConstThis->mCurrentBoundingBox.clear();
    nonConstThis->mCurrentBoundingBox.expand( corner1 );
    nonConstThis->mCurrentBoundingBox.expand( corner2 );

    return mCurrentBoundingBox;
}

bool RawFootPrintDrawOverride::disableInternalBoundingBoxDraw() const
{
    return mCustomBoxDraw;
}

bool RawFootPrintDrawOverride::excludedFromPostEffects() const
{
    return mExcludeFromPostEffects;
}

bool RawFootPrintDrawOverride::isTransparent() const
{
    return mTransparencySort;
}

MUserData* RawFootPrintDrawOverride::prepareForDraw(
    const MDagPath& objPath,
    const MDagPath& cameraPath,
    const MHWRender::MFrameContext& frameContext,
    MUserData* oldData)
{
    MDagPath cameraPathToo = frameContext.getCurrentCameraPath();
    MString ufeCamera = frameContext.getCurrentUfeCameraPath();
    
    // We only care about post effects when in shaded mode
    const unsigned int displayStyle = frameContext.getDisplayStyle();
    bool shadedMode = (displayStyle & MHWRender::MFrameContext::kGouraudShaded) ||
                      (displayStyle & MHWRender::MFrameContext::kFlatShaded);
    static bool sExcludedFromPostEffects =
        (getenv("MAYA_RAWFOOTPRINT_EXCLUDEDFROMPOSTEFFECTS") != NULL);
    mExcludeFromPostEffects = !shadedMode || sExcludedFromPostEffects;

    mTransparencySort = isTransparentSort(objPath);

    // Retrieve data cache (create if does not exist)
    RawFootPrintData* data = dynamic_cast<RawFootPrintData*>(oldData);
    if (!data)
    {
        data = new RawFootPrintData();
        data->mLastFrameStamp = 0;
    }

    // compute data and cache it
    data->fMultiplier = getMultiplier(objPath);

    MHWRender::DisplayStatus displayStatus =
        MHWRender::MGeometryUtilities::displayStatus(objPath);
    if ((displayStatus == MHWRender::kLead) ||
        (displayStatus == MHWRender::kActive) ||
        (displayStatus == MHWRender::kHilite) ||
        (displayStatus == MHWRender::kActiveComponent))
    {
        MColor color = MHWRender::MGeometryUtilities::wireframeColor(objPath);
        data->fColor[0] = color.r;
        data->fColor[1] = color.g;
        data->fColor[2] = color.b;
    }
    else
    {
        data->fColor[0] = 0.6f;
        data->fColor[1] = 0.6f;
        data->fColor[2] = 0.6f;
    }
    data->fColor[3] = getTransparency(objPath);

    data->fCustomBoxDraw = mCustomBoxDraw;
    data->fCurrentBoundingBox = mCurrentBoundingBox;

    // Get the draw override information
    data->fDrawOV = objPath.getDrawOverrideInfo();

    // Get a blend state override
    if (!mBlendState)
    {
        MHWRender::MBlendStateDesc blendStateDesc;
        for(int i = 0; i < (blendStateDesc.independentBlendEnable ? MHWRender::MBlendState::kMaxTargets : 1); ++i)
        {
            blendStateDesc.targetBlends[i].blendEnable = true;
               blendStateDesc.targetBlends[i].sourceBlend = MHWRender::MBlendState::kSourceAlpha;
            blendStateDesc.targetBlends[i].destinationBlend = MHWRender::MBlendState::kInvSourceAlpha;
            blendStateDesc.targetBlends[i].blendOperation = MHWRender::MBlendState::kAdd;
               blendStateDesc.targetBlends[i].alphaSourceBlend = MHWRender::MBlendState::kOne;
            blendStateDesc.targetBlends[i].alphaDestinationBlend = MHWRender::MBlendState::kInvSourceAlpha;
            blendStateDesc.targetBlends[i].alphaBlendOperation = MHWRender::MBlendState::kAdd;
        }

        blendStateDesc.blendFactor[0] = 1.0f;
        blendStateDesc.blendFactor[1] = 1.0f;
        blendStateDesc.blendFactor[2] = 1.0f;
        blendStateDesc.blendFactor[3] = 1.0f;

        mBlendState = MHWRender::MStateManager::acquireBlendState(blendStateDesc);
    }
    data->mBlendState = mBlendState;

    return data;
}

void RawFootPrintDrawOverride::addUIDrawables(
        const MDagPath& objPath,
        MHWRender::MUIDrawManager& drawManager,
        const MHWRender::MFrameContext& frameContext,
        const MUserData* data)
{
    // Draw a text "Foot"
    MPoint pos( 0.0, 0.0, 0.0 ); // Position of the text
    MColor textColor( 0.1f, 0.8f, 0.8f, 1.0f ); // Text color

    drawManager.beginDrawable();

    drawManager.setColor( textColor );
    drawManager.setFontSize( MHWRender::MUIDrawManager::kSmallFontSize );
    drawManager.text( pos,  MString("rawFootprint"), MHWRender::MUIDrawManager::kCenter );

    drawManager.endDrawable();
}

// Sample code to debug camera information
void footPrint_DebugCameraInformation(const MHWRender::MDrawContext& context)
{
    bool debugCameraInformation = false;
    if (debugCameraInformation)
    {
        MStatus status;
        MBoundingBox frustum = context.getFrustumBox(&status);
        MPoint min = frustum.min();
        MPoint max = frustum.max();
    
        // 1. Query informaiton available from the draw context
        //
        printf("Context Camera Info:\n");
        printf("====================\n");	
        printf("-- Frustum: W = %g, H = %g, D= %g. Min = %g,%g,%g. Max = %g,%g,%g\n", 
            frustum.width(), frustum.height(), frustum.depth(), min[0], min[1], min[2],
            max[0], max[1], max[2]);

        // Get depth range
        float nearV, farV;
        context.getDepthRange(nearV, farV);
        printf("-- Depth Range: near=%g, far=%g\n", nearV, farV);

        // Determine if view direction is along Z- (or Z+)
        bool alongNegZ = context.viewDirectionAlongNegZ(&status);
        printf("-- View direction = %s\n", alongNegZ ? "-Z" : "+Z"); 
        
        // Find out the camera coordinate system
        MDoubleArray vPos = context.getTuple(MHWRender::MFrameContext::kViewPosition);
        printf("-- View position = %g,%g,%g\n", vPos[0], vPos[1], vPos[2]);
        MDoubleArray vDir = context.getTuple(MHWRender::MFrameContext::kViewDirection);
        printf("-- View direction = %g,%g,%g\n", vDir [0], vDir [1], vDir [2]);
        MDoubleArray vals = context.getTuple(MHWRender::MFrameContext::kViewUp);
        printf("-- View up = %g,%g,%g\n", vals[0], vals[1], vals[2]);
        vals = context.getTuple(MHWRender::MFrameContext::kViewRight);
        printf("-- View right = %g,%g,%g\n", vals[0], vals[1], vals[2]);
        MDoubleArray vNear = context.getTuple(MHWRender::MFrameContext::kViewUnnormlizedNearClipValue);
        printf("-- View near clip value = %g\n", vNear[0]);
        MDoubleArray vFar = context.getTuple(MHWRender::MFrameContext::kViewUnnormalizedFarClipValue);
        printf("-- View far clip value = %g\n", vFar[0]);

        // Compute the world space planes for the near and far clip planes
        // The near is pointing away from the camera and is thus point in the
        // negative direction while the far is point towards the camera (positive 
        // direction.
        double distW = vPos[0]*vDir[0] + 
                    vPos[1]*vDir[1] + 
                    vPos[2]*vDir[2]; 
        // Face near plane away from the camera
        double distNearW = -1.0 * (distW + vNear[0]); 
        printf("-- World space near plane dist=%g. Dir=%g,%g,%g\n", distNearW,
            vDir[0], vDir[1], vDir[2]);
        // Face far plane torwards the camera
        double distFarW = distW + vFar[0];
        printf("-- World space far plane dist=%g. Dir=%g,%g,%g\n", distFarW,
            -vDir[0], -vDir[1], -vDir[2]);	
        
        // 2. Query the active Maya camera for information from the draw context
        //
        MDagPath cameraPath = context.getCurrentCameraPath();
        MFnCamera activeCamera(cameraPath);
        int uiX, uiY, uiWidth, uiHeight;
        context.getViewportDimensions(uiX, uiY, uiWidth, uiHeight);

        double windowAspect = (double)uiWidth / (double)uiHeight;

        double left, bottom, right, top;
        MString destinationName;
        MHWRender::MFrameContext::RenderingDestination dest = context.renderingDestination(destinationName);
        bool viewportMode = (dest == MHWRender::MFrameContext::k3dViewport);
        // If rendering mode ignore all the in viewport options
        if (!viewportMode)
        {
            activeCamera.getRenderingFrustum(windowAspect, left, right, bottom, top);
        }
        // If viewport mode take into consideration overscan, and pan+zoom but not squeeze.
        else
        {
            static bool overscan = true;
            static bool squeeze = false;
            const bool panAndZoom = activeCamera.panZoomEnabled();
            activeCamera.getViewingFrustum(windowAspect, left, right, bottom, top, overscan, squeeze, panAndZoom );
        }
        // Relative near and far with respect to camera position.
        // Get the unnormalized value.
        double nearD = activeCamera.unnormalizedNearClippingPlane();
        double farD = activeCamera.unnormalizedFarClippingPlane();

        printf("Camera Info:\n");
        printf("============\n");
        printf("-- Shape %s, Frustum: left=%g, bottom=%g, right=%g, top=%g near=%g, far=%g\n", 
            activeCamera.name().asChar(), left, bottom, right, top, nearD, farD);

        // Object space information
        MPoint eyePoint = activeCamera.eyePoint();
        MVector	viewDirection = activeCamera.viewDirection();
        MVector	upDirection = activeCamera.upDirection();
        MVector	rightDirection = activeCamera.rightDirection();
        printf("-- Object space values: eyePoint=%g,%g,%g. vdir=%g,%g,%g. vup=%g,%g,%g, vright=%g,%g,%g\n", 
            eyePoint[0], eyePoint[1], eyePoint[2],
            viewDirection[0], viewDirection[1], viewDirection[2],
            upDirection[0], upDirection[1], upDirection[2],
            rightDirection[0], rightDirection[1], rightDirection[2]);

        // World space information
        MPoint eyePointW = activeCamera.eyePoint(MSpace::kWorld);
        MVector	viewDirectionW = activeCamera.viewDirection(MSpace::kWorld);
        MVector	upDirectionW = activeCamera.upDirection(MSpace::kWorld);
        MVector	rightDirectionW = activeCamera.rightDirection(MSpace::kWorld);
        printf("-- World space values: eyePoint=%g,%g,%g. vdir=%g,%g,%g. vup=%g,%g,%g, vright=%g,%g,%g\n", 
            eyePointW[0], eyePointW[1], eyePointW[2],
            viewDirectionW[0], viewDirectionW[1], viewDirectionW[2],
            upDirectionW[0], upDirectionW[1], upDirectionW[2],
            rightDirectionW[0], rightDirectionW[1], rightDirectionW[2]);

        // Compute the world space planes for the near and far clip planes
        // The near is pointing away from the camera and is thus point in the
        // negative direction while the far is point towards the camera (positive 
        // direction.
        distW = eyePointW[0]*viewDirectionW[0] + 
            eyePointW[1]*viewDirectionW[1] + 
            eyePointW[2]*viewDirectionW[2]; 
        // Face near plane away from the camera
        distNearW = -1.0 * (distW + nearD); 
        printf("-- World space near plane: Dir=%g,%g,%g. Dist=%g.\n", 
            viewDirectionW[0], viewDirectionW[1], viewDirectionW[2], distNearW);
        // Face far plane torwards the camera
        distFarW = distW + farD;	
        printf("-- World space far plane: Dir=%g,%g,%g, Dist=%g. \n", 
            -viewDirectionW[0], -viewDirectionW[1], -viewDirectionW[2], distFarW);

        // World space near and far clip points.
        MPoint clipPts[4];
        activeCamera.getFilmFrustum( -nearD, clipPts );
        MMatrix icMat = cameraPath.inclusiveMatrix();
        unsigned int i = 0;
        for ( i = 0; i < 4; i ++ ) 
        {
            MPoint pt = clipPts[i] * icMat;
            printf("-- Near clip point[%d] = %g,%g,%g\n", i, pt[0], pt[1], pt[2]);
        }
        activeCamera.getFilmFrustum( -farD, clipPts );
        for ( int i = 0; i < 4; i ++ ) 
        {
            MPoint pt = clipPts[i] * icMat;
            printf("-- Far clip point[%d] = %g,%g,%g\n", i, pt[0], pt[1], pt[2]);
        }
    }
}

//	Sample code to debug object type exclusion
//
void footPrint_DebugObjectTypeExclusions(const MHWRender::MDrawContext& context)
{
    // Sample code to determine object type exclusion
    bool debugTypeExclusions = false;
    if (debugTypeExclusions)
    {
        MUint64 excluded = 
            context.objectTypeExclusions();
        if (excluded > 0)
        {
            if (excluded & MHWRender::MFrameContext::kExcludeNurbsCurves)	MGlobal::displayInfo("Exclude NURBS curves");
            if (excluded & MHWRender::MFrameContext::kExcludeNurbsSurfaces)	MGlobal::displayInfo("Exclude NURBS surface");
            if (excluded & MHWRender::MFrameContext::kExcludeMeshes)		MGlobal::displayInfo("Exclude polygonal meshes");
            if (excluded & MHWRender::MFrameContext::kExcludePlanes)		MGlobal::displayInfo("Exclude planes");
            if (excluded & MHWRender::MFrameContext::kExcludeLights)		MGlobal::displayInfo("Exclude lights");
            if (excluded & MHWRender::MFrameContext::kExcludeCameras)		MGlobal::displayInfo("Exclude camera");
            if (excluded & MHWRender::MFrameContext::kExcludeJoints)		MGlobal::displayInfo("Exclude joints");
            if (excluded & MHWRender::MFrameContext::kExcludeIkHandles)		MGlobal::displayInfo("Exclude IK handles");
            if (excluded & MHWRender::MFrameContext::kExcludeDeformers)		MGlobal::displayInfo("Exclude all deformations");
            if (excluded & MHWRender::MFrameContext::kExcludeDynamics)		MGlobal::displayInfo("Exclude all dynamics objects (emiiters, cloth)");
            if (excluded & MHWRender::MFrameContext::kExcludeParticleInstancers)	MGlobal::displayInfo("Exclude all particle instancers");
            if (excluded & MHWRender::MFrameContext::kExcludeLocators)		MGlobal::displayInfo("Exclude locators");
            if (excluded & MHWRender::MFrameContext::kExcludeDimensions)	MGlobal::displayInfo("Exclude all measurement objects");
            if (excluded & MHWRender::MFrameContext::kExcludeSelectHandles)	MGlobal::displayInfo("Exclude selection handles");
            if (excluded & MHWRender::MFrameContext::kExcludePivots)		MGlobal::displayInfo("Exclude pivots");
            if (excluded & MHWRender::MFrameContext::kExcludeTextures)		MGlobal::displayInfo("Exclude texure placement objects");
            if (excluded & MHWRender::MFrameContext::kExcludeGrid)			MGlobal::displayInfo("Exclude grid drawing");
            if (excluded & MHWRender::MFrameContext::kExcludeCVs)			MGlobal::displayInfo("Exclude NURBS control vertices");
            if (excluded & MHWRender::MFrameContext::kExcludeHulls)			MGlobal::displayInfo("Exclude NURBS hulls");
            if (excluded & MHWRender::MFrameContext::kExcludeStrokes)		MGlobal::displayInfo("Exclude PaintFX strokes");
            if (excluded & MHWRender::MFrameContext::kExcludeSubdivSurfaces) MGlobal::displayInfo("Exclude subdivision surfaces");
            if (excluded & MHWRender::MFrameContext::kExcludeFluids)		MGlobal::displayInfo("Exclude fluid objects");
            if (excluded & MHWRender::MFrameContext::kExcludeFollicles)		MGlobal::displayInfo("Exclude hair follicles");
            if (excluded & MHWRender::MFrameContext::kExcludeHairSystems)	MGlobal::displayInfo("Exclude hair system");
            if (excluded & MHWRender::MFrameContext::kExcludeImagePlane)	MGlobal::displayInfo("Exclude image planes");
            if (excluded & MHWRender::MFrameContext::kExcludeNCloths)		MGlobal::displayInfo("Exclude N-cloth objects");
            if (excluded & MHWRender::MFrameContext::kExcludeNRigids)		MGlobal::displayInfo("Exclude rigid-body objects");
            if (excluded & MHWRender::MFrameContext::kExcludeDynamicConstraints)	MGlobal::displayInfo("Exclude rigid-body constraints");
            if (excluded & MHWRender::MFrameContext::kExcludeManipulators)	MGlobal::displayInfo("Exclude manipulators");
            if (excluded & MHWRender::MFrameContext::kExcludeNParticles)	MGlobal::displayInfo("Exclude N-particle objects");
            if (excluded & MHWRender::MFrameContext::kExcludeMotionTrails)  MGlobal::displayInfo("Exclude motion trails");
            if (excluded & MHWRender::MFrameContext::kExcludeHoldOuts)		MGlobal::displayInfo("Exclude hold-outs");
            if (excluded & MHWRender::MFrameContext::kExcludePluginShapes)	MGlobal::displayInfo("Exclude plug-in shapes");
            if (excluded & MHWRender::MFrameContext::kExcludeHUD)			MGlobal::displayInfo("Exclude HUD");
            if (excluded & MHWRender::MFrameContext::kExcludeClipGhosts)	MGlobal::displayInfo("Exclude animation clip ghosts");
            if (excluded & MHWRender::MFrameContext::kExcludeGreasePencils)	MGlobal::displayInfo("Exclude grease pencil drawing");
            if (excluded & MHWRender::MFrameContext::kExcludeControllers)	MGlobal::displayInfo("Exclude controllers objects");
        }
    }

    // Sample code to determine classification type exclusion
    bool debugClassificationExclusions = false;
    if (debugClassificationExclusions )
    {
        MStringArray classifications;
        context.classificationExclusions( classifications );
        MString info("Classification strings excluded: ");
        for (unsigned int i=0; i<classifications.length(); i++)
        {
            info += classifications[i];
            info += ";";
        }
        MGlobal::displayInfo( info );
    }
}

// Sample code to debug background parameters
//
void footPrint_DebugBackground(const MHWRender::MDrawContext& context)
{
    bool debugBackground = false;
    if (debugBackground)
    {
        MHWRender::MFrameContext::BackgroundParams backParams;
        context.getBackgroundParameters(backParams);
        MString info("Display Gradient: "); info += backParams.displayGradient; MGlobal::displayInfo(info);
        info = MString("Clear color bit: "); info += backParams.clearColorFlag; MGlobal::displayInfo(info);
        info = MString("Clear depth bit: "); info += backParams.clearDepthFlag; MGlobal::displayInfo(info);
        info = MString("Clear stencil bit: "); info += backParams.clearStencilFlag; MGlobal::displayInfo(info);
        info = MString("Clear color: "); 
        info += backParams.clearColor1[0]; info += MString(",");
        info += backParams.clearColor1[1]; info += MString(",");
        info += backParams.clearColor1[2]; info += MString(",");
        info += backParams.clearColor1[3]; 
        MGlobal::displayInfo(info);
        info = MString("Clear color 2: "); 
        info += backParams.clearColor2[0]; info += MString(",");
        info += backParams.clearColor2[1]; info += MString(",");
        info += backParams.clearColor2[2]; info += MString(",");
        info += backParams.clearColor2[3]; 
        MGlobal::displayInfo(info);
        info = MString("Clear depth value: "); info += backParams.clearDepthValue; MGlobal::displayInfo(info);
        info = MString("Clear stencil value: "); info += backParams.clearStencilValue; MGlobal::displayInfo(info);
    }
}

// Sample code to debug post effect information
//
void footPrint_DebugPostEffects(const MHWRender::MDrawContext& context)
{
    bool debugPostEffects = false;
    if (debugPostEffects)
    {
        if (context.getPostEffectEnabled( MHWRender::MFrameContext::kAmbientOcclusion ))
            MGlobal::displayInfo("SSAO enabled");
        if (context.getPostEffectEnabled( MHWRender::MFrameContext::kMotionBlur ))
            MGlobal::displayInfo("Motion blur enabled");
        if (context.getPostEffectEnabled( MHWRender::MFrameContext::kViewColorTransformEnabled ))
            MGlobal::displayInfo("Color management enabled");
        if (context.getPostEffectEnabled( MHWRender::MFrameContext::kDepthOfField ))
        {
            MHWRender::MFrameContext::DOFParams dofParams;
            context.getDOFParameters(dofParams);
            MGlobal::displayInfo("Depth-of-field enabled:");
            MString info("- Focus distance: "); info += dofParams.focalDistance; MGlobal::displayInfo(info);
            info = MString("- Alpha: "); info += dofParams.alpha; MGlobal::displayInfo(info);

        }
        if (context.getPostEffectEnabled( MHWRender::MFrameContext::kAntiAliasing ))
            MGlobal::displayInfo("Anti-aliasing enabled");
    }
}

// Sample code to debug render destination
//
void footPrint_DebugDestination(const MHWRender::MDrawContext& context)
{
    // Sample code to determine the rendering destination
    bool debugDestination = false;
    if (debugDestination)
    {
        MString destinationIdentifier;
        MHWRender::MFrameContext::RenderingDestination dest = context.renderingDestination(destinationIdentifier);
        MString destinationType = " 3d viewport";
        bool found3dView = false;
        if (dest == MHWRender::MFrameContext::k3dViewport)
        {
            M3dView view;
            if (M3dView::getM3dViewFromModelPanel(destinationIdentifier, view) == MStatus::kSuccess)
            {
                found3dView = true;
            }
        }
        else if (dest == MHWRender::MFrameContext::k2dViewport)
            destinationType = " 2d viewport";
        else if (dest == MHWRender::MFrameContext::kImage)
            destinationType = "n image";
        printf("rawfootprint node render destination is a%s. Destination name=%s. Found M3dView=%d\n", 
            destinationType.asChar(), destinationIdentifier.asChar(), found3dView);
    }
}

// Sample code to debug render override
//
void footPrint_DebugRenderOverride(const MHWRender::MDrawContext& context)
{
    bool debugRenderOverride = false;
    if (debugRenderOverride)
    {
        MHWRender::MFrameContext::RenderOverrideInformation overrideInfo;
        context.getRenderOverrideInformation(overrideInfo);
        if (overrideInfo.overrideName.length())
        {
            MString info("Render Override: "); info += overrideInfo.overrideName; MGlobal::displayInfo(info);
        }
    }
}	

// Sample code to debug display style
//
void footPrint_DebugDisplayStyle(const MHWRender::MDrawContext& context,
                                        const RawFootPrintData* footData)
{
    bool debugDisplayStyle = false;
    if (debugDisplayStyle)
    {
        const unsigned int displayStyle = context.getDisplayStyle();
        bool drawAsBoundingbox =
            (displayStyle & MHWRender::MFrameContext::kBoundingBox) ||
            (footData->fDrawOV.fLOD == MDAGDrawOverrideInfo::kLODBoundingBox);
        if (drawAsBoundingbox) MGlobal::displayInfo("Bbox display style on");
        if (displayStyle & MHWRender::MFrameContext::kWireFrame) MGlobal::displayInfo("Wireframe display style on");
        bool drawShaded = ((displayStyle & MHWRender::MFrameContext::kGouraudShaded) ||
                          (displayStyle & MHWRender::MFrameContext::kFlatShaded));
        if (drawShaded) MGlobal::displayInfo("Shaded display style on");
        if (displayStyle & MHWRender::MFrameContext::kTextured) MGlobal::displayInfo("Textured display style on");
        if (displayStyle & MHWRender::MFrameContext::kDefaultMaterial) MGlobal::displayInfo("Use default material display style on");
        if (displayStyle & MHWRender::MFrameContext::kXrayJoint) MGlobal::displayInfo("X-ray joint display style on");
        if (displayStyle & MHWRender::MFrameContext::kXray) MGlobal::displayInfo("X-ray display style on");
        if (displayStyle & MHWRender::MFrameContext::kTwoSidedLighting) MGlobal::displayInfo("Two-sided lighting display style on");
        if (displayStyle & MHWRender::MFrameContext::kFlatShaded) MGlobal::displayInfo("Flat shaded display style on");
        if (displayStyle & MHWRender::MFrameContext::kShadeActiveOnly) MGlobal::displayInfo("Shaded active display style on");
        if (displayStyle & MHWRender::MFrameContext::kXrayActiveComponents) MGlobal::displayInfo("X-ray active component display style on");
        if (displayStyle & MHWRender::MFrameContext::kBackfaceCulling) MGlobal::displayInfo("Backface culling display style on");
        if (displayStyle & MHWRender::MFrameContext::kSmoothWireframe) MGlobal::displayInfo("Smooth wireframe display style on");
    }
}

// Sample code to debug fog parameters
//
void footPrint_DebugFogStatus(const MHWRender::MDrawContext& context)
{
    bool debugFogStatus = false;
    if (debugFogStatus)
    {
        MHWRender::MFrameContext::HwFogParams params = context.getHwFogParameters();
        if (params.HwFogEnabled)
        {
            MGlobal::displayInfo(MString("Fog mode = ") + params.HwFogMode);
            MGlobal::displayInfo(MString("Fog start = ") + params.HwFogStart);
            MGlobal::displayInfo(MString("Fog end = ") + params.HwFogEnd);
            MGlobal::displayInfo(MString("Fog density = ") + params.HwFogDensity);
            MColor col = params.HwFogColor;
            MString info("Fog color = ");
            info += col[0]; info += ",";
            info += col[1]; info += ",";
            info += col[2]; info += ",";
            info += col[3]; 
            MGlobal::displayInfo(info);
        }
    }
}

// Sample code to debug print out pass information
//
void footPrint_DebugPassInformation(const MHWRender::MDrawContext& context)
{
    static bool debugPassInformation = false;
    if (debugPassInformation)
    {
        const MHWRender::MPassContext & passCtx = context.getPassContext();
        const MStringArray & passSem = passCtx.passSemantics();

        const MString & passId = passCtx.passIdentifier();
        MString info("rawfootprint node drawing in pass["); 
        info += passId.asChar(); info += "]";
        MGlobal::displayInfo(info);
        if (passSem.length()) 
        {
            MString info("--- semantics[");
            for (unsigned int i=0; i<passSem.length(); i++)
            {
                info += " ";
                info += passSem[i].asChar();
            }
            info += "]";
            MGlobal::displayInfo(info);
        }
        if (passCtx.hasShaderOverride()) 
        {
            MHWRender::MShaderInstance* passShaderOverride = 
                    passCtx.shaderOverrideInstance();
            if (passShaderOverride) {
                MGlobal::displayInfo("--- use internal shaderOverride");
            }
        }
    }
}

//
// Main drawing routine
//
void RawFootPrintDrawOverride::drawImpl(
    const MHWRender::MDrawContext& context,
    const MHWRender::MSelectionInfo* selectInfo,
    const MUserData* data)
{
    // Get renderer
    MHWRender::MRenderer* theRenderer = MHWRender::MRenderer::theRenderer();
    if (!theRenderer)
    {
        return;
    }

    // Get user draw data
    const RawFootPrintData* footData = dynamic_cast<const RawFootPrintData*>(data);
    if (!footData)
    {
        return;
    }

    // Debug code to trace frame stamp numbering
    static bool traceFrameStamp = false;
    if (traceFrameStamp)
    {
        const MHWRender::MPassContext & passCtx = context.getPassContext();
        const MStringArray & passSem = passCtx.passSemantics();
        MUint64 currentFrameStamp = context.getFrameStamp();
        if (currentFrameStamp != footData->mLastFrameStamp)
        {
            bool updateColorPassFrameStamp = true;
            if (passSem.length() == 1 && 
                (passSem[0] == MHWRender::MPassContext::kSelectionPassSemantic ||
                passSem[0] == MHWRender::MPassContext::kShadowPassSemantic))
            {
                updateColorPassFrameStamp = false;
            }
            if (updateColorPassFrameStamp)
            {
                printf("RawFootPrintDrawOverride: frame stamp change from %ld to %ld\n", 					
                    (long)footData->mLastFrameStamp, (long)currentFrameStamp);
            }
            ((RawFootPrintData*)footData)->mLastFrameStamp = currentFrameStamp;
        }
    }

    // Get DAG object draw override
    const MDAGDrawOverrideInfo& objectOverrideInfo = footData->fDrawOV;

    // Just return and draw nothing, if it is overridden invisible
    if ( objectOverrideInfo.fOverrideEnabled && !objectOverrideInfo.fEnableVisible )
    {
        return;
    }

    // If in playback but hidden in playback, skip drawing
    bool animPlay = MAnimControl::isPlaying();
    bool animScrub = MAnimControl::isScrubbing();
    if (!objectOverrideInfo.fPlaybackVisible &&
        (animPlay || animScrub))
    {
        return;
    }
    
    // Optional debugging utility calls
    footPrint_DebugCameraInformation(context);
    footPrint_DebugObjectTypeExclusions(context);
    footPrint_DebugBackground(context);
    footPrint_DebugPostEffects(context);	
    footPrint_DebugDestination(context);	
    footPrint_DebugRenderOverride(context);
    footPrint_DebugDisplayStyle(context, footData);
    footPrint_DebugFogStatus(context);
    footPrint_DebugPassInformation(context);

    // Check display styles to determine what modes to draw with
    //
    const unsigned int displayStyle = context.getDisplayStyle();

    bool drawAsBoundingbox =
        (displayStyle & MHWRender::MFrameContext::kBoundingBox) ||
        (footData->fDrawOV.fLOD == MDAGDrawOverrideInfo::kLODBoundingBox);

    // Require shadeded draw
    bool drawShaded = ((displayStyle & MHWRender::MFrameContext::kGouraudShaded) ||
                      (displayStyle & MHWRender::MFrameContext::kFlatShaded));
    // Require wireframe draw
    bool drawWireframe = (displayStyle & MHWRender::MFrameContext::kWireFrame) > 0;

    // Templated, only draw wirefame and it is not selectale
    bool overideTemplated = objectOverrideInfo.fOverrideEnabled &&
        (objectOverrideInfo.fDisplayType == MDAGDrawOverrideInfo::kDisplayTypeTemplate);
    if (overideTemplated)
    {
        drawWireframe = true;
    }

    // Override no shaded, only show wireframe
    bool overrideNoShaded = objectOverrideInfo.fOverrideEnabled && !objectOverrideInfo.fEnableShading;
    if (overideTemplated || overrideNoShaded)
    {
        drawShaded = false;
    }

    // If we don't want to draw the bounds within this plugin
    // manually, then skip drawing altogether in bounding box mode
    // since the bounds draw is handled by the renderer and
    // doesn't need to be drawn here.
    //
    if ( drawAsBoundingbox && !footData->fCustomBoxDraw )
    {
        drawAsBoundingbox = false;
    }

    // For any viewport interactions switch to bounding box mode,
    // except when we are in playback. Uncomment to enable usage.
    if (context.inUserInteraction() || context.userChangingViewContext())
    {
        if (!animPlay && !animScrub)
        {
            ; //drawAsBoundingbox = true;
        }
    }

    // Check to see if we are drawing with a shader override,
    // and look for any specific branching required for different passes
    // (e.g. for selection)
    //
    bool inSelection = false;
    bool inBeautyPass = false;
    bool inTransparencyPass = false;
    const MString beautyPassSemantic("shadedBeautyGraphSemantic");
    const MHWRender::MPassContext & passCtx = context.getPassContext();
    const MStringArray & passSem = passCtx.passSemantics();
    MHWRender::MShaderInstance* passShaderOverride = NULL;
    for (unsigned int i=0; i<passSem.length(); i++)
    {
        if (passSem[i] == MHWRender::MPassContext::kSelectionPassSemantic)
        {
            inSelection = true;
        }
        else if (passSem[i] == beautyPassSemantic)
        {
            inBeautyPass = true;
        }

        // Do not use the shader override in selection drawing code path
        if (selectInfo == nullptr && passCtx.hasShaderOverride())
        {
            passShaderOverride = passCtx.shaderOverrideInstance();

            // Update single-sided state and normal multiplier as needed
            passShaderOverride->setParameter("isSingleSided", (displayStyle & MHWRender::MFrameContext::kTwoSidedLighting) == 0);
            passShaderOverride->setParameter("mayaNormalMultiplier", -1.0f);
        }

        if (passSem[i] == MHWRender::MPassContext::kCullBackSemantic ||
            passSem[i] == MHWRender::MPassContext::kCullFrontSemantic)
        {
            inTransparencyPass = true;
        }
    }

    // Get cached data
    float color[4] = {
            footData->fColor[0] * footData->fColor[3],
            footData->fColor[1] * footData->fColor[3],
            footData->fColor[2] * footData->fColor[3],
            footData->fColor[3]
    };

    bool requireBlending = false;

    if (!inSelection)
    {
        // Use some monotone version of color to show "default material mode"
        //
        //if (displayStyle & MHWRender::MFrameContext::kDefaultMaterial)
        //{
        //	color[0] = color[1] = color[2] = (color[0] + color[1] + color[2]) / 3.0f;
        //}
        // Do some alpha blending if in x-ray mode
        //
        if (displayStyle & MHWRender::MFrameContext::kXray)
        {
            // If in beauty pass should still allow blending e.g. kMaterialOverrideSemantic
            requireBlending = !passShaderOverride;
            color[3] = 0.3f;
        }
    }

    //================================================
    // Set blend and raster state
    //================================================
    MHWRender::MStateManager* stateMgr = context.getStateManager();
    const MHWRender::MBlendState* pOldBlendState = NULL;
    const MHWRender::MRasterizerState* pOldRasterState = NULL;
    const MHWRender::MRasterizerState* rasterState = NULL;

    if(stateMgr && ((displayStyle & MHWRender::MFrameContext::kGouraudShaded) ||
                   (displayStyle & MHWRender::MFrameContext::kFlatShaded)))
    {
        // draw filled, and with blending if required
        if (stateMgr && requireBlending)
        {		
            if (footData->mBlendState)
            {
                pOldBlendState = stateMgr->getBlendState();
                stateMgr->setBlendState(footData->mBlendState);
            }
        }

        // If the object is not draw as transparency, 
        // override culling mode since we always want double-sided
        //
        if (!inTransparencyPass)
        {
            pOldRasterState = stateMgr ? stateMgr->getRasterizerState() : NULL;
            if (pOldRasterState)
            {

                MHWRender::MRasterizerStateDesc desc( pOldRasterState->desc() );
                // It's also possible to change this to kCullFront or kCullBack if we
                // wanted to set it to that.
                MHWRender::MRasterizerState::CullMode cullMode = MHWRender::MRasterizerState::kCullNone;
                if (desc.cullMode != cullMode)
                {
                    // Just override the cullmode
                    desc.cullMode = cullMode;
                    rasterState = stateMgr->acquireRasterizerState(desc);
                    if (rasterState)
                    {
                        stateMgr->setRasterizerState(rasterState);
                    }
                }
            }
        }
    }

    //================================================
    // Start the draw work
    //================================================

    // Prepare draw agent, default using OpenGL
    RawFootPrintDrawAgentGL& drawAgentRef = RawFootPrintDrawAgentGL::getDrawAgent();
    RawFootPrintDrawAgent* drawAgentPtr = &drawAgentRef;
#ifdef _WIN32
    // DX Draw
    if ( !theRenderer->drawAPIIsOpenGL() )
    {
        RawFootPrintDrawAgentDX& drawAgentRef = RawFootPrintDrawAgentDX::getDrawAgent();
        drawAgentPtr = &drawAgentRef;
    }
#endif
    if ( theRenderer->drawAPI() == MHWRender::kOpenGLCoreProfile )
    {
        RawFootPrintDrawAgentCoreProfile& drawAgentRef = RawFootPrintDrawAgentCoreProfile::getDrawAgent();
        drawAgentPtr = &drawAgentRef;
    }

    assert( drawAgentPtr );

    if (drawAgentPtr) {

        // Set color
        drawAgentPtr->setColor(MColor(color[0], color[1], color[2], color[3]));
        // Set matrix
        drawAgentPtr->setMatrix( context, selectInfo);

        drawAgentPtr->beginDraw( context, passShaderOverride );

        if (drawAsBoundingbox)
        {
            // If it is in bounding bode, draw only bounding box wireframe, nothing else
            drawAgentPtr->drawBoundingBox();
        }
        else {
            if (drawWireframe || overideTemplated || overrideNoShaded) 
            {
                drawAgentPtr->drawWireframe();
            }
            if (!overideTemplated && !overrideNoShaded  && drawShaded)
            {
                drawAgentPtr->drawShaded();
            }
        }

        drawAgentPtr->endDraw();
    }

    //================================================
    // End the draw work
    //================================================

    if ( passShaderOverride ) {
        theRenderer->getShaderManager()->releaseShader( passShaderOverride );
    }

    //================================================
    // Restore old blend state and old raster state
    //================================================
    if(stateMgr && (displayStyle & MHWRender::MFrameContext::kGouraudShaded  ||
                    displayStyle & MHWRender::MFrameContext::kFlatShaded))
    {
        if (stateMgr && pOldBlendState)
        {
            stateMgr->setBlendState(pOldBlendState);
            stateMgr->releaseBlendState(pOldBlendState);
        }
        if (pOldRasterState)
        {
            stateMgr->setRasterizerState(pOldRasterState);
            stateMgr->releaseRasterizerState(pOldRasterState);
        }
        if (rasterState)
        {
            stateMgr->releaseRasterizerState(rasterState);
            rasterState = NULL;
        }
    }
}

bool RawFootPrintDrawAgentCoreProfile::initShadersCoreProfile()
{
    static const char* vertexShaderText = 
        "#version 330  \n"
        "in vec3 Pm;  \n"
        "uniform mat4 MVP;  \n"
        "void main() {  \n"
        "	gl_Position = MVP * vec4(Pm,1.0);  \n"
        "}";
    GLuint vertexShaderID = GLCP::CreateShader(GL_VERTEX_SHADER);
    GLCP::ShaderSource(vertexShaderID, 1, &vertexShaderText , NULL);
    GLCP::CompileShader(vertexShaderID);

    // Check for errors:
    GLint result;
    GLCP::GetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
    if(result==GL_FALSE){
        GLint errLength;
        GLCP::GetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &errLength);
        char* errorMessage = (char*)malloc(errLength+1);
        GLCP::GetShaderInfoLog(vertexShaderID, errLength, NULL, &errorMessage[0]);
        fprintf(stdout, "Compilation of vertex shader failed:\n%s\n", &errorMessage[0]);
        free(errorMessage);
        return false;
    }

    static const char* fragmentShaderText = 
        "#version 330  \n"
        "uniform vec4 color = vec4( 1.0f, 1.0f, 1.0f, 1.0f );  \n"
        "out vec4 colorOut; \n"
        "void main() {  \n"
        "	colorOut = color;  \n"
        "}";

    GLuint fragmentShaderID = GLCP::CreateShader(GL_FRAGMENT_SHADER);
    GLCP::ShaderSource(fragmentShaderID, 1, &fragmentShaderText , NULL);
    GLCP::CompileShader(fragmentShaderID);

    GLCP::GetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
    if(result==GL_FALSE){
        GLint errLength;
        GLCP::GetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &errLength);
        char* errorMessage = (char*)malloc(errLength+1);
        GLCP::GetShaderInfoLog(fragmentShaderID, errLength, NULL, &errorMessage[0]);
        fprintf(stdout, "Compilation of fragment shader failed:\n%s\n", &errorMessage[0]);
        free(errorMessage);
        return false;
    }

    mShaderProgram = GLCP::CreateProgram();
    GLCP::AttachShader(mShaderProgram, vertexShaderID);
    GLCP::AttachShader(mShaderProgram, fragmentShaderID);
    GLCP::LinkProgram(mShaderProgram);

    GLCP::GetProgramiv(mShaderProgram, GL_LINK_STATUS, &result);
    if(result==GL_FALSE){
        GLint errLength;
        GLCP::GetProgramiv(mShaderProgram, GL_INFO_LOG_LENGTH, &errLength);
        char* errorMessage = (char*)malloc(errLength+1);
        GLCP::GetProgramInfoLog(mShaderProgram, errLength, NULL, &errorMessage[0]);
        fprintf(stdout, "Linking of shader program failed:\n%s\n", &errorMessage[0]);
        free(errorMessage);
        return false;
    }

    mWVPIndex = GLCP::GetUniformLocation(mShaderProgram, "MVP");
    mColorIndex = GLCP::GetUniformLocation(mShaderProgram, "color");
    mVtxAttrib = GLCP::GetAttribLocation( mShaderProgram, "Pm" );

    GLCP::DeleteShader(vertexShaderID);
    GLCP::DeleteShader(fragmentShaderID);

    return true;
}

bool RawFootPrintDrawAgentCoreProfile::initBuffersCoreProfile()
{
    GLCP::GenVertexArrays(1, &mBBoxVAO);
    GLCP::BindVertexArray(mBBoxVAO);

    GLCP::GenBuffers(1, &mBoundingboxVertexBuffer);
    GLCP::BindBuffer(GL_ARRAY_BUFFER, mBoundingboxVertexBuffer);
    GLCP::BufferData(GL_ARRAY_BUFFER, sizeof(bbData), (void*)bbData, GL_STATIC_DRAW);

    GLCP::GenBuffers(1, &mBoundingboxVertexBuffer);
    GLCP::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBoundingboxVertexBuffer);
    GLCP::BufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(bbWireIndices), (void*)bbWireIndices, GL_STATIC_DRAW);

    GLCP::EnableVertexAttribArray(mVtxAttrib);
    GLCP::VertexAttribPointer(mVtxAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
    GLCP::BindVertexArray(0);


    GLCP::GenBuffers(1, &mSoleVertexBuffer);
    GLCP::BindBuffer(GL_ARRAY_BUFFER, mSoleVertexBuffer);
    GLCP::BufferData(GL_ARRAY_BUFFER, sizeof(sole), (void*)sole, GL_STATIC_DRAW);

    GLCP::GenBuffers(1, &mHeelVertexBuffer);
    GLCP::BindBuffer(GL_ARRAY_BUFFER, mHeelVertexBuffer);
    GLCP::BufferData(GL_ARRAY_BUFFER, sizeof(heel), (void*)heel, GL_STATIC_DRAW);

    GLCP::GenBuffers(1, &mSoleWireIndexBuffer);
    GLCP::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, mSoleWireIndexBuffer);
    GLCP::BufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(soleWireIndices), (void*)soleWireIndices, GL_STATIC_DRAW);

    GLCP::GenBuffers(1, &mHeelWireIndexBuffer);
    GLCP::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, mHeelWireIndexBuffer);
    GLCP::BufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(heelWireIndices), (void*)heelWireIndices, GL_STATIC_DRAW);

    GLCP::GenBuffers(1, &mSoleShadedIndexBuffer);
    GLCP::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, mSoleShadedIndexBuffer);
    GLCP::BufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(soleShadedIndices), (void*)soleShadedIndices, GL_STATIC_DRAW);

    GLCP::GenBuffers(1, &mHeelShadedIndexBuffer);
    GLCP::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, mHeelShadedIndexBuffer);
    GLCP::BufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(heelShadedIndices), (void*)heelShadedIndices, GL_STATIC_DRAW);

    GLCP::UseProgram(mShaderProgram);

    // Setup ALL VAO Combinations here for further usage:
    GLCP::GenVertexArrays(1, &mSoleWireframeVAO);
    GLCP::BindVertexArray(mSoleWireframeVAO);
    GLCP::BindBuffer(GL_ARRAY_BUFFER, mSoleVertexBuffer);
    GLCP::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, mSoleWireIndexBuffer);
    GLCP::EnableVertexAttribArray(mVtxAttrib);
    GLCP::VertexAttribPointer(mVtxAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
    GLCP::BindVertexArray(0);

    GLCP::GenVertexArrays(1, &mHeelWireframeVAO);
    GLCP::BindVertexArray(mHeelWireframeVAO);
    GLCP::BindBuffer(GL_ARRAY_BUFFER, mHeelVertexBuffer);
    GLCP::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, mHeelWireIndexBuffer);
    GLCP::EnableVertexAttribArray(mVtxAttrib);
    GLCP::VertexAttribPointer(mVtxAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
    GLCP::BindVertexArray(0);

    GLCP::GenVertexArrays(1, &mSoleShadedVAO);
    GLCP::BindVertexArray(mSoleShadedVAO);
    GLCP::BindBuffer(GL_ARRAY_BUFFER, mSoleVertexBuffer);
    GLCP::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, mSoleShadedIndexBuffer);
    GLCP::EnableVertexAttribArray(mVtxAttrib);
    GLCP::VertexAttribPointer(mVtxAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
    GLCP::BindVertexArray(0);

    GLCP::GenVertexArrays(1, &mHeelShadedVAO);
    GLCP::BindVertexArray(mHeelShadedVAO);
    GLCP::BindBuffer(GL_ARRAY_BUFFER, mHeelVertexBuffer);
    GLCP::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, mHeelShadedIndexBuffer);
    GLCP::EnableVertexAttribArray(mVtxAttrib);
    GLCP::VertexAttribPointer(mVtxAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
    GLCP::BindVertexArray(0);

    GLCP::UseProgram(0);
    GLCP::BindBuffer(GL_ARRAY_BUFFER, 0);
    GLCP::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return true;
}

bool RawFootPrintDrawAgentCoreProfile::releaseCoreProfileResources()
{
    if (mShaderProgram)
    {
        GLCP::DeleteProgram(mShaderProgram);
        mShaderProgram = 0;
    }

    if (mBBoxVAO)
    {
        GLCP::DeleteVertexArrays(1, &mBBoxVAO);
        mBBoxVAO = 0;
    }

    if (mSoleWireframeVAO)
    {
        GLCP::DeleteVertexArrays(1, &mSoleWireframeVAO);
        mSoleWireframeVAO = 0;
    }

    if (mHeelWireframeVAO)
    {
        GLCP::DeleteVertexArrays(1, &mHeelWireframeVAO);
        mHeelWireframeVAO = 0;
    }

    if (mSoleShadedVAO)
    {
        GLCP::DeleteVertexArrays(1, &mSoleShadedVAO);
        mSoleShadedVAO = 0;
    }

    if (mHeelShadedVAO)
    {
        GLCP::DeleteVertexArrays(1, &mHeelShadedVAO);
        mHeelShadedVAO = 0;
    }

    if (mBoundingboxVertexBuffer)
    {
        GLCP::DeleteBuffers(1, &mBoundingboxVertexBuffer);
        mBoundingboxVertexBuffer = 0;
    }

    if (mBoundingboxIndexBuffer)
    {
        GLCP::DeleteBuffers(1, &mBoundingboxIndexBuffer);
        mBoundingboxIndexBuffer = 0;
    }

    if (mSoleVertexBuffer)
    {
        GLCP::DeleteBuffers(1, &mSoleVertexBuffer);
        mSoleVertexBuffer = 0;
    }

    if (mHeelVertexBuffer)
    {
        GLCP::DeleteBuffers(1, &mHeelVertexBuffer);
        mHeelVertexBuffer = 0;
    }

    if (mSoleWireIndexBuffer)
    {
        GLCP::DeleteBuffers(1, &mSoleWireIndexBuffer);
        mSoleWireIndexBuffer = 0;
    }

    if (mSoleShadedIndexBuffer)
    {
        GLCP::DeleteBuffers(1, &mSoleShadedIndexBuffer);
        mSoleShadedIndexBuffer = 0;
    }

    if (mHeelWireIndexBuffer)
    {
        GLCP::DeleteBuffers(1, &mHeelWireIndexBuffer);
        mHeelWireIndexBuffer = 0;
    }

    if (mHeelShadedIndexBuffer)
    {
        GLCP::DeleteBuffers(1, &mHeelShadedIndexBuffer);
        mHeelShadedIndexBuffer = 0;
    }

    return true;
}


// DX stuff
#ifdef _WIN32

bool RawFootPrintDrawAgentDX::initShadersDX()
{
    assert( mDevicePtr );
    if ( !mDevicePtr )
        return false;

    HRESULT hr;
    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
    ID3DBlob* vsBlob = NULL;
    ID3DBlob* psBlob = NULL;
    ID3DBlob* pErrorBlob;
    

    // VS
    if (!mVertexShaderPtr)
    {
#if _MSC_VER < 1700
        hr = D3DX11CompileFromFile(
            mEffectLocation.asChar(),
            NULL,
            NULL,
            "mainVS",
            "vs_5_0",
            dwShaderFlags,
            0,
            NULL,
            &vsBlob,
            &pErrorBlob,
            NULL);
#else
        hr = D3DCompileFromFile(
            mEffectLocation.asWChar(),
            NULL,
            NULL,
            "mainVS",
            "vs_5_0",
            dwShaderFlags,
            0,
            &vsBlob,
            &pErrorBlob);
#endif
        if (FAILED(hr))
        {
            printf("Failed to compile vertex shader\n");
            if (pErrorBlob) pErrorBlob->Release();
            return false;
        }
        if (pErrorBlob) pErrorBlob->Release();
        hr = mDevicePtr->CreateVertexShader(
            vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), NULL, &mVertexShaderPtr);
        if (FAILED(hr))
        {
            printf("Failed to create vertex shader\n");
            vsBlob->Release();
            return false;
        }
    }

    // Layout
    if (!mVertexLayoutPtr)
    {
        D3D11_INPUT_ELEMENT_DESC layout[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };
        int numLayoutElements = sizeof layout/sizeof layout[0];
        hr = mDevicePtr->CreateInputLayout(
            layout, numLayoutElements, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &mVertexLayoutPtr);
        vsBlob->Release();
        if (FAILED(hr))
        {
            printf("Failed to create input layout\n");
            return false;
        }
    }

    // PS
    if (!mPixelShaderPtr)
    {
#if _MSC_VER < 1700
        hr = D3DX11CompileFromFile(
            mEffectLocation.asChar(),
            NULL,
            NULL,
            "mainPS",
            "ps_5_0",
            dwShaderFlags,
            0,
            NULL,
            &psBlob,
            &pErrorBlob,
            NULL);
#else
        hr = D3DCompileFromFile(
            mEffectLocation.asWChar(),
            NULL,
            NULL,
            "mainPS",
            "ps_5_0",
            dwShaderFlags,
            0,
            &psBlob,
            &pErrorBlob);
#endif
        if (FAILED(hr))
        {
            printf("Failed to compile vertex shader\n");
            mVertexShaderPtr->Release();
            mVertexLayoutPtr->Release();
            if (pErrorBlob) pErrorBlob->Release();
            return false;
        }
        if (pErrorBlob) pErrorBlob->Release();
        hr = mDevicePtr->CreatePixelShader(
            psBlob->GetBufferPointer(), psBlob->GetBufferSize(), NULL, &mPixelShaderPtr);
        psBlob->Release();
        if (FAILED(hr))
        {
            printf("Failed to create pixel shader\n");
            mVertexShaderPtr->Release();
            mVertexLayoutPtr->Release();
            return false;
        }
    }

    return true;
}

bool RawFootPrintDrawAgentDX::initBuffersDX()
{
    assert( mDevicePtr );
    if ( !mDevicePtr )
        return false;

    HRESULT hr;
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));


    if (!mBoundingboxVertexBufferPtr)
    {
        bd.Usage = D3D11_USAGE_IMMUTABLE;
        bd.ByteWidth = sizeof(float) * 3 * 8;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = 0;
        InitData.pSysMem = bbData;
        hr = mDevicePtr->CreateBuffer(&bd, &InitData, &mBoundingboxVertexBufferPtr);
        if (FAILED(hr)) return false;
    }
    if( !mBoundingboxIndexBufferPtr ){
        bd.Usage = D3D11_USAGE_IMMUTABLE;
        bd.ByteWidth = sizeof(unsigned short) * 2 * 12;
        bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
        bd.CPUAccessFlags = 0;
        InitData.pSysMem = bbWireIndices;
        hr = mDevicePtr->CreateBuffer(&bd, &InitData, &mBoundingboxIndexBufferPtr);
        if (FAILED(hr)) return false;
    }

    // Sole positions
    if (!mSoleVertexBufferPtr[0])
    {
        bd.Usage = D3D11_USAGE_IMMUTABLE;
        bd.ByteWidth = sizeof(float) * 3 * soleCount;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = 0;
        InitData.pSysMem = sole;
        hr = mDevicePtr->CreateBuffer(&bd, &InitData, &mSoleVertexBufferPtr[0]);
        if (FAILED(hr)) return false;
    }
    // Sole normals
    if (!mSoleVertexBufferPtr[1])
    {
        bd.Usage = D3D11_USAGE_IMMUTABLE;
        bd.ByteWidth = sizeof(float) * 3 * soleCount;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = 0;
        InitData.pSysMem = soleNorm;
        hr = mDevicePtr->CreateBuffer(&bd, &InitData, &mSoleVertexBufferPtr[1]);
        if (FAILED(hr)) return false;
    }
    // Heal positions
    if (!mHeelVertexBufferPtr[0])
    {
        bd.Usage = D3D11_USAGE_IMMUTABLE;
        bd.ByteWidth = sizeof(float) * 3 * heelCount;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = 0;
        InitData.pSysMem = heel;
        hr = mDevicePtr->CreateBuffer(&bd, &InitData, &mHeelVertexBufferPtr[0]);
        if (FAILED(hr)) return false;
    }
    // Heal normals
    if (!mHeelVertexBufferPtr[1])
    {
        bd.Usage = D3D11_USAGE_IMMUTABLE;
        bd.ByteWidth = sizeof(float) * 3 * heelCount;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = 0;
        InitData.pSysMem = heelNorm;
        hr = mDevicePtr->CreateBuffer(&bd, &InitData, &mHeelVertexBufferPtr[1]);
        if (FAILED(hr)) return false;
    }
    if (!mSoleWireIndexBufferPtr)
    {
        bd.Usage = D3D11_USAGE_IMMUTABLE;
        bd.ByteWidth = sizeof(unsigned short) * 2 * (soleCount-1);
        bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
        bd.CPUAccessFlags = 0;
        InitData.pSysMem = soleWireIndices;
        hr = mDevicePtr->CreateBuffer(&bd, &InitData, &mSoleWireIndexBufferPtr);
        if (FAILED(hr)) return false;
    }
    if (!mHeelWireIndexBufferPtr)
    {
        bd.Usage = D3D11_USAGE_IMMUTABLE;
        bd.ByteWidth = sizeof(unsigned short) * 2 * (heelCount-1);
        bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
        bd.CPUAccessFlags = 0;
        InitData.pSysMem = heelWireIndices;
        hr = mDevicePtr->CreateBuffer(&bd, &InitData, &mHeelWireIndexBufferPtr);
        if (FAILED(hr)) return false;
    }
    if (!mSoleShadedIndexBufferPtr)
    {
        bd.Usage = D3D11_USAGE_IMMUTABLE;
        bd.ByteWidth = sizeof(unsigned short) * 3 * (soleCount-2);
        bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
        bd.CPUAccessFlags = 0;
        InitData.pSysMem = soleShadedIndices;
        hr = mDevicePtr->CreateBuffer(&bd, &InitData, &mSoleShadedIndexBufferPtr);
        if (FAILED(hr)) return false;
    }
    if (!mHeelShadedIndexBufferPtr)
    {
        bd.Usage = D3D11_USAGE_IMMUTABLE;
        bd.ByteWidth = sizeof(unsigned short) * 3 * (heelCount-2);
        bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
        bd.CPUAccessFlags = 0;
        InitData.pSysMem = heelShadedIndices;
        hr = mDevicePtr->CreateBuffer(&bd, &InitData, &mHeelShadedIndexBufferPtr);
        if (FAILED(hr)) return false;
    }
    if (!mConstantBufferPtr)
    {
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(ConstantBufferDef);
        bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        bd.CPUAccessFlags = 0;
        hr = mDevicePtr->CreateBuffer(&bd, NULL, &mConstantBufferPtr);
        if (FAILED(hr)) return false;
    }

    return true;
}

bool RawFootPrintDrawAgentDX::releaseDXResources()
{
    if (mBoundingboxVertexBufferPtr)
    {
        mBoundingboxVertexBufferPtr->Release();
        mBoundingboxVertexBufferPtr = NULL;
    }
    if (mBoundingboxIndexBufferPtr)
    {
        mBoundingboxIndexBufferPtr->Release();
        mBoundingboxIndexBufferPtr = NULL;
    }

    if (mSoleVertexBufferPtr[0])
    {
        mSoleVertexBufferPtr[0]->Release();
        mSoleVertexBufferPtr[0] = NULL;
    }
    if (mSoleVertexBufferPtr[1])
    {
        mSoleVertexBufferPtr[1]->Release();
        mSoleVertexBufferPtr[1] = NULL;
    }
    if (mHeelVertexBufferPtr[0])
    {
        mHeelVertexBufferPtr[0]->Release();
        mHeelVertexBufferPtr[0] = NULL;
    }
    if (mHeelVertexBufferPtr[1])
    {
        mHeelVertexBufferPtr[1]->Release();
        mHeelVertexBufferPtr[1] = NULL;
    }
    if (mSoleWireIndexBufferPtr)
    {
        mSoleWireIndexBufferPtr->Release();
        mSoleWireIndexBufferPtr = NULL;
    }
    if (mSoleShadedIndexBufferPtr)
    {
        mSoleShadedIndexBufferPtr->Release();
        mSoleShadedIndexBufferPtr = NULL;
    }
    if (mHeelWireIndexBufferPtr)
    {
        mHeelWireIndexBufferPtr->Release();
        mHeelWireIndexBufferPtr = NULL;
    }
    if (mHeelShadedIndexBufferPtr)
    {
        mHeelShadedIndexBufferPtr->Release();
        mHeelShadedIndexBufferPtr = NULL;
    }
    if (mVertexShaderPtr)
    {
        mVertexShaderPtr->Release();
        mVertexShaderPtr = NULL;
    }
    if (mPixelShaderPtr)
    {
        mPixelShaderPtr->Release();
        mPixelShaderPtr = NULL;
    }
    if (mVertexLayoutPtr)
    {
        mVertexLayoutPtr->Release();
        mVertexLayoutPtr = NULL;
    }
    if (mConstantBufferPtr)
    {
        mConstantBufferPtr->Release();
        mConstantBufferPtr = NULL;
    }

    return true;
}

#endif // _WIN32

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Plugin Registration
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

MStatus rawfootPrint::initialize()
{
    MFnUnitAttribute unitFn;
    MFnNumericAttribute numberFn;
    
    MStatus			 stat;

    size = unitFn.create( "size", "sz", MFnUnitAttribute::kDistance );
    unitFn.setDefault( 1.0 );

    stat = addAttribute( size );
    if (!stat) {
        stat.perror("addAttribute");
        return stat;
    }
    
    transparencySort = numberFn.create( "transparencySort", "ts", MFnNumericData::kBoolean );
    numberFn.setDefault( false );
    
    stat = addAttribute( transparencySort );
    if (!stat) {
        stat.perror("addAttribute");
        return stat;
    }
    
    transparency = numberFn.create( "transparency", "t", MFnNumericData::kFloat );
    numberFn.setDefault( 1.0 );
    numberFn.setMax( 1.0 );
    numberFn.setMin( 0.0 );
    
    stat = addAttribute( transparency );
    if (!stat) {
        stat.perror("addAttribute");
        return stat;
    }

    // Add dependency on the localScale attribute, so that when size is modified RawFootPrintDrawOverride::transform will get called
    attributeAffects( rawfootPrint::size, MPxLocatorNode::localScale );

    return MS::kSuccess;
}

MStatus initializePlugin( MObject obj )
{
    MStatus   status;
    MFnPlugin plugin( obj, PLUGIN_COMPANY, "3.0", "Any");

    status = plugin.registerNode(
                "rawfootPrint",
                rawfootPrint::id,
                &rawfootPrint::creator,
                &rawfootPrint::initialize,
                MPxNode::kLocatorNode,
                &rawfootPrint::drawDbClassification);
    if (!status) {
        status.perror("registerNode");
        return status;
    }

    status = MHWRender::MDrawRegistry::registerDrawOverrideCreator(
        rawfootPrint::drawDbClassification,
        rawfootPrint::drawRegistrantId,
        RawFootPrintDrawOverride::Creator);
    if (!status) {
        status.perror("registerDrawOverrideCreator");
        return status;
    }

    return status;
}

MStatus uninitializePlugin( MObject obj)
{
    MStatus   status;
    MFnPlugin plugin( obj );

    status = MHWRender::MDrawRegistry::deregisterDrawOverrideCreator(
        rawfootPrint::drawDbClassification,
        rawfootPrint::drawRegistrantId);
    if (!status) {
        status.perror("deregisterDrawOverrideCreator");
        return status;
    }

    status = plugin.deregisterNode( rawfootPrint::id );
    if (!status) {
        status.perror("deregisterNode");
        return status;
    }

    // Release DX resources
#ifdef _WIN32
    {
        RawFootPrintDrawAgentDX& drawAgentRef = RawFootPrintDrawAgentDX::getDrawAgent();
        drawAgentRef.releaseDXResources();
    }
#endif
    // Release GL Core resources
    {
        RawFootPrintDrawAgentCoreProfile& drawAgentRef = RawFootPrintDrawAgentCoreProfile::getDrawAgent();
        drawAgentRef.releaseCoreProfileResources();
    }

    return status;
}
