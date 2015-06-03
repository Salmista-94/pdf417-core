/*
 * main.cpp
 *
 *  Created on: Oct 14, 2013
 *      Author: mkorpar
 */

#include <jni.h>
#include <stdlib.h>
#include <stdio.h>
#include <android/log.h>

#include "RecognizerApi.h"
#include "utf8.h"
#include <string>
#include <sstream>
#include <vector>

#ifdef __cplusplus
extern "C" {
#endif

#define LOGD(format, ...) __android_log_print(ANDROID_LOG_DEBUG, "Native recognizer", format, ##__VA_ARGS__);
#define LOGE(format, ...) __android_log_print(ANDROID_LOG_ERROR, "Native recognizer", format, ##__VA_ARGS__);

static Recognizer* recognizer_ = NULL;

void onDetectionStarted() {
    printf("Detection has started!\n");
}

int onDetectedObject(const PPPoint* points, const size_t pointsSize, PPSize imageSize, PPDetectionStatus ds) {
    LOGD("Detection on image of size %dx%d has finished\n", imageSize.width, imageSize.height);
    LOGD("Total %zu points have been found\n", pointsSize);
    if(pointsSize>0 && points!=NULL) {
        for(size_t i=0; i<pointsSize; ++i) {
            LOGD("Point %zu location: (%d, %d)\n", i, points[i].x, points[i].y);
        }
    }
    const char* detStatusDesc = "";
    switch(ds) {
    case DETECTION_STATUS_CAMERA_AT_ANGLE:
        detStatusDesc = "camera is at angle";
        break;
    case DETECTION_STATUS_CAMERA_TOO_HIGH:
        detStatusDesc = "camera is too high";
        break;
    case DETECTION_STATUS_FAIL:
        detStatusDesc = "detection has failed";
        break;
    case DETECTION_STATUS_PARTIAL_OBJECT:
        detStatusDesc = "some parts of object are not visible";
        break;
    case DETECTION_STATUS_QR_SUCCESS:
        detStatusDesc = "QR code has been successfuly detected";
        break;
    case DETECTION_STATUS_SUCCESS:
        detStatusDesc = "detection has succeeded";
        break;
    }
    LOGD("Detection status: %s\n", detStatusDesc);
    return 1;
}

void onDetectionFailed() {
    LOGD("Detection has completely failed\n");
}

void onRecognitionStarted() {
    LOGD("Object recognition has started\n");
}

void onRecognitionFinished() {
    LOGD("Object recognition has finished\n");
}

RecognizerCallback buildRecognizerCallback() {
    RecognizerCallback cb;
    /* onDetectedObject is called when recognizer detects an object in image */
    cb.onDetectedObject = onDetectedObject;
    /* onDetectionStarted is called when recognizer begins object detection process */
    cb.onDetectionStarted = onDetectionStarted;
    /* onDetectionFailed is called when all enabled recognizers do not detect anything on the image */
    cb.onDetectionFailed = onDetectionFailed;
    /* onRecognitionStarted is called when recognition of detected object begins */
    cb.onRecognitionStarted = onRecognitionStarted;
    /* onRecognitionFinished is called when recognition of detected object finishes */
    cb.onRecognitionFinished = onRecognitionFinished;
    /* onProgress is called in some recognizers to provide progress information. Here we are not interested in this callback. */
    cb.onProgress = NULL;
    /* onShouldStopRecognition is called multiple times from some recognizers to check if recognition should be canceled. */
    cb.onShouldStopRecognition = NULL;
    return cb;
}

/*
 * Class:     com_microblink_api_RecognizerWrapper
 * Method:    init
 * Signature: ()V
 */
JNIEXPORT jstring JNICALL Java_com_microblink_api_RecognizerWrapper_init(JNIEnv* env, jobject) {

	/* create recognizer settings object. Do not forget to delete it after usage. */
    RecognizerSettings* settings;
    recognizerSettingsCreate(&settings);

    {
        /* create settings for PDF417 barcode recognizer */
        Pdf417Settings pdf417Sett;
        /* enable the usage of autoscale to automatically determine best parameters for high resolution images */
        pdf417Sett.useAutoScale = 1;
        /* allow scanning of damaged and incomplete PDF417 barcodes */
        pdf417Sett.shouldScanUncertain = 1;
        /* add PDF417 barcode recognizer settings to global recognizer settings object */
        recognizerSettingsSetPdf417Settings(settings, &pdf417Sett);
    }

    {
        /* create settings for ZXing barcode recognizer */
        ZXingSettings zxingSett;
        /* activate scanning of QR codes */
        zxingSett.scanQRCode = 1;
        /* add ZXing barcode recognizer settings to global recognizer settings object */
        recognizerSettingsSetZXingSettings(settings, &zxingSett);
    }

    {
        /* create settings for US Driver's License barcode recognizer */
        UsdlSettings usdlSett;
        /* enable the usage of autoscale to automatically determine best parameters for high resolution images */
        usdlSett.useAutoScale = 1;
        /* add US Driver's License barcode recognizer settings to global recognizer settings object */
        recognizerSettingsSetUsdlSettings(settings, &usdlSett);
    }

    /* insert license key and licensee */
    recognizerSettingsSetLicenseKey(settings, "add licensee here", "add license key here");
    Recognizer* recognizer;
    /* create global recognizer with settings */
    RecognizerErrorStatus status = recognizerCreate(&recognizer, settings);

    const char* errorStr = "";

    if(status!=RECOGNIZER_ERROR_STATUS_SUCCESS) {
        errorStr = recognizerErrorToString(status);
        LOGE("%s", errorStr);
    }

    recognizerSettingsDelete(&settings);

    recognizer_ = recognizer;

    return env->NewStringUTF(errorStr);
}

/*
 * Class:     com_microblink_api_RecognizerWrapper
 * Method:    terminate
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_microblink_api_RecognizerWrapper_terminate(JNIEnv*, jobject) {
    if(recognizer_ != NULL) {
        recognizerDelete(&recognizer_);
    }
}

/*
 * Class:     com_microblink_api_RecognizerWrapper
 * Method:    recognize
 * Signature: ([BII)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_microblink_api_RecognizerWrapper_recognize(JNIEnv* env, jobject thiz, jbyteArray bytes) {

    jbyte* buffer = env->GetByteArrayElements(bytes, 0);
    jsize bufferLen = env->GetArrayLength(bytes);

    RecognizerErrorStatus status;
    /* build recognizer callback structure */
    RecognizerCallback cb = buildRecognizerCallback();

    // define scanning region
//    PPRectangle roi;
//    roi.x = 0.f; roi.y=0.f; roi.width=0.5f; roi.height=1.f;
//    recognizerSetROI(recognizer_, &roi);

    RecognizerResultList* resultList;
    status = recognizerRecognizeFromEncodedImage(recognizer_, &resultList, buffer, bufferLen, &cb);
    env->ReleaseByteArrayElements(bytes, buffer, 0);

    if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
        char err[1000];
        sprintf(err, "Recognizer error %s", recognizerErrorToString(status));
        return env->NewStringUTF(err);
    }

    std::stringstream ss;

    size_t numResults;
    recognizerResultListGetNumOfResults(resultList, &numResults);

    for(size_t i = 0; i< numResults; ++i) {
		RecognizerResult* result;
		int isUsdl = 0;

		/* obtain i-th result from list */
		recognizerResultListGetResultAtIndex(resultList, i, &result);

		/* check if result is obtained from US Driver's License recognizer */
		status = recognizerResultIsUSDLResult(result, &isUsdl);
		if(status == RECOGNIZER_ERROR_STATUS_SUCCESS && isUsdl) {
			int valid = 0;

			/* first determine if USDL result is valid */
			recognizerResultIsResultValid(result, &valid);
			if(valid) {
				const char* firstName;
				const char* lastName;
				/* obtain first and last name of the driver */
				recognizerResultGetUSDLField(result, &firstName, USDLFieldKeys.kCustomerFirstName);
				recognizerResultGetUSDLField(result, &lastName, USDLFieldKeys.kCustomerFamilyName);

				LOGD("Driver's license belongs to %s %s\n", firstName, lastName);
				ss << "Type: US Driver's License" << std::endl << "First name: " << firstName << ", last name: " << lastName << std::endl << std::endl;
			} else {
				LOGD("Invalid driver license result!\n");
			}

		} else {
			const char* str;
			BarcodeType barcodeType;

			/* obtain the type of the barcode */
			status = recognizerResultGetBarcodeType(result, &barcodeType);

			if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
				char err[1000];
				sprintf(err, "Error obtaining barcode type: %s", recognizerErrorToString(status));
				return env->NewStringUTF(err);
			}

			/* obtain string version of data in barcode */
			status = recognizerResultGetBarcodeStringData(result, &str);

			if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
				char err[1000];
				sprintf(err, "Error obtaining barcode string data: %s", recognizerErrorToString(status));
				return env->NewStringUTF(err);
			}

			LOGD("%s Result: %s\n", barcodeTypeToString(barcodeType), str);

			ss << "Type: " << barcodeTypeToString(barcodeType) << std::endl << str << std::endl << std::endl;
		}
	}


    jstring jStr;
    // As of Android 4.4, you cannot blindly call NewStringUTF.
    // If given non-UTF8 string, ART will crash.
    std::string cppStr = ss.str();
    if(utf8::is_valid(cppStr.begin(), cppStr.end())) {
        jStr = env->NewStringUTF(cppStr.c_str());
    } else {
        // invalid UTF-8 string, attempt to convert literally
        std::vector<jchar> wChars;
        for(size_t i=0; i<cppStr.size(); ++i) {
            wChars.push_back((jchar)cppStr[i]);
        }
        jStr = env->NewString(&wChars[0], wChars.size());
    }

    recognizerResultListDelete(&resultList);

    return jStr;
}

#ifdef __cplusplus
}
#endif
