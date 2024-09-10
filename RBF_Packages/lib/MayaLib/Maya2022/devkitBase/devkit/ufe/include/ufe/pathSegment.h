#ifndef _ufePathSegment
#define _ufePathSegment
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================


#include "rtid.h"
#include "pathComponent.h"

#include <string>
#include <vector>

UFE_NS_DEF {
    class Path;
//! \brief A segment of a path within a single runtime.
/*!
A path segment is part of a path to an object (either containment or 3D
hierarchy) where all objects on the path segment live in a single runtime.

By itself, a path segment cannot be considered relative or absolute.  The
result of concatenating one or more path segments into a path will result in an
absolute or relative path.

The separator character can be '\0', which is interpreted as meaning no
separator.  This can be used to represent a path segment with a single
component, where there is no hierarchy.
*/

class UFE_SDK_DECL PathSegment
{
public:

    typedef std::vector<PathComponent> Components;

    /*!
        Creates a PathSegment for the argument runtime.
    
        \param[in] segment   String representation of segment, with separators.
        \param[in] runTimeId ID of runtime for this segment.
        \param[in] separator Separator character for segment string.
    */
    PathSegment(
        const std::string& segment,
        Rtid               runTimeId,
        char               separator
    );

    /*!
        Creates a PathSegment for the argument runtime.
    
        \param[in] components Vector of PathComponents.
        \param[in] runTimeId  ID of runtime for this segment.
        \param[in] separator  Separator character for segment string.
    */
    PathSegment(
        const Components& components,
        Rtid              runTimeId,
        char              separator
    );

    /*!
    Creates a PathSegment for the argument runtime [noexcept].
    Move construction.  Right hand side becomes empty.
    \param[in] components Vector of path components.
    \param[in] runTimeId  ID of runtime for this segment.
    \param[in] separator  Separator character for segment string.
    */
    PathSegment(
        Components&& components,
        Rtid              runTimeId,
        char              separator
    ) noexcept; 

    /*! Convenience for single-component path.
        \param[in] component  Single path components.
        \param[in] runTimeId  ID of runtime for this segment.
        \param[in] separator  Separator character for segment string.
    */
    PathSegment(
        const PathComponent& component,
        Rtid                 runTimeId,
        char                 separator
    );

    //! Default copy constructor.
    PathSegment(const PathSegment&) = default;

    //! Default assignment operator.
    PathSegment& operator=(const PathSegment&) = default;

    //! Move construction.  Right hand side becomes empty.
    PathSegment(PathSegment&&) noexcept;

    //! Move assignment.  Right hand side becomes empty.
    PathSegment& operator=(PathSegment&& rhs) noexcept;

    //! \return Segment runtime ID.
    Rtid runTimeId() const;

    //! \return Segment separator.
    char separator() const;

    //! \return PathComponents list.
    const Components& components() const;

    //! \return A copy of the PathSegment with the tail PathComponent removed.
    //! \exception InvalidOperationOnPathSegment If PathSegment is empty.
    PathSegment pop() const;

    //! \return A copy of the PathSegment with the head PathComponent removed.
    //! \exception InvalidOperationOnPathSegment If PathSegment is empty.
    PathSegment popHead() const;

    /*!
        \param nbComponents The first n PathComponent to retrieve.
        \return A copy of the PathSegment that includes only the first n PathComponents.  If n is negative, the last n PathComponents are removed.
        \exception InvalidOperationOnPathSegment If argument is 0, > size(), <= -size().
    */
    PathSegment head(int nbComponents) const;

    //@{
    //! Unfortunately no compiler-generated default equality operators.
    bool operator==(const PathSegment& rhs) const;
    bool operator!=(const PathSegment& rhs) const;
    //@}

    // Container of components interface.
    //! \return Size of PathComponents.
    Components::size_type      size() const;
    //! \return True if there are no PathComponents.
    bool                       empty() const;
    
    //@{
    //! Iteration interface on PathComponents.
    Components::const_iterator cbegin() const;
    Components::const_iterator begin() const;
    Components::const_iterator cend() const;
    Components::const_iterator end() const;
    Components::iterator       begin();
    Components::iterator       end();
    //@}

    //! \return The string representation of the PathSegment.
    std::string string() const;

private:
    friend Path;
    Components fComponents;
    Rtid       fRunTimeId;
    char       fSeparator;
};

}

#endif /* _ufePathSegment */
