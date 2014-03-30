@echo off
for %%i in (*.adt) do CheckForMCLYFlags.exe "%%i"
pause