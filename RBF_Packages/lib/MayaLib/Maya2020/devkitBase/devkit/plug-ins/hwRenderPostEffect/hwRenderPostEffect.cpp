//-
// ==========================================================================
// Copyright 2015 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
// ==========================================================================
//+

///////////////////////////////////////////////////////////////////
//
// DESCRIPTION: 
//		This is an example of adding processing
//		effects to the hardware renderer.
//		Simple operations such as inverting the image and adding color triangles to the view
//		 are performed by this plug-in.
//
//		To use this shader, load the plug-in and turn on the Legacy High Quality Viewport. 
//
///////////////////////////////////////////////////////////////////

#ifdef WIN32
#pragma warning( disable : 4786 )		// Disable STL warnings.
#endif

#include <maya/MIOStream.h> 
#include <maya/MString.h>
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFloatVector.h>
#include <maya/MFnPlugin.h>


#include <maya/MDrawProcedureBase.h>
#include <maya/MHardwareRenderer.h>
#include <maya/MGL.h>

#include <stdio.h>

class hwRenderEffect : public MDrawProcedureBase
{
public:
	hwRenderEffect();
	~hwRenderEffect() override;

	bool execute() override;

	static void deleteEffect( hwRenderEffect * ePtr);	
	static MDrawProcedureBase *createEffect( const MString &name, 
							hwRenderEffect *ePtr,
							MHardwareRenderer::CallLocation location);


	MHardwareRenderer::CallLocation mLocation;
};


hwRenderEffect::hwRenderEffect()
{
}

hwRenderEffect::~hwRenderEffect()
{
	// No-op
}

void
hwRenderEffect::deleteEffect( hwRenderEffect * ePtr) 
{
	if (ePtr)
	{
		// Deprecated Methods: MHardwareRenderer::theRenderer(), pRenderer->backEndString() 
		_OPENMAYA_DEPRECATION_PUSH_AND_DISABLE_WARNING
		MHardwareRenderer *pRenderer = MHardwareRenderer::theRenderer();
		if (pRenderer)
		{
			const MString & backEndStr = pRenderer->backEndString();
			MHardwareRenderer::DrawProcedureStatusCode result = 
				pRenderer->removeDrawProcedure( backEndStr, ePtr, ePtr->mLocation );
			cout<<"Call hwRenderEffect::deleteEffect("<<ePtr->fName.asChar()
				<<") loc("<<ePtr->mLocation
				<<") = "<<result<<endl;
		}
		delete ePtr;
		ePtr = 0;
		_OPENMAYA_POP_WARNING
	}
}

/*static*/
MDrawProcedureBase *
hwRenderEffect::createEffect( const MString &name, 
							hwRenderEffect *ePtr,
							MHardwareRenderer::CallLocation location)
{

	// Deprecated Methods: MHardwareRenderer::theRenderer(), pRenderer->backEndString() 
	_OPENMAYA_DEPRECATION_PUSH_AND_DISABLE_WARNING
	MHardwareRenderer *pRenderer = MHardwareRenderer::theRenderer();
	if (pRenderer)
	{
		if (!ePtr)
		{
			ePtr = new hwRenderEffect();
			ePtr->setEnabled( true );
			ePtr->setName( name );	
			ePtr->mLocation = location;

			const MString & backEndStr = pRenderer->backEndString();
			MHardwareRenderer::DrawProcedureStatusCode result = 
				pRenderer->addDrawProcedure( backEndStr, ePtr, location );
			cout<<"Call hwRenderEffect::createEffect("<<ePtr->name().asChar()
				<<") loc("<<ePtr->mLocation
				<<") = "<<result<<endl;
		}
	}
	_OPENMAYA_POP_WARNING
	return (MDrawProcedureBase *)ePtr;
}

