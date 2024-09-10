#ifndef _ufe_types
#define _ufe_types

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

#include <array>
#include <limits>

UFE_NS_DEF {

//! \brief Typed vector template for creating a specialized vector.
template<typename T>
struct UFE_SDK_DECL TypedVector3
{
    TypedVector3()
    {
        set(0, 0, 0);
    }

    TypedVector3(T x, T y, T z)
    {
        set(x, y, z);
    }

    T x() const { return std::get<0>(vector); }
    T y() const { return std::get<1>(vector); }
    T z() const { return std::get<2>(vector); }
    void set(T x, T y, T z) { std::get<0>(vector) = x; std::get<1>(vector) = y; std::get<2>(vector) = z; }

    bool operator==(const TypedVector3<T>& rhs) const
    {
        return vector == rhs.vector;
    }

    bool operator!=(const TypedVector3<T>& rhs) const
    {
        return vector != rhs.vector;
    }

    std::array<T, 3> vector;
};

typedef TypedVector3<int> Vector3i;
typedef TypedVector3<float> Vector3f;
typedef TypedVector3<double> Vector3d;

//! \brief Typed color template for creating a specialized color.
template<typename T>
struct UFE_SDK_DECL TypedColor3
{
    TypedColor3()
    {
        set(0, 0, 0);
    }

    TypedColor3(T r, T g, T b)
    {
        set(r, g, b);
    }

    T r() const { return std::get<0>(color); }
    T g() const { return std::get<1>(color); }
    T b() const { return std::get<2>(color); }
    void set(T r, T g, T b) { std::get<0>(color) = r; std::get<1>(color) = g; std::get<2>(color) = b; }

    bool operator==(const TypedColor3<T>& rhs) const
    {
        return color == rhs.color;
    }

    bool operator!=(const TypedColor3<T>& rhs) const
    {
        return color != rhs.color;
    }

    std::array<T, 3> color;
};

typedef TypedColor3<float> Color3f;

//! \brief 3D bounding box class.
/*!
    Axis-aligned 3D bounding box class.  Considered empty if the minimum in any
    dimension is greater than the maximum.
*/
struct UFE_SDK_DECL BBox3d
{
    Vector3d min;
    Vector3d max;

    //! Create an empty bounding box.
    BBox3d() {
        min.vector.fill(std::numeric_limits<double>::max());
        max.vector.fill(std::numeric_limits<double>::lowest());
    }

    BBox3d(const Vector3d& aMin, const Vector3d& aMax) : min(aMin), max(aMax) {}

    bool empty() const {
        return min.x() > max.x() || min.y() > max.y() || min.z() > max.z();
    }
};

//! \brief Matrix class for 3D transforms.
/*!
    Matrix class to allow matrix operations across different runtimes.
*/
struct UFE_SDK_DECL Matrix4d
{
    std::array<std::array<double, 4>, 4> matrix;

    //! Star operator overload to multiply matrices.
    Matrix4d operator*(const Matrix4d& rhs) const;
};


} // end namespace

#endif /* _ufe_types */
