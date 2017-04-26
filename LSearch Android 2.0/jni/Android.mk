LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

OpenCV_INSTALL_MODULES:=on
OPENCV_CAMERA_MODULES:=off


OPENCV_LIB_TYPE:=STATIC


ifeq ("$(wildcard $(OPENCV_MK_PATH))","")
include F:\android\opencv4android\OpenCV-2.4.10-android-sdk\sdk\native\jni\OpenCV.mk
else  
include $(OPENCV_MK_PATH)  
endif 

LOCAL_MODULE    := LSearch
### Add all source file names to be included in lib separated by a whitespace
LOCAL_SRC_FILES := LSearch.cpp
LOCAL_LDLIBS    += -lm -llog

include $(BUILD_SHARED_LIBRARY)