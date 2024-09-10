// ==================================================================
//
// Copyright (c) 2014 Autodesk, Inc.
// All rights reserved.
//
// This computer source code and related instructions and comments are the
// unpublished confidential and proprietary information of Autodesk, Inc.
// and are protected under applicable copyright and trade secret law.
// They may not be disclosed to, copied or used by any third party without
// the prior written consent of Autodesk, Inc.
// ==================================================================
//
//! \mainpage SynColor Color Management Library
//!
//! Autodesk Color Management, or SynColor, is a software component that provides state-of-the-
//! art color management services. It has been designed as a reusable technology, and is shared
//! across several Autodesk applications. Libraries are available for Linux, MacOS, and Windows,
//! and there is also a Python binding.
//!
//! SynColor is both fast and accurate. It is used by Autodesk Lustre for real-time color grading.
//! A wide variety of integer and float pixel layouts are supported for input and output, but
//! internal calculations are done at 32-bit float for high precision.
//!
//! The SynColor API can apply color transforms on both the CPU and GPU. The CPU renderer takes
//! advantage of SSE intrinsics, while the GPU renderer creates customized shader programs on-demand
//! based on the color transform chain being applied. There are several methods of invoking GPU
//! rendering, including via Autodesk OGS (One Graphics System). Supported shading languages are
//! GLSL, Cg, and HLSL.
//!
//! This single API provides access to many other official and industry standards for color management,
//! including the Academy Color Encoding System (ACES), International Color Consortium (ICC) profiles,
//! American Society of Cinematographers Color Decision Lists (ASC CDLs), OpenColorIO configurations,
//! and over a dozen different third-party look-up table (LUT) formats.
//!
//! SynColor also includes a library of over a hundred transforms for converting between common
//! color spaces (including sRGB, AdobeRGB, ProPhotoRGB, ACES, CIE XYZ, digital cinema P3 and DCDM,
//! as well as SD, HD, and UHD video).
//!
//! The native color transform format for SynColor is CTF, which is based on the Academy/ASC XML color
//! transform format with some Autodesk extensions. This format defines a superset of all color operations
//! available in the other supported formats. Color transforms can be authored a flexible, human-readable
//! XML format, and transform chains to be combined and losslessly exported.
//!
//! The basic math operators in the CTF file format provide building blocks for assembling arbitrarily
//! complex color transform chains. Each of the available operators maps onto a corresponding operator
//! in the CPU and GPU renderers. There is also support for "dynamic parameters" which are
//! designed to be fed into the rendering engines at runtime based on the settings of knobs or
//! sliders in applications.
//!
//! The CTF file format is documented here in the Autodesk Color Management guide.
//!
//!
//! \file synColor.h
//!
//! \brief Color Transform class and transform file loading functions.
//!

#ifndef _SYNCOLOR_H
#define _SYNCOLOR_H

#include <synColor/synExport.h>
#include <synColor/synColorNamespace.h>

#include <synColor/template.h>

#include <synColor/bitdepth.h>
#include <synColor/dynamicProperty.h>
#include <synColor/errorIds.h>
#include <synColor/ioFormat.h>
#include <synColor/loggerTypes.h>
#include <synColor/pixelFormat.h>
#include <synColor/roi.h>
#include <synColor/optimizer.h>
#include <synColor/resolveFlags.h>
#include <synColor/GPUAdaptor.h>
#include <synColor/sharedPtr.h>
#include <synColor/synStatus.h>
#include <synColor/stockTemplate.h>


SYN_VISIBILITY_PUSH

//! \brief Autodesk SynColor SDK public namespace.
namespace SYNCOLOR
{
  //! \brief Renderer type flag list.
  enum RendererFlags
  {
    RENDERER_NONE =     0x00, //!< No renderer.

    RENDERER_CPU =      0x01, //!< CPU renderer flag.
    RENDERER_GPU =      0x02, //!< GPU renderer flag.

