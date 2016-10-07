@echo off

cd jni
call ndk-build
cd ..
pause

call ant debug install