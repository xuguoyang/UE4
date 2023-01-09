@echo off
echo 1 > ..\..\..\Engine\Build\InstalledBuild.txt

if not exist "%~dp0..\..\..\Engine\Build\BatchFiles\GenerateProjectFiles.bat" goto Error_BatchFileInWrongLocation
call "%~dp0..\..\..\Engine\Build\BatchFiles\GenerateProjectFiles.bat" "%~dp0MyActionRPG.uproject" -game -Installed %*
exit /B %ERRORLEVEL%

:Error_BatchFileInWrongLocation
echo GenerateProjectFiles ERROR: The batch file does not appear to be located in the root UE4 directory.  This script must be run from within that directory.
pause
exit /B 1
