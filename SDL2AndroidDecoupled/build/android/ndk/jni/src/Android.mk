LOCAL_PATH := $(call my-dir)

# External module
##################
include $(CLEAR_VARS)
LOCAL_MODULE := SDL2
LOCAL_SRC_FILES := $(LOCAL_PATH)/../../../lib/SDL2/$(TARGET_ARCH_ABI)/libSDL2.so
# LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../include/SDL2
# LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../../include/SDL2
include $(PREBUILT_SHARED_LIBRARY)

# Application Module
#####################
include $(CLEAR_VARS)
LOCAL_MODULE := main
#SDL_PATH := ../SDL2
MY_CODE_PATH := $(LOCAL_PATH)/../../../../../src
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../include
LOCAL_SRC_FILES := $(MY_CODE_PATH)/SDL_android_main.c \
    $(MY_CODE_PATH)/main.cpp
LOCAL_SHARED_LIBRARIES := SDL2
LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -llog
include $(BUILD_SHARED_LIBRARY)
