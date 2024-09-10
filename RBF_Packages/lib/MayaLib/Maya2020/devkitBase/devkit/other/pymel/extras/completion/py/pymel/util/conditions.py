import exceptions


"""
#------------------------------------------------------------------------------
# Condition objects - used for chaining together tests that yield True/False results
#------------------------------------------------------------------------------
"""


if False:
    from typing import Dict, List, Tuple, Union, Optional

class NO_DATA(exceptions.Exception):
    __weakref__ = None


class Condition(object):
    """
    Used to chain together objects for conditional testing.
    """
    
    
    
    def __and__(self, other): pass
    def __init__(self, value='None'): pass
    def __invert__(self): pass
    def __nonzero__(self): pass
    def __or__(self, other): pass
    def __rand__(self, other): pass
    def __ror__(self, other): pass
    def __str__(self): pass
    def eval(self, data='"<class \'pymel.util.conditions.NO_DATA\'>"'): pass
    __dict__ = None
    
    
    __weakref__ = None


class Inverse(Condition):
    def __init__(self, toInvert): pass
    def __str__(self): pass
    def eval(self, data='"<class \'pymel.util.conditions.NO_DATA\'>"'): pass


class AndOrAbstract(Condition):
    def __init__(self, *args): pass
    def __str__(self): pass
    def eval(self, data='"<class \'pymel.util.conditions.NO_DATA\'>"'): pass


class Or(AndOrAbstract):
    pass


class And(AndOrAbstract):
    pass




Always = Condition()

Never = Condition()


