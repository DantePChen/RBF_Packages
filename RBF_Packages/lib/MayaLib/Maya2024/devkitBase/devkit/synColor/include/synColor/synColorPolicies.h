// ==================================================================
//
// Copyright (c) 2016 Autodesk, Inc.
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
//! \file synColor/synColorPolicies.h
//!
//! \brief Policy management
//!
//! Policy management is achieved via this set of classes, namely
//! PolicyCollection, providing access to a list of policies via a visitor,
//! Policy, the incarnation of, you guessed it, a policy and, finally,
//! Preferences, an editable copy of a policy. Both PolicyCollection and
//! Preferences provide factory class methods for instantiation. Preferences
//! also provides a copy method.
//!
//! Physically, a policy consists of a directory holding a config file and
//! whatever files this policy may provide.  In this initial implementation,
//! a policy directory contains a 'policy.cfg' file and, optionally, an input
//! rules file, a viewing rules file and a directory containing a user colour
//! space collection.
//!
//! Initially, a policy collection will likely be empty. Clients can create
//! Preferences objects and save them as policies in a policy collection (see.
//! Preferences::saveAsPolicy).  These policies are persisted and can later
//! be retrieved to initialize the preferences of a project (see 
//! Policy::saveAsPreferences).
//!

#ifndef _SYNCOLOR_POLICIES_H
#define _SYNCOLOR_POLICIES_H

#include <synColor/synExport.h>
#include <synColor/synColorNamespace.h>
#include <synColor/synColorURL.h>
#include <synColor/sharedPtr.h>

SYN_VISIBILITY_PUSH

namespace SYNCOLOR {

class SynStatus;

class PolicyCollection;

//------------------------------------------------------------------------------------------------
//! \brief An abstract interface for any policy
//
class SYN_EXPORT PolicyInterface
{
public:
  //! \brief Destructor.
  //!
  //! Releases all resources by the PolicyInterface instance.
  virtual ~PolicyInterface() {}

  //! \brief Get the name of the policy.
  //! 
  //! This name is always defined for a policy used as a template.  It will be
  //! defined for a Preferences object derived from a Policy unless the
  //! Preferences object was later modified.
  //
  //! \return The original name of the policy.
  virtual const char* getPolicyName() const = 0;

  //! \brief Get the working colour space.
  //!
  //! \return The working colour space's unique (short) name.
  virtual const char* getWorkingColourSpace() const = 0;

  //! \brief Get the rendering colour space.
  //!
  //! \return The rendering colour space unique (short) name.
  virtual const char* getRenderingColourSpace() const = 0;

  //! \brief Get the full path to the input rules file.
  //!
  //! \return The location of the input rules file.
  virtual URL::Ptr getInputRules() const = 0;
  
  //! \brief Get the full path to the viewing rules file.
  //!
  //! \return The location of the viewing rules file.
  virtual URL::Ptr getViewingRules() const = 0;

  //! \brief Get the full path to the user colour space collection.
  //!
  //! \return The location of the user colour space collection.
  virtual URL::Ptr getUserColourSpaces() const = 0;

  //! \brief Get the value of an arbitrary variable.
  //!
  //! \param var The name of the variable
  //!
  //! \return The value assigned to a user defined variable
  virtual const char* getUserVariable(const char* var) const = 0;

  //! \brief Gets the standard name of the policy file in which variables are stored.
  //!
  //! \return The name of the policy file (a constant).
  static const char* policyFile();
};

//------------------------------------------------------------------------------------------------
//! \brief An editable policy
//
class SYN_EXPORT Preferences : public PolicyInterface
{
public:
  //! \brief Destructor.
  //!
  //! Releases all resources used by the Preferences instance.
  virtual ~Preferences();

  //! \brief A pointer to a Preferences instance with shared ownership type.
  //!        The instance is automatically destroyed once no Ptr points to it.
  typedef SharedPtr<Preferences> Ptr;

  //! \brief Creates Preferences at location 'dir'.
  //! \param dir The storage location of the created Preferences instance.
  //! \return A shared pointer to a Preferences instance.  Always succeeds
  //!         if enough memory is available.  Loads existing preferences at
  //!         'dir' when possible.
  static Ptr create(const URL::Ptr& dir);

  //! \brief Copies 'from' at location 'to'.  The new Preferences object is
  //!        returned to caller via optional 'copy'.  Pass null for 'copy' if
  //!        not interested.
  //!
  //! \param to The target location for the copy
  //! \param from The Preferences instance to copy
  //! \param copy An optional Ptr to hold the resulting Preferences copy.  Can be null.
  //!
  //! \return A SynStatus. Check the error code to determine if an error
  //!         occurred.
  static SynStatus copy(const URL::Ptr& to, const Preferences& from, Ptr* copy);

  //! \brief Like 'copy' but any existing Preferences at 'to' is lost.
  //!
  //! \param to The target location for the copy
  //! \param from The Preferences instance to copy
  //! \param copy An optional Ptr to hold the resulting Preferences copy.  Can be null.
  //!
  //! \return A SynStatus. Check the error code to determine if an error
  //!         occurred.
  static SynStatus replace(const URL::Ptr& to, const Preferences& from, Ptr* copy);