    RENDERER_ALL = RENDERER_CPU | RENDERER_GPU //!< Both CPU and GPU renderer flags set.
  };

  class Transform;
  //------------------------------------------------------------------------------------------------
  //! \brief Transform smart pointer..
  //!
  //! Class that manages sharing of SYNCOLOR::Transform instances.  Implements a shared pointer where only
  //! the last instance holding a specific Transform instance will delete it.
  //!
  class SYN_EXPORT TransformPtr
  {
  public:
    //! \brief Empty constructor.
    //!
    TransformPtr();

    //! \brief Initialization constructor.
    //!
    //! \param t Transform instance to manage.
    //!
    explicit TransformPtr(Transform* t);

    //! \brief Copy Constructor
    //!
    //! The resulting TransformPtr shares the instance pointer with the
    //! TransformPtr passed as argument.
    //!
    //! \param t TransformPtr containing the Transform pointer to share.
    //!
    TransformPtr(const TransformPtr& t);

    //! \brief Destructor.
    //!
    //! If no other TransformPtr shares the underlying pointer, the Transform is deleted.
    //!
    ~TransformPtr();

    //! \brief Equality operator.
    //!
    //! The resulting TransformPtr shares the instance pointer with the
    //! TransformPtr passed as argument.
    //!
    //! \param tp Constant TransformPtr to share.
    //!
    //! \return Reference to the current instance.
    //!
    TransformPtr& operator=(const TransformPtr& tp);

    //! \brief Dereferencing operator.
    //!
    //! \return Reference to the underlying Transform instance.
    //!
    Transform& operator*() const;

    //! \brief Dereferencing operator.
    //!
    //! \return Pointer to the underlying Transform instance.
    //!
    Transform* operator->() const;

    //! \brief Boolean operator
    //!
    //! \return True if the TransformPtr contains a non-null Transform pointer.
    //!
    operator bool() const;

    //! \brief Count of references to the underlying Transform.
    //!
    //! \return Number of TransformPtr instances with the same underlying Transform pointer.
    //!
    long getUseCount() const;

    //! \brief Get the instance pointer.
    //!
    //! \return Pointer to the underlying Transform instance.
    //!
    Transform* get() const;

    //! \brief  Release the underlying transform.
    //!
    //! The underlying Transform instance is deleted if no other TransformPtr has the same Transform pointer.
    //!
    void reset();

    //! \brief  Reassign to a different Transform.
    //!
    //! The initial underlying Transform instance is deleted if no other TransformPtr has the
    //! same pointer.
    //!
    //! \param tp Pointer to the Transform to reassign to.
    //!
    void reset(Transform* tp);

  private:
    void* _data; //!< Opaque smart pointer data.
  };
  
  //! \brief Color transform class.
  //!
  //! A color transform is a sequence of color operators to be applied to pixel values.  Each
  //! operator can be one of many different types: a matrix, a 1-d LUT, a 3-d LUT, a gamma, and more.
  //! For more details on the structure of color transforms and for a list of color operators
  //! see the _Autodesk Color Management_ Guide included with this SDK.
  //!
  class SYN_EXPORT Transform
  {
  public:

    //! \brief Destructor.
    //!
    virtual ~Transform();

    //! \brief Clone the Transform.
    //!
    //! \return TransformPtr containing a copy of the Transform.
    //!
    virtual SYNCOLOR::TransformPtr clone() const = 0;

    //! \brief Color transform ID.
    //!
    //! If the transform is read from a CTF file, the id can found in the XML description.
    //!
    //! \return ID string. This string is not to be deleted.
    //!
    virtual const char *getId() const = 0;

    //! \brief Set the color transform ID.
    //!
    //! \param id New transform ID.
    //!
    //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error
    //!         occurred.
    //!
    virtual SynStatus setId( const char* id ) = 0;

