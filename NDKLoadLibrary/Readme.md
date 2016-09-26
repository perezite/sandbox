# Content
Shows, how to use a shared library (OpenGLEngine) in a project (Application), where both modules are built with NDK

# Build
cd OpenGLEngine/jni
ndk-build
Copy: OpenGLEngine/Engine/engine.h -> Application/jni/src/include
Copy: OpenGLEngine/libs -> Application/jni/src/libs
cd Application
ndk-build
ant debug install

# Reference
https://www.youtube.com/watch?v=iVir4N55YQg