  //! \brief Persists the Preferences.
  //!
  //! \return A SynStatus. Check the error code to determine if an error
  //!         occurred.
  virtual SynStatus save() const = 0;

  //! \brief Saves the Preferences as a 'Policy' with name 'name' in collection
  //!        'lib'.
  //!
  //! \param lib The PolicyCollection in which to add the new Policy.
  //! \param name The name to assign to the new Policy.
  //!
  //! \return A SynStatus. Check the error code to determine if an error
  //!         occurred.
  virtual SynStatus saveAsPolicy(PolicyCollection& lib, const char* name) const = 0;

  //! \brief Sets the working colour space
  //!
  //! \param cs The unique(short) colour space name.
  virtual void setWorkingColourSpace(const char* cs) = 0;

  //! \brief Sets the rendering colour space
  //!
  //! \param cs The unique(short) colour space name.
  virtual void setRenderingColourSpace(const char* cs) = 0;
  
  //! \brief Sets the name of input rules file (always relative to the
  //!        Preferences location, see 'create' above).
  //!
  //! \param name The name of the input rules file.
  virtual void setInputRulesFileName(const char* name) = 0;

  //! \brief Sets the name of viewing rules file (always relative to the
  //!        Preferences location, see 'create' above).
  //!
  //! \param name The name of the viewing rules file.
  virtual void setViewingRulesFileName(const char* name) = 0;

  //! \brief Sets the name of colour space collection directory (always
  //!        relative to the Preferences location, see 'create' above).
  //!
  //! \param name The name of the user colour space collection directory.
  virtual void setUserColourSpacesDirectoryName(const char* name) = 0;

  //! \brief Sets the value of an arbitrary variable 'var'.
  //!
  //! \param var The name of the variable.
  //! \param value The value of the variable.
  virtual void setUserVariable(const char* var, const char* value) = 0;

private:
  // \brief Implementation detail.
  virtual SynStatus assign(const Preferences& r, bool replace) = 0;
};

//------------------------------------------------------------------------------------------------
//! \brief A read-only policy intended as a Preferences template.
//
class SYN_EXPORT Policy : public PolicyInterface
{
public:
  //! \brief Destructor.
  //!
  //! Releases all resources used by the Policy instance.
  virtual ~Policy();

  //! \brief A pointer to a Policy instance with shared ownership type.
  //!        The instance is automatically destroyed once no Ptr points to it.
  typedef SharedPtr<Policy> Ptr;

  //! \brief saveAsPreferences will preserve existing preferences if any.
  //!
  //! \param to The directory location of the Preferences to be saved.
  //!
  //! \return A SynStatus. Check the error code to determine if an error
  //!         occurred.
  virtual SynStatus saveAsPreferences(const URL::Ptr& to) const = 0;

  //! \brief like 'saveAsPreferences' but any existing preferences at 'to'
  //!        are lost.
  //!
  //! \param to The directory location of the Preferences to be saved.
  //!
  //! \return A SynStatus. Check the error code to determine if an error
  //!         occurred.
  virtual SynStatus replacePreferences(const URL::Ptr& to) const = 0;
};

//------------------------------------------------------------------------------------------------
//! \brief A collection of policies
//
class SYN_EXPORT PolicyCollection
{
public:
  //! \brief Destructor.
  //!
  //! Releases all resources used by the PolicyCollection instance.
  virtual ~PolicyCollection();

  //! \brief A pointer to a PolicyCollection instance with shared ownership type.
  //!        The instance is automatically destroyed once no Ptr points to it.
  typedef SharedPtr<PolicyCollection> Ptr;

  //! \brief Creates PolicyCollection at location 'dir'.
  //!
  //! \param dir The storage location of the created PolicyCollection instance.
  //!
  //! \return A shared pointer to a PolicyCollection instance.  Always succeeds
  //!         if enough memory is available.  Provides access via 'visit' to
  //!         existing collection at 'dir' when applicable.
  static Ptr create(const URL::Ptr& dir);

  //! \brief Callback interface for collection traversal.
  struct Visitor
  {
    //! \brief Destructor.
    //!
    //! Releases all resources used by the Visitor instance.
    virtual ~Visitor() {}

    //! \brief Overload to handle all policies in a collection.
    //!
    //! \param policy The current policy (provided by the traversal).
    //!
    //! \return A SynStatus indicating the error, if any.  Errors abort traversal.
    virtual SynStatus operator()(const Policy::Ptr& policy) = 0;
  };

  //! \brief Traverses the collection, calling Visitor 'v' for every policy.
  //!
  //! \param v A Visitor to handle each policy in the collection.
  //!
  //! \return A SynStatus. Check the error code to determine if an error
  //!         occurred.
  virtual SynStatus visit(Visitor& v) const = 0;
};

}

#endif
