/****************************************************************************
**
** Copyright (C) 2018 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qt for Python.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef TYPESPEC_H
#define TYPESPEC_H

#include "sbkpython.h"
#include "shibokenmacros.h"

#if PY_MAJOR_VERSION < 3
extern "C"
{

typedef struct{
    int slot; // slot is somehow reserved in Qt   /* slot id, see below */
    void *pfunc; /* function pointer */
} PyType_Slot;

typedef struct{
    const char *name;
    int basicsize;
    int itemsize;
    unsigned int flags;
    PyType_Slot *slots; /* terminated by slot==0. */
} PyType_Spec;

LIBSHIBOKEN_API PyObject *PyType_FromSpec(PyType_Spec *);
#if !defined(Py_LIMITED_API) || Py_LIMITED_API+0 >= 0x03030000
LIBSHIBOKEN_API PyObject *PyType_FromSpecWithBases(PyType_Spec *, PyObject *);
#endif
#if !defined(Py_LIMITED_API) || Py_LIMITED_API+0 >= 0x03040000
LIBSHIBOKEN_API void *PyType_GetSlot(PyTypeObject *, int);
#endif

// from typeslots.h
/* Do not renumber the file; these numbers are part of the stable ABI. */
/* Disabled, see #10181 */
#undef Py_bf_getbuffer
#undef Py_bf_releasebuffer
#define Py_mp_ass_subscript 3
#define Py_mp_length 4
#define Py_mp_subscript 5
#define Py_nb_absolute 6
#define Py_nb_add 7
#define Py_nb_and 8
#define Py_nb_nonzero 9
#define Py_nb_divmod 10
#define Py_nb_float 11
#define Py_nb_floor_divide 12
#define Py_nb_index 13
#define Py_nb_inplace_add 14
#define Py_nb_inplace_and 15
#define Py_nb_inplace_floor_divide 16
#define Py_nb_inplace_lshift 17
#define Py_nb_inplace_multiply 18
#define Py_nb_inplace_or 19
#define Py_nb_inplace_power 20
#define Py_nb_inplace_remainder 21
#define Py_nb_inplace_rshift 22
#define Py_nb_inplace_subtract 23
#define Py_nb_inplace_true_divide 24
#define Py_nb_inplace_xor 25
#define Py_nb_int 26
#define Py_nb_invert 27
#define Py_nb_lshift 28
#define Py_nb_multiply 29
#define Py_nb_negative 30
#define Py_nb_or 31
#define Py_nb_positive 32
#define Py_nb_power 33
#define Py_nb_remainder 34
#define Py_nb_rshift 35
#define Py_nb_subtract 36
#define Py_nb_true_divide 37
#define Py_nb_xor 38
#define Py_sq_ass_item 39
#define Py_sq_concat 40
#define Py_sq_contains 41
#define Py_sq_inplace_concat 42
#define Py_sq_inplace_repeat 43
#define Py_sq_item 44
#define Py_sq_length 45
#define Py_sq_repeat 46
#define Py_tp_alloc 47
#define Py_tp_base 48
#define Py_tp_bases 49
#define Py_tp_call 50
#define Py_tp_clear 51
#define Py_tp_dealloc 52
#define Py_tp_del 53
#define Py_tp_descr_get 54
#define Py_tp_descr_set 55
#define Py_tp_doc 56
#define Py_tp_getattr 57
#define Py_tp_getattro 58
#define Py_tp_hash 59
#define Py_tp_init 60
#define Py_tp_is_gc 61
#define Py_tp_iter 62
#define Py_tp_iternext 63
#define Py_tp_methods 64
#define Py_tp_new 65
#define Py_tp_repr 66
#define Py_tp_richcompare 67
#define Py_tp_setattr 68
#define Py_tp_setattro 69
#define Py_tp_str 70
#define Py_tp_traverse 71
#define Py_tp_members 72
#define Py_tp_getset 73
#define Py_tp_free 74
#define Py_nb_long 75
#define Py_nb_divide 76
#define Py_sq_slice 77
} // extern "C"
#endif // PY_MAJOR_VERSION < 3
#endif // TYPESPEC_H