    //! \brief Transform unique ID.
    //!
    //! This ID is different from the color transform ID above.  It is used by the transform
    //! caching system.
    //!
    //! \return Unique ID string.  This string is not to be deleted.
    //!
    virtual const char* getUniqueId() const = 0;

    //! \brief Set the unique transform ID.
    //!
    //! This method is used by the transform caching system and should not be used.
    //!
    //! \param id New unique transform ID.
    //!
    //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error
    //!         occurred.
    //!
    virtual SynStatus setUniqueId( const char* id ) = 0;

    //! \brief Transform name.
    //!
    //! This name is an optional attribute of the xml description in CTF files. The name
    //! empty string when not specified.
    //!
    //! \return Name string.  This char* is not to be deleted.
    //!
    virtual const char *getName() const = 0;

    //! \brief Set the color transform name.
    //!
    //! \param name New transform name.
    //!
    virtual void setName( const char* name ) = 0;

    //! \brief Color transform description.
    //!
    //! The description is an optional segment of the CTF file.
    //! If more than one description entry is present in the file, the
    //! entries are separated by newlines in the returned string.
    //!
    //! \return Concatenated descriptions string. This char* is not to be deleted.
    //!         The char* is owned by a static string shared by all calls made to
    //!         getDescriptions(). Returned value should be copied inside a new string
    //!         if it needs to be preserved.
    //!
    virtual const char *getDescriptions() const = 0;

    //! \brief Set the transform description.
    //!
    //! \param description New transform description.
    //!
    virtual void setDescription( const char* description ) = 0;

    //! \brief Color transform input description.
    //!
    //! This description is an optional element of the CTF file. If the description is not
    //! specified, the empty string is returned.
    //!
    //! \return Input description string. This char* is not to be deleted.
    //!
    virtual const char* getInputDescriptor() const = 0;

    //! \brief Set the transform input description.
    //!
    //! \param inDesc New input descriptor.
    //!
    virtual void setInputDescriptor( const char* inDesc ) = 0;

    //! \brief Color transform output description.
    //!
    //! This description is an optional element of the CTF file. If the description is not
    //! specified, the empty string is returned.
    //!
    //! \return Output description string. This char* is not to be deleted.
    //!
    virtual const char* getOutputDescriptor() const = 0;

    //! \brief Set the transform output description.
    //!
    //! \param outDesc New output descriptor.
    //!
    virtual void setOutputDescriptor( const char* outDesc ) = 0;

    //! \brief Transform input bit depth.
    //! \return Input bit depth.
    virtual SYNCOLOR::BitDepth getInputBitDepth() const = 0;

    //! \brief Transform output bit depth.
    //!
    //! \return Output bit depth.
    //!
    virtual SYNCOLOR::BitDepth getOutputBitDepth() const = 0;

    //! \brief Transform input pixel format.
    //!
    //! \return Input pixel format.
    //!
    virtual SYNCOLOR::PixelFormat getInputPixelFormat() const = 0;

    //! \brief Set the transform input pixel format.
    //!
    //! \param pf New input pixel format.
    //!
    //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error
    //!         occurred.
    //!
    virtual SYNCOLOR::SynStatus setInputPixelFormat(SYNCOLOR::PixelFormat pf) = 0;

    //! \brief Transform ouutput pixel format.
    //!
    //! \return Output pixel format.
    //!
    virtual SYNCOLOR::PixelFormat getOutputPixelFormat() const = 0;

    //! \brief Set the transform output pixel format.
    //!
    //! \param pf New output pixel format.
    //!
    //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error
    //!         occurred.
    //!
    virtual SYNCOLOR::SynStatus setOutputPixelFormat(SYNCOLOR::PixelFormat pf) = 0;

    //! \brief Set both the input and the output pixel format.
    //!
    //! \param inPF New input pixel format
    //! \param outPF New output pixel format
    //!
    //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error
    //!         occurred.
    //!
    virtual SYNCOLOR::SynStatus setPixelFormats(SYNCOLOR::PixelFormat inPF,
                                                SYNCOLOR::PixelFormat outPF) = 0;

