from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya


"""
This module exists to avoid cyclic dependencies between modules.
"""


if False:
    from typing import Dict, List, Tuple, Union, Optional

def copyPasteLabelColorEnabled(): ...
def computeValidObjectName(dict, mergeType, prependToName, objectTypeName): ...
def importLabelColorEnabled(): ...
def enableImportLabelColor(value): ...
def exportLabelColorEnabled(): ...
def enableCopyPasteLabelColor(value): ...
def enableExportLabelColor(value): ...


SCENE_AOVS_ATTRIBUTE_NAME : str
kMissingTypeName : unicode
CHILDREN_ATTRIBUTE_NAME : str
RENDERABLE_ATTRIBUTE_NAME : str
kUnknownData : unicode
kWrongMergeType : unicode
DECODE_AND_ADD : int
_enableExportLabelColor : bool
NOTES_ATTRIBUTE_NAME : str
kUnknownKeys : unicode
kMissingProperty : unicode
kUnknownTypeNode : unicode
SELECTOR_ATTRIBUTE_NAME : str
kTypeNodeCreationFailed : unicode
ACCEPT_COLOR : int
VISIBILITY_ATTRIBUTE_NAME : str
_copyPasteColor : bool
LAYERS_ATTRIBUTE_NAME : str
LABEL_COLOR_ATTRIBUTE_NAME : str
SCENE_SETTINGS_ATTRIBUTE_NAME : str
NAME_ATTRIBUTE_NAME : str
IMPORTED_ATTRIBUTE_NAME : str
COLLECTIONS_ATTRIBUTE_NAME : str
kObjectAlreadyExists : unicode
kFaultyTypeName : unicode
DECODE_AND_RENAME : int

