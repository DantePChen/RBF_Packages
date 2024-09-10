from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import pymel.api as _api
import pymel.versions as versions
import pymel.util as util
import colorsys
import pymel.internal.factories as _f
import pymel.internal.factories as _factories
import os


"""
A wrap of Maya's Vector, Point, Color, Matrix, TransformationMatrix, Quaternion, EulerRotation types
"""


from pymel.util.arrays import *


from maya.OpenMaya import MPoint as _MPoint
from maya.OpenMaya import MFloatVector as _MFloatVector
from pymel.util.arrays import _toCompOrArrayInstance
from maya.OpenMaya import MColor as _MColor
from pymel.util.enum import Enum
from maya.OpenMaya import MBoundingBox as _MBoundingBox
from maya.OpenMaya import MQuaternion as _MQuaternion
from maya.OpenMaya import MTransformationMatrix as _MTransformationMatrix
from maya.OpenMaya import MSpace as _MSpace
from maya.OpenMaya import MFloatMatrix as _MFloatMatrix
from maya.OpenMaya import MMatrix as _MMatrix
from maya.OpenMaya import MEulerRotation as _MEulerRotation
from maya.OpenMaya import MVector as _MVector
from maya.OpenMaya import MFloatPoint as _MFloatPoint


if False:
    from typing import Dict, List, Tuple, Union, Optional

class Vector(VectorN, _MVector):
    """
    A 3 dimensional vector class that wraps Maya's api Vector class
    
        >>> from pymel.all import *
        >>> import pymel.core.datatypes as dt
        >>>
        >>> v = dt.Vector(1, 2, 3)
        >>> w = dt.Vector(x=1, z=2)
        >>> z = dt.Vector( dt.Vector.xAxis, z=1)
    
        >>> v = dt.Vector(1, 2, 3, unit='meters')
        >>> print v
        [1.0, 2.0, 3.0]
    """
    
    
    
    def __add__(self, other):
        """
        u.__add__(v) <==> u+v
        Returns the result of the addition of u and v if v is convertible to a VectorN (element-wise addition),
        adds v to every component of u if v is a scalar
        """
        ...
    def __contains__(self, value):
        """
        True if at least one of the vector components is equal to the argument
        """
        ...
    def __div__(self, other):
        """
        u.__div__(v) <==> u/v
        Returns the result of the division of u by v if v is convertible to a VectorN (element-wise division),
        divide every component of u by v if v is a scalar
        """
        ...
    def __eq__(self, other: Any) -> bool:
        """
        u.__eq__(v) <==> u == v
        Equivalence test
        """
        ...
    def __getitem__(self, i):
        """
        Get component i value from self
        """
        ...
    def __iadd__(self, other):
        """
        u.__iadd__(v) <==> u += v
        In place addition of u and v, see __add__
        """
        ...
    def __idiv__(self, other):
        """
        u.__idiv__(v) <==> u /= v
        In place division of u by v, see __div__
        """
        ...
    def __imul__(self, other):
        """
        u.__imul__(v) <==> u *= v
        Valid for Vector * Matrix multiplication, in place transformation of u by Matrix v
        or Vector by scalar multiplication only
        """
        ...
    def __init__(self, *args, **kwargs):
        """
        __init__ method, valid for Vector, Point and Color classes
        """
        ...
    def __isub__(self, other):
        """
        u.__isub__(v) <==> u -= v
        In place substraction of u and v, see __sub__
        """
        ...
    def __iter__(self, *args, **kwargs):
        """
        Iterate on the api components
        """
        ...
    def __ixor__(self, other):
        """
        u.__xor__(v) <==> u^=v
        Inplace cross product or transformation by inverse transpose of v is v is a MatrixN
        """
        ...
    def __len__(self) -> int:
        """
        Number of components in the Vector instance, 3 for Vector, 4 for Point and Color
        """
        ...
    def __mul__(self, other):
        """
        u.__mul__(v) <==> u*v
        The multiply '*' operator is mapped to the dot product when both objects are Vectors,
        to the transformation of u by matrix v when v is a MatrixN,
        to element wise multiplication when v is a sequence,
        and multiplies each component of u by v when v is a numeric type.
        """
        ...
    def __ne__(self, other: Any) -> bool:
        """
        u.__ne__(v) <==> u != v
        Equivalence test
        """
        ...
    def __neg__(self):
        """
        u.__neg__() <==> -u
        The unary minus operator. Negates the value of each of the components of u
        """
        ...
    def __radd__(self, other):
        """
        u.__radd__(v) <==> v+u
        Returns the result of the addition of u and v if v is convertible to a VectorN (element-wise addition),
        adds v to every component of u if v is a scalar
        """
        ...
    def __rdiv__(self, other):
        """
        u.__rdiv__(v) <==> v/u
        Returns the result of of the division of v by u if v is convertible to a VectorN (element-wise division),
        invert every component of u and multiply it by v if v is a scalar
        """
        ...
    def __repr__(self) -> str: ...
    def __rmul__(self, other):
        """
        u.__rmul__(v) <==> v*u
        The multiply '*' operator is mapped to the dot product when both objects are Vectors,
        to the left side multiplication (pre-multiplication) of u by matrix v when v is a MatrixN,
        to element wise multiplication when v is a sequence,
        and multiplies each component of u by v when v is a numeric type.
        """
        ...
    def __rsub__(self, other):
        """
        u.__rsub__(v) <==> v-u
        Returns the result of the substraction of u from v if v is convertible to a VectorN (element-wise substration),
        replace every component c of u by v-c if v is a scalar
        """
        ...
    def __setattr__(self, name, value):
        """
        Fixes __setattr__ to work properly with properties
        
        Maya has a bug on windows where some api objects have a __setattr__
        that bypasses properties (and other data descriptors).
        """
        ...
    def __setitem__(self, i, a):
        """
        Set component i value on self
        """
        ...
    def __sub__(self, other):
        """
        u.__sub__(v) <==> u-v
        Returns the result of the substraction of v from u if v is convertible to a VectorN (element-wise substration),
        substract v to every component of u if v is a scalar
        """
        ...
    def __xor__(self, other):
        """
        u.__xor__(v) <==> u^v
        Defines the cross product operator between two 3D vectors,
        if v is a MatrixN, u^v is equivalent to u.transformAsNormal(v)
        """
        ...
    def angle(self, other):
        """
        u.angle(v) <==> angle(u, v) --> float
        Returns the angle (in radians) between the two vectors u and v
        Note that this angle is not signed, use axis to know the direction of the rotation
        """
        ...
    def assign(self, value):
        """
        Wrap the Vector api assign method
        """
        ...
    def axis(self, other, normalize='False'):
        """
        u.axis(v) <==> angle(u, v) --> Vector
        Returns the axis of rotation from u to v as the vector n = u ^ v
        if the normalize keyword argument is set to True, n is also normalized
        """
        ...
    def cotan(self, other):
        """
        u.cotan(v) <==> cotan(u, v) --> float :
        cotangent of the a, b angle, a and b should be MVectors
        """
        ...
    def cross(self, other):
        """
        cross product, only defined for two 3D vectors
        """
        ...
    def distanceTo(self, other): ...
    def dot(self, other):
        """
        dot product of two vectors
        """
        ...
    def get(self):
        """
        Wrap the Vector api get method
        """
        ...
    def isEquivalent(self, other, tol='None'):
        """
        Returns true if both arguments considered as Vector are equal within the specified tolerance
        """
        ...
    def isParallel(self, other, tol='None'):
        """
        Returns true if both arguments considered as Vector are parallel within the specified tolerance
        """
        ...
    def length(self):
        """
        Return the length of the vector
        """
        ...
    def normal(self):
        """
        Return a normalized copy of self
        """
        ...
    def normalize(self):
        """
        Performs an in place normalization of self
        """
        ...
    def rotateBy(self, *args):
        """
        u.rotateBy(*args) --> Vector
        Returns the result of rotating u by the specified arguments.
        There are several ways the rotation can be specified:
        args is a tuple of one Matrix, TransformationMatrix, Quaternion, EulerRotation
        arg is tuple of 4 arguments, 3 rotation value and an optionnal rotation order
        args is a tuple of one Vector, the axis and one float, the angle to rotate around that axis in radians
        """
        ...
    def rotateTo(self, other):
        """
        u.rotateTo(v) --> Quaternion
        Returns the Quaternion that represents the rotation of the Vector u into the Vector v
        around their mutually perpendicular axis. It amounts to rotate u by angle(u, v) around axis(u, v)
        """
        ...
    def sqlength(self):
        """
        Return the square length of the vector
        """
        ...
    def transformAsNormal(self, other):
        """
        Returns the vector transformed by the matrix as a normal
        Normal vectors are not transformed in the same way as position vectors or points.
        If this vector is treated as a normal vector then it needs to be transformed by
        post multiplying it by the inverse transpose of the transformation matrix.
        This method will apply the proper transformation to the vector as if it were a normal.
        """
        ...
    @staticmethod
    def __new__(cls, *args, **kwargs): ...
    @property
    def data(self):
        """
        The Vector/FloatVector/Point/FloatPoint/Color data
        """
        ...
    @data.setter
    def data(self, value): ...
    @property
    def x(self): ...
    @x.setter
    def x(self, value): ...
    @property
    def y(self): ...
    @y.setter
    def y(self, value): ...
    @property
    def z(self): ...
    @z.setter
    def z(self, value): ...
    Axis : Enum
    
    __dict__ : dictproxy
    
    
    
    __readonly__ : dict
    
    __weakref__ : getset_descriptor
    
    apicls : Type[_MVector]
    
    cnames : tuple
    
    ndim : int
    
    one : Vector
    
    shape : tuple
    
    size : int
    
    unit : NoneType
    
    xAxis : Vector
    
    xNegAxis : Vector
    
    yAxis : Vector
    
    yNegAxis : Vector
    
    zAxis : Vector
    
    zNegAxis : Vector
    
    zero : Vector


