# Integration instructions

The package contains a static library build of Microblink scanning library. Also, a demo app is provided as a reference for the integration.

Native static library is located in `libRecognizer` folder.

## Integration steps

1. In your Android.mk file, add `libRecognizer/inc` folder to compiler include paths.
2. Add `libRecognizer/lib/armeabi`, `libRecognizer/lib/armeabi-v7a` or `libRecognizer/lib/x86` to your linker paths.
3. Add following linker flags: `-lRecognizerApi -lz -llog`
4. Add `cpufeatures` android module.

The easiest way to achieve this is to create a prebuild static library module with following code in your Android.mk:

```
include $(CLEAR_VARS)
RECOGNIZER_API_DIR = ../libRecognizer
LOCAL_MODULE := RecognizerApi
LOCAL_SRC_FILES := ../$(RECOGNIZER_API_DIR)/lib/$(TARGET_ARCH_ABI)/lib$(LOCAL_MODULE).a
LOCAL_EXPORT_C_INCLUDES := $(RECOGNIZER_API_DIR)/inc
include $(PREBUILT_STATIC_LIBRARY)
```
    
Then, simply add following statements in your local module in Android.mk

```
LOCAL_LDLIBS += -lz -llog -Wl,--gc-sections
LOCAL_STATIC_LIBRARIES += RecognizerApi cpufeatures
```

`-Wl,--gc-sections` option is optional, but reduces the final size of the native library.
 
Consult `RecognizerApiDemo` demo application and its Android.mk file for more information.

 See the [documentation](doc/index.html) for code examples and API reference.

## Example with JPEG image inside java byte array

The complete tutorial and API reference is in [documentation](doc/index.html). Here is only part that explains how to extract bytes from java byte array in order to present them to RecognizerAPI.
    
In this example, `bytes` is java byte array containing JPEG encoded image. Function will return java string representation of the barcode data.

```c
jbyte* buffer = env->GetByteArrayElements(bytes, 0);
jsize bufferLen = env->GetArrayLength(bytes);

RecognizerErrorStatus status;

RecognizerResultList* resultList;
status = recognizerRecognizeFromEncodedImage(recognizer_, &result, buffer, bufferLen, NULL);
env->ReleaseByteArrayElements(bytes, buffer, 0);

if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
	char err[1000];
	sprintf(err, "Recognizer error %s", recognizerErrorToString(status));
	return env->NewStringUTF(err);
}
```
     
To see the tutorial with examples in settings initialization and how to obtain results, check [documentation](doc/index.html).	

## Demo application

You can use the provided demo application to test the barcode scanning library.

You can test it in two ways:

- By taking a photo and performing a scan on that photo
- By opening an image from file manager app; when asked "Complete action using", select `RecogizerApiDemo` app.

### Importing the demo application into Eclipse workspace

In order to import the demo application into your Eclipse workspace, please follow these steps:

1. You need [ADT](https://developer.android.com/sdk/installing/installing-adt.html) and [CDT](https://www.eclipse.org/cdt/) Eclipse plugins.
2. You need [Android Native Development Kit (NDK)](https://developer.android.com/tools/sdk/ndk/index.html)
3. In Eclipse, click `File->Import` and select `Android->Existing Android Code into Workspace` and click `Next`
4. Select `RecognizerApiDemoEclipse` in `Root Directory` field, select the demo project from the list and click `Finish`.
5. After Eclipse imports the project, right click on it and select `Properties`.
6. In properties windows select `C/C++ build` from the left pane and enter the path to your `ndk-build` script in `Build command` field.
7. Finally, open `jni/main.cpp` file and add your license key to and your name to line 137:

	```c
	recognizerSettingsSetLicenseKey(settings, "add licensee here", "add license key here");
	```

8. That's it. You are now ready to run the demo.

### Android Studio

In order to import the demo application to your Android Studio, please follow these steps:

1. You need [Android Native Development Kit (NDK)](https://developer.android.com/tools/sdk/ndk/index.html)
2. In Android Studio's welcome screen, choose _Import project (Eclipse ADT, Gradle, etc._
3. In dialog, select `RecognizerApiDemoAndroidStudio` folder and click OK
4. Android Studio will import project and will fail with error "Please create a define ndk.dir property in local.properties file!"
5. Open the local.properties file (it will be created by Android Studio during import) and beneath `sdk.dir=/path/to/your/android/sdk` line add `ndk.dir=/path/to/your/ndk/folder`. Your NDK folder is folder where you extracted Android Native Development Kit from step 1.
6. Finally, open `src/main/jni/main.cpp` file and add your license key to and your name to line 137:

	```c
	recognizerSettingsSetLicenseKey(settings, "add licensee here", "add license key here");
	```
7. That's it. You are now ready to run the demo.
        
## Additional info
 
For any inquiries, additional information or instructions please contact us at <support@microblink.com>.
    