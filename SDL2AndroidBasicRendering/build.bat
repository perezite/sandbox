@echo off

cd jni
call ndk-build
cd ..
call ant debug install