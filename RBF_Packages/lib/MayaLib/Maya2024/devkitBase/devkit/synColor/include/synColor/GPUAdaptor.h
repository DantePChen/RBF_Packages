//
// ==================================================================
// Copyright 2014 Autodesk, Inc.  All rights reserved.
// 
// This computer source code  and related  instructions and comments are
// the unpublished confidential and proprietary information of Autodesk,
// Inc. and are  protected  under applicable  copyright and trade secret
// law. They may not  be disclosed to, copied or used by any third party
// without the prior written consent of Autodesk, Inc.
// ==================================================================
//
//! \file synColor/GPUAdaptor.h
//!
//! \brief GPU adaptor abstraction layer definition and smart pointer definition.
//!        See the SYNCOLOR::GPUAdaptor description for the usage pattern.
//!

#ifndef _SYNCOLOR_GPU_ADAPTOR_H
#define _SYNCOLOR_GPU_ADAPTOR_H

#include <synColor/synExport.h>
#include <synColor/synColorNamespace.h>

#include <synColor/synStatus.h>
#include <synColor/roi.h>
#include <synColor/dynamicProperty.h>


SYN_VISIBILITY_PUSH

namespace SYNCOLOR
{
  class GPUAdaptorPtr;

  //------------------------------------------------------------------------------------------------
  //
  //!
  //! When finalizing a color transform for rendering, SynColor can create a GPU renderer for the
  //! the transform.  When this is requested, a GPU shader program that implements the color transform
  //! rendering is built.  The shader can be applied to the active graphics frame buffer via the 
  //! SYNCOLOR::Transform::applyGPU call.
  //!
  //! To achieve this SynColor requires a GPU adaptor at transform finalization.  GPU adaptors
  //! for OpenGL using the GLSL or the Cg shader languages are available and a GPU adaptor for DirectX 11
  //! using HLSL is in the works.  If it is necessary to use a different graphics toolkit or if the
  //! adaptor behavior is not adequate for the intended application then a custom GPU adaptor needs
  //! to be written. The following information explains the calling sequence of adaptor methods.
  //! See each method description for details about their intent.
  //!
  //! __GPU adaptor method call sequence:__
  //!
  //! Setup phase methods:
  //!
  //! 		begin()                 // Perform any required set-up.
  //! 		releaseProgram()        // Clear any GPU resources held by the adaptor.
  //! 		acquireInputTexture()   // Obtain any input texture related resources.
  //! 		getInputTextureName()   // Obtain the string corresponding to the input texture resource name.
  //!
  //! For each color operator in the transform any of these methods could be called:
  //!
  //! 		getResourceNamePrefix() // Obtain a prefix to use for all GPU resources.
  //! 		getTextureDimensions()  // Calculate the width and height of a texture to hold the given texture size.
  //! 		acquireUniform()        // Create and store a uniform variable of the specified type.  See UniformType.
  //! 		acquire1DTexture()      // Create and store a linear texture.
  //! 		acquire2DTexture()      // Create and store a planar texture.
  //! 		acquire3DTexture()      // Create and store a volumetric texture.
  //!
  //! GPU shader program finalization methods:
  //!
  //! 		acquireProgram()      // Create and store a GPU program handle.
  //! 		buildProgram()        // Compile and link the shader program.
  //! 		end()                 // Perform any specific clean up.
  //!
  //! Method used when calling SYNCOLOR::Transform::applyGPU():
  //!
  //! 		drawQuad()            // Render into the currently active destination render buffer.
  //!
  //! All GPU adaptor methods are required to return a SYNCOLOR::SynStatus.  An error code
  //! other that SYNCOLOR::ERROR_NONE will cause the SYNCOLOR::finalize or SYNCOLOR::Transform::applyGPU
  //! calls to abort and return the error as well.   A special error code is reserved for GPU
  //! adaptor errors.  If the GPU adaptor method encounters an error, the SYNCOLOR::SynStatus
  //! usage pattern should be as follows:
  //!
  //! 		SYNCOLOR::SynStatus status; // Initialized to SYNCOLOR::ERROR_NONE.
  //! 		...
  //! 		if(<error condition>) {
  //! 		    status.setErrorCode(SYNCOLOR::ERROR_RENDERER_GPU_ADAPTOR_ERROR);
  //! 		    status.setErrorMessage(<error message string>);
  //! 		}
  //!
  //! 		return status;
  //!
  //! On the application side, the error message can be retrieved by calling SYNCOLOR::SynStatus::getErrorMessage().
  //!
  class SYN_EXPORT GPUAdaptor
  {
  public:
    //----------------------------------------------------------------------------------------------
    //! List of supported shader languages.
    //!
    enum ShaderLanguage
    {
      LANGUAGE_GLSL = 0,  //!< GLSL
      LANGUAGE_CG,        //!< Cg
      LANGUAGE_HLSL,      //!< HLSL
      LANGUAGE_GLSL_CORE  //!< GLSL Core profile
    };

