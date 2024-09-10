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
//! \file synColorRules.h
//!
//! \brief Main Synergy color management component rules API header file.
//!

#ifndef _SYNCOLOR_RULES_H
#define _SYNCOLOR_RULES_H

#include <synColor/synExport.h>
#include <synColor/synStatus.h>


SYN_VISIBILITY_PUSH

namespace SYNCOLOR 
{
  //------------------------------------------------------------------------------------------------
  //! This class represents a generic rule 
  //
  class SYN_EXPORT Rule
  {
  public:
    //! Default destructor
    virtual ~Rule() {}

    //! All supported types of rule
    enum RuleType
    {
      OPAQUE_RULE = 0,    //!< An opaque rule is a rule where the parameters are hidden
      DEFAULT_RULE,       //!< The default rule is the fallback rule if all other rules fail
      STANDARD_RULE       //!< The standard rule is a fully customizable and manageable rule
    };
    //! \brief Returns the type of the rule
    //! \return the rule's type
    virtual RuleType getType() const = 0;

    //! \brief Returns the name of the rule
    //! \return the rule's name which could never be null or empty
    virtual const char* getName() const = 0;
    
  protected:
    //! Default constructor
    Rule() {}

  private:
    //! Forbidden method
    Rule(const Rule&);
    //! \brief Forbidden method
    //! \return the instance itself
    Rule& operator=(const Rule&);
  };
  
  //------------------------------------------------------------------------------------------------
  //! This class is a smart pointer on a rule
  //
  class SYN_EXPORT RulePtr
  {
  public:
    //! Constructor
    RulePtr();

    //! \brief Custom Constructor
    //! \param r The pointer to a file rule to manage
    explicit RulePtr(Rule* r);

    //! \brief Copy Constructor
    //! \param r reference to a RulePtr to Manage
    RulePtr(const RulePtr& r);

    //! Destructor
    ~RulePtr();

    //! \brief Operator =
    //! \param r The reference of the Rule that is on the RHS of the "=" operation
    //! \return a reference to the new RulePtr.
    RulePtr& operator=(const RulePtr& r);

    //! \brief Operator ->
    //! \return a pointer to the Rule
    Rule* operator->() const;

    //! \brief bool operator
    //! \return a bool, true if the Rule is defined, false otherwise
    operator bool() const;

    //! \brief  Obtains a pointer to the Rule from the RulePtr
    //! \return a pointer to Rule. This Rule* is not to be deleted.
    Rule* get() const;

  private:
    //! Pointer to _data memory location (hiding the implementation)
    void* _data;
  };
  
  //------------------------------------------------------------------------------------------------
  //! \brief The method creates a file rule
  //! \param name The name of the rule
  //! \param fileNameExtension The file name extension to use
  //! \param filePathPattern The pattern to be applied to the file path
  //! \param colorSpace The color space associated to the regular expression
  //! \param rule The corresponding file rule
  //! \return returns a SYNCOLOR::SynStatus.  Check the error code to
  //!         determine if an error occurred. See SYNCOLOR::SynStatus above.
  //
  SYN_EXPORT SYNCOLOR::SynStatus createFileRule(const char* name, 
                                                const char* fileNameExtension,
                                                const char* filePathPattern,
                                                const char* colorSpace,
                                                RulePtr& rule);

  //------------------------------------------------------------------------------------------------
  //! This class represents a rule where the file pattern and the input color space are hidden
  //
  class SYN_EXPORT OpaqueFileRule : public Rule
  {
  public:
    //! \copydoc Rule::getType
    RuleType getType() const { return Rule::OPAQUE_RULE; }
  };

  //------------------------------------------------------------------------------------------------
  //! This class represents a rule where only the input color space could be user managed
  //
  class SYN_EXPORT DefaultFileRule : public Rule
  {
  public:
    //! \copydoc Rule::getType
    RuleType getType() const { return Rule::DEFAULT_RULE; }

    //! \brief Get the file name extension pattern used by the rule
    //
    //! \return the file name extension
    //
    virtual const char* getFileNameExtension() const = 0;

    //! \brief Get the file path pattern used by the rule
    //
    //! \return the pattern
    //
    virtual const char* getFilePathPattern() const = 0;

