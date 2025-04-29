cd /d "%~dp0"
START "Backend" "backend\backend.bat"
cd /d "%~dp0"
g++ -c main.cpp -Isfml\include
if exist main.o g++ main.o -o chess-game -mwindows -Lsfml\lib -lsfml-graphics -lsfml-window -lsfml-system
if exist main.o del main.o 
if exist  chess-game.exe set PATH=%PATH%;%cd%\sfml\bin
:loop
if exist chess-game.exe (chess-game.exe)
else(goto end)
goto loop
:end
if exist chess-game.exe del chess-game.exe