class Space(_MSpace):
    Space : Enum
    
    
    
    __readonly__ : dict
    
    apicls : Type[_MSpace]


class EulerRotation(Array, _MEulerRotation):
    """
    unit handling:
    >>> from pymel.all import *
    >>> import pymel.core.datatypes as dt
    >>>
    >>> currentUnit(angle='degree')
    u'degree'
    >>> e = dt.EulerRotation([math.pi,0,0], unit='radians')
    >>> e
    dt.EulerRotation([3.14159265359, 0.0, 0.0], unit='radians')
    >>> e2 = dt.EulerRotation([180,0,0], unit='degrees')
    >>> e2
    dt.EulerRotation([180.0, 0.0, 0.0])
    >>> e.isEquivalent( e2 )
    True
    >>> e == e2
    True
    
    units are only displayed when they do not match the current ui unit
    >>> dt.Angle.getUIUnit() # check current angular unit
    'degrees'
    >>> e
    dt.EulerRotation([3.14159265359, 0.0, 0.0], unit='radians')
    >>> dt.Angle.setUIUnit('radians')  # change to radians
    >>> e
    dt.EulerRotation([3.14159265359, 0.0, 0.0])
    """
    
    
    
    def __add__(self, other):
        """
        u.__add__(v) <==> u+v
        Returns the result of the addition of u and v if v is convertible to a VectorN (element-wise addition),
        adds v to every component of u if v is a scalar
        """
        ...
    def __contains__(self, value):
        """
        True if at least one of the vector components is equal to the argument
        """
        ...
    def __div__(self, other):
        """
        u.__div__(v) <==> u/v
        Returns the result of the division of u by v if v is convertible to a VectorN (element-wise division),
        divide every component of u by v if v is a scalar
        """
        ...
    def __eq__(self, other: Any) -> bool:
        """
        u.__eq__(v) <==> u == v
        Equivalence test
        """
        ...
    def __getitem__(self, i): ...
    def __iadd__(self, other):
        """
        u.__iadd__(v) <==> u += v
        In place addition of u and v, see __add__
        """
        ...
    def __idiv__(self, other):
        """
        u.__idiv__(v) <==> u /= v
        In place division of u by v, see __div__
        """
        ...
    def __imul__(self, other):
        """
        u.__imul__(v) <==> u *= v
        Valid for EulerRotation * Matrix multiplication, in place transformation of u by Matrix v
        or EulerRotation by scalar multiplication only
        """
        ...
    def __init__(self, *args, **kwargs):
        """
        __init__ method for EulerRotation
        """
        ...
    def __isub__(self, other):
        """
        u.__isub__(v) <==> u -= v
        In place substraction of u and v, see __sub__
        """
        ...
    def __iter__(self): ...
    def __len__(self) -> int: ...
    def __mul__(self, other):
        """
        u.__mul__(v) <==> u*v
        The multiply '*' operator is mapped to the dot product when both objects are Vectors,
        to the transformation of u by matrix v when v is a MatrixN,
        to element wise multiplication when v is a sequence,
        and multiplies each component of u by v when v is a numeric type.
        """
        ...
    def __ne__(self, other: Any) -> bool:
        """
        u.__ne__(v) <==> u != v
        Equivalence test
        """
        ...
    def __neg__(self):
        """
        u.__neg__() <==> -u
        The unary minus operator. Negates the value of each of the components of u
        """
        ...
    def __radd__(self, other):
        """
        u.__radd__(v) <==> v+u
        Returns the result of the addition of u and v if v is convertible to a VectorN (element-wise addition),
        adds v to every component of u if v is a scalar
        """
        ...
    def __rdiv__(self, other):
        """
        u.__rdiv__(v) <==> v/u
        Returns the result of of the division of v by u if v is convertible to a VectorN (element-wise division),
        invert every component of u and multiply it by v if v is a scalar
        """
        ...
    def __repr__(self) -> str: ...
    def __rmul__(self, other):
        """
        u.__rmul__(v) <==> v*u
        The multiply '*' operator is mapped to the dot product when both objects are Vectors,
        to the left side multiplication (pre-multiplication) of u by matrix v when v is a MatrixN,
        to element wise multiplication when v is a sequence,
        and multiplies each component of u by v when v is a numeric type.
        """
        ...
    def __rsub__(self, other):
        """
        u.__rsub__(v) <==> v-u
        Returns the result of the substraction of u from v if v is convertible to a VectorN (element-wise substration),
        replace every component c of u by v-c if v is a scalar
        """
        ...
    def __setattr__(self, name, value):
        """
        Fixes __setattr__ to work properly with properties
        
        Maya has a bug on windows where some api objects have a __setattr__
        that bypasses properties (and other data descriptors).
        """
        ...
    def __setitem__(self, key, val): ...
    def __sub__(self, other):
        """
        u.__sub__(v) <==> u-v
        Returns the result of the substraction of v from u if v is convertible to a VectorN (element-wise substration),
        substract v to every component of u if v is a scalar
        """
        ...
    def alternateSolution(self):
        """
        Returns an alternate solution to this rotation. The resulting rotation will be bound between +/- PI, and the rotation order will remain unchanged.
        
        Returns:
        'EulerRotation'
        
        Derived from api method `maya.OpenMaya.MEulerRotation.alternateSolution`
        """
        ...
    def asMatrix(self):
        """
        Converts an euler rotation to a rotation matrix.
        
        Returns:
        'Matrix'
        
        Derived from api method `maya.OpenMaya.MEulerRotation.asMatrix`
        """
        ...
    def asQuaternion(self):
        """
        Converts an euler rotation to a quaternion.
        
        Returns:
        'Quaternion'
        
        Derived from api method `maya.OpenMaya.MEulerRotation.asQuaternion`
        """
        ...
    def asVector(self):
        """
        Converts an euler rotation to a vector. The rotation order component is dropped.
        
        Returns:
        'Vector'
        
        Derived from api method `maya.OpenMaya.MEulerRotation.asVector`
        """
        ...
    def assign(self, *args, **kwargs):
        """
        Wrap the Quaternion api assign method
        """
        ...
    def bound(self):
        """
        Returns the result of bounding this rotation to be within +/- PI. Bounding a rotation to be within +/- PI is defined to be the result of offsetting the rotation by +/- 2nPI (term by term) such that the offset is within +/- PI.
        
        Returns:
        'EulerRotation'
        
        Derived from api method `maya.OpenMaya.MEulerRotation.bound`
        """
        ...
    def boundIt(self, src):
        """
        Sets this euler rotation to be the input rotation that has been bound to be within +/- PI. Bounding a rotation to be within +/- PI is defined to be the result of offsetting the rotation by +/- 2nPI (term by term) such that the offset is within +/- PI.
        
        Parameters:
        src : 'EulerRotation'
            the input rotation that will be bound
        
        
        Returns:
        'EulerRotation'
        
        Derived from api method `maya.OpenMaya.MEulerRotation.boundIt`
        """
        ...
    def closestCut(self, dst):
        """
        Returns the closest cut of this rotation to "dst". The closest cut of rotation A to rotation B is defined to be the rotation that is +/- 2nPI to rotation A (term by term) and within +/- PI to rotation B.
        
        Parameters:
        dst : 'EulerRotation'
            the range of the closest cut
        
        
        Returns:
        'EulerRotation'
        
        Derived from api method `maya.OpenMaya.MEulerRotation.closestCut`
        """
        ...
    def closestSolution(self, dst):
        """
        Returns the euler rotation that is the closest solution to the "dst" euler rotation.
        
        Parameters:
        dst : 'EulerRotation'
            the euler rotation to which the solution should be closest
        
        
        Returns:
        'EulerRotation'
        
        Derived from api method `maya.OpenMaya.MEulerRotation.closestSolution`
        """
        ...
    def get(self):
        """
        Wrap the MEulerRotation api get method
        """
        ...
    def incrementalRotateBy(self, axis, angle):
        """
        Perform an incremental rotation by the specified axis and angle. The rotation is broken down and performed in smaller steps so that the angles update properly.
        
        Parameters:
        axis : 'Vector'
            the axis to rotate around
        angle : 'float'
            the angle by which to rotate around the axis
        
        
        Returns:
        'EulerRotation'
        
        Derived from api method `maya.OpenMaya.MEulerRotation.incrementalRotateBy`
        """
        ...
    def inverse(self):
        """
        Returns the inverse of this euler rotation. The rotation order will be reversed.
        
        Returns:
        'EulerRotation'
        
        Derived from api method `maya.OpenMaya.MEulerRotation.inverse`
        """
        ...
    def invertIt(self):
        """
        Performs an in place inversion of this euler rotation. The rotation order will be reversed.
        
        Returns:
        'EulerRotation'
        
        Derived from api method `maya.OpenMaya.MEulerRotation.invertIt`
        """
        ...
    def isZero(self, tolerance='1e-10'):
        """
        This method returns true if this euler rotation is zero, within some given tolerance.
        
        Parameters:
        tolerance : 'float'
            the amount of variation allowed for equivalency to zero
        
        
        Returns:
        'bool'
        
        Derived from api method `maya.OpenMaya.MEulerRotation.isZero`
        """
        ...
    def reorder(self, ord):
        """
        Returns the reordering of this euler rotation, such that the euler rotation will have the specified rotation order.
        
        Parameters:
        ord : 'EulerRotation.RotationOrder'
            the new rotation order of the euler rotation
        
            values: 'XYZ', 'YZX', 'ZXY', 'XZY', 'YXZ', 'ZYX'
        
        
        Returns:
        'EulerRotation'
        
        Derived from api method `maya.OpenMaya.MEulerRotation.reorder`
        """
        ...
    def reorderIt(self, ord):
        """
        Performs an in place reordering of this euler rotation, such that the euler rotation will have the specified rotation order.
        
        Parameters:
        ord : 'EulerRotation.RotationOrder'
            the new rotation order of the euler rotation
        
            values: 'XYZ', 'YZX', 'ZXY', 'XZY', 'YXZ', 'ZYX'
        
        
        Returns:
        'EulerRotation'
        
        Derived from api method `maya.OpenMaya.MEulerRotation.reorderIt`
        """
        ...
    def setDisplayUnit(self, unit): ...
    def setToAlternateSolution(self, src):
        """
        Sets this euler rotation to an alternate solution of the input rotation. The resulting rotation will be bound between +/- PI, and the rotation order will remain unchanged.
        
        Parameters:
        src : 'EulerRotation'
            the rotation to compute an alternate solution to
        
        
        Returns:
        'EulerRotation'
        
        Derived from api method `maya.OpenMaya.MEulerRotation.setToAlternateSolution`
        """
        ...
    def setToClosestCut(self, src, dst):
        """
        Sets this rotation to be the closest cut of "src" to "dst". The closest cut of rotation A to rotation B is defined to be the rotation that is +/- 2nPI to rotation A (term by term) and within +/- PI to rotation B.
        
        Parameters:
        src : 'EulerRotation'
            the euler rotation whose terms will be offset by +/- 2nPI
        dst : 'EulerRotation'
            the range of the closest cut
        
        
        Returns:
        'EulerRotation'
        
        Derived from api method `maya.OpenMaya.MEulerRotation.setToClosestCut`
        """
        ...
    def setToClosestSolution(self, src, dst):
        """
        Sets this euler rotation to the euler rotation that is the closest solution of the "src" euler rotation to the "dst" euler rotation.
        
        Parameters:
        src : 'EulerRotation'
            the euler rotation whose closest solution will be calculated
        dst : 'EulerRotation'
            the euler rotation to which the solution should be closest
        
        
        Returns:
        'EulerRotation'
        
        Derived from api method `maya.OpenMaya.MEulerRotation.setToClosestSolution`
        """
        ...
    def setValue(self, v, ord="'XYZ'"):
        """
        Sets the euler rotation to the values contained in the vector and with the specified rotation order.
        
        Parameters:
        v : 'Vector'
            vector from which to set the x, y, and z rotation components
        ord : 'EulerRotation.RotationOrder'
            the rotation order; the default rotation order is XYZ
        
            values: 'XYZ', 'YZX', 'ZXY', 'XZY', 'YXZ', 'ZYX'
        
        
        Returns:
        'EulerRotation'
        
        Derived from api method `maya.OpenMaya.MEulerRotation.setValue`
        """
        ...
    @classmethod
    def decompose(self, matrix, ord):
        """
        Decompose a rotation matrix into the desired euler angles with the specified order.
        
        Parameters:
        matrix : 'Matrix'
            the matrix that will be decomposed into an euler rotation with the specified order
        ord : 'EulerRotation.RotationOrder'
            the order which the euler rotation will have
        
            values: 'XYZ', 'YZX', 'ZXY', 'XZY', 'YXZ', 'ZYX'
        
        
        Returns:
        'EulerRotation'
        
        Derived from api method `maya.OpenMaya.MEulerRotation.decompose`
        """
        ...
    @staticmethod
    def __new__(cls, *args, **kwargs): ...
    @property
    def order(self): ...
    @order.setter
    def order(self, value): ...
    @property
    def x(self): ...
    @x.setter
    def x(self, value): ...
    @property
    def y(self): ...
    @y.setter
    def y(self, value): ...
    @property
    def z(self): ...
    @z.setter
    def z(self, value): ...
    RotationOrder : Enum
    
    __dict__ : dictproxy
    
    
    
    __readonly__ : dict
    
    __weakref__ : getset_descriptor
    
    apicls : Type[_MEulerRotation]
    
    cnames : tuple
    
    identity : EulerRotation
    
    ndim : int
    
    shape : tuple
    
    size : int


