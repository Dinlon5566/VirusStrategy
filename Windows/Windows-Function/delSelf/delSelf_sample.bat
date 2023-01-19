:startExe

if not exist delSelf.exe goto done

del /f /q delSelf.exe

goto startExe

:done

del /f /q %0
