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

//! \brief Typed vector template for creating a specialized vector with N elements.
template <typename T, std::size_t SZ>
struct UFE_SDK_DECL TypedVectorN
{
    typedef std::array<T, SZ> value_type;
    value_type vector;

    TypedVectorN() : vector{{0}} {}

    TypedVectorN(value_type&& v) : vector(v) {}

    template <std::size_t S = SZ, typename = typename std::enable_if<(S == 2)>::type>
    TypedVectorN(T x, T y)
    {
        set(x, y);
    }

    template <std::size_t S = SZ, typename = typename std::enable_if<(S == 3)>::type>
    TypedVectorN(T x, T y, T z)
    {
        set(x, y, z);
    }

    template <std::size_t S = SZ, typename = typename std::enable_if<(S == 4)>::type>
    TypedVectorN(T x, T y, T z, T w)
    {
        set(x, y, z, w);
    }

    template <std::size_t S = SZ>
    typename std::enable_if<(S == 2)>::type
    set(T x, T y)
    {
        std::get<0>(vector) = x;
        std::get<1>(vector) = y;
    }

    template <std::size_t S = SZ>
    typename std::enable_if<(S == 3)>::type
    set(T x, T y, T z)
    {
        std::get<0>(vector) = x;
        std::get<1>(vector) = y;
        std::get<2>(vector) = z;
    }

    template <std::size_t S = SZ>
    typename std::enable_if<(S == 4)>::type
    set(T x, T y, T z, T w)
    {
        std::get<0>(vector) = x;
        std::get<1>(vector) = y;
        std::get<2>(vector) = z;
        std::get<3>(vector) = w;
    }

    T x() const { return std::get<0>(vector); }
    T y() const { return std::get<1>(vector); }
    template <std::size_t S = SZ>
    typename std::enable_if<(S > 2), T>::type
    z() const { return std::get<2>(vector); }
    template <std::size_t S = SZ>
    typename std::enable_if<(S > 3), T>::type
    w() const { return std::get<3>(vector); }

    bool operator==(const TypedVectorN<T, SZ> &rhs) const
    {
        return vector == rhs.vector;
    }

    bool operator!=(const TypedVectorN<T, SZ> &rhs) const
    {
        return vector != rhs.vector;
    }

};

typedef TypedVectorN<int, 2> Vector2i;
typedef TypedVectorN<float, 2> Vector2f;
typedef TypedVectorN<double, 2> Vector2d;
typedef TypedVectorN<int, 3> Vector3i;
typedef TypedVectorN<float, 3> Vector3f;
typedef TypedVectorN<double, 3> Vector3d;
typedef TypedVectorN<int, 4> Vector4i;
typedef TypedVectorN<float, 4> Vector4f;
typedef TypedVectorN<double, 4> Vector4d;

//! \brief Typed color template for creating a specialized color.
template <typename T, std::size_t SZ>
struct UFE_SDK_DECL TypedColorN
{
    typedef std::array<T, SZ> value_type;
    value_type color;

    TypedColorN() : color{{0}} {}

    TypedColorN(value_type&& c) : color(c) {}

    template <std::size_t S = SZ, typename = typename std::enable_if<(S == 3)>::type>
    TypedColorN(T r, T g, T b)
    {
        set(r, g, b);
    }

    template <std::size_t S = SZ, typename = typename std::enable_if<(S == 4)>::type>
    TypedColorN(T r, T g, T b, T a)
    {
        set(r, g, b, a);
    }

    template <std::size_t S = SZ>
    typename std::enable_if<(S == 3)>::type
    set(T r, T g, T b)
    {
        std::get<0>(color) = r;
        std::get<1>(color) = g;
        std::get<2>(color) = b;
    }

    template <std::size_t S = SZ>
    typename std::enable_if<(S == 4)>::type
    set(T r, T g, T b, T a)
    {
        std::get<0>(color) = r;
        std::get<1>(color) = g;
        std::get<2>(color) = b;
        std::get<3>(color) = a;
    }

    T r() const { return std::get<0>(color); }
    T g() const { return std::get<1>(color); }
    T b() const { return std::get<2>(color); }
    template <std::size_t S = SZ>
    typename std::enable_if<(S > 3), T>::type
    a() const { return std::get<3>(color); }

    bool operator==(const TypedColorN<T, SZ> &rhs) const
    {
        return color == rhs.color;
    }

    bool operator!=(const TypedColorN<T, SZ> &rhs) const
    {
        return color != rhs.color;
    }
};

typedef TypedColorN<float, 3> Color3f;
typedef TypedColorN<float, 4> Color4f;

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

//! \brief Typed square matrix template.
/*!
    Matrices are defined to be in row-major order, so <c>matrix[i][j]</c>
    indexes the element in the \e i th row and the \e j th column.
*/
template <typename T, std::size_t SZ>
struct UFE_SDK_DECL TypedSquareMatrixN
{
    typedef std::array<std::array<T, SZ>, SZ> value_type;
    value_type matrix;

    TypedSquareMatrixN()
    {
        TypedVectorN<T,SZ> zero;
        matrix.fill(zero.vector);
    }

    TypedSquareMatrixN(value_type&& m) : matrix(m) {}

    //! Star operator overload to multiply matrices.
    TypedSquareMatrixN operator*(const TypedSquareMatrixN& rhs) const;

    bool operator==(const TypedSquareMatrixN &rhs) const
    {
        return matrix == rhs.matrix;
    }

    bool operator!=(const TypedSquareMatrixN &rhs) const
    {
        return matrix != rhs.matrix;
    }
};

typedef TypedSquareMatrixN<double, 3> Matrix3d;
typedef TypedSquareMatrixN<double, 4> Matrix4d;
} // end namespace

#endif /* _ufe_types */