    //! \brief Create the inverse transform.
    //! \param t Return TransformPtr containing the inverse.
    //!
    //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error
    //!         occurred.
    //!
    virtual SYNCOLOR::SynStatus inverse(SYNCOLOR::TransformPtr& t) const = 0;

    //! \brief Checks if a transform is the  inverse of this instance.
    //!
    //! With the help of the transform IDs, this method determines whether a transform is
    //! the inverse of this one.  This method is useful only if the two transforms originate from
    //! CTF files in the Autodesk color transform collection.
    //!
    //! \param t Transform to check against.
    //! \return True if the supplied transform is the inverse of this one.
    //!
    virtual bool isInverseOf(const SYNCOLOR::TransformPtr& t) const = 0;

    //! \brief Checks if this transform has references to resolve.
    //! \return True if there are unresolved references.
    //!
    virtual bool  hasReferences() const = 0;

    //! \brief Checks if this transform has aliases to resolve.
    //!
    //! \return true if there are unresolved aliase.
    //!
    //! \sa finalize
    //!
    virtual bool  hasAliasReferencesToResolve() const = 0;

    //! \brief Callback class for extracting color transform metadata.
    //!
    //! This class needs to be implemented to obtain metadata via Transform::extractContent().
    //! Metadata content extraction begins with the metadata list of the transform proper first,
    //! followed by the metadata list of each color operator in order.
    //!
    //! The result is a list of metadata entries with sublists for each operator.  The callback
    //! calling sequence is as follows:
    //!
    //! 	beginEntry("Transform")
    //!     haveAttribute("Transform attribute tag 1", "Transform attribute value 1", false)
    //!     haveAttribute("Transform attribute tag 2", "Transform attribute value 2", false)
    //! 	...
    //! 		beginEntry("Operator 1")
    //!     	haveAttribute("Operator 1 attribute tag 1", "Operator 1 attribute value 1", false)
    //! 		...
    //! 		endEntry()
    //!
    //! 		beginEntry("Operator 2")
    //!     	haveAttribute("Operator 2 attribute tag 1", "Operator 2 attribute value 1", false)
    //!			...
    //! 		endEntry()
    //! 	...
    //! 	endEntry()
    //!
    class SYN_EXPORT ContentCallbacks
    {
    public:
      //! Destructor.
      virtual ~ContentCallbacks();

      //! \brief Indicates the beginning of a new list of attributes.  Could be that of the
      //!        transform or of one of the color operators.
      //!
      //! \param type String expressing the type of list.
      //!
      virtual void beginEntry(const char* type) = 0;

      //! \brief Indicates the end of an attribute list/
      //!
      virtual void endEntry() = 0;

      //! \brief Provides an attribute/value pair of metadata information.
      //! \param tag Attribute name.
      //! \param value Attribute value.
      //! \param isDynamic Attribute is dynamic.
      virtual void haveAttribute(const char* tag, const char* value, bool isDynamic) = 0;
    };

    //! \brief Color transform metadata extraction.
    //!
    //! \param callbacks Metadata extraction callback class implementation instance.
    //!
    virtual void extractContent(ContentCallbacks& callbacks) const = 0;

    //! \brief Set the value of a dynamic property.
    //!
    //! \param prop Property description.
    //!
    //! \sa DynamicProperty.
    //!
    //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error
    //!         occurred. If the property is not used by this transform instance, the error
    //!         returned is ERR_PROPERTY_NOT_AVAILABLE.
    //!
    virtual SynStatus setDynamicProperty(const DynamicProperty& prop) = 0;