    //----------------------------------------------------------------------------------------------
    //! List of supported GPU pipelines.
    //!
    enum PipelineType
    {
      PIPELINE_RGBA_FLOAT = 0 //!< Only floating point RGBA GPU pipelines are supported.
    };

    //----------------------------------------------------------------------------------------------
    //! List of supported uniform types.
    //!
    enum UniformType
    {
      UNIFORM_DOUBLE = 0,//!< Double.
      UNIFORM_BOOL       //!< Boolean.
    };
  
    //----------------------------------------------------------------------------------------------
  public:
    //! \brief Constructor.
    //!
    //! \param lang Adaptor shader language.
    //! \param pipeline Adaptor GPU pipeline type.
    //!
    GPUAdaptor(ShaderLanguage lang, PipelineType pipeline);

    //! \brief Destructor.
    //!
    virtual ~GPUAdaptor();

    //! \brief Adaptor unique ID
    //! 
    //! This function is used as a key in the SynColor caching system.   Each GPU adaptor
    //! instance should return a different ID.
    //!
    //! \return 
    virtual const char* getUniqueId() const = 0;
    
    //! \brief Clone the GPU Adaptor.
    //!
    //! This method needs to return a copy of 'this' GPUAdaptor.
    //!
    //! \return Cloned instance.
    virtual GPUAdaptorPtr clone() const  = 0; 

    //! \brief Perform any GPU toolkit specific set-up for creating shader programs.
    //!
    //! This method is always called before any other adaptor method when finalizing a color transform.
    //! The default implementation simply returns without error.
    //!
    //! \param uid Transform unique id. See Transfor::getUniqueId().
    //!
    //! \return Must return a SYNCOLOR::SynStatus.  See the GPUAdaptor class description for details.
    //!
    virtual SynStatus begin(const char* uid);

    //! \brief Perform any GPU toolkit specific cleanup following shader program creation.
    //!
    //! This method is the last one called when finalizing a transform.  It is always called when
    //! an adaptor method returns an error. The default implementation simply returns without error.
    //!
    //! \return Must return a SYNCOLOR::SynStatus.  See the GPUAdaptor class description for
    //!         details.
    //!
    virtual SynStatus end();

    //! \brief Prefix for uniquely identifying resources names.
    //!
    //! Some applications may require that textures, uniforms, and helper methods be uniquely
    //! named.  If this is the case the implementation of this method needs to return a prefix
    //! unique to the GPUAdaptor instance.
    //!
    //! A unique prefix would be necessary for example if the shader code from multiple color
    //! transform need to be integrated into a larger shader program.  In this case the resources
    //! need to be uniquely identified.
    //!
    //! \return Resource name prefix unique to the GPUAdaptor instance.
    //!
    virtual const char* getResourcesNamesPrefix() const = 0;

    //! \brief Create and store a GPU program handle.
    //!
    //! In the OpenGL model, this method corresponds to a call to glCreateProgram().
    //!
    //! \return Must return a SYNCOLOR::SynStatus.  See the GPUAdaptor class description for
    //!         details.
    virtual SynStatus acquireProgram() = 0;

    //! \brief Clear any GPU resources held by the adaptor.
    //!
    //! The intent of this method is to release any textures and uniforms created by the adaptor
    //! instance.  It also needs to delete the shader program.
    //!
    //! \return Must return a SYNCOLOR::SynStatus.  See the GPUAdaptor class description for
    //!         details.
    //!
    virtual SynStatus releaseProgram() = 0;

    //! \brief Build a shader program and finalize its associated resources.
    //!
    //! This method receives the shader program text pieces for compilation or integration
    //! into another shader program.  The pieces provided are the variable and resource
    //! declarations, the helper methods, and the color transform shader program header, body,
    //! and footer.
    //!
    //! An OpenGL implementation of this methods that produces a single shader program to render
    //! the color transform would for example concatenate the shader text pieces and build the
    //! shader program via glCompileShader() and glLinkProgram().
    //!
    //! \param shaderDeclarations Variable and resource declaration text.
    //! \param shaderHelperMethods Helper function definitions text.
    //! \param shaderMainHeader Color transform shader header text.
    //! \param shaderMainBody Color transform shader body text.
    //! \param shaderMainFooter Color transform shader text footer.
    //!
    //! \return Must return a SYNCOLOR::SynStatus.  See the GPUAdaptor class description for
    //!         details.
    virtual SynStatus buildProgram(const char* shaderDeclarations,
                                   const char* shaderHelperMethods,
                                   const char* shaderMainHeader,
                                   const char* shaderMainBody,
                                   const char* shaderMainFooter) = 0;

