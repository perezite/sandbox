@echo off
echo Before we start: Check the variable 'androidNDKRoot' in 'boost_1_56_0\tools\build\src\user-config.jam' and point it to the folder 'android-ndk-r10b' right next to this *.bat file
pause

cd boost_1_56_0

call bootstrap
call b2 --clean-all -n
call b2 --without-python toolset=gcc-android4.8_armeabi link=static runtime-link=static target-os=linux architecture=arm --stagedir=android-armeabi-v7a threading=multi

cd ..
echo Check the boost compile log for errors. Libs should have been written to ./boost_1_56_0/android-armeabi-v7a