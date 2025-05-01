cd /d "%~dp0"
type nul > backend\run
del mouseOutput.txt
type nul > mouseOutput.txt

rem START "Backend" "backend\backend.bat"
cd /d "%~dp0"

g++ -c main.cpp -Isfml\include
if exist main.o g++ main.o -o chess-game -mwindows -Lsfml\lib -lsfml-graphics -lsfml-window -lsfml-system
if exist main.o del main.o
if exist  chess-game.exe set PATH=%PATH%;%cd%\sfml\bin

if exist  chess-game.exe chess-game.exe
rem if exist chess-game.exe cls
if exist chess-game.exe del chess-game.exe

del mouseOutput.txt
