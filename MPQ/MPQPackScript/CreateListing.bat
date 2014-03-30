:: How is the directory with the data called?
set files=patch-d.mpq

del %CD%\files
del %CD%\filesb
del %CD%\listfile_%files%
cd %files%
dir /b /s /a:-H-D >> %CD%\..\files
findstr /v .svn %CD%\..\files >> %CD%\..\filesb
..\replace.bat "%CD%\" "" %CD%\..\filesb >> %CD%\..\listfile_%files%
