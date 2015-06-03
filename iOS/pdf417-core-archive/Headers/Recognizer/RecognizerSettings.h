/**
 * @file RecognizerSettings.h
 *
 *  Created on: Oct 9, 2013
 *      Authors: mkorpar, dodo
 */

#ifndef RECOGNIZERSETTINGS_H_
#define RECOGNIZERSETTINGS_H_

#include "RecognizerError.h"
#include "RecognizerDeviceInfo.h"
#include "Export.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 @struct RecognizerSettings
 @brief Recognizer settings data structure
 
 Used for initializing the Recognizer object.
 @see Recognizer.h
 */
typedef PP_EXPORTED_TYPE struct RecognizerSettings RecognizerSettings;

/**
 @memberof RecognizerSettings
 @brief Allocates and initializes the RecognizerSettings object.
 Example:
 @code
    RecognizerSettings* settings;
    recognizerSettingsCreate(&settings);
 @endcode

 @param     settings    Pointer to pointer referencing the created RecognizerSettings object
 @return    errorStatus status of the operation. Here it's always RECOGNIZER_ERROR_STATUS_SUCCESS
 */
PP_API RecognizerErrorStatus PP_CALL recognizerSettingsCreate(RecognizerSettings** settings);
    
/**
 @memberof RecognizerSettings
 @brief Deletes the RecognizerSettings object and sets the settings pointer to NULL.
 Example:
 @code
    RecognizerSettings* settings;
    recognizerSettingsCreate(&settings);
    recognizerSettingsDelete(&settings);
 @endcode
 
 @param     settings    Double Pointer to the RecognizerSettings object which is to be deleted
 @return    errorStatus status of the operation. Here it's always RECOGNIZER_ERROR_STATUS_SUCCESS, except when settings is NULL pointer when status is RECOGNIZER_ERROR_STATUS_POINTER_IS_NULL.
 */
PP_API RecognizerErrorStatus PP_CALL recognizerSettingsDelete(RecognizerSettings** settings);

/**
  @memberof RecognizerSettings
  @brief Sets the license key required for initialization of Recognizer object.

  @param     settings      Pointer to the RecognizerSettings object that holds all settings information.
  @param     licensee      You should enter here the licensee name to which license key is bound.
  @param     licenseKey    License key used for unlocking the library.
  @return    errorStatus   Status of the operation. Here it's always RECOGNIZER_ERROR_STATUS_SUCCESS, except when settings is NULL pointer when status is RECOGNIZER_ERROR_STATUS_POINTER_IS_NULL.
                           License check will be perfomed while creating recognizer object with recognizerCreate method, not here.
  */
PP_API RecognizerErrorStatus PP_CALL recognizerSettingsSetLicenseKey(RecognizerSettings* settings, const char* licensee, const char* licenseKey);

/**
  * @memberof RecognizerSettings
  * @brief Sets whether or not outputting of multiple scan results from same image is allowed.
  *
  * If that is true, it is possible to return multiple recognition results from same image (e.g. both PDF417 and QR code from same image can be decoded). By default, this
  * option is true. If you turn it off, then RecognizerResultList outputted by recognizerRecognize* functions will contain at most 1 result.
  * The upside of turning that option off is the speed - if you enable lots of recognizers, as soon as the first recognizer succeeds in
  * recognizing the object, function will return and other recognizers will not get a chance to analyze the image. The downside is that
  * you are then unable to obtain multiple results from single image.
  *
  * @param     settings      Pointer to the RecognizerSettings object that holds all settings information.
  * @param     outputMulti   true for enabling output of multiple results, false otherwise
  * @return    errorStatus   Status of the operation.
  */
PP_API RecognizerErrorStatus PP_CALL recognizerSettingsSetOutputMultipleResults(RecognizerSettings* settings, int outputMulti);

/**
  * @memberof RecognizerSettings
  * @brief Sets the device information.
  * Device information holds information about name of the device, number of processors for optimal multithreaded support, CPU frequency etc.
  * This method replaces old recognizerSettingsSetNumberOfProcessors function.
  * Example:
  * @code
  * RecognizerSettings* settings;
  * RecognizerDeviceInfo* deviceInfo;
  * recognizerSettingsCreate(&settings);
  * recognizerDeviceInfoCreate(&deviceInfo);
  * recognizerDeviceSetNumberOfProcessors(deviceInfo, 2);
  * recognizerSettingsSetDeviceInfo(settings, deviceInfo);
  * @endcode
  *
  * @param      settings    pointer to RecognizerSettings object that holds all settings information
  * @param      deviceInfo  pointer to RecognizerDeviceInfo object that holds all device information
  * @return     errorStatus status of the operation.
  */
PP_API RecognizerErrorStatus PP_CALL recognizerSettingsSetDeviceInfo(RecognizerSettings* settings, const RecognizerDeviceInfo* deviceInfo);