/* virtual */
bool
hwRenderEffect::execute()
{
	// Deprecated Methods: MHardwareRenderer::theRenderer(), pRenderer->backEndString() 
	_OPENMAYA_DEPRECATION_PUSH_AND_DISABLE_WARNING
	MHardwareRenderer *pRenderer = MHardwareRenderer::theRenderer();
	if (!pRenderer)
		return false;

	const MString & backEndStr = pRenderer->backEndString();
	unsigned int width = 0, height = 0;

	MStatus stat = 
		pRenderer->getBufferSize( backEndStr, width, height );
	if (width <= 0 || height <= 0)
		return false;

	unsigned int exposureNum;
	unsigned int totalExposureCount;
	pRenderer->getCurrentExposureNumber( backEndStr, exposureNum );
	pRenderer->getTotalExposureCount( backEndStr, totalExposureCount );

	// Perform some post-rendering. Invert the image.
	bool xxxx = true;
	if ( xxxx && mLocation == MHardwareRenderer::kPostRendering )
	{
		cout<<"Call EXP["<<exposureNum<<"/"<<totalExposureCount
			<<"] hwRenderEffect::execute("<<width<<", "<<height
			<<") -- "<<fName.asChar()<<endl;

		MHardwareRenderer::BufferPixelFormat colFmt;
		stat = pRenderer->getColorBufferPixelFormat( backEndStr, colFmt );
		void *pixels = 0;
		unsigned int data_type = GL_UNSIGNED_BYTE; 
		unsigned int format = GL_RGBA;

		unsigned int totalSize = width*height*4;

		if (totalSize == 0)
			return false;

		if (colFmt == MHardwareRenderer::kRGBA_Fix8)
		{
			data_type = GL_UNSIGNED_BYTE; 
			format = GL_RGBA;
			pixels = new unsigned char[totalSize];
		}
#ifdef _SUPPORT_16_FLOAT_
		else if (MHardwareRenderer::kRGBA_Float16)
		{		
			data_type = GL_HALF_FLOAT; 
			format = GL_RGBA;
			pixels = new half[totalSize];
		}
#endif
		glPushAttrib(GL_ALL_ATTRIB_BITS); 

		glMatrixMode(GL_PROJECTION);	
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glPixelZoom( 1.0f, 1.0f );
		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glReadPixels(0, 0, width, height, format, data_type, pixels);

		unsigned char *pixelPtr = (unsigned char*)pixels;
		if (pixelPtr)
		{
			unsigned int numPixels = width * height;
			for (unsigned int i=0; i < numPixels; i++)
			{
				*pixelPtr = (255 - *pixelPtr);	
				pixelPtr++;
				*pixelPtr = (255 - *pixelPtr);	
				pixelPtr++;			
				*pixelPtr = (255 - *pixelPtr);	
				pixelPtr++;
				*pixelPtr = 255;	
				pixelPtr++;
			}
		}

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glRasterPos2i( -1, -1 );

		// We only want to clobber RGB. Leave alpha as is.
		glColorMask( 1, 1, 1, 0 );

		// For orthographic views, we need to make sure that depth
		// test is disabled, otherwise the pixels will not draw.
		glDisable(GL_DEPTH_TEST);

		glDrawPixels(width, height, format, data_type, pixels);	

		if (pixels)
		{
			if (colFmt == MHardwareRenderer::kRGBA_Fix8)
			{
				unsigned char *pixelPtr2 = (unsigned char*)pixels;
				delete[] pixelPtr2;
			}
#ifdef _SUPPORT_16_FLOAT_
			else
			{
				half pixelPtr2 = (half*)pixels;
				delete[] pixelPtr2;
			}
#endif
		}


		glPopAttrib();
	}
	else
	{
		// Perform some pre-rendering, pre-exposure. Draw coloured
		// triangles.
		if ( (mLocation == MHardwareRenderer::kPreRendering ) ||
			 ( mLocation == MHardwareRenderer::kPreExposure) )
		{
			cout<<"Call EXP["<<exposureNum<<"/"<<totalExposureCount
				<<"] hwRenderEffect::execute("<<width<<", "<<height
				<<") -- "<<fName.asChar()<<endl;

			glPushAttrib(GL_ALL_ATTRIB_BITS); 

			glMatrixMode(GL_PROJECTION);	
			glLoadIdentity();
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			glShadeModel(GL_SMOOTH);
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ZERO);

			glColorMask( 1, 1, 1, 0 );
			glBegin(GL_QUADS);
			glColor4f(1.0f,1.0f,1.0f,0.5f);
			glVertex3f( -1.0f, 1.0f, 0.0f);
			glColor4f(0.0f,1.0f,1.0f,0.5f);
			glVertex3f( 0.0f, 1.0f, 0.0f);
			glColor4f(0.0f,1.0f,1.0f,0.5f);
			glVertex3f(-1.0f,-1.0f, 0.0f);
			glColor4f(1.0f,0.0f,1.0f,0.5f);
			glVertex3f( 1.0f,-1.0f, 0.0f);
			glEnd();

			glEnable(GL_DEPTH_TEST);
			glDisable(GL_BLEND);

			glPopAttrib();

		}

		// Do some post exposure. Nothing for now...
		else if ( mLocation == MHardwareRenderer::kPostExposure )
		{
			cout<<"Call EXP["<<exposureNum<<"/"<<totalExposureCount
				<<"] hwRenderEffect::execute("<<width<<", "<<height<<") -- POST-EXPOSURE"<<endl;
		}
	}
	_OPENMAYA_POP_WARNING

	return true;
}

// Four callback classes.
static hwRenderEffect *fsPreRender = 0;
static hwRenderEffect *fsPreExposure = 0;
static hwRenderEffect *fsPostExposure = 0;
static hwRenderEffect *fsPostRender = 0;

MStatus initializePlugin( MObject obj )
{ 
	MStatus   status = MS::kSuccess;
	
	MFnPlugin plugin( obj, PLUGIN_COMPANY, "7.0", "Any");

	// Register the callbacks.
	fsPreRender = (hwRenderEffect *)hwRenderEffect::createEffect(MString("PRE-RENDER"), fsPreRender, MHardwareRenderer::kPreRendering );
	fsPreExposure = (hwRenderEffect *)hwRenderEffect::createEffect(MString("PRE-EXPOSURE"), fsPreExposure, MHardwareRenderer::kPreExposure );
	fsPostExposure = (hwRenderEffect *)hwRenderEffect::createEffect(MString("POST-EXPOSURE"), fsPostExposure, MHardwareRenderer::kPostExposure );
	fsPostRender = (hwRenderEffect *)hwRenderEffect::createEffect(MString("POST-RENDER"), fsPostRender, MHardwareRenderer::kPostRendering );
	return status;
}

MStatus uninitializePlugin( MObject obj )
{
	MStatus   status = MS::kSuccess;
	
	// Unregister the callbacks.
	hwRenderEffect::deleteEffect(fsPreRender);
	fsPreRender = 0;
	hwRenderEffect::deleteEffect(fsPreExposure);
	fsPreExposure = 0;
	hwRenderEffect::deleteEffect(fsPostExposure);
	fsPostExposure = 0;
	hwRenderEffect::deleteEffect(fsPostRender);
	fsPostRender = 0;

	MFnPlugin plugin( obj );

	return status;
}
