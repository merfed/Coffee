@echo off
:: Where is the MPQ Editor?
set ladik=I:\MPQEditor.exe
:: How is the output MPQ named?
set mpq=patch-2.mpq
:: How is the directory with the data called?
set files=patch-d.mpq

del %mpq%
del %CD%\files
del %CD%\filesb
del tempscript.scp
echo n %mpq% > tempscript.scp
for /f %%i in (%CD%\listfile_%files%) do (
echo a %mpq% patch-d.mpq\%%i >> tempscript.scp
echo r %mpq% %%~ni%%~xi %%i >> tempscript.scp
)
echo c >> tempscript.scp
%ladik% /console tempscript.scp
del tempscript.scp