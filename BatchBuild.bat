@echo off
set MAYA_VERSIONS=20 22 23 24

for %%v in (%MAYA_VERSIONS%) do (
    echo "-----------------------------------------Building mll plugin for maya version %%v-----------------------------------------"
    Build.bat %%v
    if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%
    echo "-----------------------------------------Building mll plugin for maya version %%v Done-----------------------------------------"
    pause
)