from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import sys
import os
import ctypes
import fnmatch


"""
#############################################################################
##
## Copyright (C) 2017 The Qt Company Ltd.
## Contact: https://www.qt.io/licensing/
##
## This file is part of PySide2.
##
## $QT_BEGIN_LICENSE:LGPL$
## Commercial License Usage
## Licensees holding valid commercial Qt licenses may use this file in
## accordance with the commercial license agreement provided with the
## Software or, alternatively, in accordance with the terms contained in
## a written agreement between you and The Qt Company. For licensing terms
## and conditions see https://www.qt.io/terms-conditions. For further
## information use the contact form at https://www.qt.io/contact-us.
##
## GNU Lesser General Public License Usage
## Alternatively, this file may be used under the terms of the GNU Lesser
## General Public License version 3 as published by the Free Software
## Foundation and appearing in the file LICENSE.LGPL3 included in the
## packaging of this file. Please review the following information to
## ensure the GNU Lesser General Public License version 3 requirements
## will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
##
## GNU General Public License Usage
## Alternatively, this file may be used under the terms of the GNU
## General Public License version 2.0 or (at your option) the GNU General
## Public license version 3 or any later version approved by the KDE Free
## Qt Foundation. The licenses are as published by the Free Software
## Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
## included in the packaging of this file. Please review the following
## information to ensure the GNU General Public License requirements will
## be met: https://www.gnu.org/licenses/gpl-2.0.html and
## https://www.gnu.org/licenses/gpl-3.0.html.
##
## $QT_END_LICENSE$
##
#############################################################################
"""


from ctypes import c_ulong as DWORD
from ctypes import create_unicode_buffer
from ctypes import c_wchar_p as LPCWSTR
from ctypes import c_wchar_p as LPWSTR


if False:
    from typing import Dict, List, Tuple, Union, Optional

def _get_win32_long_name(s):
    """
    Returns long name
    """
    ...
def _dir_contains(dir, filter): ...
def _get_win32_short_name(s):
    """
    Returns short name
    """
    ...
def u(x): ...
def register_qt_conf(prefix, binaries, plugins, imports, translations, force='False'):
    """
    Register qt.conf in Qt resource system to override the built-in
    configuration variables, if there is no default qt.conf in
    executable folder and another qt.conf is not already registered in
    Qt resource system.
    """
    ...
def _get_win32_case_sensitive_name(s):
    """
    Returns long name in case sensitive format
    """
    ...
def byref(*args, **kwargs):
    """
    byref(C instance[, offset=0]) -> byref-object
    Return a pointer lookalike to a C instance, only usable
    as function argument
    """
    ...
def u_fs(x): ...
def POINTER(*args, **kwargs): ...
def get_pyside_dir(): ...
def _get_qt_conf_resource(prefix, binaries, plugins, imports, translations):
    """
    Generate Qt resource with embedded qt.conf
    """
    ...
def sizeof(*args, **kwargs):
    """
    sizeof(C type) -> integer
    sizeof(C instance) -> integer
    Return the size in bytes of a C instance
    """
    ...
def _rcc_write_data(out, data): ...
def _filter_match(name, patterns): ...
def _rcc_write_number(out, number, width): ...


GetShortPathNameW : ctypes._FuncPtr
PY_2 : bool
GetLongPathNameW : ctypes._FuncPtr
MAX_PATH : int

