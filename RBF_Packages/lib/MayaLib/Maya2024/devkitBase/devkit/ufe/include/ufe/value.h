#ifndef _ufeValue
#define _ufeValue

// =======================================================================
// Copyright 2021 Autodesk, Inc. All rights reserved.
//
// This computer source code and related instructions and comments are the
// unpublished confidential  and proprietary information of Autodesk, Inc.
// and are protected under applicable copyright and trade secret law. They 
// may not be disclosed to, copied  or used by any third party without the 
// prior written consent of Autodesk, Inc.
// =======================================================================

#include "common/ufeExport.h"

#include <memory>
#include <string>
#include <unordered_map>

UFE_NS_DEF {

//! \brief Value class that can hold a wide set of types.
/*!

This class is instantiated for the following types:
  \li \c bool
  \li \c int
  \li \c float
  \li \c double
  \li \c string
*/

class UFE_SDK_DECL Value {
public:
    //! Default constructor. Creates an empty Value.
    Value();

    /*!
        Create a Value from the argument.
        \param v Value.
    */
    template<typename T> Value(const T& v);

    //! Default copy constructor.
    Value(const Value&);

    //! Move constructor.
    Value(Value&&) noexcept;

    //! Default assignment operator.
    Value& operator=(const Value& v);

    //! Move assignment. Right hand side becomes empty.
    Value& operator=(Value&&) noexcept;

    //! Destructor
    ~Value();

    //! \return The current value if template type matches actual type.
    //! \exception InvalidValueGet If the template type doesn't match the type of the actual value.
    template<typename T> T get() const;

    //! Returns a copy of the value if the template type matches actual type.
    //! Returns the input argument value otherwise.
    //! Especially useful to return a default when dealing with an empty Ufe::Value.
    //! \return The current value if template type matches actual type, otherwise arg.
    template<typename T> T safeGet(T arg) const;

    //! \return True iff this value is empty.
    bool empty() const;

    //! \return The type name of the contained value if non-empty, otherwise empty string.
    std::string typeName() const;

    //! \return True if the contained value is an object of type T.
    template<typename T> bool isType() const;

    //@{
    //! Equality operators.
    bool operator==(const Value& rhs) const;
    bool operator!=(const Value& rhs) const;
    //@}

private:

    struct Imp;

    std::unique_ptr<Imp> _imp;
};

using ValueDictionary = std::unordered_map<std::string, Value>;

}

#endif /* _ufeValue */