    //! \brief Obtain a dynamic property value.
    //!
    //! This method puts the return value in the value union based on the type provided in the
	//! DynamicProperty.
    //!
    //! \sa Detailed description of DynamicProperty.
    //!
    //! \param prop Property instance.
    //!
    //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error
    //!         occurred. If the property is not used by this transform instance, the error
    //!         returned is ERR_PROPERTY_NOT_AVAILABLE.
    //!
    virtual SynStatus getDynamicProperty(DynamicProperty& prop) = 0;

    //! \brief Set the GPU Adaptor.
    //!
    //! If GPU rendering is needed, an adaptor must be supplied to the color transform prior to
    //! the call to SYNCOLOR::finalize.
    //!
    //! \sa Detailed description of GPUAdaptor.
    //!
    //! \brief Obtain the GPUAdaptor used by the transform..
    //
    //! \return A GPUAdaptorPtr containing the transform's GPUAdaptor.
    virtual const GPUAdaptorPtr& getGPUAdaptor() const = 0;

    //! \brief Render the color transform to a memory buffer using the CPU renderer.
    //!        You must call finalize on the transform before calling apply!
    //!
    //! \param srcBuf Memory area with the image to process.  Must be of the appropriate input
    //!               pixel format.
    //! \param roi Region of interest (ROI) to apply the transform to.
    //! \param dstBuf Memory area to write the transformed pixel values to.  Must be of the
    //!               appropriate output pixel format.
    //!
    //! \note If the input and output pixel formats are the same, the source and destination pointers
    //!       can be the same (in-place processing).
    //!
    //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error
    //!         occurred.
    //!
    virtual SynStatus applyCPU(const void* srcBuf,
                               const ROI& roi,
                               void* dstBuf) = 0;

    //! \brief Render the color transform to a padded memory buffer using the CPU renderer.
    //!        You must call finalize on the transform before calling apply!
    //!
    //! This version of applyCPU provides parameters to deal with images in padded memory buffers.
    //!
    //! \param srcBuf Memory area with the image to process.  Must be of the appropriate input
    //!               pixel format.
    //! \param srcRowSize Source image row size in bytes.
    //! \param roi Region of interest (ROI) to apply the transform to.
    //! \param dstBuf Memory area to write the transformed pixel values to.  Must be of the
    //!               appropriate output pixel format.
    //! \param dstRowSize Destination image row size in bytes.
    //!
    //! \note If the input and output pixel formats and the row sizes are the same, the source and
    //!       destination pointers can be the same (in-place processing).
    //!
    //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error
    //!         occurred.
    //!
    virtual SynStatus applyCPU(const void* srcBuf,
                               unsigned srcRowSize,
                               const ROI& roi,
                               void* dstBuf,
                               unsigned dstRowSize) = 0;

    //! \brief Render the color transform for a single pixel using the CPU renderer.
    //!        You must call finalize on the transform before calling apply!
    //!
    //! \param srcBuf Pointer to the pixel to process.  Must be of the appropriate input
    //!               pixel format.
    //! \param dstBuf Pointer for where to put the output pixel value to.  Must be of the
    //!               appropriate output pixel format.
    //! \param x is the column coordinate of the pixel location
    //! \param y is the row coordinate of the pixel location
    //!
    //! \note If the input and output pixel formats are the same, the source and destination pointers
    //!       can be the same (in-place processing).
    //!
    virtual void applyCPUSinglePixel(const void* srcBuf,
                                     void* dstBuf,
                                     unsigned x, unsigned y) = 0;

    //! \brief Render the color transform to the active render destination using the GPU renderer.
    //!        You must call finalize on the transform before calling apply!
    //!
    //! Using the GPUAdaptor::drawQuad method, the color transform is applied to the input texture
    //! specified and rendered into the active rendering destination.
    //!
    //! \param inTexHandle Handle to the texture to process.
    //! \param texCoords Input texture coordinates.
    //! \param screenCoords Destination coordinates.
    //!
    //! \remark This method assumes normalized color values in the input texture.
    //!
    //! \remark Coordinates can be stored using a SYNCOLOR::Vertices
    //!         or a SYNCOLOR::Rect as the vertices class accepts
    //!         a class rect as input.
    //!
    //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error
    //!         occurred.
    //!
    virtual SynStatus applyGPU(unsigned inTexHandle,
                               const SYNCOLOR::Vertices& texCoords,
                               const SYNCOLOR::Vertices& screenCoords) const =0;
  };

