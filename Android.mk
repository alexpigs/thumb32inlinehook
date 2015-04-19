LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE    := libarmv7inlinehook
LOCAL_SRC_FILES :=  armv7inlinehook.c.arm
include $(BUILD_STATIC_LIBRARY)
