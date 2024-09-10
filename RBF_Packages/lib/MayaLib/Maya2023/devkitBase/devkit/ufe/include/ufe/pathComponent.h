#ifndef _ufePathComponent
#define _ufePathComponent
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "common/ufeExport.h"
#include <string>
#include <memory>
#include <vector>

UFE_NS_DEF {

/**
 * \class PathComponent
 *
 * \brief Constant string representation with fixed space and O(1) comparison.
 *
 * This class provides a representation for strings used as path
 * components.  Identical strings share a representation, and thus can be
 * stored with low storage cost, and O(1) comparison cost.  The shared
 * strings are reference counted, so that when no copies remain, the shared
 * representation is deleted.
 *
 * Default member-wise copy construction is used.
 */

class UFE_SDK_DECL PathComponent {
public:
    class Data;

    /*!
        Static method to verify if the given string exists in the table of strings.
        \param aString String to check if is already in the table of strings.
        \return True if the string already exists in the table of strings.
    */
    static bool exists(const std::string& aString);

    /*!
        Convenience static method to turn a vector of strings into a vector of Path Components.
        \param strings A vector of strings.
        \return A vector of PathComponents.
    */
    static std::vector<PathComponent> toComponents(const std::vector<std::string>& strings);

    //! \return Number of shared strings in the table.  Useful for testing and debugging.
    static std::size_t tableSize();

    //! Create a PathComponent from a string, adding it to the table if it does not exist.
    //! \param aString String to create a PathComponent from. 
    PathComponent(const std::string& aString);

    //! Constructor to create a null PathComponent.
    PathComponent();

    //! Destructor.
    ~PathComponent();

    //! \return Whether the PathComponent is null.
    operator bool() const;

    //@{
    //! Member-wise copy is fine.
    PathComponent(const PathComponent&) = default;
    PathComponent& operator=(const PathComponent&);
    //@}

    //! Equal operator overload.
    bool operator==(const PathComponent& rhs) const;
    //! Not equal operator overload.
    bool operator!=(const PathComponent& rhs) const;

    //! Access the underlying shared string, and its ID.  A null PathComponent has an empty string and an ID of -1.
    const std::string& string() const;
    //! Access the underlying shared string, and its ID.  A null PathComponent has an empty string and an ID of -1.
    std::int64_t id() const;

private:
    void releaseSharedData();

    std::shared_ptr<Data> fData;
};

}

// Hashing support.
namespace std {
template<> struct hash<Ufe::PathComponent> {
    std::size_t operator()(const Ufe::PathComponent& c) const { return c.id(); }
};
}

#endif /* _ufePathComponent */
