set LIB_ROOT=G:/AL/Projects/OpenGLBMaint/Server
set MINGW_ROOT=C:/MinGW
set PATH=%PATH%:%LIB_ROOT%/lib/win
del CMakeCache.txt
%MINGW_ROOT%/bin/mingw32-make.exe clean
REM if the line below gives an error about sh.exe run it again
ECHO "RUN cmake -G \"MinGW Makefiles\" again if you get an error"
cmake -G "MinGW Makefiles"
%MINGW_ROOT%/bin/mingw32-make.exe
