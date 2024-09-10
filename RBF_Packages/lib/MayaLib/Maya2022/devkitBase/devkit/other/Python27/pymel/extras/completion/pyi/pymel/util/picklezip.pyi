from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import gzip
import cPickle as pickle


if False:
    from typing import Dict, List, Tuple, Union, Optional

def _loads(filename):
    """
    Load a compressed pickle from disk to an upicklable string
    """
    ...
def load(filename):
    """
    Load a compressed pickle from disk
    """
    ...
def dump(object, filename, protocol='-1'):
    """
    Save an compressed pickle to disk.
    """
    ...

