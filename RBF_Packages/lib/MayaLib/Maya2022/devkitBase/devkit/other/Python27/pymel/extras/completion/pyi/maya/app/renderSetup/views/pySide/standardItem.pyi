from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
from PySide2.QtGui import QStandardItem


if False:
    from typing import Dict, List, Tuple, Union, Optional

class StandardItem(QStandardItem):
    def __eq__(self, other: Any) -> bool: ...
    def __ne__(self, other: Any) -> bool: ...
    def depth(self): ...



