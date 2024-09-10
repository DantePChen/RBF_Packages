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
//! \file synColor/catalogMgr.h
//!
//! \brief SynColor transform catalog manager.
//!

#ifndef _SYNCOLOR_CATALOG_MANAGER_H
#define _SYNCOLOR_CATALOG_MANAGER_H

#include <synColor/colorSpaceTypes.h>
#include <synColor/template.h>
#include <synColor/transformTypes.h>

SYN_VISIBILITY_PUSH


//--------------------------------------------------------------------------------------------------
//
namespace SYNCOLOR
{
  class SynStatus;
}


//--------------------------------------------------------------------------------------------------
//
namespace SYNCOLOR
{
  //------------------------------------------------------------------------------------------------
  //! \brief This class is the callback to serialize a user color space
  //
  class SYN_EXPORT UserColorSpaceSerializer
  {
  public:
    //! Default destructor
    virtual ~UserColorSpaceSerializer() {}
    
    //! \brief Provide the serialized version of a user color space
    //! \param data The string to write
    //! \return returns a SYNCOLOR::SynStatus.  Check the error code to
    //!         determine if an error occurred. See SYNCOLOR::SynStatus above.
    virtual SynStatus serialize(const char* data) = 0;
  };

  //------------------------------------------------------------------------------------------------
  //! \brief SynColor transform catalog manager.
  //!
  //! The SynColor native transform catalog can be used to create native color transform
  //! templates and permits adding and removing additional transforms to the template system.
  //!
  //! The catalog manager looks at all CTF files in the Autodesk SynColor transform collection
  //! and in the shared transform directory defined in the SynColor configuration (see
  //! SYNCOLOR::getSharedColorTransformPath() ).  If metadata is present in these files, the manager
  //! determines which of the various template parameters (eg. working spaces, input color
  //! spaces and viewing transforms) the transform corresponds to.
  //!
  //! If a transform is added to the catalog manager, this transform will be available as template
  //! parameters of the corresponding type.  When a transform is added, a wrapper CTF file 
  //! containing the required metadata and referencing the added transform is added in the shared
  //! transform directory.
  //!
  //! \warning Manually removing wrapper CTF files from the shared transform directory effectively
  //!          removes the added transforms from the catalog.  Conversely adding a CTF file with the
  //!          appropriate metadata will add the associated transform to the catalog.  These changes take
  //!          effect after a restart of the client application using SynColor.
  //!
  class SYN_EXPORT NativeCatalogManager
  {
  public:
    //! \brief Get the catalog manager.
    //!
    //! \return Catalog manager singleton instance.
    //!
    static NativeCatalogManager& instance();

    //! \brief Create a native color transform template of the specified type.
    //!
    //! \param type Template type. See SYNCOLOR::TemplateType.
    //! \param pTemplate Returned template instance.
    //!
    //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error occurred.
    //!
    //! \sa Detailed description of SYNCOLOR::Template.
    //!
    virtual SynStatus loadTemplate(TemplateType type, 
                                   TemplatePtr& pTemplate) = 0;

    //! \brief Refresh the catalog.
    //!
    //! This method reloads the transform catalogs and by default, rebuilds the shared transform
    //! directory catalog.
    //!
    //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error occurred.
    //!
    virtual SynStatus reloadCatalogs() = 0;

    //! \brief Callback function to obtain the list of connection color spaces.
    //!
    //! This callback is called once per available connection space.  A connection space must be
    //! specified when adding a user color space to the catalog manager.
    //!
    //! \sa NativeCatalogManager::addUserColorSpace().
    //!
    //! \param name Connection space name.
    //!
    typedef SynStatus (*ConnectionSpaceCallback)(const char* name);

    //! \brief Connection space list.
    //!
    //! A connection space must be specified when adding a transform to the catalog manager. With
    //! the help of the ConnectionSpaceCallback, the list of connecting spaces is made available for
    //! UI selection.
    //!
    //! \param type Color space type.  See SYNCOLOR::UseColorSpaceType.
    //! \param cb Callback instance.
    //!
    //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error occurred.
    //!
    virtual SynStatus getConnectionSpaces(ColorSpaceType type,
                                          ConnectionSpaceCallback cb) = 0;