class Matrix(MatrixN, _MMatrix):
    """
    A 4x4 transformation matrix based on api Matrix
    
        >>> from pymel.all import *
        >>> import pymel.core.datatypes as dt
        >>>
        >>> i = dt.Matrix()
        >>> print i.formated()
        [[1.0, 0.0, 0.0, 0.0],
         [0.0, 1.0, 0.0, 0.0],
         [0.0, 0.0, 1.0, 0.0],
         [0.0, 0.0, 0.0, 1.0]]
    
        >>> v = dt.Matrix(1, 2, 3)
        >>> print v.formated()
        [[1.0, 2.0, 3.0, 0.0],
         [1.0, 2.0, 3.0, 0.0],
         [1.0, 2.0, 3.0, 0.0],
         [1.0, 2.0, 3.0, 0.0]]
    """
    
    
    
    def __add__(self, other):
        """
        m.__add__(v) <==> m+v
        Returns the result of the addition of m and v if v is convertible to a MatrixN (element-wise addition),
        adds v to every component of m if v is a scalar
        """
        ...
    def __delitem__(self, index):
        """
        Cannot delete from a class with a fixed shape
        """
        ...
    def __delslice__(self, start, end): ...
    def __eq__(self, other: Any) -> bool:
        """
        m.__eq__(v) <==> m == v
        Equivalence test
        """
        ...
    def __getitem__(self, index):
        """
        m.__getitem__(index) <==> m[index]
        Get component index value from self.
        index can be a single numeric value or slice, thus one or more rows will be returned,
        or a row,column tuple of numeric values / slices
        """
        ...
    def __getslice__(self, start, end):
        """
        # deprecated and __getitem__ should accept slices anyway
        """
        ...
    def __iadd__(self, other):
        """
        m.__iadd__(v) <==> m += v
        In place addition of m and v, see __add__
        """
        ...
    def __imul__(self, other):
        """
        m.__imul__(n) <==> m *= n
        Valid for Matrix * Matrix multiplication, in place multiplication of MatrixN m by MatrixN n
        """
        ...
    def __init__(self, *args, **kwargs):
        """
        __init__ method, valid for Vector, Point and Color classes
        """
        ...
    def __isub__(self, other):
        """
        m.__isub__(v) <==> m -= v
        In place substraction of m and v, see __sub__
        """
        ...
    def __iter__(self, *args, **kwargs):
        """
        Iterate on the Matrix rows
        """
        ...
    def __len__(self) -> int:
        """
        Number of components in the Matrix instance
        """
        ...
    def __melobject__(self):
        """
        Special method for returning a mel-friendly representation. In this case, a flat list of 16 values
        """
        ...
    def __mul__(self, other):
        """
        m.__mul__(x) <==> m*x
        If x is a MatrixN, __mul__ is mapped to matrix multiplication m*x, if x is a VectorN, to MatrixN by VectorN multiplication.
        Otherwise, returns the result of the element wise multiplication of m and x if x is convertible to Array,
        multiplies every component of b by x if x is a single numeric value
        """
        ...
    def __ne__(self, other: Any) -> bool:
        """
        m.__ne__(v) <==> m != v
        Equivalence test
        """
        ...
    def __neg__(self):
        """
        m.__neg__() <==> -m
        The unary minus operator. Negates the value of each of the components of m
        """
        ...
    def __radd__(self, other):
        """
        m.__radd__(v) <==> v+m
        Returns the result of the addition of m and v if v is convertible to a MatrixN (element-wise addition),
        adds v to every component of m if v is a scalar
        """
        ...
    def __rmul__(self, other):
        """
        m.__rmul__(x) <==> x*m
        If x is a MatrixN, __rmul__ is mapped to matrix multiplication x*m, if x is a VectorN (or Vector or Point or Color),
        to transformation, ie VectorN by MatrixN multiplication.
        Otherwise, returns the result of the element wise multiplication of m and x if x is convertible to Array,
        multiplies every component of m by x if x is a single numeric value
        """
        ...
    def __rsub__(self, other):
        """
        m.__rsub__(v) <==> v-m
        Returns the result of the substraction of m from v if v is convertible to a MatrixN (element-wise substration),
        replace every component c of m by v-c if v is a scalar
        """
        ...
    def __setattr__(self, name, value):
        """
        Fixes __setattr__ to work properly with properties
        
        Maya has a bug on windows where some api objects have a __setattr__
        that bypasses properties (and other data descriptors).
        """
        ...
    def __setitem__(self, index, value):
        """
        m.__setitem__(index, value) <==> m[index] = value
        Set value of component index on self
        index can be a single numeric value or slice, thus one or more rows will be returned,
        or a row,column tuple of numeric values / slices
        """
        ...
    def __setslice__(self, start, end, value):
        """
        # deprecated and __setitem__ should accept slices anyway
        """
        ...
    def __sub__(self, other):
        """
        m.__sub__(v) <==> m-v
        Returns the result of the substraction of v from m if v is convertible to a MatrixN (element-wise substration),
        substract v to every component of m if v is a scalar
        """
        ...
    def adjoint(self):
        """
        Returns the adjoint (adjugate) Matrix
        """
        ...
    def asMatrix(self, percent='None'):
        """
        The matrix representation for this Matrix/TransformationMatrix/Quaternion/EulerRotation instance
        """
        ...
    def assign(self, value):
        """
        # overloads for assign and get though standard way should be to use the data property
        # to access stored values
        """
        ...
    def blend(self, other, weight='0.5'):
        """
        Returns a 0.0-1.0 scalar weight blend between self and other Matrix,
        blend mixes Matrix as transformation matrices
        """
        ...
    def det(self):
        """
        Returns the determinant of this Matrix instance
        """
        ...
    def det3x3(self):
        """
        Returns the determinant of the upper left 3x3 submatrix of this Matrix instance,
        it's the same as doing det(m[0:3, 0:3])
        """
        ...
    def det4x4(self):
        """
        Returns the 4x4 determinant of this Matrix instance
        """
        ...
    def get(self):
        """
        Wrap the Matrix api get method
        """
        ...
    def homogenize(self):
        """
        Returns a homogenized version of the Matrix
        """
        ...
    def inverse(self):
        """
        Returns the inverse Matrix
        """
        ...
    def isEquivalent(self, other, tol='1e-10'):
        """
        Returns true if both arguments considered as Matrix are equal within the specified tolerance
        """
        ...
    def isSingular(self):
        """
        Returns True if the given Matrix is singular
        """
        ...
    def setToIdentity(self):
        """
        m.setToIdentity() <==> m = a * b
        Sets MatrixN to the identity matrix
        """
        ...
    def setToProduct(self, left, right):
        """
        m.setToProduct(a, b) <==> m = a * b
        Sets MatrixN to the result of the product of MatrixN a and MatrixN b
        """
        ...
    def transpose(self):
        """
        Returns the transposed Matrix
        """
        ...
    def weighted(self, weight):
        """
        Returns a 0.0-1.0 scalar weighted blend between identity and self
        """
        ...
    @staticmethod
    def __new__(cls, *args, **kwargs): ...
    @property
    def a00(self): ...
    @a00.setter
    def a00(self, value): ...
    @property
    def a01(self): ...
    @a01.setter
    def a01(self, value): ...
    @property
    def a02(self): ...
    @a02.setter
    def a02(self, value): ...
    @property
    def a03(self): ...
    @a03.setter
    def a03(self, value): ...
    @property
    def a10(self): ...
    @a10.setter
    def a10(self, value): ...
    @property
    def a11(self): ...
    @a11.setter
    def a11(self, value): ...
    @property
    def a12(self): ...
    @a12.setter
    def a12(self, value): ...
    @property
    def a13(self): ...
    @a13.setter
    def a13(self, value): ...
    @property
    def a20(self): ...
    @a20.setter
    def a20(self, value): ...
    @property
    def a21(self): ...
    @a21.setter
    def a21(self, value): ...
    @property
    def a22(self): ...
    @a22.setter
    def a22(self, value): ...
    @property
    def a23(self): ...
    @a23.setter
    def a23(self, value): ...
    @property
    def a30(self): ...
    @a30.setter
    def a30(self, value): ...
    @property
    def a31(self): ...
    @a31.setter
    def a31(self, value): ...
    @property
    def a32(self): ...
    @a32.setter
    def a32(self, value): ...
    @property
    def a33(self): ...
    @a33.setter
    def a33(self, value): ...
    @property
    def data(self):
        """
        The Matrix/FloatMatrix/TransformationMatrix/Quaternion/EulerRotation data
        """
        ...
    @data.setter
    def data(self, value): ...
    @property
    def matrix(self):
        """
        The Matrix representation for this Matrix/TransformationMatrix/Quaternion/EulerRotation instance
        """
        ...
    @property
    def rotate(self):
        """
        The rotation expressed in this Matrix, in transform space
        """
        ...
    @rotate.setter
    def rotate(self, value): ...
    @property
    def scale(self):
        """
        The scale expressed in this Matrix, in transform space
        """
        ...
    @scale.setter
    def scale(self, value): ...
    @property
    def translate(self):
        """
        The translation expressed in this Matrix, in transform space
        """
        ...
    @translate.setter
    def translate(self, value): ...
    __dict__ : dictproxy
    
    
    
    __readonly__ : dict
    
    __weakref__ : getset_descriptor
    
    apicls : Type[_MMatrix]
    
    cnames : tuple
    
    identity : Matrix
    
    ndim : int
    
    shape : tuple
    
    size : int


