LOCAL_PATH := $(call my-dir)

# Shared Library module
#######################
include $(CLEAR_VARS)
LOCAL_MODULE := SharedLibrary
LOCAL_SRC_FILES := ../../libs/$(TARGET_ARCH_ABI)/libSharedLibrary.so
include $(PREBUILT_SHARED_LIBRARY)

# Main module
#################
include $(CLEAR_VARS)
LOCAL_MODULE    := hello-jni
LOCAL_SRC_FILES := hello-jni.cpp
LOCAL_SHARED_LIBRARIES := SharedLibrary
include $(BUILD_SHARED_LIBRARY)
