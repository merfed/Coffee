@echo OFF 
timeout 1 
echo.  
echo removing extensions from all wmo files.
for /r %%x in (*.wmo) do ren "%%x" *.
timeout 1
cls  
echo Plugging in Files into 010 Editor now... 
echo Average time for this procedure is 1-3 Seconds per file.... 
@echo ON
for /r %%x in (*.*) do 010editor "%%x" -script:"C:\scripts\datadumpermacro2015rewrite_004v.1sc" -nowarnings -noui 
timeout 1
@echo OFF
cls
echo All Done!! You can close this out now or just hit any key! 
echo.
echo Contact - Twitter: PhillyTNGee
timeout 20 
