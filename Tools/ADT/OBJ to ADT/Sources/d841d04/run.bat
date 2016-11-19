@echo off

for %%i in (*.adt) do java -jar OBJ_to_ADT.jar "%%i"

pause
