#ifndef _adskRepresentations_h_
#define _adskRepresentations_h_

#include <maya/MPxRepresentation.h>
#include "adskRepresentationFactory.h"

/*==============================================================================
 * CLASS CacheRepresentation
 *============================================================================*/

// Representation which uses a gpuCache node to implement its behavior.

class CacheRepresentation : public MPxRepresentation {

public: 

   /*----- types and enumerations ----*/

   typedef MPxRepresentation BaseClass;

   /*----- classes -----*/

   // Cache representation input is the path to the Alembic file.
   class Factory : public AdskRepresentationFactory {

   public:

      /*----- types and enumerations ----*/

      typedef AdskRepresentationFactory BaseClass;

      /*----- member functions -----*/
      
      Factory() : BaseClass(CacheRepresentation::type()) {}
      ~Factory() override {}

      MPxRepresentation* create(
         MPxAssembly* assembly, const MString& name, const MString& data
      ) const override;

      // Cache representation name is the file base name, including
      // extension (i.e. directory component removed).
      MString creationName(
         MPxAssembly* assembly, const MString& input
      ) const override;

      // Cache representation label is the same as the representation name.
      MString creationLabel(
         MPxAssembly* assembly, const MString& input
      ) const override;

      // Cache representation persistent data is the input, i.e. the
      // path to the Alembic file.
      MString creationData(
         MPxAssembly* assembly, const MString& input
      ) const override;
   };

   /*----- static member functions -----*/

   static MString type();

   /*----- member functions -----*/
      
   // Construct.  The representation data is the path to the Alembic
   // file.
   CacheRepresentation( MPxAssembly* assembly, const MString& name );
   ~CacheRepresentation() override;

   bool activate() override;

   MString getType() const override;

   void getExternalContent( MExternalContentInfoTable& table ) const override;
   void setExternalContent(
      const MExternalContentLocationTable& table
   ) override;

private:

   /*----- member functions -----*/

   // Assignment optor undefined.
   CacheRepresentation& operator=(const CacheRepresentation&);
};

/*==============================================================================
 * CLASS SceneRepresentation
 *============================================================================*/

// Representation which imports nodes from a Maya file.

class SceneRepresentation : public MPxRepresentation {

public: 

   /*----- types and enumerations ----*/

   typedef MPxRepresentation BaseClass;

   /*----- classes -----*/

   // Scene representation input is the path to the Maya scene file.
   class Factory : public AdskRepresentationFactory {

   public:

      /*----- types and enumerations ----*/

      typedef AdskRepresentationFactory BaseClass;

      /*----- member functions -----*/
      
      Factory() : BaseClass(SceneRepresentation::type()) {}
      ~Factory() override {}

      MPxRepresentation* create(
         MPxAssembly* assembly, const MString& name, const MString& data
      ) const override;

      // Scene representation name is the file base name, including
      // extension (i.e. directory component removed).
      MString creationName(
         MPxAssembly* assembly, const MString& input
      ) const override;

      // Scene representation label is the same as the representation name.
      MString creationLabel(
         MPxAssembly* assembly, const MString& input
      ) const override;

      // Scene representation persistent data is the input, i.e. the
      // path to the Maya scene file.
      MString creationData(
         MPxAssembly* assembly, const MString& input
      ) const override;
   };

   /*----- static member functions -----*/

   static MString type();

   /*----- member functions -----*/
      
   // Construct.  The representation data is the path to the Maya
   // scene file.
   SceneRepresentation( MPxAssembly* assembly, const MString& name );
   ~SceneRepresentation() override;

   // Populate the container with nodes imported from the file
   // associated with this representation.
   bool activate() override;

   MString getType() const override;

   // This representation type can apply edits.
   bool canApplyEdits() const override;

   void getExternalContent( MExternalContentInfoTable& table ) const override;
   void setExternalContent(
      const MExternalContentLocationTable& table
   ) override;

private:

   /*----- member functions -----*/

   // Assignment optor undefined.
   SceneRepresentation& operator=(const SceneRepresentation&);
};

/*==============================================================================
 * CLASS LocatorRepresentation
 *============================================================================*/

// Representation which uses a locator node to implement its behavior.

class LocatorRepresentation : public MPxRepresentation {

public: 

   /*----- types and enumerations ----*/

   typedef MPxRepresentation BaseClass;

   /*----- classes -----*/

   // Locator representation has empty input, which means that
   // initially there is no annotation.
   class Factory : public AdskRepresentationFactory {

   public:

      /*----- types and enumerations ----*/

      typedef AdskRepresentationFactory BaseClass;

      /*----- member functions -----*/
      
      Factory() : BaseClass(LocatorRepresentation::type()) {}
      ~Factory() override {}

      MPxRepresentation* create(
         MPxAssembly* assembly, const MString& name, const MString& data
      ) const override;

      // Locator representation creation name is "Locator".
      MString creationName(
         MPxAssembly* assembly, const MString& input
      ) const override;

      // Locator representation label is the same as the representation name.
      MString creationLabel(
         MPxAssembly* assembly, const MString& input
      ) const override;

      // Locator representation persistent data is its annotation.
      MString creationData(
         MPxAssembly* assembly, const MString& input
      ) const override;
   };

   /*----- static member functions -----*/

   static MString type();

   /*----- member functions -----*/
      
   // Construct.  The representation data is the locator text
   // annotation, which is initially empty (no annotation).
   LocatorRepresentation(
      MPxAssembly* assembly, const MString& name, const MString& data
   );
   ~LocatorRepresentation() override;

   bool activate() override;

   MString getType() const override;

private:

   /*----- member functions -----*/
      
   // Assignment optor undefined.
   LocatorRepresentation& operator=(const LocatorRepresentation&);

   /*----- data members -----*/

   const MString fAnnotation;   // Locator text annotation.
};

#endif

//-
//*****************************************************************************
// Copyright 2015 Autodesk, Inc.  All rights reserved.
// 
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy
// form.
//*****************************************************************************
//+
