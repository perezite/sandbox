LOCAL_PATH := $(call my-dir)
MY_CODE_PATH := $(LOCAL_PATH)/../../../../../../src
MY_INCLUDE_PATH := $(LOCAL_PATH)/../../../../../../src

include $(CLEAR_VARS)
LOCAL_MODULE := sanangeles
LOCAL_SRC_FILES := $(MY_CODE_PATH)/app-android.c \
    $(MY_CODE_PATH)/demo.c \
    $(MY_CODE_PATH)/importgl.c
LOCAL__C_INCLUDES := $(MY_INCLUDE_PATH)
LOCAL_LDLIBS := -landroid -ldl -lGLESv1_CM -llog -lm
include $(BUILD_SHARED_LIBRARY)