  //! \brief Color transform list class.
  //!
  //! Contains a list of transform shared pointers.
  //!
  class SYN_EXPORT TransformList
  {
  public:
    //! \brief Default Destructor.
    //!
    virtual ~TransformList();

    //! \brief Number of transforms.
    //!
    //! \return Number of transforms.
    //!
    virtual unsigned size() const = 0;

    //! \brief I-th transforms.
    //!
    //! \param index Element index.
    //! \param pTransform TransformPtr at specified index.
    //!
    //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error
    //!         occurred.
    //!
    virtual SynStatus at(const unsigned index, TransformPtr& pTransform) const = 0;
  };

  //! Helper shared pointer
  typedef SharedPtr<TransformList> TransformListPtr;
   
  //! \brief Message logging callback registration.
  //!
  //! \param logFunc Logging function pointer. If NULL, logging is set to the standard output.
  //!
  //! \sa SYNCOLOR::LogLevel
  //!
  //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error
  //!         occurred.
  //!
  SYN_EXPORT SynStatus setLoggerFunction(SYNCOLOR::LogFunc logFunc);

  //! \brief Finalize the color transform for processing in CPU rendering only.
  //!
  //! Prepares a transform to be used for processing images and returns the finalized version
  //! of itself.   This function must be called before calls to Transform::applyCPU().
  //! A transform can be finalized 'in-place' by providing the same
  //! TransformPtr as _inTransform_ and as _finalizedTransform_.
  //!
  //! Finalized transforms are placed in a cache.  Finalizing the same transform with the same parameters will
  //! return the same finalized transform.
  //!
  //! \param inTransform Transform to finalize
  //! \param srcPF Source buffer format for CPU rendering.
  //! \param dstPF Destination buffer format for CPU rendering.
  //! \param optimizerFlags Optimization level for speed/quality trade-off. See SYNCOLOR::OptimizerFlags.
  //! \param resolveFlag Alias resolution hint.  See SYNCOLOR::ResolveFlags.
  //! \param finalizedTransform Returned finalized transform ready to be used for processing.
  //!
  //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error
  //!         occurred.
  //!
  SYN_EXPORT SynStatus finalize(const TransformPtr& inTransform,
                                PixelFormat srcPF,
                                PixelFormat dstPF,
                                OptimizerFlags optimizerFlags,
                                ResolveFlags resolveFlag,
                                TransformPtr& finalizedTransform);

  //! \brief Finalize the color transform for processing including a GPU adaptor.
  //!
  //! Prepares a transform to be used for processing images and returns the finalized version
  //! of itself.   This function must be called before calls to Transform::applyCPU() or
  //! Transform::applyGPU().  A transform can be finalized 'in-place' by providing the same
  //! TransformPtr as _inTransform_ and as _finalizedTransform_.
  //!
  //! Finalized transforms are placed in a cache.  Finalizing the same transform with the same parameters will
  //! return the same finalized transform.
  //!
  //! \param inTransform Transform to finalize
  //! \param srcPF Source buffer format for CPU rendering.
  //! \param dstPF Destination buffer format for CPU rendering.
  //! \param rendererFlags Renderers to prepare.  See SYNCOLOR::RendererFlags.
  //! \param optimizerFlags Optimization level for speed/quality trade-off. See SYNCOLOR::OptimizerFlags.
  //! \param resolveFlag Alias resolution hint.  See SYNCOLOR::ResolveFlags.
  //! \param pAdaptor is the GPU adaptor to use.
  //! \param finalizedTransform Returned finalized transform ready to be used for processing.
  //!
  //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error
  //!         occurred.
  //!
  SYN_EXPORT SynStatus finalize(const TransformPtr& inTransform,
                                PixelFormat srcPF,
                                PixelFormat dstPF,
                                RendererFlags rendererFlags,
                                OptimizerFlags optimizerFlags,
                                ResolveFlags resolveFlag,
                                SYNCOLOR::GPUAdaptorPtr& pAdaptor,
                                TransformPtr& finalizedTransform);