class MetaMayaArrayTypeWrapper(_f.MetaMayaTypeRegistry):
    """
    A metaclass to wrap Maya array type classes such as Vector, Matrix
    """
    
    
    
    @staticmethod
    def __new__(mcl, classname, bases, classdict):
        """
        Create a new wrapping class for a Maya api type, such as Vector or Matrix
        """
        ...


class Unit(float):
    def __repr__(self) -> str: ...
    def asInternalUnit(self): ...
    def asUIUnit(self): ...
    def asUnit(self, unit): ...
    def assign(self, *args): ...
    def getUnit(self):
        """
        Returns the units currently in effect for this instance
        """
        ...
    @classmethod
    def getInternalUnit(cls):
        """
        Returns the internal units currently in use for that type
        """
        ...
    @classmethod
    def getUIUnit(cls):
        """
        Returns the global UI units currently in use for that type
        """
        ...
    @classmethod
    def kUnit(cls, unit='None'):
        """
        Converts a string unit name to the internal int unit enum representation
        """
        ...
    @classmethod
    def sUnit(cls, unit='None'):
        """
        Converts an internal int unit enum representation to the string unit name
        """
        ...
    @classmethod
    def setUIUnit(cls, unit='None'):
        """
        Sets the global UI units currently to use for that type
        """
        ...
    @classmethod
    def uiToInternal(cls, value): ...
    @staticmethod
    def __new__(cls, value, unit='None'): ...
    @property
    def unit(self):
        """
        The units currently in effect for this instance
        """
        ...
    data : member_descriptor


class BoundingBox(_MBoundingBox):
    def __getitem__(self, item): ...
    def __init__(self, *args): ...
    def __melobject__(self):
        """
        A flat list of 6 values [minx, miny, minz, maxx, maxy, maxz]
        """
        ...
    def __repr__(self) -> str: ...
    def __str__(self) -> str: ...
    def center(self):
        """
        Returns the center of the bounding box.
        
        Returns:
        'Point'
        
        Derived from api method `maya.OpenMaya.MBoundingBox.center`
        """
        ...
    def clear(self):
        """
        Empties the current bounding box.
        Derived from api method `maya.OpenMaya.MBoundingBox.clear`
        """
        ...
    def contains(self, point):
        """
        Returns true if the bounding box contains the given point.
        
        Parameters:
        point : 'Point'
            point to check for inclusion in this bounding box
        
        
        Returns:
        'bool'
        
        Derived from api method `maya.OpenMaya.MBoundingBox.contains`
        """
        ...
    def depth(self):
        """
        Returns the depth of the bounding box.
        
        Returns:
        'float'
        
        Derived from api method `maya.OpenMaya.MBoundingBox.depth`
        """
        ...
    def expand(self, point):
        """
        Expand the bounding box to include the given point.
        
        Parameters:
        point : 'Point'
            new point to include in the bounding box.
        
        Derived from api method `maya.OpenMaya.MBoundingBox.expand`
        """
        ...
    def height(self):
        """
        Returns the height of the bounding box.
        
        Returns:
        'float'
        
        Derived from api method `maya.OpenMaya.MBoundingBox.height`
        """
        ...
    def intersects(self, box, tol='0.0'):
        """
        Returns true if the bounding box intersects another given bounding box
        
        Parameters:
        box : 'BoundingBox'
            bounding box to check for intersection
        tol : 'float'
            tolerance of the intersection check
        
        
        Returns:
        'bool'
        
        Derived from api method `maya.OpenMaya.MBoundingBox.intersects`
        """
        ...
    def max(self):
        """
        Returns the maximum point for the bounding box. That is the point whose x, y, and z components represent the bounding box's maximum value in each dimension.
        
        Returns:
        'Point'
        
        Derived from api method `maya.OpenMaya.MBoundingBox.max`
        """
        ...
    def min(self):
        """
        Returns the minimum point for the bounding box. That is the point whose x, y, and z components represent the bounding box's minimum value in each dimension.
        
        Returns:
        'Point'
        
        Derived from api method `maya.OpenMaya.MBoundingBox.min`
        """
        ...
    def repr(self): ...
    def transformUsing(self, matrix):
        """
        Apply the given transformation to this bounding box.
        
        Parameters:
        matrix : 'Matrix'
            transformation matrix
        
        Derived from api method `maya.OpenMaya.MBoundingBox.transformUsing`
        """
        ...
    def width(self):
        """
        Returns the width of the bounding box.
        
        Returns:
        'float'
        
        Derived from api method `maya.OpenMaya.MBoundingBox.width`
        """
        ...
    @property
    def d(self):
        """
        Returns the depth of the bounding box.
        
        Returns:
        'float'
        
        Derived from api method `maya.OpenMaya.MBoundingBox.depth`
        """
        ...
    @property
    def h(self):
        """
        Returns the height of the bounding box.
        
        Returns:
        'float'
        
        Derived from api method `maya.OpenMaya.MBoundingBox.height`
        """
        ...
    @property
    def w(self):
        """
        Returns the width of the bounding box.
        
        Returns:
        'float'
        
        Derived from api method `maya.OpenMaya.MBoundingBox.width`
        """
        ...
    
    
    __readonly__ : dict
    
    apicls : Type[_MBoundingBox]


