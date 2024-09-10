#ifndef _ufe_attribute
#define _ufe_attribute

// =======================================================================
// Copyright 2019 Autodesk, Inc. All rights reserved.
//
// This computer source code and related instructions and comments are the
// unpublished confidential  and proprietary information of Autodesk, Inc.
// and are protected under applicable copyright and trade secret law. They 
// may not be disclosed to, copied  or used by any third party without the 
// prior written consent of Autodesk, Inc.
// =======================================================================

#include "common/ufeExport.h"
#include "sceneItem.h"
#include "observer.h"
#include "types.h"
#include "undoableCommand.h"    // For UndoableCommand::Ptr

#include <memory>
#include <vector>
#include <string>

UFE_NS_DEF {

/*!
  \class Attribute
  \brief Abstract base class for Attribute interface.

  This base class defines an interface for factory objects that runtimes
  implement to handle the various supported attribute types (such as bool, 
  int, float, etc).

*/
class UFE_SDK_DECL Attribute
{
public:
    typedef std::shared_ptr<Attribute> Ptr;
    typedef std::string Type;

    //@{
    /*! Built-in attribute types */
    static constexpr char kInvalid[] = "Invalid";
    static constexpr char kBool[] = "Bool";
    static constexpr char kInt[] = "Int";
    static constexpr char kFloat[] = "Float";
    static constexpr char kDouble[] = "Double";
    static constexpr char kString[] = "String";
    static constexpr char kColorFloat3[] = "ColorFloat3";
    static constexpr char kEnumString[] = "EnumString";
    static constexpr char kInt3[] = "Int3";
    static constexpr char kFloat3[] = "Float3";
    static constexpr char kDouble3[] = "Double3";
    static constexpr char kGeneric[] = "Generic";
    //@}

    //! Constructor.
    Attribute(const SceneItem::Ptr&);
    virtual ~Attribute();

    //@{
    //! No copy or move constructor/assignment.
    Attribute(const Attribute&) = delete;
    Attribute& operator=(const Attribute&) = delete;
    Attribute(Attribute&&) = delete;
    Attribute& operator=(Attribute&&) = delete;
    //@}

    //! \return The object's SceneItem.
    SceneItem::Ptr sceneItem() const;

    //! \return true if this attribute has a value.
    virtual bool hasValue() const = 0;

    //! \return The attributes name.
    virtual std::string name() const = 0;

    //! \return The attributes documentation string.
    virtual std::string documentation() const = 0;

    //! \return The attributes type.
    virtual Type type() const = 0;

    //! \return The string representation of the attribute value.
    virtual std::string string() const = 0;

private:
    const SceneItem::Ptr fItem;

}; // end class Attribute

//! \brief Generic attribute which doesn't match any defined type.
class UFE_SDK_DECL AttributeGeneric : public Attribute
{
public:
    typedef std::shared_ptr<AttributeGeneric> Ptr;

    using Attribute::Attribute;

    // Ufe::Attribute overrides
    Type type() const final;

    //! \return The attributes native type as a string.
    virtual std::string nativeType() const = 0;
};

//! \brief Attribute which has a list of strings as enumerated values.
class UFE_SDK_DECL AttributeEnumString :
    public Attribute,
    public std::enable_shared_from_this<AttributeEnumString>
{
public:
    typedef std::shared_ptr<AttributeEnumString> Ptr;
    typedef std::vector< std::string > EnumValues;

    using Attribute::Attribute;

    // Ufe::Attribute overrides
    Type type() const final;

    //! \return Attribute enum current value.
    virtual std::string get() const = 0;

    //! Set the attribute enum value.
    virtual void set(const std::string& value) = 0;

    //! Return a command for undo / redo that sets the attribute enum value.
    //! The returned command is not executed; it is up to the caller to call
    //! execute().
    virtual UndoableCommand::Ptr setCmd(const std::string& value);

    //! \return The list of allowed enum values for this attribute.
    virtual EnumValues getEnumValues() const = 0;
};

//! \brief Typed attribute template for creating a specialized type.
template<typename T>
class UFE_SDK_DECL TypedAttribute :
    public Attribute,
    public std::enable_shared_from_this<TypedAttribute<T>>
{
public:
    typedef std::shared_ptr<TypedAttribute<T>> Ptr;

    using Attribute::Attribute;

    // Ufe::Attribute overrides
    Type type() const final;

    //! \return The attribute value.
    virtual T get() const = 0;

    //! Set the attribute value.
    virtual void set(const T& value) = 0;

    //! Set the attribute value, and return a command for undo / redo.
    virtual UndoableCommand::Ptr setCmd(const T& value);
};

//! \typedef TypedAttribute<bool> AttributeBool
//! \brief Specialized typed attribute for boolean values.
typedef TypedAttribute<bool> AttributeBool;

//! \typedef TypedAttribute<int> AttributeInt
//! \brief Specialized typed attribute for integer values.
typedef TypedAttribute<int> AttributeInt;

//! \typedef TypedAttribute<float> AttributeFloat
//! \brief Specialized typed attribute for float values.
typedef TypedAttribute<float> AttributeFloat;

//! \typedef TypedAttribute<double> AttributeDouble
//! \brief Specialized typed attribute for double values.
typedef TypedAttribute<double> AttributeDouble;

//! \typedef TypedAttribute<std::string> AttributeString
//! \brief Specialized typed attribute for string values.
typedef TypedAttribute<std::string> AttributeString;

//! \typedef TypedAttribute<Color3f> AttributeColorFloat3
//! \brief Specialized typed attribute for color (3 float) values.
typedef TypedAttribute<Color3f> AttributeColorFloat3;

//! \typedef TypedAttribute<Vector3i> AttributeInt3
//! \brief Specialized typed attribute for 3 integer (vector) values.
typedef TypedAttribute<Vector3i> AttributeInt3;

//! \typedef TypedAttribute<Vector3f> AttributeFloat3
//! \brief Specialized typed attribute for 3 float (vector) values.
typedef TypedAttribute<Vector3f> AttributeFloat3;

//! \typedef TypedAttribute<Vector3d> AttributeDouble3
//! \brief Specialized typed attribute for 3 double (vector) values.
typedef TypedAttribute<Vector3d> AttributeDouble3;

} // end namespace

#endif /* _ufe_attribute */