/**
 * @struct Pdf417Settings
 * @brief Settings for configuring PDF417 recognizer.
 */
struct Pdf417Settings {
    /**
     * Enables scanning of barcodes with inverse intensity values (e.g. white barcode on black background)
     */
    int shouldScanInverse;
    /**
     * Allow scanning barcodes which don't have quiet zone surrounding it (e.g. text
     * concatenated with barcode). This option can drastically reduce recognition time and
     * can even reduce the recognition quality of barcodes that do have quiet zone
     * surrounding it.
     */
    int nullQuietZoneAllowed;
    /**
     * Allow scanning of uncertain PDF417 barcodes. Uncertain scanning refers to returning result even on barcodes
     * which checksum does not match. Ideal for incomplete or damaged barcodes.
     */
    int shouldScanUncertain;
    /**
     * Allow enabling the autodetection of image scale when scanning barcodes. Enabling this can improve recognition
     * quality of high resolution images, at the cost of additional processing time.
     */
    int useAutoScale;

#ifdef __cplusplus
    /**
     * Default constructor for c++.
     */
    Pdf417Settings() :
        shouldScanInverse(0),
        nullQuietZoneAllowed(0),
        shouldScanUncertain(0),
        useAutoScale(0) {}
#endif
};

PP_EXPORTED_TYPE typedef struct Pdf417Settings Pdf417Settings;

/**
 * @memberof RecognizerSettings
 * @brief Set PDF417 recognizer settings to global settings object.
 * PDF417 recognizer settings structure defines how the PDF417 recognition will be performed. Set NULL to disable PDF417 recognizer.
 * @param settings pointer to RecognizerSettings object that holds all settings information
 * @param pdf417Settings pointer to PDF417 recognizer settings object that contains information about PDF417 recognition or NULL to disable PDF417 recognizer
 * @return status of the operation. If settings is NULL, status is RECOGNIZER_ERROR_STATUS_POINTER_IS_NULL, otherwise RECOGNIZER_ERROR_STATUS_SUCCESS.
 */
PP_API RecognizerErrorStatus PP_CALL recognizerSettingsSetPdf417Settings(RecognizerSettings* settings, const Pdf417Settings* pdf417Settings);



/**
 * @struct UsdlSettings
 * @brief Settings for configuring US Driver's License recognizer.
 */
struct UsdlSettings {
    /**
     * Allow scanning barcodes which don't have quiet zone surrounding it (e.g. text
     * concatenated with barcode). This option can drastically reduce recognition time and
     * can even reduce the recognition quality of barcodes that do have quiet zone
     * surrounding it.
     */
    int nullQuietZoneAllowed;
    /**
     * Allow scanning of uncertain PDF417 barcodes. Uncertain scanning refers to returning result even on barcodes
     * which checksum does not match. Ideal for incomplete or damaged barcodes.
     */
    int shouldScanUncertain;
    /**
     * Allow enabling the autodetection of image scale when scanning barcodes. Enabling this can improve recognition
     * quality of high resolution images, at the cost of additional processing time.
     */
    int useAutoScale;

#ifdef __cplusplus
    /**
     * Default constructor for c++.
     */
    UsdlSettings() :
        nullQuietZoneAllowed(0),
        shouldScanUncertain(0),
        useAutoScale(0) {}
#endif
};

PP_EXPORTED_TYPE typedef struct UsdlSettings UsdlSettings;

/**
 * @memberof RecognizerSettings
 * @brief Set US Driver's License recognizer settings to global settings object.
 * USDL recognizer settings structure defines how the US Driver's license recognition
 * will be performed. Set NULL to disable US Driver's License recognizer.
 * @param settings pointer to RecognizerSettings object that holds all settings information
 * @param usdlSettings pointer to USDL recognizer settings object that contains information about USDL recognition or NULL to disable US Driver's License recognizer
 * @return  status of the operation. If settings is NULL, status is RECOGNIZER_ERROR_STATUS_POINTER_IS_NULL, otherwise RECOGNIZER_ERROR_STATUS_SUCCESS. Please note that
 *          if license key does not allow usage of USDL recognizer, you will still get RECOGNIZER_ERROR_STATUS_SUCCESS because license check is performed while initializing
 *          the global recognizer object (function recognizerCreate(Recognizer**, const RecognizerSettings*) from Recognizer.h).
 */
PP_API RecognizerErrorStatus PP_CALL recognizerSettingsSetUsdlSettings(RecognizerSettings* settings, const UsdlSettings* usdlSettings);



/**
 * @struct BarDecoderSettings
 * @brief Settings for configuring Microblink's implementation of 1D barcode scanner.
 */