class Quaternion(Matrix, _MQuaternion):
    def __contains__(self, value):
        """
        True if at least one of the vector components is equal to the argument
        """
        ...
    def __getitem__(self, i): ...
    def __init__(self, *args, **kwargs):
        """
        __init__ method for Quaternion
        """
        ...
    def __iter__(self): ...
    def __len__(self) -> int: ...
    def __setattr__(self, name, value):
        """
        Fixes __setattr__ to work properly with properties
        
        Maya has a bug on windows where some api objects have a __setattr__
        that bypasses properties (and other data descriptors).
        """
        ...
    def __setitem__(self, i, a):
        """
        Set component i value on self
        """
        ...
    def asEulerRotation(self):
        """
        Converts a quaternion to an euler rotation.
        
        Returns:
        'EulerRotation'
        
        Derived from api method `maya.OpenMaya.MQuaternion.asEulerRotation`
        """
        ...
    def assign(self, value):
        """
        Wrap the Quaternion api assign method
        """
        ...
    def conjugateIt(self):
        """
        Performs an in place conjugation of this quaternion. The result is a quaternion whose x, y, and z values have been negated.
        
        Returns:
        'Quaternion'
        
        Derived from api method `maya.OpenMaya.MQuaternion.conjugateIt`
        """
        ...
    def exp(self):
        """
        Exponentiates a quaternion that has a scalar part of zero. The precondition for using this method is that w is zero.
        
        Returns:
        'Quaternion'
        
        Derived from api method `maya.OpenMaya.MQuaternion.exp`
        """
        ...
    def get(self):
        """
        Wrap the Quaternion api get method
        """
        ...
    def invertIt(self):
        """
        Performs an in place inversion of this quaternion.
        
        Returns:
        'Quaternion'
        
        Derived from api method `maya.OpenMaya.MQuaternion.invertIt`
        """
        ...
    def log(self):
        """
        Returns the natural log of a quaternion. The precondition for using this method is that the quaternion must be normalized.
        
        Returns:
        'Quaternion'
        
        Derived from api method `maya.OpenMaya.MQuaternion.log`
        """
        ...
    def negateIt(self):
        """
        Performs an in place negation of the quaternion. The result is a quaternion whose x, y, z, and w values have been negated.
        
        Returns:
        'Quaternion'
        
        Derived from api method `maya.OpenMaya.MQuaternion.negateIt`
        """
        ...
    def normalizeIt(self):
        """
        Performs an in place normalization of this quaternion. The result is a quaternion of unit length.
        
        Returns:
        'Quaternion'
        
        Derived from api method `maya.OpenMaya.MQuaternion.normalizeIt`
        """
        ...
    def scaleIt(self, scale):
        """
        Performs an in place scaling of the quaternion. The result is a quaternion whose x, y, z, and w values have been scaled by the specified amount.
        
        Parameters:
        scale : 'float'
            the amount by which the quaternion should be scaled
        
        
        Returns:
        'Quaternion'
        
        Derived from api method `maya.OpenMaya.MQuaternion.scaleIt`
        """
        ...
    def setToXAxis(self, theta):
        """
        Sets this quaternion to be the rotation about the X axis of theta (in angular units). If the length of the axis is too small the quaternion returned will be the identity quaternion.
        
        Parameters:
        theta : 'float'
            the angle of rotation about the X axis in radians
        
        
        Returns:
        'Quaternion'
        
        Derived from api method `maya.OpenMaya.MQuaternion.setToXAxis`
        """
        ...
    def setToYAxis(self, theta):
        """
        Sets this quaternion to be the rotation about the Y axis of theta (in angular units). If the length of the axis is too small the quaternion returned will be the identity quaternion.
        
        Parameters:
        theta : 'float'
            the angle of rotation about the Y axis in radians
        
        
        Returns:
        'Quaternion'
        
        Derived from api method `maya.OpenMaya.MQuaternion.setToYAxis`
        """
        ...
    def setToZAxis(self, theta):
        """
        Sets this quaternion to be the rotation about the Z axis of theta (in angular units). If the length of the axis is too small the quaternion returned will be the identity quaternion.
        
        Parameters:
        theta : 'float'
            the angle of rotation about the Z axis in radians
        
        
        Returns:
        'Quaternion'
        
        Derived from api method `maya.OpenMaya.MQuaternion.setToZAxis`
        """
        ...
    @staticmethod
    def __new__(cls, *args, **kwargs): ...
    @property
    def rotate(self):
        """
        The rotation expressed in this Quaternion, in transform space
        """
        ...
    @rotate.setter
    def rotate(self, value): ...
    @property
    def scale(self):
        """
        The scale expressed in this Quaternion, which is always (1.0, 1.0, 1.0)
        """
        ...
    @property
    def translate(self):
        """
        The translation expressed in this MMQuaternion, which is always (0.0, 0.0, 0.0)
        """
        ...
    @property
    def w(self): ...
    @w.setter
    def w(self, value): ...
    @property
    def x(self): ...
    @x.setter
    def x(self, value): ...
    @property
    def y(self): ...
    @y.setter
    def y(self, value): ...
    @property
    def z(self): ...
    @z.setter
    def z(self, value): ...
    __readonly__ : dict
    
    apicls : Type[_MQuaternion]
    
    cnames : tuple
    
    identity : Quaternion
    
    ndim : int
    
    shape : tuple
    
    size : int


class Point(Vector, _MPoint):
    """
    A 4 dimensional vector class that wraps Maya's api Point class,
    """
    
    
    
    def __add__(self, other):
        """
        u.__add__(v) <==> u+v
        Returns the result of the addition of u and v if v is convertible to a VectorN (element-wise addition),
        adds v to every component of u if v is a scalar
        """
        ...
    def __iadd__(self, other):
        """
        u.__iadd__(v) <==> u += v
        In place addition of u and v, see __add__
        """
        ...
    def __iter__(self, *args, **kwargs):
        """
        Iterate on the api components
        """
        ...
    def __len__(self) -> int:
        """
        # we only show the x, y, z components on an iter
        """
        ...
    def __melobject__(self):
        """
        Special method for returning a mel-friendly representation. In this case, a cartesian 3D point
        """
        ...
    def __radd__(self, other):
        """
        u.__radd__(v) <==> v+u
        Returns the result of the addition of u and v if v is convertible to a VectorN (element-wise addition),
        adds v to every component of u if v is a scalar
        """
        ...
    def __setattr__(self, name, value):
        """
        Fixes __setattr__ to work properly with properties
        
        Maya has a bug on windows where some api objects have a __setattr__
        that bypasses properties (and other data descriptors).
        """
        ...
    def angle(self, start, end):
        """
        a.angle(b, c) --> float
        Returns the angle (in radians) of rotation from point b to c around a.
        Note that this angle is not signed, use axis to know the direction of the rotation
        """
        ...
    def axis(self, start, end, normalize='False'):
        """
        a.axis(b, c) --> Vector
        Returns the axis of rotation from point b to c around a as the vector n = (b-a)^(c-a)
        if the normalize keyword argument is set to True, n is also normalized
        """
        ...
    def bWeights(self, *args):
        """
        p.bWeights(p0, p1, (...), pn) --> tuple
        Returns a tuple of (n0, n1, ...) normalized barycentric weights so that n0*p0 + n1*p1 + ... = p.
        This method works for n points defining a concave or convex n sided face,
        always returns positive normalized weights, and is continuous on the face limits (on the edges),
        but the n points must be coplanar, and p must be inside the face delimited by (p0, ..., pn)
        """
        ...
    def cartesian(self):
        """
        p.cartesian() --> Point
        Returns the cartesianized version of p, without changing p.
        """
        ...
    def cartesianize(self):
        """
        p.cartesianize() --> Point
        If the point instance p is of the form P(W*x, W*y, W*z, W), for some scale factor W != 0,
        then it is reset to be P(x, y, z, 1).
        This will only work correctly if the point is in homogenous form or cartesian form.
        If the point is in rational form, the results are not defined.
        """
        ...
    def center(self, *args):
        """
        p.center(q, r, s (...)) --> Point
        Returns the Point that is the center of p, q, r, s (...)
        """
        ...
    def cotan(self, start, end):
        """
        a.cotan(b, c) --> float :
        cotangent of the (b-a), (c-a) angle, a, b, and c should be MPoints representing points a, b, c
        """
        ...
    def homogen(self):
        """
        p.homogen() --> Point
        Returns the homogenized version of p, without changing p.
        """
        ...
    def homogenize(self):
        """
        p.homogenize() --> Point
        If the point instance p is of the form P(x, y, z, W) (ie. is in rational or (for W==1) cartesian form),
        for some scale factor W != 0, then it is reset to be P(W*x, W*y, W*z, W).
        """
        ...
    def isEquivalent(self, other, tol='None'):
        """
        Returns true if both arguments considered as Point are equal within the specified tolerance
        """
        ...
    def planar(self, *args, **kwargs):
        """
        p.planar(q, r, s (...), tol=tolerance) --> bool
        Returns True if all provided points are planar within given tolerance
        """
        ...
    def rational(self):
        """
        p.rational() --> Point
        Returns the rationalized version of p, without changing p.
        """
        ...
    def rationalize(self):
        """
        p.rationalize() --> Point
        If the point instance p is of the form P(W*x, W*y, W*z, W) (ie. is in homogenous or (for W==1) cartesian form),
        for some scale factor W != 0, then it is reset to be P(x, y, z, W).
        This will only work correctly if the point is in homogenous or cartesian form.
        If the point is already in rational form, the results are not defined.
        """
        ...
    @property
    def w(self): ...
    @w.setter
    def w(self, value): ...
    @property
    def x(self): ...
    @x.setter
    def x(self, value): ...
    @property
    def y(self): ...
    @y.setter
    def y(self, value): ...
    @property
    def z(self): ...
    @z.setter
    def z(self, value): ...
    __readonly__ : dict
    
    apicls : Type[_MPoint]
    
    cnames : tuple
    
    ndim : int
    
    origin : Point
    
    shape : tuple
    
    size : int


class Angle(Unit):
    def asAngMinutes(self): ...
    def asAngSeconds(self): ...
    def asDegrees(self): ...
    def asRadians(self): ...
    Unit : Enum
    
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor
    
    apicls : Type[_api.MAngle]


class Time(Unit):
    Unit : Enum
    
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor
    
    apicls : Type[_api.MTime]


class FloatVector(Vector, _MFloatVector):
    """
    A 3 dimensional vector class that wraps Maya's api FloatVector class,
    It behaves identically to Vector, but it also derives from api's FloatVector
    to keep api methods happy
    """
    
    
    
    def __setattr__(self, name, value):
        """
        Fixes __setattr__ to work properly with properties
        
        Maya has a bug on windows where some api objects have a __setattr__
        that bypasses properties (and other data descriptors).
        """
        ...
    @property
    def x(self): ...
    @x.setter
    def x(self, value): ...
    @property
    def y(self): ...
    @y.setter
    def y(self, value): ...
    @property
    def z(self): ...
    @z.setter
    def z(self, value): ...
    __readonly__ : dict
    
    apicls : Type[_MFloatVector]
    
    cnames : tuple
    
    ndim : int
    
    one : FloatVector
    
    shape : tuple
    
    size : int
    
    xAxis : FloatVector
    
    xNegAxis : FloatVector
    
    yAxis : FloatVector
    
    yNegAxis : FloatVector
    
    zAxis : FloatVector
    
    zNegAxis : FloatVector
    
    zero : FloatVector


