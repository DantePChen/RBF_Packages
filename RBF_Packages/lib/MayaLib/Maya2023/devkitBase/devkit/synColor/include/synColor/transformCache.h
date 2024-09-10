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
//! \file synColor/transformCache.h
//!
//! \brief Accessors to the color transform cache.
//!

#ifndef SYNCOLOR_TRANSFORM_CACHE_H
#define SYNCOLOR_TRANSFORM_CACHE_H

#include <synColor/synExport.h>
#include <synColor/synColor.h>

SYN_VISIBILITY_PUSH

namespace SYNCOLOR
{
  //--------------------------------------------------------------------------------------------------
  //
  //!
  //! \brief Color transform cache.
  //!
  //! During an application session, some transform caching may occur.  The intent of caching
  //! is to save applications from doing costly transform creation operations multiple times if this can
  //! be avoided.
  //!
  //! There are currently two circumstances where transforms will be cached.
  //! 	- When a transform is finalized with the SYNCOLOR::Transform::finalize() call.
  //! 	- When a SYNCOLOR::Template creates a new transform.
  //!
  //! If during a session a given transform is finalized with the same parameters, or if a template is used
  //! with the same parameters to create a transform, the resulting transform will be retrieved from the cache.
  //! Currently, transforms created by reading files or by using the factory methods are not cached until
  //! they are finalized (whereas templates are cached at creation).
  //!
  //! It is possible to extract from the cache the transform data for saving to file, and to re-populate the cache
  //! with the same transforms data in a subsequent session.  This approach is useful for example for providing
  //! transforms to an external renderer application.
  //!
  namespace TransformCache
  {
    //! \brief Cache content wrinting callback class.
    //!
    //! This callback class is required by SYNCOLOR::TransformCache::serializeTransformCache.
    //! The write() method stores the cache information to a destination.  It is called multiple times.
    //!
    class SYN_EXPORT Writer
    {
    public:
      //! Destructor.
      virtual ~Writer();

      //! \brief Write the specified cache content. May be called multiple times.
      //!
      //! \note The cache content is not guaranteed to have ASCII characters only.
      //!
      //! \param content Opaque cache content to be written.
      //! \param size Opaque content size.
      //!
      virtual void write(const void* content, unsigned size) = 0;
    };

    //! \brief Number of transforms in the cache.
    //!
    //! This method is intended for testing purposes.
    //!
    //! \return Number of transforms in the cache.
    //!
    SYN_EXPORT unsigned numItems();

    //! \brief Transform instance by ID.
    //!
    //! If there is no transform corresponding to the ID supplied, a
    //! SYNCOLOR::ERROR_SYN_COLOR_CACHE_MISS error is returned.
    //!
    //! \param id Transform unique ID.  See SYNCOLOR::Transform::getUniqueId().
    //! \param pT Returned transform.
    //!
    //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error occurred.
    //!
    SYN_EXPORT SynStatus getTransform(const char *id, TransformPtr& pT);

    //! \brief Extract the color transform cache data.
    //!
    //! \param writerCallback Required callback instance.  See SYNCOLOR::TransformCache::Writer.
    //!
    //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error occurred.
    //!
    //! \sa SYNCOLOR::TransformCache::populateTransformCache().
    //!
    SYN_EXPORT SynStatus serializeTransformCache(Writer& writerCallback);

    //! \brief Populate the transform cache using the supplied data buffer.
    //!
    //! \param cacheData Cache data block pointer.
    //!
    //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error occurred.
    //!
    //! \sa SYNCOLOR::TransformCache::serializeTransformCache().
    //!
    SYN_EXPORT SynStatus populateTransformCache(const char* cacheData);

    //! \brief Empties the cache
    //!
    //! This method is intended for testing only and should not be used.
    //!
    //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error occurred.
    //!
    SYN_EXPORT SynStatus clear();
  }
}

SYN_VISIBILITY_POP

#endif
