@echo off
for %%i in (*.adt) do CheckForMDDFFlags.exe "%%i"
pause