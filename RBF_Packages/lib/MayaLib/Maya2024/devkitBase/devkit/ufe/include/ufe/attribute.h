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
#include "value.h"

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
class UFE_SDK_DECL Attribute : public std::enable_shared_from_this<Attribute>
{
public:
    typedef std::shared_ptr<Attribute> Ptr;
    typedef std::string Type;

    /// \name Built-in attribute types:
    /// @{
    static constexpr char kInvalid[] = "Invalid";
    static constexpr char kBool[] = "Bool";
    static constexpr char kInt[] = "Int";
    static constexpr char kFloat[] = "Float";
    static constexpr char kDouble[] = "Double";
    static constexpr char kString[] = "String";
    static constexpr char kColorFloat3[] = "ColorFloat3";
    static constexpr char kColorFloat4[] = "ColorFloat4";
    static constexpr char kFilename[] = "Filename";
    static constexpr char kEnumString[] = "EnumString";
    static constexpr char kInt3[] = "Int3";
    static constexpr char kFloat2[] = "Float2";
    static constexpr char kFloat3[] = "Float3";
    static constexpr char kFloat4[] = "Float4";
    static constexpr char kDouble3[] = "Double3";
    static constexpr char kMatrix3d[] = "Matrix3d";
    static constexpr char kMatrix4d[] = "Matrix4d";
    static constexpr char kGeneric[] = "Generic";
    /// @}

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

    //@{
    //! Unfortunately no compiler-generated default equality operators. Attribute names
    //! are only unique within a SceneItem, so an Attribute is equal if both the SceneItem
    //! (which matches path) and name match.
    bool operator==(const Attribute& rhs) const;
    bool operator!=(const Attribute& rhs) const;
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

    // --------------------------------------------------------------------- //
    /// \name Metadata Keys:
    /// @{
    // --------------------------------------------------------------------- //

    /*!
        Metadata key: Locked, value type = bool\n
        A locked attribute value cannot be changed.
    */
    static constexpr char kLocked[] = "Locked";

    // --------------------------------------------------------------------- //
    /// @}
    // --------------------------------------------------------------------- //


    // --------------------------------------------------------------------- //
    /// \name Metadata Access:
    /// @{
    // --------------------------------------------------------------------- //

    /*!
        Get the value of the metadata named key.
        \param[in] key The metadata key to query.
        \return The value of the metadata key. If the key does not exist an empty Value is returned.
    */
    virtual Value getMetadata(const std::string& key) const = 0;

    /*!
        Set the metadata key's value to value.
        \param[in] key The metadata key to set.
        \param[in] value The value to set.
        \return True if the metadata was set successfully, otherwise false.
    */
    virtual bool setMetadata(const std::string& key, const Value& value) = 0;

    //! Return a command for undo / redo that sets the metadata key's value to value.
    //! The returned command is not executed; it is up to the caller to call execute().
    virtual UndoableCommand::Ptr setMetadataCmd(const std::string& key, const Value& value);

    /*!
        Clear the metadata key's value.
        \param[in] key The metadata key to clear.
        \return True if the metadata was cleared successfully, otherwise false.
    */
    virtual bool clearMetadata(const std::string& key) = 0;

    //! Returns true if metadata key has a non-empty value.
    virtual bool hasMetadata(const std::string& key) const = 0;

    // --------------------------------------------------------------------- //
    /// @}
    // --------------------------------------------------------------------- //

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

//! \brief Attribute which represents a filename.
class UFE_SDK_DECL AttributeFilename :
    public Attribute
{
public:
    typedef std::shared_ptr<AttributeFilename> Ptr;

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
};

//! \brief Attribute which has a list of strings as enumerated values.
class UFE_SDK_DECL AttributeEnumString :
    public Attribute
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
    public Attribute
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

//! \typedef TypedAttribute<Color4f> AttributeColorFloat4
//! \brief Specialized typed attribute for color (4 float) values.
typedef TypedAttribute<Color4f> AttributeColorFloat4;

//! \typedef TypedAttribute<Vector3i> AttributeInt3
//! \brief Specialized typed attribute for 3 integer (vector) values.
typedef TypedAttribute<Vector3i> AttributeInt3;

//! \typedef TypedAttribute<Vector2f> AttributeFloat2
//! \brief Specialized typed attribute for 2 float (vector) values.
typedef TypedAttribute<Vector2f> AttributeFloat2;

//! \typedef TypedAttribute<Vector3f> AttributeFloat3
//! \brief Specialized typed attribute for 3 float (vector) values.
typedef TypedAttribute<Vector3f> AttributeFloat3;

//! \typedef TypedAttribute<Vector4f> AttributeFloat4
//! \brief Specialized typed attribute for 4 float (vector) values.
typedef TypedAttribute<Vector4f> AttributeFloat4;

//! \typedef TypedAttribute<Vector3d> AttributeDouble3
//! \brief Specialized typed attribute for 3 double (vector) values.
typedef TypedAttribute<Vector3d> AttributeDouble3;

//! \typedef TypedAttribute<Matrix3d> AttributeMatrix3d
//! \brief Specialized typed attribute for 3x3 double (matrix) values.
/*!
    Matrices are defined to be in row-major order, so <c>matrix[i][j]</c>
    indexes the element in the \e i th row and the \e j th column.
*/
typedef TypedAttribute<Matrix3d> AttributeMatrix3d;

//! \typedef TypedAttribute<Matrix4d> AttributeMatrix4d
//! \brief Specialized typed attribute for 4x4 double (matrix) values.
/*!
    Matrices are defined to be in row-major order, so <c>matrix[i][j]</c>
    indexes the element in the \e i th row and the \e j th column.
*/
typedef TypedAttribute<Matrix4d> AttributeMatrix4d;

} // end namespace

#endif /* _ufe_attribute */
