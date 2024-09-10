from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
"""
Low-level maya and pymel utilities.  Functions in this module are used by both `pymel.api` and `pymel.core`,
and are able to be defined before maya.standalone is initialized.
"""


from pymel.internal.plogging import getLogger


if False:
    from typing import Dict, List, Tuple, Union, Optional