    //! \brief Add a user color space to the native catalog manager.
    //!
    //! A wrapper CTF file containing the specified metadata and referencing the added transform 
    //! is added in the shared transform directory.  This ensures that the transform remains in
    //! the catalog manager across application sessions.
    //!
    //! \warning Manually removing wrapper CTF files from the shared transform directory effectively
    //!          removes the added transforms from the catalog.  Conversely adding a CTF file with the
    //!          appropriate metadata will add the associated transform to the catalog. In either case,
    //!          the catalog must be reloaded by the client application for these changes to take effect.
    //!          For reference: SYNCOLOR::NativeCatalogManager::reloadCatalogs().
    //!
    //! \param type Color space type.  See SYNCOLOR::ColorSpaceType.
    //! \param connectionSpace Color space connecting the new transform to a known reference.  This corresponds to:
    //! 	- the input color space for type SYNCOLOR::ColorSpaceView.
    //!		- the output color space for type SYNCOLOR::ColorSpaceInput and SYNCOLOR::ColorSpaceWorking.
    //! \param uniqueName Unique name of the new transform.  This name will appear in the template parameter list corresponding to the type.
    //!             As its name implies, this name must be unique.
    //! \param userFacingName User facing name of the new transform. This name is only meant to be displayed in the UI.
    //!             This name is not required to be unique.
    //! \param hierarchy hierarchy of the new transform.  This value will be used to organize
    //!        the transform in hierarchical groups.  NULL or empty string can be used to place
    //!        the new transform at the root of the hierarchy.
    //! \param displayType specifies the type attribute for user displays only.
    //!        This is a noop for other type of color spaces.
    //! \param imageState specifies the image state attribute for user input color space.
    //!        This is a noop for other type of color spaces.
    //! \param transformPath File path of the user color space transform.
    //! \param useResolvedTransform Whether the definition contains a reference
    //!           to the user color space transform or a self-contained resolved
    //!           version of the user color space transform.
    //!
    //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error occurred.
    //!
    virtual SynStatus addUserColorSpace(ColorSpaceType type,
                                        const char* connectionSpace,
                                        const char* uniqueName,
                                        const char* userFacingName,
                                        const char* hierarchy,
                                        const char* displayType,
                                        const char* imageState,
                                        const char* transformPath,
                                        bool useResolvedTransform) = 0;

    //! \brief Add a user pure view transform to the native catalog manager.
    //!
    //! A wrapper CTF file containing the specified metadata and containing the added transform 
    //! is added in the shared transform directory.  This ensures that the transform remains in
    //! the catalog manager across application sessions.
    //!
    //! \warning Manually removing wrapper CTF files from the shared transform directory effectively
    //!          removes the added transforms from the catalog.  Conversely adding a CTF file with the
    //!          appropriate metadata will add the associated transform to the catalog. In either case,
    //!          the catalog must be reloaded by the client application for these changes to take effect.
    //!          For reference: SYNCOLOR::NativeCatalogManager::reloadCatalogs().
    //! \param inputSpace is the input color space of the pure view
    //! \param outputSpace is the output color space of the pure view
    //! \param uniqueName Unique name of the new transform.  This name will appear in the template parameter list corresponding to the type.
    //!             As its name implies, this name must be unique.
    //! \param userFacingName User facing name of the new transform. This name is only meant to be displayed in the UI.
    //!             This name is not required to be unique.
    //! \param hierarchy hierarchy of the new transform.  This value will be used to organize
    //!        the transform in hierarchical groups.  NULL or empty string can be used to place
    //!        the new transform at the root of the hierarchy.
    //! \param lut3dInterpolation. If the user's LUT includes any Lut3DOps, the SynColor InterpolationAlgo
    //!        member variable of those ops is set to the requested value.
    //! \param transformPath File path of the user color space transform.
    //!
    //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error occurred.
    //!
    virtual SynStatus addUserPureViewTransform(const char*                    inputSpace,
                                               const char*                    outputSpace,
                                               const char*                    uniqueName,
                                               const char*                    userFacingName,
                                               const char*                    hierarchy,
                                               const Lut3DOpInterpolationAlgo lut3dInterpolation,
                                               const char*                    transformPath) = 0;
           
    //! \brief Add a user color space or a user pure view transform to the native catalog manager
    //!        from a serialized version.
    //!
    //! \note
    //! \parblock
    //!        A user color space can be created externally without using the SynColor API methods:
    //!        SYNCOLOR::addUserColorSpace() or SYNCOLOR::addUserPureViewTransform().
    //!        For example, a user color space can be defined in a color transform file (.ctf)
    //!        using a text editor, providing that the following minimally required metadata
    //!        information is present:
    //!
    //!        The transform's "ProcessList" element must contain an "Info" child element.
    //!
    //!        For "Input", "InputTagOnly", "LogWorkingSpace", "SceneLinearWorkingSpace",
    //!        or "VideoWorkingSpace" color space tag, the user defined color space unique name
    //!        must be specified in the Info/InputSpace/ShortName child element, whereas the
    //!        connecting space must be specified in the Info/OutputSpace/ShortName child element:
    //!
    //! \code
    //!        <Info>
    //!            <Category>
    //!                <Tags>
    //!                    <Flame />
    //!                    <Maya />
    //!                    <[colorSpaceTag] />
    //!                </Tags>
    //!            </Category>
    //!            <InputSpace>
    //!                <ImageState>[userColorSpaceState]</ImageState>
    //!                <ShortName>[userColorSpaceUniqueName]</ShortName>
    //!                <ID>[userColorSpaceUniqueName]</ID>
    //!            </InputSpace>
    //!            <OutputSpace>
    //!                <ImageState>[outputSpaceState]</ImageState>
    //!                <ShortName>[outputSpaceUniqueName]</ShortName>
    //!                <ID>[outputSpaceID]</ID>
    //!            </OutputSpace>
    //!        </Info>
    //! \endcode
    //!
    //! \note  For "OutputTagOnly", "Display", "Display_PreProcess", "View", or "View_PreProcess"
    //!        color space tag, the user defined color space unique name must be specified in the
    //!        Info/OutputSpace/ShortName child element, whereas the connecting space must be
    //!        specified in the Info/InputSpace/ShortName child element:
    //! \note
    //! \code
    //!        <Info>
    //!            <Category>
    //!                <Tags>
    //!                    <Flame />
    //!                    <Maya />
    //!                    <[colorSpaceTag] />
    //!                </Tags>
    //!            </Category>
    //!            <InputSpace>
    //!                <ImageState>[inputSpaceState]</ImageState>
    //!                <ShortName>[inputSpaceUniqueName]</ShortName>
    //!                <ID>[inputSpaceID]</ID>
    //!            </InputSpace>
    //!            <OutputSpace>
    //!                <ImageState>[userColorSpaceState]</ImageState>
    //!                <ShortName>[userColorSpaceUniqueName]</ShortName>
    //!                <ID>[userColorSpaceUniqueName]</ID>
    //!            </OutputSpace>
    //!        </Info>
    //! \endcode
    //!
    //! \note  For "PureView" color space tag,
    //! \note
    //! \code
    //!        <ProcessList version="1.3" id="[userColorSpaceUniqueName]" name="[userColorSpaceUniqueName]">
    //!        <Info>
    //!            <Category>
    //!                <Tags>
    //!                    <Flame />
    //!                    <Maya />
    //!                    <PureView />
    //!                </Tags>
    //!            </Category>
    //!            <InputSpace>
    //!                <ImageState>[inputSpaceState]</ImageState>
    //!                <ShortName>[inputSpaceUniqueName]</ShortName>
    //!                <ID>[inputSpaceID]</ID>
    //!            </InputSpace>
    //!            <OutputSpace>
    //!                <ImageState>[outputSpaceState]</ImageState>
    //!                <ShortName>[outputSpaceUniqueName]</ShortName>
    //!                <ID>[outputSpaceID]</ID>
    //!            </OutputSpace>
    //!        </Info>
    //! \endcode
    //! \endparblock
    //!
    //! \param serializedUserColorSpace A serialized user color space.
    //!
    //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error occurred.
    //!
    virtual SynStatus addUserColorSpace(const char* serializedUserColorSpace) = 0;

    //! \brief Serialize the user defined color space in XML
    //!
    //! \param uniqueName Name of the user color space to be serialized.
    //! \param writer The writer to receive the UTF-8 string containing XML formatting
    //!
    //! \return returns a SYNCOLOR::SynStatus.  Check the error code to
    //!         determine if an error occurred. See SYNCOLOR::SynStatus above.
    //!
    virtual SynStatus serializeUserColorSpace(const char* uniqueName,
                                              UserColorSpaceSerializer& writer) const = 0;
    
    //! \brief Remove a previously added user color space from the native catalog manager.
    //!
    //! \param uniqueName Name of user color space to be removed.
    //!
    //! \return Returns a SYNCOLOR::SynStatus. Check the error code to determine if an error occurred.
    virtual SynStatus removeUserColorSpace(const char* uniqueName) = 0;

    //! \brief Checks if a color space is a user defined color space.
    //!
    //! \param uniqueName Unique name of the color space to check.
    //!
    //! \return True if the supplied color space is a user color space.
    //!
    virtual bool isUserColorSpace(const char* uniqueName) const = 0;

  protected:
    // Protected default constructor.
    NativeCatalogManager() {}

    // Protected default destructor.
    virtual ~NativeCatalogManager() {}

  private:
    //! \brief Forbidden copy constructor.
    //! \param other N/A.
    NativeCatalogManager(const NativeCatalogManager& other);
    //! \brief Forbidden assignment operator.
    //! \param other N/A.
    //! \return N/A.
    NativeCatalogManager& operator=(const NativeCatalogManager& other);
  };
};

SYN_VISIBILITY_POP

#endif // _SYNCOLOR_CATALOG_MANAGER_H
