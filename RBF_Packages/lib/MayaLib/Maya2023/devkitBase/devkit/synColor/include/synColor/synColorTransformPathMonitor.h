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
//! \file synColor/synColorTransformPathMonitor.h
//!
//! \brief Monitor a path for transform files.
//!

#ifndef _SYNCOLOR_TRANSFORM_PATH_MONITOR_H
#define _SYNCOLOR_TRANSFORM_PATH_MONITOR_H

#include <synColor/synExport.h>
#include <synColor/synColor.h>

SYN_VISIBILITY_PUSH

namespace SYNCOLOR
{
  //! \brief Monitor a path for transform directories and files.
  //!
  //! This class observes a directory and provides the list of directories and supported color
  //! transform files in its hierarchy.   If a directory or file is added within the hierarchy,
  //! subsequent calls to _listDirectories()_ and _listFiles()_ will contain the updated lists.
  //!
  class SYN_EXPORT TransformPathMonitor
  {
  public:
    //! \brief Constructor.
	//!
	//! \param path Base path to monitor.
	//!
    TransformPathMonitor(const char* path );

    //! Destructor.
    ~TransformPathMonitor();

    //! \brief  Set the base path to monitor.
    //!
	//! \param path Base path to monitor.
    //!
    //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error occurred.
    //!
    SynStatus setPath(const char* path);

    //! \brief Monitored path.
    //!
    //! \return Base path being monitored.  This string is not to be deleted.
    //!
    const char* getPath() const;

    //! \brief Callback class required for retrieving directory and file lists.
    //!
    //! This interface allows the caller to process each monitored transform dub-directory name or transform file name.
    //!
    class SYN_EXPORT Callback
    {
    public:
      //! Destructor.
      virtual ~Callback();

      //! \brief Process the name of a monitored directory or file.
      //!
      //! \param name Directory or file name.
      //!
      virtual void processEntry(const char* name) = 0;
    };

    //! \brief List of monitored directories.
    //!
    //! Directory names are provided to the Callback::processEntry() method one by one in a lexically sorted order.
    //!
    //! \param relativePath Sub-path relative to the monitor base path. Use the empty string for the TransformPathMonitor base path.
    //! \param callback Callback implementation.
    //!
    //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error occurred.
    //!
    SynStatus listDirectories(const char* relativePath, Callback& callback);

    //! \brief List of monitored transforms files in a given directory.
    //!
    //! File names are provided to the Callback::processEntry() method one by one in a lexically sorted order.
    //!
    //! \param relativePath Sub-path relative to the monitor base path. Use the empty string for TransformPathMonitor base path.
    //! \param callback Callback implementation.
    //!
    //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error occurred.
    //!
    SynStatus listFiles(const char* relativePath, Callback& callback);

    //! \brief Full path for the specified transform directory and filename.
    //!
    //! \param relativePath Sub-path relative to the monitor base path. Use the empty string for TransformPathMonitor base path.
    //! \param fileName File name  provided by _getList()_.
    //! \param fullPath pointer Return full path buffer.
    //! \param fullPathSize Return buffer size.
    //!
    //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error occurred.
    //!
    SynStatus getFileFullPath(const char* relativePath,
                              const char* fileName,
                              char* fullPath,
                              unsigned fullPathSize
                              );

  private:
    //! Opaque implementation class.
    class Impl;
    Impl* _pImpl; //!< Opaque implementation instance.
  };
};

SYN_VISIBILITY_POP

#endif // _SYNCOLOR_TRANSFORM_PATH_MONITOR_H
