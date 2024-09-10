from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya.app.renderSetup.model.jsonTranslatorUtils as jsonTranslatorUtils
import maya
import maya.app.renderSetup.model.renderSetupPreferences as userPrefs
import maya.app.renderSetup.model.jsonTranslatorGlobals as jsonTranslatorGlobals
import maya.cmds as cmds


"""
These classes are the UI builders for the options of import and Export
of a render setup.
"""


if False:
    from typing import Dict, List, Tuple, Union, Optional

class ExportUI(object):
    """
    Helper class to build the Options UI for the fileDialog2 command used when exporting all
    """
    
    
    
    @staticmethod
    def addOptions(parent, exportOption): ...
    @staticmethod
    def setLabelColorExport(value):
        """
        User can decide to export label colors to json
        """
        ...
    @staticmethod
    def setNotesText(data):
        """
        Preserve the notes because it's consumed after the UI is gone.
        Note: Trap the focus changed which is the only way to have the text for a scroll field.
        """
        ...
    @staticmethod
    def setRenderSettingsExport(value):
        """
        User can decide to export Render Settings and AOVs to json
        """
        ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor
    
    exportColorType : int
    
    exportTextEditor : NoneType
    
    notesText : NoneType
    
    notesTextEditor : NoneType


class ImportAOVsUI(object):
    """
    Helper class to build the Options UI for the fileDialog2 command used for importing AOVs
    """
    
    
    
    @staticmethod
    def addOptions(parent): ...
    @staticmethod
    def setMergeImportType(data):
        """
        Merge the content of the existing render setup with the imported content. 
        If an unexpected render setup object is found it will renamed using the 'importText'.
        """
        ...
    @staticmethod
    def setOverwriteImportType(data):
        """
        Completely overwrite the content of the existing render setup with the imported content.
        """
        ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor
    
    importType : int


class ParentGuard(object):
    def __enter__(self): ...
    def __exit__(self, type, value, traceback): ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor


class ImportAllUI(object):
    """
    Helper class to build the Options UI for the fileDialog2 command used when importing all
    """
    
    
    
    @staticmethod
    def addOptions(parent): ...
    @staticmethod
    def setImportText(data):
        """
        Preserve the text because it's consumed after the UI is gone.
        """
        ...
    @staticmethod
    def setLabelColorImport(value):
        """
        User can decide to import label colors from json
        """
        ...
    @staticmethod
    def setMergeImportType(data):
        """
        Merge the content of the existing render setup with the imported content. 
        If an unexpected render setup object is found it will renamed using the 'importText'.
        """
        ...
    @staticmethod
    def setOverwriteImportType(data):
        """
        Completely overwrite the content of the existing render setup with the imported content.
        """
        ...
    @staticmethod
    def setRenameImportType(data):
        """
        Always rename the imported render setup content using the 'importText'.
        """
        ...
    @staticmethod
    def updateContent(parent, selectedFilename):
        """
        Update the displayed content following the file selection 
        Note: If the file is not a render setup file or is a directory, 
              the content (notes & preview) will be empty.
        """
        ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor
    
    importColorType : int
    
    importText : str
    
    importTextEditor : NoneType
    
    importType : int
    
    notesEditor : NoneType
    
    previewEditor : NoneType




kOverwriteExplanation : unicode
kRename : unicode
kLabelColorImportExplanation : unicode
kLabelColorOptions : unicode
kLabelColorExportExplanation : unicode
kUnknownFile : unicode
kPreview : unicode
kDefaultTextToPrepend : str
kTextToPrepend : unicode
kMerge : unicode
kRenderSettingsOptions : unicode
kLabelColorExport : unicode
kOverwrite : unicode
kLabelColorImport : unicode
kGeneralOptions : unicode
DEFAULT_UI_INDENTATION : int
kRenderSettingsExport : unicode
kMergeExplanation : unicode
kMergeAOVExplanation : unicode
kNotes : unicode
kRenameExplanation : unicode
kRenderSettingsExportExplanation : unicode

