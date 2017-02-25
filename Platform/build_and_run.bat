@echo off

echo Compile for Android...
g++ main.cpp Input.cpp Input_Windows.cpp Input_Android.cpp -UWIN32 -DANDROID
pause
cls
a.exe
pause

echo Compile for Windows...
g++ main.cpp Input.cpp Input_Windows.cpp Input_Android.cpp -DWIN32
pause
cls
a.exe
pause
