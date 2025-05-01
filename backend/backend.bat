cd /d "%~dp0"
g++ backend.cpp -o backend -mwindows
backend.exe < ../mouseOutput.txt
del backend.exe
cls
exit