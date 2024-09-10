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
//!
//! \file synColor/synColorPrefs.h
//!
//! \brief Functions for managing SynColor directories, transform aliases, error message languages and other preferences.
//!

#ifndef _SYNCOLOR_PREFS_H
#define _SYNCOLOR_PREFS_H

#include <synColor/synExport.h>
#include <synColor/synStatus.h>
#include <synColor/basePath.h>


SYN_VISIBILITY_PUSH

namespace SYNCOLOR {

  //! \brief Transform collection location.
  //!
  //! The path of the SYNCOLOR::BASE_PATH_AUTODESK location. Used to resolve Reference operators.
  //!
  //! \return Absolute path where the SynColor transform collection is located. This string is not to be deleted.
  //!
  SYN_EXPORT const char* getSystemColorTransformPath();

  //! \brief SynColor configuration file directory location.
  //!
  //! \return Absolute path of the directory containing the synColorConfig.xml file. This string is not to be deleted.
  //!
  SYN_EXPORT const char* getConfigFilePath();

  //! \brief SynColor configuration file location.
  //!
  //! \return Absolute path of the synColorConfig.xml file including the file name. This string is not to be deleted.
  //!
  SYN_EXPORT const char* getConfigFileNamePath();

  //! \brief Configure the location of the SynColor configuration file, the transform collection
  //!        and the shared directory.
  //!
  //! This method is useful if the SynColor elements are not at the location used by the SynColor
  //! installer.
  //!
  //! \warning This method must be called prior to any SynColor SDK function or transform creation
  //!          that requires access to the aliases.
  //!
  //! \param collectionPath Absolute path to the SynColor transform collection.
  //! \param configPath Absolute path to the SynColor configuration file including the file name.
  //! \param defaultSharedPath Absolute path to the shared color transform directory.
  //!
  //! \return Returns a SYNCOLOR::SynStatus. If a configuration already has been loaded,
  //!         ERROR_SYN_COLOR_PREFS_ALREADY_LOADED is returned.  Check the error code to
  //!         determine if an error occurred.
  //!
  SYN_EXPORT SynStatus configurePaths(const char* collectionPath,
                                      const char* configPath,
                                      const char* defaultSharedPath);

  //! \brief Configure SynColor paths only using the configuration file.
  //!
  //! This method is useful if the SynColor elements are not at the location used by the SynColor
  //! installer and is dedicated to standalone usage where the catalogs are supposed to be up to date.
  //! A usage is when SynColor is embedded in a standalone tool which only consumes an existing configuration
  //! (i.e. no changes to the configuration)
  //!
  //! \warning This method must be called prior to any SynColor SDK function or transform creation
  //!          that requires access to the aliases.
  //
  //! \note This method can be called as many time as you want as it only reads configuration files.
  //!
  //! \param configPath Absolute path to the SynColor configuration file including the file name.
  //!
  //! \return Returns a SYNCOLOR::SynStatus. If a configuration already has been loaded,
  //!         ERROR_SYN_COLOR_PREFS_ALREADY_LOADED is returned.  Check the error code to
  //!         determine if an error occurred.
  //!
  SYN_EXPORT SynStatus configureAsStandalone(const char* configPath);

  //! \brief Color transform file extension.
  //!
  //! \return Extension string excluding the leading dot, i.e. 'ctf'.
  //!
  SYN_EXPORT const char* getColorTransformFileExtension();

  //! \brief Active shared color transform directory.

  //! \param pBuffer Return reference to the shared path string.
  //!
  //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error occurred.
  //!
  SYN_EXPORT SynStatus getSharedColorTransformPath( const char *& pBuffer );

  //! \brief Shared color transform directory from config file.  This directory may
  //!        differ from the active shared directory (see overrideSharedColorTransformsDir()).

  //! \param pBuffer Return reference to the shared path string.
  //!
  //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error occurred.
  //!
  SYN_EXPORT SynStatus getSavedSharedColorTransformPath( const char *& pBuffer );

