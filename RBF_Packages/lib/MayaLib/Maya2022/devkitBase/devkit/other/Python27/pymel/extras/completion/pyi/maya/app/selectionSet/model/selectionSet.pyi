from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
if False:
    from typing import Dict, List, Tuple, Union, Optional

class SelectionSet(object):
    """
    This class defines DG behavior related to the 'selection set'.
    
    Note: This class keeps nothing in memory. The way to retrieve the information 
          is to query the DG using the name to build the DG path
    """
    
    
    
    def __init__(self, name, tobeCreated): ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor




def get(name):
    """
    Get a specific Selection Set
    """
    ...
def getAll(selectedNodesOnly, parentNameFiler):
    """
    Get the list of all Selection Sets
    """
    ...
def create(name):
    """
    Create a specific Selection Set
    """
    ...