    //! \brief Create a uniform variable of the specified type.
    //!
    //! Uniforms in color transforms correspond to dynamic properties of the transforms such as
    //! exposure or contrast.  Changes to the values of these properties need to be updated prior
    //! to rendering the transform.  The GPU adaptor needs to retain the memory location of the
    //! uniform value and apply its content to the uniform in the drawQuad() method.  Dynamic property
    //! values are updated via the SYNCOLOR::Transform::setDynamicProperty call.
    //!
    //!
    //! \param id Dynamic property id for this uniform.  See DynamicProperty::Ids.
    //! \param name Uniform name.
    //! \param type Uniform type
    //! \param value Memory location of the uniform value. Corresponds to a pointer of the uniform type.
    //!
    //! \sa UniformType, drawQuad().
    //!
    //! \return Must return a SYNCOLOR::SynStatus.  See the GPUAdaptor class description for
    //!         details.
    //!
    virtual SynStatus acquireUniform(DynamicProperty::Ids id,
                                     const char* name,
                                     UniformType type,
                                     const void* value) = 0;

    //! \brief Calculate the width and height of a texture to hold the given texture size.
    //!
    //! When deciding whether to use a 1-d or 2-d texture to hold a look-up table, SynColor needs
    //! a hint.  The implementation of this method needs to provide,  based on the GPU toolkit or
    //! GPU device constraints, the width and height of a texture that will hold the specified
    //! number of values.
    //!
    //! If the height returned is 1, SynColor will request a 1-d texture via the acquire1DTexture()
    //! adaptor method.  Otherwise, acquire2DTexture() will be called to create a 2-d texture.
    //!
    //! \param numValues Number of values that the texture needs to hold.
    //! \param width Texture width return value.
    //! \param height Texture height return value.
    //!
    //! \return Must return a SYNCOLOR::SynStatus.  See the GPUAdaptor class description for
    //!         details.
    //!
    virtual SynStatus getTextureDimensions(unsigned numValues,
                                           unsigned& width, unsigned& height) = 0;

    //! \brief Create and store a linear texture.
    //!
    //! The GPU adaptor needs to retain the handle to the texture and release it in the releaseProgram() call.
    //!
    //! \param name Texture name.
    //! \param values Values to copied into the texture.
    //! \param size Number of texture values.
    //
    //! \return Must return a SYNCOLOR::SynStatus.  See the GPUAdaptor class description for
    //!         details.
    //!
    virtual SynStatus acquire1DTexture(const char* name, const void* values, unsigned size) = 0;

    //! \brief Create and store a planar texture.
    //!
    //! The GPU adaptor needs to retain the handle to the texture and release it in the releaseProgram() call.
    //!
    //! \param name Texture name.
    //! \param width Texture width.
    //! \param height Texture height.
    //! \param values Values to be copied into the texture.
    //
    //! \return Must return a SYNCOLOR::SynStatus.  See the GPUAdaptor class description for
    //!         details.
    //!
    virtual SynStatus acquire2DTexture(const char* name,
                                       unsigned width,
                                       unsigned height,
                                       const void* values) = 0;

    //! \brief Create and store a volumetric texture.
    //!
    //! The GPU adaptor needs to retain the handle to the texture and release it in the releaseProgram() call.
    //!
    //! \param name Texture name.
    //! \param dim Texture dimension.  The texture will be of size dim x dim x dim.
    //! \param linearInterpolation Whether linear interpolation of texel values is needed.
    //! \param values Values to be copied into the texture.
    //! \param size Values buffer size.
    //
    //! \return Must return a SYNCOLOR::SynStatus.  See the GPUAdaptor class description for
    //!         details.
    //!
    virtual SynStatus acquire3DTexture(const char* name,
                                       unsigned dim,
                                       bool linearInterpolation,
                                       const void* values,
                                       unsigned size) = 0;

    //! \brief Obtain any input texture related resources.
    //!
    //! This method needs to create the input texture name to be used in the shader program.
    //!
    //! \sa GPUAdaptor::getInputTextureName().
    //!
    //! \return Must return a SYNCOLOR::SynStatus.  See the GPUAdaptor class description for
    //!         details.
    //!
    virtual SynStatus acquireInputTexture() = 0;

