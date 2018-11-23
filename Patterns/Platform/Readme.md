# Purpose
This is a test for platform-dependent compilation in C++
The file Input.cpp has one function which is used for both test-platforms (Windows and Android).
The Input_Android.cpp and Input_Windows.cpp files contain platform dependent implementations of two functions.
These three files make up the class 'Input'.
build_and_run.bat compiles the project twice, once for windows and once for android.