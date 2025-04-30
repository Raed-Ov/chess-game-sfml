cd /d "%~dp0"
g++ backend.cpp -o backend
backend.exe
del backend.exe
cls
del ..\run
exit