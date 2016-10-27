LOCAL_PATH := $(call my-dir)

# OpenGLEngine module
##################
include $(CLEAR_VARS)
LOCAL_MODULE := openGLSharedLibrary
LOCAL_SRC_FILES := ../../lib/OpenGLEngine/$(TARGET_ARCH_ABI)/libOpenGLEngine.so
include $(PREBUILT_SHARED_LIBRARY)

# SDL2 module
##################
include $(CLEAR_VARS)
LOCAL_MODULE := SDL2
LOCAL_SRC_FILES := ../../lib/SDL2/$(TARGET_ARCH_ABI)/libSDL2.so
include $(PREBUILT_SHARED_LIBRARY)

# Application Module
#####################
include $(CLEAR_VARS)
LOCAL_MODULE    := hello-jni
LOCAL_SRC_FILES := ../../../../src/hello-jni.c
LOCAL_SHARED_LIBRARIES := SDL2 openGLSharedLibrary
include $(BUILD_SHARED_LIBRARY)