class Distance(Unit):
    """
    >>> from pymel.core import *
    >>> import pymel.core.datatypes as dt
    >>>
    >>> dt.Distance.getInternalUnit()
    'centimeters'
    >>> dt.Distance.setUIUnit('meters')
    >>> dt.Distance.getUIUnit()
    'meters'
    
    >>> d = dt.Distance(12)
    >>> d.unit
    'meters'
    >>> print d
    12.0
    >>> print repr(d)
    dt.Distance(12.0, unit='meters')
    >>> print d.asUIUnit()
    12.0
    >>> print d.asInternalUnit()
    1200.0
    
    >>> dt.Distance.setUIUnit('centimeters')
    >>> dt.Distance.getUIUnit()
    'centimeters'
    >>> e = dt.Distance(12)
    >>> e.unit
    'centimeters'
    >>> print e
    12.0
    >>> str(e)
    '12.0'
    >>> print repr(e)
    dt.Distance(12.0, unit='centimeters')
    >>> print e.asUIUnit()
    12.0
    >>> print e.asInternalUnit()
    12.0
    
    >>> f = dt.Distance(12, 'feet')
    >>> print f
    12.0
    >>> print repr(f)
    dt.Distance(12.0, unit='feet')
    >>> f.unit
    'feet'
    >>> print f.asUIUnit()
    365.76
    >>> dt.Distance.setUIUnit('meters')
    >>> dt.Distance.getUIUnit()
    'meters'
    >>> print f.asUIUnit()
    3.6576
    >>> dt.Distance.getInternalUnit()
    'centimeters'
    >>> print f.asInternalUnit()
    365.76
    
    >>> print f.asFeet()
    12.0
    >>> print f.asMeters()
    3.6576
    >>> print f.asCentimeters()
    365.76
    
    >>> dt.Distance.setUIUnit()
    >>> dt.Distance.getUIUnit()
    'centimeters'
    """
    
    
    
    def asCentimeters(self): ...
    def asFeet(self): ...
    def asInches(self): ...
    def asKilometers(self): ...
    def asMeters(self): ...
    def asMiles(self): ...
    def asMillimeter(self): ...
    def asYards(self): ...
    Unit : Enum
    
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor
    
    apicls : Type[_api.MDistance]