  //! \brief Read a single color transform from any of the file formats supported by SynColor.
  //!
  //! For details about the CTF file XML format, please see the _Autodesk Color Management_ Guide
  //! included with this SDK.
  //!
  //! If the file is missing, does not contain a color transform or there is a
  //! catastrophic error reading the XML, the transform handle returned is null.
  //!
  //! Based on the file extension, this function will try various parsers in addition to the CTF
  //! one (e.g. OCIO, ICC, CDL, VLT).
  //!
  //! \param filePath Path of the file to be read.
  //! \param pTransform Returned transform.
  //!
  //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error
  //!         occurred.
  //!
  SYN_EXPORT SynStatus loadColorTransform(const char *filePath,
                                          TransformPtr& pTransform);

  //! \brief Read a list of color transforms from any of the file formats supported by SynColor,
  //!        particularly those that may return more than a single transform (for example, a .ccc
  //!        file that may contain more than one ASC CDL transform). When loading a format that
  //!        only supports single transforms, a list with one entry is returned.
  //!
  //! If the file is missing, does not contain a color transform or there is a
  //! catastrophic error reading the XML, the transform list handle returned is null.
  //!
  //! \param filePath Path of the file to be read.
  //! \param pTransformList Returned transform list.
  //!
  //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error
  //!         occurred.
  //!
  SYN_EXPORT SynStatus loadColorTransformList(const char *filePath,
                                              TransformListPtr& pTransformList);
   
  //! \brief Read a color transform from a memory buffer.
  //!
  //! For details about the CTF file XML format, please see the _Autodesk Color Management_ Guide
  //! included with this SDK.
  //!
  //! If the buffer does not contain a color transform or there is a
  //! catastrophic error parsing the xml, the transform handle returned is 0.
  //!
  //! Unlike loadColorTransform, this function will only try the one parser specified in the
  //! format argument.  Hence this function is for more specialized purposes and should not
  //! be considered an equivalent of loadColorTransform.
  //!
  //! \param xmlBuffer Memory buffer containing the color transform XML description.
  //! \param pTransform Returned transform.
  //! \param format Format of the buffer to be loaded. Currently only supports
  //!               IO_FORMAT_CTF and IO_FORMAT_CC.
  //!
  //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error
  //!         occurred.
  //!
  SYN_EXPORT SynStatus loadColorTransformFromBuffer(const char* xmlBuffer,
                                                    TransformPtr& pTransform,
                                                    IOFormat format = IO_FORMAT_CTF);

  //! \brief Writes a color transform XML to file.
  //!
  //! For details about the CTF file XML format, please see the _Autodesk Color Management_ Guide
  //! included with this SDK.
  //!
  //! \note Overwrites the existing file if permissions allow it.
  //!
  //! \param fileName File name to write to including the extension.  The standard extension for a
  //!                 color transform file is '.ctf'.
  //! \param saveResolvedTransform Whether reference operators are resolved prior to writing to file.
  //! \param pTransform Transform to be written.
  //! \param format Transform will be serialized using this format type.
  //!
  //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error
  //!         occurred.
  //!
  SYN_EXPORT SynStatus saveColorTransform(const char *fileName,
                                          bool saveResolvedTransform,
                                          const TransformPtr& pTransform,
                                          IOFormat format = IO_FORMAT_CTF);

