LOCAL_PATH := $(call my-dir)
SDL_PATH := ../SDL2
MY_CODE_PATH := $(LOCAL_PATH)/../../../../../src

# External module
##################
include $(CLEAR_VARS)
LOCAL_MODULE := openGLSharedLibrary
LOCAL_SRC_FILES := $(LOCAL_PATH)/../../../lib/OpenGLEngine/$(TARGET_ARCH_ABI)/libOpenGLEngine.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../../include/OpenGLEngine
include $(PREBUILT_SHARED_LIBRARY)

# Application module
####################
include $(CLEAR_VARS)
LOCAL_MODULE := Application
LOCAL_SRC_FILES := $(wildcard $(MY_CODE_PATH)/Application/*.cpp)
LOCAL_EXPORT_C_INCLUDES := MY_CODE_PATH)/Application
LOCAL_SHARED_LIBRARIES := SDL2 openGLSharedLibrary
include $(BUILD_STATIC_LIBRARY)

# Main module
#####################
include $(CLEAR_VARS)
LOCAL_MODULE := main
LOCAL_SRC_FILES := $(SDL_PATH)/src/main/android/SDL_android_main.c \
    $(MY_CODE_PATH)/main.cpp  
LOCAL_STATIC_LIBRARIES := Application openGLSharedLibrary
LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -llog
include $(BUILD_SHARED_LIBRARY)