  //! \brief Set the shared color transform directory and, indirectly, the
  //!        path to the policy collection (always a sibling of the transforms
  //!        directory).  This directory is saved to the config file.
  //!
  //! \param path Absolute path to the shared color transform directory.
  //!
  //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error occurred.
  //!
  SYN_EXPORT SynStatus setSharedColorTransformPath(const char* path);

  //! \brief Overrides the saved shared color transform directory.
  //!
  //! \param dir Absolute path to a shared color transform directory.
  //!
  //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error occurred.
  //!
  SYN_EXPORT SynStatus overrideSharedColorTransformsDir(const char* dir);

  //! \brief Default shared color transform directory.
  //!
  //! \return Returns the default shared path location.
  //!
  SYN_EXPORT const char* getDefaultSharedColorTransformPath();

  //! \brief Default color transform path of a SynColor specific alias.
  //!
  //! SynColor defines some default transform aliases.  Additional aliases
  //! can be added with SYNCOLOR::addAliasInPreferences().  
  //!
  //! \param alias SynColor alias name. Valid values for alias are:
  //!              - defaultLook,
  //!              - broadcastMonitor,
  //!              - graphicsMonitor.
  //!
  //! \param defaultPath Return reference to the alias transform path value.
  //!
  //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error occurred.
  //!
  SYN_EXPORT SynStatus getDefaultPathForAlias(const char* alias,
                                              const char *& defaultPath);

  //! \brief Retrieve a color transform alias.
  //!
  //! Color transforms may contain Reference operators that use an alias. This method returns
  //! the path to the color transform that corresponds the specified alias.
  //!
  //! \param alias Alias requested.
  //! \param path Return reference to the alias absolute path.
  //! \param basePath Base path to use if the alias contains a relative path. See SynColor::BasePath.
  //!
  //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error occurred.
  //!
  SYN_EXPORT SynStatus findAliasInPreferences(const char* alias, 
                                              const char *& path,
                                              SYNCOLOR::BasePath& basePath);

  //! \brief Add alias in the Preferences.
  //!
  //! The color transform alias set can be extended.  This function adds a new alias to the
  //! preferences.  This alias is stored in the SynColor configuration file.
  //!
  //! \param alias New alias name.
  //! \param path Path to the color transform file corresponding to the alias.  Should be an absolute path.
  //! \param basePath Base path to use if the transform path is relative.  Should be SYNCOLOR::BASE_PATH_NONE.
  //!
  //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error occurred.
  //!
  SYN_EXPORT SynStatus addAliasInPreferences(const char* alias,
                                             const char* path,
                                             SYNCOLOR::BasePath basePath);  

  //! \brief AutoConfigure switch value for the graphicsMonitor alias.
  //!
  //! For OS/X only.
  //! If set to true, SynColor will use the monitor profile used by the operating system as the graphicsMonitor alias.
  //!
  //! \param setting Return auto-configure value.
  //!
  //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error occurred.
  //!
  SYN_EXPORT SynStatus getAutoConfigureMonitorProfile(bool& setting);

  //! \brief Sets the AutoConfigure switch for the graphicsMonitor alias.
  //!
  //! For OS/X only.
  //! If set to true, SynColor will use the monitor profile used by the operating system as the graphicsMonitor alias.
  //!
  //! \param newSetting Whether to use the OS monitor profile as the graphicsMonitor alias.
  //! \param wrapInUserDisplayTransform Whether to wrap the OS monitor profile in a user display transform.
  //!
  //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error occurred.
  //!
  SYN_EXPORT SynStatus setAutoConfigureMonitorProfile(bool newSetting, bool wrapInUserDisplayTransform);

  //! \brief Policy collection directory.
  //!
  //! \return Returns the path to the policy collection directory.
  //!
  SYN_EXPORT const char* getPoliciesDir();
};

SYN_VISIBILITY_POP

#endif // _SYNCOLOR_PREFS_H
