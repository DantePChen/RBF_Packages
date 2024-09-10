#ifndef _ufePath
#define _ufePath
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include "pathSegment.h"

#include <vector>

UFE_NS_DEF {

class PathComponent;

//! \brief Identify an object or 3D path in the scene.
/*!

A path consists of one or more \ref Ufe::PathSegment "path segments" that identify an object in the
scene.  The objects are identified by name.

A path can represent containment, through compound nodes that recursively
contain other nodes, or a 3D hierarchy, through parent nodes down to a child.

The runtime of a path is defined to be the runtime of its node, which is the
runtime of the final segment.

The following examples show how to create a path.

\snippet test/python/TestPath.py Creating a Path
*/

class UFE_SDK_DECL Path
{
public:

    typedef std::vector<PathSegment> Segments;
    typedef PathSegment::Components  Components;

    //! Default constructor.  Creates an empty path.
    Path() = default;

    /*!
        Create a Path by copying a vector of PathSegment.
    
        \param segments PathSegments vector of PathSegment.
    */
    Path(const Segments& segments);

    /*!
        Create a Path by moving a vector of PathSegment.
    
        \param segments PathSegments vector of PathSegment.
    */
    Path(Segments&& segments) noexcept;

    /*!
        Convenience to create a Path from a single PathSegment.
        \param segment  PathSegment.
    */
    Path(const PathSegment& segment);

    //! Move constructor.
    Path(PathSegment&& segment) noexcept;

    //! Default copy constructor.
    Path(const Path&) = default;

    //! Default assignment operator.
    Path& operator=(const Path&) = default;

    //! Move construction.  Right hand side becomes empty.
    Path(Path&&) noexcept;

    //! Move assignment.  Right hand side becomes empty.
    Path& operator=(Path&& rhs) noexcept;

    //! \return The number of components in the path.
    Components::size_type size() const;

    //! Convenience method for size() == 0.
    //! \return True if the path is empty.
    bool empty() const;

    //@{
    //! Unfortunately no compiler-generated default equality operators.
    bool operator==(const Path& rhs) const;
    bool operator!=(const Path& rhs) const;
    //@}

    //@{
    //! Return a copy of the path with the argument appended.  Versions with a
    //! path component or string argument throw InvalidOperationOnPath if the
    //! path is empty, as adding a component must be done to a segment with a
    //! valid runtime ID.
    Path operator+(const PathSegment& rhs) const;
    Path operator+(const PathComponent& rhs) const;
    Path operator+(const std::string& rhs) const;
    //@}

    //! \return A copy of the Path with the tail PathComponent removed.
    //! \exception InvalidOperationOnPath If Path is empty.
    Path pop() const;

    //! \return A copy of the Path with the tail PathSegment removed.
    //! \exception InvalidOperationOnPath If Path is empty.
    Path popSegment() const;

    //! \return A copy of the Path with the head PathComponent removed.
    //! \exception InvalidOperationOnPath If Path is empty.
    Path popHead() const;

    //! \param nbComponents The first n PathComponent to retrieve.
    //! \return A copy of the Path that includes only the first n PathComponent.  If n is negative, the last n PathComponent are removed.
    //! \exception InvalidOperationOnPath If argument is 0, > size(), <= -size().
    Path head(int nbComponents) const;

    //! \return The PathComponent at the tail of the Path.
    //! \exception InvalidOperationOnPath If Path is empty.
    PathComponent back() const;

    //! \param newTail The tail component of the sibling path.
    //! \return A copy of the Path with its tail component set to newTail.
    //! \exception InvalidOperationOnPath If Path is empty.
    Path sibling(const PathComponent& newTail) const;

    //! Checks if the path begins with the given prefix. Will return true if
    //! the two paths are equal.
    //! \return True if path starts with the prefix.
    bool startsWith(const Path& prefix) const;

    //! Create a copy of the path, with the path component at index replaced
    //! with the argument.
    //! \param index Index of component to replace.
    //! \param replacement New component.
    //! \return A copy of the path with a single component replaced.
    //! \exception InvalidOperationOnPath If index is < 0 or >= size().
    Path replaceComponent(int index, const PathComponent& replacement) const;

    //! Create a copy of the path, with the originalPrefix replaced by 
    //! newPrefix. If the last PathSegment of newPrefix and the first
    //! PathSegment of *this which follows originalPrefix share a runTimeId
    //! and separator then those PathSegments will be merged into a single
    //! segment.
    //! If originalPrefix is empty then reparent appends *this to newPrefix.
    //! \param originalPrefix Path of *this to be replaced. startsWith(originalPrefix) must return true or originalPrefix must be empty.
    //! \param newPrefix Path which replaces originalPrefix.
    //! \return A copy of the path with originalPrefix replaced with newPrefix.
    //! \exception InvalidOperationOnPath If originalPrefix is not empty and *this does not begin with originalPrefix.
    Path reparent(const Path& originalPrefix, const Path& newPrefix) const;

    //@{
    //! Iteration interface on PathComponents.
    Components::const_iterator cbegin() const;
    Components::const_iterator begin() const;
    Components::const_iterator cend() const;
    Components::const_iterator end() const;
    Components::iterator       begin();
    Components::iterator       end();
    //@}

    //! \return The runtime ID of the final PathSegment.  If the Path is empty, returns a 0 (invalid) ID.
    Rtid runTimeId() const;

    //! \return PathSegments of Path.
    const Segments& getSegments() const;

    //! \return The number of segments in the Path.
    Segments::size_type nbSegments() const;

    //! \return The string representation of the Path.
    std::string string() const;

    //! \return An integer hash value for the Path.  This can be used to add Paths to hash maps.
    std::int32_t hash() const;

private:

    Segments   fSegments;
    // Provide support for component-based access and iteration.  This
    // doubles up space requirements, but components are not expensive
    // (size of a shared pointer, typically two plain pointers).
    Components fComponents;

    // A Path is immutable, so its hash value can be cached once computed.
    // -1 is an illegal hash value.
    mutable std::int32_t fHashValue {-1};
};

}

// Allow for use of Path with std::unordered_map.
namespace std {
template <> struct hash<UFE_VERSIONED_NS::Path> {
    std::size_t operator()(const UFE_VERSIONED_NS::Path& path) const {return path.hash();}
};
}

#endif /* _ufePath */