  //! \brief Callback class for writing color transform xml to a memory buffer.
  //!
  //! This class is needed to manage the memory buffer required to hold a color
  //! transform XML description.
  //!
  //! \sa SYNCOLOR::saveColorTransformToBuffer
  //!
  class SYN_EXPORT TransformWriterCallback
  {
  public:
    //! Destructor.
    virtual ~TransformWriterCallback();

    //! \brief Write the specified content to the buffer. May be called multiple
    //!        times during one save.
    //! \param content String containing the partial color transform XML description.
    //! \param size Content string size.
    virtual void write(const char* content, unsigned size) = 0;
  };

  //! \brief Writes a color transform XML to a memory buffer.
  //!
  //! \param writerCallback Callback instance to use. See SYNCOLOR::TransformWriterCallback.
  //! \param pTransform Transform to be written.
  //! \param format Transform will be serialized using this format type.
  //!
  //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error
  //!         occurred.
  //!
  SYN_EXPORT SynStatus saveColorTransformToBuffer(TransformWriterCallback& writerCallback,
                                                  const TransformPtr& pTransform,
                                                  IOFormat format = IO_FORMAT_CTF);

  //! \brief Check if a file can be loaded as a color transform.
  //!
  //! \param fileName File name to check.
  //!
  //! \return True if the file has a supported extension.
  //!
  //! \note This function is meant as a quick check to validate a file. It
  //!       does not guarantee that the file will successfully load.
  //!
  SYN_EXPORT bool isFileSupported(const char *fileName);

  //! \brief Callback class for obtaining the list of supported file extensions
  //!        that may be read. Clients will need to implement this class and use this implementation
  //!        as the callback for retrieving supported extensions through getSupportedFileExtensions().
  //!
  class SYN_EXPORT SupportedExtensionsCallBack
  {
  public:
    //! Destructor.
    virtual ~SupportedExtensionsCallBack();

    //! \brief Supported file extension callback.
    //!
    //! \param extension Supported file extension.
    //! \param description brief File extension description.
    //!
    virtual void add(const char* extension, const char* description) = 0;
  };

  //! \brief Obtain the list of supported file extensions.
  //!
  //! The supported files types are color transform files (ctf/xml) and any
  //! file type supported by the OCIO version supplied with this SDK.
  //!
  //! \param extensionsCallback Callback instance to use. See SYNCOLOR::SupportedExtensionsCallBack.
  //!                           For each supported extension, the add() method of the callback
  //!                           will be invoked.
  //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error
  //!         occurred.
  //!
  SYN_EXPORT SynStatus getSupportedFileExtensions(SupportedExtensionsCallBack& extensionsCallback);

  //! \brief Callback class for obtaining the list of supported output formats for a specific
  //!        transform. Clients will need to implement this class and use this implementation
  //!        as the callback for retrieving supported formats through getSupportedOutputFormats().
  class SYN_EXPORT SupportedOutputFormatsCallBack
  {
  public:
    //! Destructor.
    virtual ~SupportedOutputFormatsCallBack();

    //! \brief Supported file output formats.
    //!
    //! \param format Supported output format.
    //! \param extension Output format extension.
    //! \param description brief Output format description
    //!
    virtual void add(IOFormat format, const char* extension, const char* description) = 0;
  };
  
  //! \brief Obtain the list of supported output formats for a specified transform.
  //!        Only formats that may losslessly represent the provided transform will be returned.
  //!
  //! \param pTransform Transform used to determine all possible output formats.
  //! \param outputFormatsCallback Callback to use. See SYNCOLOR::SupportedOutputFormatsCallBack.
  //!                              For each supported format, the add() method of the callback
  //!                              will be invoked.
  //!
  //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error
  //!         occurred.
  //!
  SYN_EXPORT SynStatus getSupportedOutputFormats(
     const TransformPtr& pTransform, SupportedOutputFormatsCallBack& exportFormatsCallback);
};

SYN_VISIBILITY_POP

#endif // _SYNCOLOR_H
