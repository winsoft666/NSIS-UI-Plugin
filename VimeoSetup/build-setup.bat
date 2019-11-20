@echo off
python.exe ..\NsisScriptGenerate.py "%~dp0vimeo-template.nsi" "%~dp0App"
"%NSIS_DIR%\makensisw.exe" "%~dp0\vimeo.nsi"

PAUSE