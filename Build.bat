@echo off
set "MAYA_VERSION=%1"
set MAYA_FULL_VERSION=20%MAYA_VERSION%
if not defined MAYA_VERSION SET "MAYA_VERSION=23"
set "VS_VERSION=%2"
if not defined VS_VERSION SET "VS_VERSION=22"
set "PLUGIN_ARCHIVE_DIR=%3"
set "SOURCE_ROOT_DIR=%~dp0\RBF_Packages"
set SLN_NAME=RBF_Packages.sln


call :check_msbuild_exists %VS_VERSION% Professional
if not exist %VS_MSBUILD_EXEC% call :check_msbuild_exists %VS_VERSION% Community
if not exist %VS_MSBUILD_EXEC% (
    echo error: cannot find Visual Studio %VS_VERSION%
    exit /b -1
)

call :check_Plugins_dir

if defined PLUGIN_ARCHIVE_DIR (
    %VS_MSBUILD_EXEC% "%SOURCE_ROOT_DIR%/%SLN_NAME%" /property:Configuration=Release /property:Platform=x64 /property:MayaVersion=%MAYA_VERSION% /property:mllOutputPath=%PLUGIN_ARCHIVE_DIR%
) else (
    setlocal enabledelayedexpansion
    %VS_MSBUILD_EXEC% "%SOURCE_ROOT_DIR%/%SLN_NAME%" /property:Configuration=Release /property:Platform=x64 /property:MayaVersion=%MAYA_VERSION%
)
if %ERRORLEVEL% neq 0 (
    echo error: build failed for Maya 20%MAYA_VERSION%
    exit /b %ERRORLEVEL%
)
goto :eof

@REM check_msbuild_exists VS_VERSION VS_SERVICE
:check_msbuild_exists
set VS_FULL_VERSION=20%~1
set VS_ROOT_DIR=%ProgramFiles%\Microsoft Visual Studio\%VS_FULL_VERSION%\%~2
set VS_MSBUILD_EXEC="%VS_ROOT_DIR%\MSBuild\Current\Bin\MSBuild.exe"
echo: got: %VS_MSBUILD_EXEC%
if exist %VS_MSBUILD_EXEC% echo found MSBuild.exe: %VS_MSBUILD_EXEC%
goto :eof

@REM check if have plug-ins folder, if not ,create it
:check_Plugins_dir
set plug_dir=%USERPROFILE%\Documents\maya\20%MAYA_VERSION%\plug-ins
echo %plug_dir%
if exist "%plug_dir%" (
    echo Found plugin directory: %plug_dir%
) else (
    echo Plugin directory not found. Creating directory: %plug_dir%
    mkdir %plug_dir%
)