struct BarDecoderSettings {
    /**
     * Enables scanning of barcodes with inverse intensity values (e.g. white barcode on black background)
     */
    int shouldScanInverse;
    /**
     * Use this flag to use more processing time for barcode detection and decoding. With this flag enabled, better recognition quality can be expected.
     */
    int tryHarder;
    /**
     * Allow enabling the autodetection of image scale when scanning barcodes. Enabling this can improve recognition
     * quality of high resolution images, at the cost of additional processing time.
     */
    int useAutoScale;
    /**
     * Use this flag to enable reading of Code 39 1D barcodes.
     */
    int scanCode39;
    /**
     * Use this flag to enable reading of Code 128 1D barcodes.
     */
    int scanCode128;

#ifdef __cplusplus
    /** Default constructor for c++ */
    BarDecoderSettings() :
        shouldScanInverse(0),
        tryHarder(0),
        useAutoScale(0),
        scanCode39(0),
        scanCode128(0) {}
#endif
};

PP_EXPORTED_TYPE typedef struct BarDecoderSettings BarDecoderSettings;

/**
 * @memberof RecognizerSettings
 * @brief Set settings for 1D barcode recognizer (Microblink's implementation).
 * BarDecodeSettings structure defines how the 1D barcode recognizer (Microblink's implementation) will behave.
 * Set NULL to disable 1D barcode recognizer (Microblink's implementation).
 * @param settings pointer to RecognizerSettings object that holds all settings information
 * @param barDecoderSettings pointer to 1D barcode recognizer (Microblink's implementation) settings object that contains information how 1D barcode recognizer will behave or NULL
 *                           to disable 1D barcode recognizer (Microblink's implementation)
 * @return status of the operation. If settings is NULL, status is RECOGNIZER_ERROR_STATUS_POINTER_IS_NULL, otherwise RECOGNIZER_ERROR_STATUS_SUCCESS.
 */
PP_API RecognizerErrorStatus PP_CALL recognizerSettingsSetBarDecoderSettings(RecognizerSettings* settings, const BarDecoderSettings* barDecoderSettings);



/**
 * @struct ZXingSettings
 * @brief Settings for configuring ZXing implementation of barcode scanners.
 */
struct ZXingSettings {
    /**
     * Enables scanning of barcodes with inverse intensity values (e.g. white barcode on black background)
     */
    int shouldScanInverse;
    /**
     * Use this flag to enable reading of Aztec 2D barcodes.
     */
    int scanAztec;
    /**
     * Use this flag to enable reading of Code 128 1D barcodes.
     */
    int scanCode128;
    /**
     * Use this flag to enable reading of Code 39 1D barcodes.
     */
    int scanCode39;
    /**
     * Use this flag to enable reading of DataMatrix 2D barcodes.
     */
    int scanDataMatrix;
    /**
     * Use this flag to enable reading of EAN 13 1D barcodes.
     */
    int scanEAN13;
    /**
     * Use this flag to enable reading of EAN 8 1D barcodes.
     */
    int scanEAN8;
    /**
     * Use this flag to enable reading of ITF 1D barcodes.
     */
    int scanITF;
    /**
     * Use this flag to enable reading of QR codes.
     */
    int scanQRCode;
    /**
     * Use this flag to enable reading of UPC A 1D barcodes.
     */
    int scanUPCA;
    /**
     * Use this flag to enable reading of UPC E 1D barcodes.
     */
    int scanUPCE;

#ifdef __cplusplus
    /**
     * Default constructor for c++.
     */
    ZXingSettings() :
        shouldScanInverse(0),
        scanAztec(0),
        scanCode128(0),
        scanCode39(0),
        scanDataMatrix(0),
        scanEAN13(0),
        scanEAN8(0),
        scanITF(0),
        scanQRCode(0),
        scanUPCA(0),
        scanUPCE(0) {}
#endif
};

PP_EXPORTED_TYPE typedef struct ZXingSettings ZXingSettings;

/**
 * @memberof RecognizerSettings
 * @brief Set settings for ZXing barcode recognizer.
 * You should use ZXing barcode recognizer for all barcodes that Microblink does not have implementation for. ZXingSettings structure
 * defines how the ZXing barcode recognizer should behave. Set NULL to disable ZXing barcode recognizer.
 * @param settings pointer to RecognizerSettings object that holds all settings information
 * @param zxingSettings pointer to ZXing barcode recognizer settings object that contains information how ZXing barcode recognizer should behave or NULL to disable ZXing barcode recognizer
 * @return status of the operation. If settings is NULL, status is RECOGNIZER_ERROR_STATUS_POINTER_IS_NULL, otherwise RECOGNIZER_ERROR_STATUS_SUCCESS.
 */
PP_API RecognizerErrorStatus PP_CALL recognizerSettingsSetZXingSettings(RecognizerSettings* settings, const ZXingSettings* zxingSettings);



#ifdef __cplusplus
}
#endif

#endif
