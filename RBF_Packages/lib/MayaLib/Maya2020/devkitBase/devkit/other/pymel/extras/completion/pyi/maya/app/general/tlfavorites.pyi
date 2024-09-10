from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya
import pprint
import os
import maya.cmds as cmds


from functools import partial


if False:
    from typing import Dict, List, Tuple, Union, Optional

class TLFavoriteStore(object):
    """
    Manages syncing treeLister favorites to a file
    """
    
    
    
    def __init__(self, fname): ...
    def attachClient(self, lister):
        """
        Add a treeLister instance as a client of this favorites store.
        Any changes from the lister will propagate to other clients.
        """
        ...
    def get(self):
        """
        returns the list of favorites
        """
        ...
    def removeClient(self, lister): ...
    def update(self, lister, path, isAdded):
        """
        updates the persistant favorite store 
        
        lister  - name of the treeLister which has added/removed the favorite
        path    - item path to be updated
        isAdded - True means add to store, False means remove
        """
        ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor




def detachStore(lister):
    """
    Disconnects the given tree lister from the favorites store.  The
    lister will no longer be updated when the store changes.
    """
    ...
def addPath(path, key):
    """
    Add a path to key pair.
    """
    ...
def attachStore(lister, fpath):
    """
    Connects the TLFavoriteStore instance for the specified file path
    to the specified tree lister.  Populates the treeLister with the
    stored favorites.
    
    lister   - the treeLister 
    fpath    - full path to the favorites file
    """
    ...
def readFavorites(fname):
    """
    returns the favorites from the specified file 
    as a MEL-friendly flattened list 
    ["path","key","path2","key2",...]
    """
    ...


_header : str
_pathToStoredKey : dict
_storesByFilename : dict

