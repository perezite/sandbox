rem ## initialize
SET basePath=%~dp0

rem ## build
g++ Main.cpp -ggdb -o ../Build/Main.exe -static-libgcc -static-libstdc++

rem ## run
SET executablePath=%basePath:~0,-1%\..\Build\Main.exe
start %executablePath%
