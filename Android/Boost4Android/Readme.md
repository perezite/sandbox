# Contents
My try at compiling boost using android ndk. 
It worked partially....
I wanted to use Boost::Coroutine, which has a dependeny on Boost::Context and Boost::Thread which are precisely the two libs that failed to compile..
so in short: This sandbox is not in a usable state yet. But it may become at a later date....
 
# Compile
Boost compilation for Boost 1.56.0 using NDK R10b: Execute compile.bat
 
# Interesting articles about Boost and coroutines in general
http://www.technical-recipes.com/2014/how-to-use-the-boost-compiled-libraries-in-windows/
http://www.technical-recipes.com/2014/using-subset-boost-in-windows/
:https://github.com/boostorg/build/issues/236 (Latest Boost seems not to be compilable out of the box)
https://stackoverflow.com/questions/37679587/how-to-compile-boost-1-61-for-android-ndk-11	
http://www.ilikebigbits.com/blog/2016/3/20/coroutines-for-games	
https://svn.boost.org/trac10/ticket/12711 (Fix for boost compilation hanging when compiling with gcc-x64 cross compilation)
https://gist.github.com/yizhang82/28842f7dbae34b59fcd7b4d74b4a19d4
http://munteanumarian.blogspot.ch/2014/10/build-boost-for-android.html
https://schneide.wordpress.com/2016/09/19/c-coroutines-on-windows-with-the-fiber-api/
https://www.chiark.greenend.org.uk/~sgtatham/coroutines.html
https://www.geeksforgeeks.org/coroutines-in-c-cpp/
https://github.com/moritz-wundke/Boost-for-Android (Uses MSYS, this may be the way to go...)
http://www.mingw.org/wiki/msys