    //! \brief  Get the color space
    //! \return returns the color space to be used
    virtual const char* getColorSpace() const = 0;

    //! \brief Set the color space to use
    //! \param colorSpace The color space
    //! \return returns a SYNCOLOR::SynStatus.  Check the error code to
    //!         determine if an error occurred. See SYNCOLOR::SynStatus above.
    virtual SynStatus setColorSpace(const char* colorSpace) = 0;
  };

  //------------------------------------------------------------------------------------------------
  //! This class represents a rule where the file pattern and the input color space could be user managed
  //
  class SYN_EXPORT FilePathRule : public Rule
  {
  public:
    //! \copydoc Rule::getType
    RuleType getType() const { return Rule::STANDARD_RULE; }

    //! \brief Get the file name extension used by the rule
    //! \return the file name extension
    virtual const char* getFileNameExtension() const = 0;

    //! \brief Set the file name extension
    //! \param fileNameExtension The extension to be used by the rule
    //! \return returns a SYNCOLOR::SynStatus.  Check the error code to
    //!         determine if an error occurred. See SYNCOLOR::SynStatus above.
    virtual SynStatus setFileNameExtension(const char* fileNameExtension) = 0;

    //! \brief Get the file pattern used by the rule
    //! \return the pattern
    virtual const char* getFilePathPattern() const = 0;

    //! \brief Set the pattern to find in the file path
    //! \param filePathPattern The pattern to be used by the rule
    //! \return returns a SYNCOLOR::SynStatus.  Check the error code to
    //!         determine if an error occurred. See SYNCOLOR::SynStatus above.
    //
    //! \note  "g?mma" accepts gamma, gimma but refuses gmma, gaaaamma
    //!        "g*mma" accepts gamma, gimma, gmma and gaaaamma
    //!        "g[ab]mma" only accepts gamma and gbmma, and refuses gmma, gaaamma
    //!        "g[!ab]mma" refuses gmma, gamma, gbmma and gaaamma, and accepts gcmma
    //!        "g[a-c]mma" only accepts gamma, gbmma and gcmma
    //!        "g[!a-c]mma" refuses gamma, gbmma, gcmma and gmma, but accepts gemma
    //!        "g\\*mma" only accepts g*mma
    //
    virtual SynStatus setFilePathPattern(const char* filePathPattern) = 0;

    //! \brief  Get the color space
    //! \return returns the color space to be used
    virtual const char* getColorSpace() const = 0;

    //! \brief Set the color space to use
    //! \param colorSpace the color space
    //! \return returns a SYNCOLOR::SynStatus.  Check the error code to
    //!         determine if an error occurred. See SYNCOLOR::SynStatus above.
    virtual SynStatus setColorSpace(const char* colorSpace) = 0;   
  };

  //------------------------------------------------------------------------------------------------
  //! \brief The class is the callback to serialize all the rules
  //! \note Line by line xml writer
  //
  class SYN_EXPORT RuleWriter
  {
  public:
    //! Default destructor
    virtual ~RuleWriter() {}
    
    //! \brief Provide the serialized version of a rule
    //! \param data The string to write
    //! \return returns a SYNCOLOR::SynStatus.  Check the error code to
    //!         determine if an error occurred. See SYNCOLOR::SynStatus above.
    virtual SynStatus write(const char* data) = 0;
  };

  //------------------------------------------------------------------------------------------------
  //! The base class for rule containers that contain all the rules, and the capabilities to manage them
  //
  class SYN_EXPORT Rules
  {
  public:
    //! \brief Restore the default rules
    //
    //! \return The status of the request. Check the error code to
    //!         determine if an error occurred. See SYNCOLOR::SynStatus above.
    //
    virtual SynStatus restoreDefaults() = 0;
    
    //! \brief Load rules from preferences
    //! \return returns a SYNCOLOR::SynStatus.  Check the error code to
    //!         determine if an error occurred. See SYNCOLOR::SynStatus above.
    virtual SynStatus load() = 0;

    //! \brief Save the rules in the preferences
    //! \return returns a SYNCOLOR::SynStatus.  Check the error code to
    //!         determine if an error occurred. See SYNCOLOR::SynStatus above.
    virtual SynStatus save() const = 0;

    //! \brief Get the number of rules
    //! \return The number of rules
    virtual unsigned int getNumRules() const = 0;

