//
// ==================================================================
// Copyright 2017 Autodesk, Inc.  All rights reserved.
//
// This computer source code  and related  instructions and comments are
// the unpublished confidential and proprietary information of Autodesk,
// Inc. and are  protected  under applicable  copyright and trade secret
// law. They may not  be disclosed to, copied or used by any third party
// without the prior written consent of Autodesk, Inc.
// ==================================================================
//
//
//! \file synColor/config.h
//!
//! \brief Class to get information about a configuration.
//!

#ifndef _SYNCOLOR_CONFIG_H
#define _SYNCOLOR_CONFIG_H

#include <synColor/synExport.h>
#include <synColor/synColorNamespace.h>

#include <synColor/colorSpace.h>
#include <synColor/template.h>


SYN_VISIBILITY_PUSH


//--------------------------------------------------------------------------------------------------
//
namespace SYNCOLOR
{
  //------------------------------------------------------------------------------------------------
  //!
  //! \brief Configuration
  //!
  class SYN_EXPORT Config
  {
  public:
    //! shared pointer
    typedef SharedPtr<Config> Ptr;
    
    //! Destructor
    virtual ~Config();
    
    //! \brief Get an initialized configuration using a configuration file
    //! 
    //! \param configFile is the configuration file
    //! \param ocioConfigFile is an OCIO configuration file; it could be null
    //! \param pConfig [out] is the config class instance
    //! 
    //! \return Returns a SYNCOLOR::SynStatus.
    //!
    static SynStatus get(const char* configFile, const char* ocioConfigFile, Ptr& pConfig);

    //! \brief Get an initialized configuration using existing native catalog 
    //!           and shared transform directories.
    //! 
    //! \note The method will revert all the aliases to their default values.
    //! 
    //! The method is initializing a configuration using a minimum set of information.
    //! It's dedicated to external tools consuming existing a color managed 'scene'
    //! (i.e. without the need to change anything, Arnold makeTx, Arnold kick).
    //! As a concrete example, the default Maya windows paths are:
    //!  - transformPath is [Maya installation path]\synColor
    //!  - sharedPath is C:\Users\[user name]\Documents\maya\synColor\Shared
    //! 
    //! \param transformPath is the directory of the native catalog
    //! \param sharedPath is the directory of the custom color space catalog
    //! \param ocioConfigFile is an OCIO configuration file; it could be null
    //! \param pConfig [out] is the config class instance
    //! 
    //! \return Returns a SYNCOLOR::SynStatus.
    //!
    static SynStatus get(
        const char* transformPath, const char* sharedPath, const char* ocioConfigFile, Ptr& pConfig);

    //! \brief Create an initialized configuration using the catalog and shared transform paths
    //! 
    //! The method creates a new configuration file using the native catalog
    //! and shared catalog directories. It overwrites any existing configuration file.
    //! 
    //! \param configFile is the configuration file to be created
    //! \param transformPath is the directory of the native catalog
    //! \param sharedPath is the directory of the custom color space catalog
    //! \param ocioConfigFile is an OCIO configuration file; it could be null if not used
    //! \param pConfig [out] is the config class instance
    //! 
    //! \return Returns a SYNCOLOR::SynStatus.
    //!
    static SynStatus create(
        const char* configFile,
        const char* transformPath, 
        const char* sharedPath, 
        const char* ocioConfigFile,
        Ptr& pConfig);

    //! Get a color space using its name
    //! 
    //! \param name is the color space unique name
    //! \param pColorSpace [out] is the color space class instance
    //! 
    //! \return Returns a SYNCOLOR::SynStatus.
    //!
    virtual SynStatus getColorSpace(const char* name, ColorSpace::Ptr& pColorSpace) const = 0;

    //! List of all types of color space
    enum ColorSpaceTypes
    {
        InputColorSpaces, //!< Only the input color spaces
        ViewTransforms,   //!< Only the view transforms
        AllColorSpaces    //!< All the color spaces
    };

    //! Get the number of color spaces
    //! 
    //! \param type is the type of the requested color spaces
    //! 
    //! \return the number of color spaces
    //!
    virtual unsigned getNumColorSpaces(ColorSpaceTypes type) const = 0;

    //! Get a color space using its index
    //! 
    //! \param type is the type of the requested color spaces
    //! \param index is the color space index
    //! 
    //! \return the color space name or NULL in case of error
    //!
    virtual const char* getColorSpaceName(ColorSpaceTypes type, unsigned index) const = 0;

    //! Get the template based on its type
    //! 
    //! \param type is the template type
    //! \param pTemplate [out] is the template class instance
    //! 
    //! \return Returns a SYNCOLOR::SynStatus.
    //!
    virtual SynStatus getTemplate(TemplateType type, TemplatePtr& pTemplate) const = 0;
  };
}

SYN_VISIBILITY_POP

#endif //_SYNCOLOR_CONFIG_H
