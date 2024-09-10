// ===========================================================================
// Copyright 2021 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#ifndef _debugTypesOstream
#define _debugTypesOstream

#include "types.h"

#include <ostream>

UFE_NS_DEF {

    // Partial list of possible output stream operators for types objects. They can be added as required.

    template<class T>
    std::ostream& operator << (std::ostream& os, const TypedVector3<T>& v)
    {
        os << "[" << v.x() << "," << v.y() << "," << v.z() << "]" ;
        return os;
    }

    std::ostream& operator << (std::ostream& os, const Matrix4d& m)
    {
        os << "[" << std::endl ;
        os << m.matrix[0][0] << "," << m.matrix[0][1] << "," << m.matrix[0][2] << "," << m.matrix[0][3] << std::endl;
        os << m.matrix[1][0] << "," << m.matrix[1][1] << "," << m.matrix[1][2] << "," << m.matrix[1][3] << std::endl;
        os << m.matrix[2][0] << "," << m.matrix[2][1] << "," << m.matrix[2][2] << "," << m.matrix[2][3] << std::endl;
        os << m.matrix[3][0] << "," << m.matrix[3][1] << "," << m.matrix[3][2] << "," << m.matrix[3][3] << std::endl;
        os << "]" ;
        return os;
    }

    std::ostream& operator << (std::ostream& os, const BBox3d& bbox)
    {
        os << "min("
           << bbox.min.x() << " "
           << bbox.min.y() << " "
           << bbox.min.z() << ")"
           << "max("
           << bbox.max.x() << " "
           << bbox.max.y() << " "
           << bbox.max.z() << ")\n";
        return os;
    }
}
#endif /* _debugTypesOstream */