    //! \brief Obtain the input texture name.
    //!
    //! The input texture name is needed for inclusion in the shader declaration text.
    //!
    //! \return Texture identification.
    //!
    virtual const char* getInputTextureName() const = 0;

    //! \brief Render into the currently active destination render buffer.
    //!
    //! This method is for applying the color transform shader on the input texture.
    //! The input texture provided is mapped to the input texture name supplied by
    //! getInputTextureName().
    //!
    //! \param inTexHandle Input texture handle.
    //! \param texCoords Input texture coordinates.
    //! \param screenCoords Destination screen coordinates.
    //
    //! \return Must return a SYNCOLOR::SynStatus.  See the GPUAdaptor class description for
    //!         details.
    //!
    virtual SynStatus drawQuad(unsigned inTexHandle, 
                               const SYNCOLOR::Vertices& texCoords, 
                               const SYNCOLOR::Vertices& screenCoords) const = 0;

    //! \brief Shader language configured.
    //!
    //! The shader language is specified at the constructor.
    //!
    //! \return Adaptor shader language.
    //!
    ShaderLanguage getShaderLanguage() const;

  protected:
    //! \brief GPU pipeline type configured.
    //!
    //! The pipeline type is specified at the constructor.
    //!
    //! \return Adaptor expected pipelined type.
    //!
    PipelineType _getPipelineType() const;

  private:
    ShaderLanguage  _lang;         //!< Shader language for constructing the color transform shader.
    PipelineType    _pipelineType; //!< GPU pipeline type for which the color transform shader is built.
  };

  //------------------------------------------------------------------------------------------------
  //! \brief GPU Adaptor smart pointer.
  //!
  //! Class that manages sharing of GPU adaptor instances.  Implements a shared pointer where only
  //! the last instance holding a specific GPUAdaptor instance will delete it.
  //!
  class SYN_EXPORT GPUAdaptorPtr
  {
  public:
    //! \brief Empty constructor.
    //!
    GPUAdaptorPtr();

    //! \brief Initialization Constructor.
    //!
    //! \param pAdaptor GPUAdaptor pointer to manage.
    //!
    explicit GPUAdaptorPtr(GPUAdaptor* pAdaptor);

    //! \brief Destructor.
    //!
    //! If no other GPUAdaptorPtr shares the underlying pointer, the GPUAdaptor is deleted.
    //!
    ~GPUAdaptorPtr();

    //! \brief Constant dereferencing operator.
    //!
    //! \return Pointer to the underlying non-modifiable GPUAdaptor instance.
    //!
    const GPUAdaptor* operator->() const;

    //! \brief Dereferencing operator.
    //!
    //! \return Pointer to the underlying GPUAdaptor instance.
    //!
    GPUAdaptor* operator->();

    //! \brief Get the constant instance pointer.
    //!
    //! \return Pointer to the underlying non-modifiable GPUAdaptor instance.
    //!
    const GPUAdaptor* get() const;

    //! \brief Get the instance pointer.
    //!
    //! \return Pointer to the underlying GPUAdaptor instance.
    //!
    GPUAdaptor* get();

    //! \brief Copy constructor.
    //!
    //! The resulting GPUAdaptorPtr shares the instance pointer with the
    //! GPUAdaptorPtr passed as argument.
    //!
    //! \param rhs GPUAdaptorPtr containing the pointer to share.
    //!
    GPUAdaptorPtr(const GPUAdaptorPtr& rhs);

    //! \brief Equality operator.
    //!
    //! The resulting GPUAdaptorPtr shares the instance pointer with the
    //! GPUAdaptorPtr passed as argument.
    //!
    //! \param rhs Constant GPUAdaptorPtr to share.
    //!
    //! \return Reference to the current instance.
    //!
    GPUAdaptorPtr& operator=(const GPUAdaptorPtr& rhs);

    //! \brief Equality operator.
    //!
    //! The resulting GPUAdaptorPtr shares the instance pointer with the
    //! GPUAdaptorPtr passed as argument.
    //!
    //! \param rhs GPUAdaptorPtr to share.
    //!
    //! \return Refernce to the current instance.
    //!
    GPUAdaptorPtr& operator=(GPUAdaptor * rhs);

  private:
    void* _pImpl; //!< Opaque smart pointer data.
  };
};

SYN_VISIBILITY_POP

#endif // _SYNCOLOR_GPU_ADAPTOR_H