    //! \brief Get a specific rule using its position
    //! \param position The position of the rule to get
    //! \param rule The rule found
    //! \return returns a SYNCOLOR::SynStatus.  Check the error code to
    //!         determine if an error occurred. See SYNCOLOR::SynStatus above.
    virtual SynStatus getRule(unsigned int position, RulePtr& rule) const = 0;

    //! \brief Get a specific rule using its name
    //! \param name The name of the rule
    //! \param rule The rule found
    //! \return returns a SYNCOLOR::SynStatus.  Check the error code to
    //!         determine if an error occurred. See SYNCOLOR::SynStatus above.
    virtual SynStatus getRule(const char* name, RulePtr& rule) const = 0;

    //! \brief Move a rule to a new position
    //! \param name The name of the rule to move
    //! \param offset The offset to be applied to the current rule position
    //! \return returns a SYNCOLOR::SynStatus.  Check the error code to
    //!         determine if an error occurred. See SYNCOLOR::SynStatus above.
    virtual SynStatus moveRule(const char* name, int offset) = 0;

    //! \brief Remove a rule using its name
    //! \param name The name of the rule
    //! \return returns a SYNCOLOR::SynStatus.  Check the error code to
    //!         determine if an error occurred. See SYNCOLOR::SynStatus above.
    virtual SynStatus removeRule(const char* name) = 0;

    //! \brief Serialize the rule container in XML
    //! \param writer The writer to receive the ASCII string containing XML formatting
    //! \return returns a SYNCOLOR::SynStatus.  Check the error code to
    //!         determine if an error occurred. See SYNCOLOR::SynStatus above.
    virtual SynStatus serialize(RuleWriter& writer) const = 0;
    
    //! \brief Populate the rule container
    //! \param data The XML string
    //! \param length The length of the string
    //! \return returns a SYNCOLOR::SynStatus.  Check the error code to
    //!         determine if an error occurred. See SYNCOLOR::SynStatus above.
    virtual SynStatus populate(const char* data, unsigned int length) = 0;

  protected:
    //! Default destructor
    virtual ~Rules() {}
  };
  
  //------------------------------------------------------------------------------------------------
  //! The class contains all the file rules, and the capabilities to manage them
  //
   class SYN_EXPORT FileRules : public Rules
  {
  public:
    //! \brief Get the current container (could be the native one or an OCIO one)
    //! \return the container instance
    static FileRules& instance();

    //! Set the rule container to the native one
    static void setNativeMode();
    
    //! \enum OCIOModes  Defines the OCIO modes
    //
    enum OCIOModes
    {
      //! \brief Use the default OCIO rules
      //
      //! That mode offers two read-only rules. The OCIO rule which is read-only, uses 
      //! the OCIO library rule. And then the default rule which is used when no color space 
      //! was found by the previous rule and allows the user to change its color space.
      //
      USE_OCIO_RULES = 0,

      //! \brief Use the file path mechanism
      //
      //! That mode offers the default rule and the capability to create file path based 
      //! rules. The default rule is used when no color space where found by the previous 
      //! rule(s) and allows the user to change its color space. 
      //
      USE_FILE_PATH_RULES
    };

    //! \brief Set the rule container to the OCIO one
    //
    //! \param ocioFilePath The file path to the OCIO configuration file
    //! \param ruleMode The rules to use in OCIO mode
    //
    //! \return The status of the request. Check the error code to
    //!         determine if an error occurred. See SYNCOLOR::SynStatus above.
    static SynStatus setOCIOMode(const char* ocioFilePath, OCIOModes ruleMode);

    //! \brief Is the rule container read only or not ?
    //! \note A read only rule container forbids any changes to the rules.
    //! \return true if the rule container is read only
    virtual bool isReadOnly() const = 0;
    
    //! \brief Evaluate the file path to find the associated color space
    //! \param filePath The file path to evaluate
    //! \return The associated color space; otherwise, a null or empty string 
    //!         when no color space was found
    virtual const char* evaluateFilePath(const char* filePath) const = 0;

  protected:
    //! Default destructor
    virtual ~FileRules() {}
  };
};

SYN_VISIBILITY_POP

#endif // _SYNCOLOR_RULES_H
