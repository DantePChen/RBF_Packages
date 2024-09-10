from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
if False:
    from typing import Dict, List, Tuple, Union, Optional

class Issue(object):
    """
    Class representing an issue that contains 
    - a description (a short string explaining what's the issue)
    - a type, mostly used for UI purpose (icon for the issue will be RS_<type>.png)
    - a callback to resolve the issue (assisted resolve).
    """
    
    
    
    def __eq__(self, other: Any) -> bool: ...
    def __hash__(self): ...
    def __init__(self, description, type="'warning'", resolveCallback='None'): ...
    def __str__(self) -> str: ...
    def resolve(self): ...
    @property
    def description(self): ...
    @property
    def type(self): ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor



