LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
RECOGNIZER_API_DIR = ../../../../libRecognizer
LOCAL_MODULE := RecognizerApi
LOCAL_SRC_FILES := $(RECOGNIZER_API_DIR)/lib/$(TARGET_ARCH_ABI)/lib$(LOCAL_MODULE).a
LOCAL_EXPORT_C_INCLUDES := $(RECOGNIZER_API_DIR)/inc
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE := NativeRecognizer

LOCAL_SRC_FILES += 	main.cpp

LOCAL_C_INCLUDES += utf8
LOCAL_LDLIBS +=  -Wl,--gc-sections -lz -llog -fuse-ld=gold

LOCAL_STATIC_LIBRARIES += RecognizerApi
LOCAL_STATIC_LIBRARIES += cpufeatures

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/cpufeatures)