class TransformationMatrix(Matrix, _MTransformationMatrix):
    def __getattribute__(self, name): ...
    def addRotation(self, rot, order, space):
        """
        Add to the rotation component by rotating relative to the existing transformation. The only valid transformation spaces for this method are MSpace::kTransform and MSpace::kPreTransform. All other spaces are treated as being equivalent to MSpace::kTransform.
        
        Parameters:
        rot : ('float', 'float', 'float')
            relative value to rotate by
        order : 'TransformationMatrix.RotationOrder'
            order in which to apply the rotation components
        
            values: 'XYZ', 'YZX', 'ZXY', 'XZY', 'YXZ', 'ZYX'
        space : 'Space.Space'
            transform space in which to perform the rotation
        
            values: 'transform', 'preTransform', 'object', 'world'
        
        Derived from api method `maya.OpenMaya.MSpace.addRotation`
        """
        ...
    def addRotationQuaternion(self, x, y, z, w, space):
        """
        Add to the rotation component by rotating relative to the existing transformation. Rotation is a quaternion. The only valid transformation spaces for this method are MSpace::kTransform and MSpace::kPreTransform. All other spaces are treated as being equivalent to MSpace::kTransform.
        
        Parameters:
        x : 'float'
            x component of quaternion
        y : 'float'
            y component of quaternion
        z : 'float'
            z component of quaternion
        w : 'float'
            w component of quaternion
        space : 'Space.Space'
            transformation space in which to perform the operation
        
            values: 'transform', 'preTransform', 'object', 'world'
        
        Derived from api method `maya.OpenMaya.MSpace.addRotationQuaternion`
        """
        ...
    def addScale(self, scale, space):
        """
        Add to the scale component by scaling relative to the existing transformation.
        
        Parameters:
        scale : ('float', 'float', 'float')
            relative value to scale by
        space : 'Space.Space'
            transform space in which to perform the scale
        
            values: 'transform', 'preTransform', 'object', 'world'
        
        Derived from api method `maya.OpenMaya.MSpace.addScale`
        """
        ...
    def addShear(self, shear, space):
        """
        Add to the shear component by shearing relative to the existing transformation.
        
        Parameters:
        shear : ('float', 'float', 'float')
            relative value to shear by
        space : 'Space.Space'
            transform space in which to perform the shear
        
            values: 'transform', 'preTransform', 'object', 'world'
        
        Derived from api method `maya.OpenMaya.MSpace.addShear`
        """
        ...
    def addTranslation(self, vector, space):
        """
        Add to the translation component by translating relative to the existing transformation.
        
        Parameters:
        vector : 'Vector'
            relative value to translate by
        space : 'Space.Space'
            transform space in which to perform the scale
        
            values: 'transform', 'preTransform', 'object', 'world'
        
        Derived from api method `maya.OpenMaya.MSpace.addTranslation`
        """
        ...
    def asMatrixInverse(self):
        """
        Returns the inverse of the four by four matrix that describes this transformation.
        
        Returns:
        'Matrix'
        
        Derived from api method `maya.OpenMaya.MTransformationMatrix.asMatrixInverse`
        """
        ...
    def asRotateMatrix(self):
        """
        Returns rotate space matrix. The rotate space matrix takes points from object space to the space immediately following the scale/shear/rotation transformations.
        
        Returns:
        'Matrix'
        
        Derived from api method `maya.OpenMaya.MTransformationMatrix.asRotateMatrix`
        """
        ...
    def asScaleMatrix(self):
        """
        Returns scale space matrix. The scale space matrix takes points from object space to the space immediately following scale and shear transformations.
        
        Returns:
        'Matrix'
        
        Derived from api method `maya.OpenMaya.MTransformationMatrix.asScaleMatrix`
        """
        ...
    def eulerRotation(self): ...
    def getRotatePivot(self, space):
        """
        Returns the pivot around which the rotation is applied.
        
        Parameters:
        space : 'Space.Space'
            space in which to get the pivot
        
            values: 'transform', 'preTransform', 'object', 'world'
        
        
        Returns:
        'Point'
        
        Derived from api method `maya.OpenMaya.MSpace.rotatePivot`
        """
        ...
    def getRotatePivotTranslation(self, space):
        """
        Returns the rotation pivot translation. This is the translation that is used to compensate for the movement of the rotation pivot.
        
        Parameters:
        space : 'Space.Space'
            space in which to get the pivot translation
        
            values: 'transform', 'preTransform', 'object', 'world'
        
        
        Returns:
        'Vector'
        
        Derived from api method `maya.OpenMaya.MSpace.rotatePivotTranslation`
        """
        ...
    def getRotation(self):
        """
        # The apicls getRotation needs a "RotationOrder &" object, which is
        # impossible to make in python...
        # So instead, wrap eulerRotation
        """
        ...
    def getRotationOrientation(self):
        """
        Returns the rotation orientation for the transformation matrix. The rotation orientation is the rotation that orients the local rotation space. The rotation is returned in MSpace::kTransform space.
        
        Returns:
        'Quaternion'
        
        Derived from api method `maya.OpenMaya.MTransformationMatrix.rotationOrientation`
        """
        ...
    def getRotationQuaternion(self):
        """
        Get the rotation component of the transformation matrix as a quaternion. The rotation is retrieved in MSpace::kTransform space.
        
        Returns:
        Tuple['float', 'float', 'float', 'float']
        
        Derived from api method `maya.OpenMaya.MTransformationMatrix.getRotationQuaternion`
        """
        ...
    def getScale(self, space):
        """
        Get the scale component of the transformation matrix.
        
        Parameters:
        space : 'Space.Space'
            transform space in which to get the scale
        
            values: 'transform', 'preTransform', 'object', 'world'
        
        
        Returns:
        ('float', 'float', 'float')
        
        Derived from api method `maya.OpenMaya.MSpace.getScale`
        """
        ...
    def getScalePivot(self, space):
        """
        Returns the pivot around which the scale is applied.
        
        Parameters:
        space : 'Space.Space'
            space in which to get the pivot
        
            values: 'transform', 'preTransform', 'object', 'world'
        
        
        Returns:
        'Point'
        
        Derived from api method `maya.OpenMaya.MSpace.scalePivot`
        """
        ...
    def getScalePivotTranslation(self, space):
        """
        Returns the scale pivot translation. This is the translation that is used to compensate for the movement of the scale pivot.
        
        Parameters:
        space : 'Space.Space'
            space in which to get the pivot
        
            values: 'transform', 'preTransform', 'object', 'world'
        
        
        Returns:
        'Vector'
        
        Derived from api method `maya.OpenMaya.MSpace.scalePivotTranslation`
        """
        ...
    def getShear(self, space):
        """
        Get the shear component of the transformation matrix.
        
        Parameters:
        space : 'Space.Space'
            Transform space in which to get the shear.
        
            values: 'transform', 'preTransform', 'object', 'world'
        
        
        Returns:
        ('float', 'float', 'float')
        
        Derived from api method `maya.OpenMaya.MSpace.getShear`
        """
        ...
    def getTranslation(self, space):
        """
        Returns the translation component of the translation as a vector in linear units.
        
        Parameters:
        space : 'Space.Space'
            space in which to perform the translation
        
            values: 'transform', 'preTransform', 'object', 'world'
        
        
        Returns:
        'Vector'
        
        Derived from api method `maya.OpenMaya.MSpace.getTranslation`
        """
        ...
    def reorderRotation(self, order):
        """
        Reorders the x, y, and z components of the rotation of this transform. The overall rotation will remain the same. This operation is not unique, so spin information will be lost.
        
        Parameters:
        order : 'TransformationMatrix.RotationOrder'
            new order of the rotations
        
            values: 'XYZ', 'YZX', 'ZXY', 'XZY', 'YXZ', 'ZYX'
        
        Derived from api method `maya.OpenMaya.MTransformationMatrix.reorderRotation`
        """
        ...
    def rotateBy(self, q, space):
        """
        Adds to the rotation component of the rotation matrix by rotating relative to the existing transformation using a quaternion. The only valid transformation spaces for this method are MSpace::kTransform and MSpace::kPreTransform/MSpace::kObject. All other spaces are treated as being equivalent to MSpace::kTransform.
        
        Parameters:
        q : 'Quaternion'
            the quaternion that indicates how much the transformation matrix will be rotated by
        space : 'Space.Space'
            the space in which the rotation is performed
        
            values: 'transform', 'preTransform', 'object', 'world'
        
        
        Returns:
        'TransformationMatrix'
        
        Derived from api method `maya.OpenMaya.MSpace.rotateBy`
        """
        ...
    def rotateTo(self, value):
        """
        Set to the given rotation (and result self)
        
        Value may be either a Quaternion, EulerRotation object, or a list of
        floats; if it is floats, if it has length 4 it is interpreted as
        a Quaternion; if 3, as a EulerRotation.
        """
        ...
    def rotationOrder(self):
        """
        Returns the rotation order for the transform matrix. That is the order in which the Euler angles are applied to create the end rotation.
        
        Returns:
        'TransformationMatrix.RotationOrder'
        
        Derived from api method `maya.OpenMaya.MTransformationMatrix.rotationOrder`
        """
        ...
    def setRotatePivot(self, point, space, balance):
        """
        Set the pivot around which the rotation is applied.
        
        Parameters:
        point : 'Point'
            new rotation pivot
        space : 'Space.Space'
            transform space in which to set the pivot
        
            values: 'transform', 'preTransform', 'object', 'world'
        balance : 'bool'
            whether to balance the matrix
        
        Derived from api method `maya.OpenMaya.MSpace.setRotatePivot`
        """
        ...
    def setRotatePivotTranslation(self, vector, space):
        """
        Set the pivot translation. This component is used to preserve existing transformations when moving pivot. This method will only be useful to advanced users.
        
        Parameters:
        vector : 'Vector'
            new rotation pivot translation
        space : 'Space.Space'
            transform space in which to set the rotation translation
        
            values: 'transform', 'preTransform', 'object', 'world'
        
        Derived from api method `maya.OpenMaya.MSpace.setRotatePivotTranslation`
        """
        ...
    def setRotation(self, *args): ...
    def setRotationOrientation(self, q):
        """
        Sets the rotation orientation for the transformation matrix. The rotation orientation is the rotation that orients the local rotation space. The rotation is set in MSpace::kTransform space.
        
        Parameters:
        q : 'Quaternion'
            Rotation quaternion.
        
        
        Returns:
        'TransformationMatrix'
        
        Derived from api method `maya.OpenMaya.MTransformationMatrix.setRotationOrientation`
        """
        ...
    def setRotationQuaternion(self, x, y, z, w):
        """
        Set the rotation component of the transformation matrix using a quaternion. The rotation is set in MSpace::kTransform space.
        
        Parameters:
        x : 'float'
            x component of new quaternion
        y : 'float'
            y component of new quaternion
        z : 'float'
            z component of new quaternion
        w : 'float'
            w component of new quaternion
        
        Derived from api method `maya.OpenMaya.MTransformationMatrix.setRotationQuaternion`
        """
        ...
    def setScale(self, scale, space):
        """
        Set the scale component of the transformation matrix.
        
        Parameters:
        scale : ('float', 'float', 'float')
            new scale component
        space : 'Space.Space'
            transform space in which to perform the scale
        
            values: 'transform', 'preTransform', 'object', 'world'
        
        Derived from api method `maya.OpenMaya.MSpace.setScale`
        """
        ...
    def setScalePivot(self, point, space, balance):
        """
        Set the pivot around which the scale is applied.
        
        Parameters:
        point : 'Point'
            new scale pivot
        space : 'Space.Space'
            transform space in which to set the scale pivot
        
            values: 'transform', 'preTransform', 'object', 'world'
        balance : 'bool'
            whether to balance the matrix
        
        Derived from api method `maya.OpenMaya.MSpace.setScalePivot`
        """
        ...
    def setScalePivotTranslation(self, vector, space):
        """
        Set the pivot translation. This component is used to preserve existing scale transformations when moving pivot. This method will only be useful to advanced users.
        
        Parameters:
        vector : 'Vector'
            new scale pivot translation
        space : 'Space.Space'
            transform space in which to set the scale translation
        
            values: 'transform', 'preTransform', 'object', 'world'
        
        Derived from api method `maya.OpenMaya.MSpace.setScalePivotTranslation`
        """
        ...
    def setShear(self, shear, space):
        """
        Set the shear component of the transformation matrix. The shear values represent (xy, xz, yx).
        
        Parameters:
        shear : ('float', 'float', 'float')
            new shear component
        space : 'Space.Space'
            transform space in which to perform the shear
        
            values: 'transform', 'preTransform', 'object', 'world'
        
        Derived from api method `maya.OpenMaya.MSpace.setShear`
        """
        ...
    def setToRotationAxis(self, axis, rotation):
        """
        Sets the rotation given an axis and a rotation about it.
        
        Parameters:
        axis : 'Vector'
            axis to rotate about
        rotation : 'float'
            rotation in radians
        
        Derived from api method `maya.OpenMaya.MTransformationMatrix.setToRotationAxis`
        """
        ...
    def setTranslation(self, vector, space):
        """
        Set the translation component of the transformation matrix in linear units.
        
        Parameters:
        vector : 'Vector'
            new translation component in centimeters.
        space : 'Space.Space'
            transform space in which to perform the translation
        
            values: 'transform', 'preTransform', 'object', 'world'
        
        Derived from api method `maya.OpenMaya.MSpace.setTranslation`
        """
        ...
    @property
    def a00(self): ...
    @a00.setter
    def a00(self, value): ...
    @property
    def a01(self): ...
    @a01.setter
    def a01(self, value): ...
    @property
    def a02(self): ...
    @a02.setter
    def a02(self, value): ...
    @property
    def a03(self): ...
    @a03.setter
    def a03(self, value): ...
    @property
    def a10(self): ...
    @a10.setter
    def a10(self, value): ...
    @property
    def a11(self): ...
    @a11.setter
    def a11(self, value): ...
    @property
    def a12(self): ...
    @a12.setter
    def a12(self, value): ...
    @property
    def a13(self): ...
    @a13.setter
    def a13(self, value): ...
    @property
    def a20(self): ...
    @a20.setter
    def a20(self, value): ...
    @property
    def a21(self): ...
    @a21.setter
    def a21(self, value): ...
    @property
    def a22(self): ...
    @a22.setter
    def a22(self, value): ...
    @property
    def a23(self): ...
    @a23.setter
    def a23(self, value): ...
    @property
    def a30(self): ...
    @a30.setter
    def a30(self, value): ...
    @property
    def a31(self): ...
    @a31.setter
    def a31(self, value): ...
    @property
    def a32(self): ...
    @a32.setter
    def a32(self, value): ...
    @property
    def a33(self): ...
    @a33.setter
    def a33(self, value): ...
    @property
    def euler(self):
        """
        The euler rotation expressed in this TransformationMatrix, in transform space
        """
        ...
    @euler.setter
    def euler(self, value): ...
    @property
    def rotate(self):
        """
        The quaternion rotation expressed in this TransformationMatrix, in transform space
        """
        ...
    @rotate.setter
    def rotate(self, value): ...
    @property
    def scale(self):
        """
        The scale expressed in this TransformationMatrix, in transform space
        """
        ...
    @scale.setter
    def scale(self, value): ...
    @property
    def translate(self):
        """
        The translation expressed in this TransformationMatrix, in transform space
        """
        ...
    @translate.setter
    def translate(self, value): ...
    RotationOrder : Enum
    
    __readonly__ : dict
    
    apicls : Type[_MTransformationMatrix]
    
    cnames : tuple
    
    identity : TransformationMatrix
    
    ndim : int
    
    shape : tuple
    
    size : int


