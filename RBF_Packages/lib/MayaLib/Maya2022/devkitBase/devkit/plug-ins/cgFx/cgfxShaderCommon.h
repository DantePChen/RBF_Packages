//
// Copyright (C) 2002-2003 NVIDIA 
// 
// File: cgfxShaderCommon.h

//-
// ==========================================================================
// Copyright 2020 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
// ==========================================================================
//+

#ifndef _cgfxShaderCommon_h_
#define _cgfxShaderCommon_h_

#ifdef _WIN32
#  pragma warning(disable: 4786)
#endif

// This header file simply defines some things that I
// want to use throughout the plug-in
//


// If assertion is false, throw an InternalError exception.
// Note: A Maya MStatus object can be used as the assertion... true means success.
#define M_CHECK(assertion)  if (assertion) ; else throw ((cgfxShaderCommon::InternalError*)__LINE__)

namespace cgfxShaderCommon
{
	struct InternalError
	{
		char* message;
	};
	//   throw (InternalError*)__LINE__;
}


#define RETURNSTAT(s, msg)	\
	if (!s)					\
{						\
	s.perror(msg);		\
	return s;			\
}


#define lengthof(array) (sizeof(array) / sizeof(array[0]))

#if !defined(TEXTURES_BY_NAME) && !defined(TEXTURES_BY_NODE)
#  define TEXTURES_BY_NODE 1
#endif

#if defined(MAYA_PRINT_DEBUG_INFO) && defined(_WIN32) && defined(_DEBUG)
#  ifndef _CRTDBG_MAP_ALLOC
#    define _CRTDBG_MAP_ALLOC
#  endif
#endif /* MAYA_PRINT_DEBUG_INFO && _WIN32 && _DEBUG */


#include <stdlib.h>

#if defined(_WIN32)
// We must include <stdlib.h> before <crtdbg.h> or we get
// errors about overloading calloc.
//
#  include <crtdbg.h>
#endif /* _WIN32 */

#if defined(MAYA_PRINT_DEBUG_INFO)
#  define OutputDebugString(s)			fprintf(stderr, "%s", s)
#  define OutputDebugStrings(s1, s2)	fprintf(stderr, "%s%s\n", s1, s2);
#else
#  if defined (_WIN32)
// In optimized mode, send the string to the debugger
#     define OutputDebugStrings(s1, s2) \
	(OutputDebugString(s1), OutputDebugString(s2), OutputDebugString("\n"))
#  else
#    define OutputDebugString(s)		((void) 0)
#    define OutputDebugStrings(s1, s2)	((void) 0)
#  endif
#endif /* MAYA_PRINT_DEBUG_INFO */

// Return true if item is found in Maya array.
template < typename Tarray, typename Titem >
bool
arrayContains( const Tarray& array, const Titem& item )
{
	int i;
	for ( i = array.length(); i > 0; --i )
		if ( array[ i - 1 ] == item )
			break;
	return i > 0;
}


// Append item to Maya array if not already present.
// Returns index of the new or existing array element.
template < typename Tarray, typename Titem >
int
findOrAppend( Tarray& array, const Titem& item )
{
	int i;
	int n = array.length();
	for ( i = 0; i < n; ++i )
		if ( array[ i ] == item )
			break;
	if ( i == n )
		array.append( item );
	return i;
}

#ifndef CGFXSHADER_VERSION  
#define CGFXSHADER_VERSION  "4.4"
#endif

#include <maya/MObject.h>
#include <maya/MString.h>
#include <maya/MStringArray.h>
#include <maya/MTypes.h>

#if !defined(_WIN32) && !defined(GL_GLEXT_PROTOTYPES)
	#define GL_GLEXT_PROTOTYPES
#endif

#if defined(__APPLE__)
#include <GL/glew.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#elif defined(__linux__) || defined(__linux__)
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#elif defined(_WIN32)
#include <GL/glew.h>
#include <gl/Gl.h>
#include <gl/Glu.h>
#else
#error Unknown OS
#endif

#include <Cg/cg.h>
#include <Cg/cgGL.h>


//
// The gl Registers
//
class glRegister
{
public:
	enum
	{
		kUnknown,
		kPosition,
		kVertexWeight,
		kNormal,
		kColor,
		kSecondaryColor,
		kFogCoord,
		kTexCoord,
		kLastTexCoord = kTexCoord + 7,
		kVertexAttrib,
		kLastVertexAttrib = kVertexAttrib + 15,
		kLast
	};
};


//
// A wee cache to minimise our gl state changes
//
class glStateCache
{
public:
				glStateCache();
	static 	glStateCache& instance() { return gInstance; }
	inline void reset() { fRequiredRegisters = 0; fEnabledRegisters = 0; fActiveTextureUnit = -1; }

	void		flushState();
	inline void disableAll();

	inline void enablePosition();
	inline void enableNormal();
	inline void disableNormal();
	inline void enableColor();
	inline void enableSecondaryColor();
	void		activeTexture( int i);
	inline void enableAndActivateTexCoord( int i);
	void		enableVertexAttrib( int i);

	static int	sMaxTextureUnits;

private:

	static glStateCache gInstance;
	long	fRequiredRegisters;
	long	fEnabledRegisters;
	int		fActiveTextureUnit;
};

inline void glStateCache::disableAll() { fRequiredRegisters = 0; flushState(); }

inline void glStateCache::enablePosition() { if( !(fEnabledRegisters & (1 << glRegister::kPosition))) { glEnableClientState(GL_VERTEX_ARRAY); fEnabledRegisters |= (1 << glRegister::kPosition); } fRequiredRegisters |= (1 << glRegister::kPosition); }
inline void glStateCache::enableNormal() { if( !(fEnabledRegisters & (1 << glRegister::kNormal))) { glEnableClientState(GL_NORMAL_ARRAY); fEnabledRegisters |= (1 << glRegister::kNormal); } fRequiredRegisters |= (1 << glRegister::kNormal); }
inline void glStateCache::disableNormal() { if( fEnabledRegisters & (1 << glRegister::kNormal)) { glDisableClientState(GL_NORMAL_ARRAY); fEnabledRegisters &= ~(1 << glRegister::kNormal); } fRequiredRegisters &= ~(1 << glRegister::kNormal); }
inline void glStateCache::enableColor() { if( !(fEnabledRegisters & (1 << glRegister::kColor))) { glEnableClientState(GL_COLOR_ARRAY); fEnabledRegisters |= (1 << glRegister::kColor); }  fRequiredRegisters |= (1 << glRegister::kColor); }
inline void glStateCache::enableSecondaryColor() { if( !(fEnabledRegisters & (1 << glRegister::kSecondaryColor))) { glEnableClientState(GL_SECONDARY_COLOR_ARRAY_EXT); fEnabledRegisters |= (1 << glRegister::kSecondaryColor); }  fRequiredRegisters |= (1 << glRegister::kSecondaryColor); }
inline void glStateCache::enableAndActivateTexCoord( int i) { activeTexture( i); if( !(fEnabledRegisters & (1 << (glRegister::kTexCoord + i)))) { glEnableClientState(GL_TEXTURE_COORD_ARRAY); fEnabledRegisters |= (1 << (glRegister::kTexCoord + i)); } fRequiredRegisters |= (1 << (glRegister::kTexCoord + i)); }

#endif /* _cgfxShaderCommon_h */