class Color(Vector, _MColor):
    """
    A 4 dimensional vector class that wraps Maya's api Color class,
    It stores the r, g, b, a components of the color, as normalized (Python) floats
    """
    
    
    
    def __add__(self, other):
        """
        c.__add__(d) <==> c+d
        Returns the result of the addition of MColors c and d if d is convertible to a Color,
        adds d to every component of c if d is a scalar
        """
        ...
    def __iadd__(self, other):
        """
        c.__iadd__(d) <==> c += d
        In place addition of c and d, see __add__
        """
        ...
    def __imul__(self, other):
        """
        a.__imul__(b) <==> a *= b
        In place multiplication of VectorN a and b, see __mul__, result must fit a's type
        """
        ...
    def __init__(self, *args, **kwargs):
        """
        Init a Color instance
        Can pass one argument being another Color instance , or the color components
        """
        ...
    def __isub__(self, other):
        """
        c.__isub__(d) <==> c -= d
        In place substraction of d from c, see __sub__
        """
        ...
    def __melobject__(self):
        """
        Special method for returning a mel-friendly representation. In this case, a 3-component color (RGB)
        """
        ...
    def __mul__(self, other):
        """
        a.__mul__(b) <==> a*b
        If b is a 1D sequence (Array, VectorN, Color), __mul__ is mapped to element-wise multiplication,
        If b is a MatrixN, __mul__ is similar to Point a by MatrixN b multiplication (post multiplication or transformation of a by b),
        multiplies every component of a by b if b is a single numeric value
        """
        ...
    def __radd__(self, other):
        """
        c.__radd__(d) <==> d+c
        Returns the result of the addition of MColors c and d if d is convertible to a Color,
        adds d to every component of c if d is a scalar
        """
        ...
    def __rmul__(self, other):
        """
        a.__rmul__(b) <==> b*a
        If b is a 1D sequence (Array, VectorN, Color), __mul__ is mapped to element-wise multiplication,
        If b is a MatrixN, __mul__ is similar to MatrixN b by Point a matrix multiplication,
        multiplies every component of a by b if b is a single numeric value
        """
        ...
    def __rsub__(self, other):
        """
        c.__rsub__(d) <==> d-c
        Returns the result of the substraction of Color c from d if d is convertible to a Color,
        replace every component c[i] of c by d-c[i] if d is a scalar
        """
        ...
    def __setattr__(self, name, value):
        """
        Fixes __setattr__ to work properly with properties
        
        Maya has a bug on windows where some api objects have a __setattr__
        that bypasses properties (and other data descriptors).
        """
        ...
    def __sub__(self, other):
        """
        c.__add__(d) <==> c+d
        Returns the result of the substraction of Color d from c if d is convertible to a Color,
        substract d from every component of c if d is a scalar
        """
        ...
    def gamma(self, g):
        """
        c.gamma(g) applies gamma correction g to Color c, g can be a scalar and then will be applied to r, g, b
        or an iterable of up to 3 (r, g, b) independant gamma correction values
        """
        ...
    def hsvblend(self, other, weight='0.5'):
        """
        c1.hsvblend(c2) --> Color
        Returns the result of blending c1 with c2 in hsv space, using the given weight
        """
        ...
    def over(self, other):
        """
        c1.over(c2): Composites c1 over other c2 using c1's alpha, the resulting color has the alpha of c2
        """
        ...
    def premult(self):
        """
        Premultiply Color r, g and b by it's alpha and resets alpha to 1.0
        """
        ...
    def set(self, colorModel, c1, c2, c3, alpha='1.0'):
        """
        Color component assigment.
        
        Parameters:
        colorModel : 'Color.MColorType'
            The color model.
        
            values: 'RGB', 'HSV', 'CMY', 'CMYK'
        c1 : 'float'
            First component of color.
        c2 : 'float'
            Second component of color.
        c3 : 'float'
            Third component of color.
        alpha : 'float'
            Alpha component of color.
        
        
        Returns:
        'bool'
        
        Derived from api method `maya.OpenMaya.MColor.set`
        """
        ...
    @staticmethod
    def hsvtorgb(c): ...
    @staticmethod
    def rgbtohsv(c):
        """
        # static methods
        """
        ...
    @property
    def a(self): ...
    @a.setter
    def a(self, value): ...
    @property
    def b(self): ...
    @b.setter
    def b(self, value): ...
    @property
    def g(self): ...
    @g.setter
    def g(self, value): ...
    @property
    def h(self):
        """
        The h Color component
        """
        ...
    @h.setter
    def h(self, value): ...
    @property
    def hsv(self):
        """
        The h,s,v,a Color components
        """
        ...
    @hsv.setter
    def hsv(self, value): ...
    @property
    def hsva(self):
        """
        The h,s,v,a Color components
        """
        ...
    @hsva.setter
    def hsva(self, value): ...
    @property
    def r(self): ...
    @r.setter
    def r(self, value): ...
    @property
    def rgb(self):
        """
        The r,g,b Color components
        """
        ...
    @rgb.setter
    def rgb(self, value): ...
    @property
    def rgba(self):
        """
        The r,g,b,a Color components
        """
        ...
    @rgba.setter
    def rgba(self, value): ...
    @property
    def s(self):
        """
        The s Color component
        """
        ...
    @s.setter
    def s(self, value): ...
    @property
    def v(self):
        """
        The v Color component
        """
        ...
    @v.setter
    def v(self, value): ...
    MColorType : Enum
    
    __readonly__ : dict
    
    apicls : Type[_MColor]
    
    black : Color
    
    blue : Color
    
    clear : Color
    
    cnames : tuple
    
    green : Color
    
    kOpaqueBlack : Color
    
    modes : tuple
    
    ndim : int
    
    opaque : Color
    
    red : Color
    
    shape : tuple
    
    size : int
    
    white : Color


class FloatMatrix(Matrix, _MFloatMatrix):
    """
    A 4x4 matrix class that wraps Maya's api FloatMatrix class,
    It behaves identically to Matrix, but it also derives from api's FloatMatrix
    to keep api methods happy
    """
    
    
    
    def __setattr__(self, name, value):
        """
        Fixes __setattr__ to work properly with properties
        
        Maya has a bug on windows where some api objects have a __setattr__
        that bypasses properties (and other data descriptors).
        """
        ...
    @property
    def a00(self): ...
    @a00.setter
    def a00(self, value): ...
    @property
    def a01(self): ...
    @a01.setter
    def a01(self, value): ...
    @property
    def a02(self): ...
    @a02.setter
    def a02(self, value): ...
    @property
    def a03(self): ...
    @a03.setter
    def a03(self, value): ...
    @property
    def a10(self): ...
    @a10.setter
    def a10(self, value): ...
    @property
    def a11(self): ...
    @a11.setter
    def a11(self, value): ...
    @property
    def a12(self): ...
    @a12.setter
    def a12(self, value): ...
    @property
    def a13(self): ...
    @a13.setter
    def a13(self, value): ...
    @property
    def a20(self): ...
    @a20.setter
    def a20(self, value): ...
    @property
    def a21(self): ...
    @a21.setter
    def a21(self, value): ...
    @property
    def a22(self): ...
    @a22.setter
    def a22(self, value): ...
    @property
    def a23(self): ...
    @a23.setter
    def a23(self, value): ...
    @property
    def a30(self): ...
    @a30.setter
    def a30(self, value): ...
    @property
    def a31(self): ...
    @a31.setter
    def a31(self, value): ...
    @property
    def a32(self): ...
    @a32.setter
    def a32(self, value): ...
    @property
    def a33(self): ...
    @a33.setter
    def a33(self, value): ...
    __readonly__ : dict
    
    apicls : Type[_MFloatMatrix]
    
    cnames : tuple
    
    ndim : int
    
    shape : tuple
    
    size : int


class FloatPoint(Point, _MFloatPoint):
    """
    A 4 dimensional vector class that wraps Maya's api FloatPoint class,
    It behaves identically to Point, but it also derives from api's FloatPoint
    to keep api methods happy
    """
    
    
    
    def __setattr__(self, name, value):
        """
        Fixes __setattr__ to work properly with properties
        
        Maya has a bug on windows where some api objects have a __setattr__
        that bypasses properties (and other data descriptors).
        """
        ...
    def setCast(self, srcpt):
        """
        Copy the values of x, y, z, and w from srcpt to the instance.
        
        Parameters:
        srcpt : 'Point'
            the point to copy the x, y, z and w values from.
        
        Derived from api method `maya.OpenMaya.MFloatPoint.setCast`
        """
        ...
    @property
    def w(self): ...
    @w.setter
    def w(self, value): ...
    @property
    def x(self): ...
    @x.setter
    def x(self, value): ...
    @property
    def y(self): ...
    @y.setter
    def y(self, value): ...
    @property
    def z(self): ...
    @z.setter
    def z(self, value): ...
    __readonly__ : dict
    
    apicls : Type[_MFloatPoint]
    
    cnames : tuple
    
    ndim : int
    
    origin : FloatPoint
    
    shape : tuple
    
    size : int




def _testMTransformationMatrix(): ...
def equivalentSpace(space1, space2, rotationOnly='False'):
    """
    Compare the two given space values to see if they are equal
    
    Parameters
    ----------
    space1 : Union[int, str]
        the first space to compare (may be either the integer enum value, or the
        api enum name - ie, "kPostTransform" - or the pymel enum name - ie,
        "postTransform" )
    space2 : Union[int, str]
        the seoncd space to compare (may be either the integer enum value, or
        the api enum name - ie, "kPostTransform" - or the pymel enum name - ie,
        "postTransform")
    rotationOnly : bool
        If true, then compare the spaces, assuming we are only considering
        rotation - in rotation, transform is the same as preTransform/object
        (the reason being that in maya, preTransform means rotation +
        translation are both defined in the preTransform/object coordinate
        system, while transform means rotation is defined in preTransform/object
        coordinates, while translate is given in the postTransform space...
        which matches the way maya applies transforms)
    """
    ...
def _patchMMatrix(): ...
def getPlugValue(plug):
    """
    given an MPlug, get its value as a pymel-style object
    """
    ...
def _patchMEulerRotation(): ...
def bWeights(p, *args):
    """
    bWeights(p[, p0, p1, (...), pn]) --> tuple
    Returns a tuple of (n0, n1, ...) normalized barycentric weights so that n0*p0 + n1*p1 + ... = p
    """
    ...
def _patchMFloatPoint(): ...
def _testMVector(): ...
def _patchMFloatMatrix(): ...
def _patchMColor(): ...
def _patchMVector():
    """
    # patch some Maya api classes that miss __iter__ to make them
    # iterable / convertible to list
    """
    ...
def _patchMPoint(): ...
def _testMPoint(): ...
def _testMMatrix(): ...
def _patchMQuaternion(): ...
def _patchMFloatVector(): ...
def _testMColor(): ...
def planar(p, *args, **kwargs):
    """
    planar(p[, q, r, s (...), tol=tolerance]) --> bool
    Returns True if all provided MPoints are planar within given tolerance
    """
    ...
def _patchMTransformationMatrix(): ...
def center(p, *args):
    """
    center(p[, q, r, s (...)]) --> Point
    Returns the Point that is the center of p, q, r, s (...)
    """
    ...


Spaces : Enum

