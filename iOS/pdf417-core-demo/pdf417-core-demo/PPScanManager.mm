//
//  PPScanner.m
//  pdf417-core-demo
//
//  Created by Jura on 06/12/13.
//  Copyright (c) 2013 PhotoPay. All rights reserved.
//

#import "PPScanManager.h"
#import "RecognizerApi.h"
#import <string>

@implementation PPScanManager

+ (UIImage *)imageWithImage:(UIImage *)image scaledToSize:(CGSize)newSize {
    UIGraphicsBeginImageContextWithOptions(newSize, NO, 1.0);
    [image drawInRect:CGRectMake(0, 0, newSize.width, newSize.height)];
    UIImage *newImage = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    return newImage;
}

+ (NSString*)stringRepresentationOfData:(NSData*)data {
    
    NSMutableString* bytesString = [NSMutableString stringWithString:@"{"];
    
    const unsigned char* bytes = (const unsigned char*)[data bytes];
    NSUInteger length = [data length];
    
    for (int i = 0; i < length; ++i) {
        [bytesString appendFormat:@"%d", bytes[i]];
        
        // delimit bytes with comma
        if (i != length - 1) {
            [bytesString appendString:@", "];
        }
    }
    [bytesString appendString:@"}"];
    return bytesString;
}

+ (PPScanResult*)scanImage:(UIImage*)image {
    
    PPScanResult *scanResult = [[PPScanResult alloc] init];
    scanResult.text = @"Sorry, the scan failed. Please try again with some other image.";
    scanResult.bytes = @"";
    
    CGImageRef cgimage = image.CGImage;
    
    int width = (int) CGImageGetWidth(cgimage);
    int height = (int) CGImageGetHeight(cgimage);
    
    size_t bytesPerRow = CGImageGetBytesPerRow(cgimage);
    size_t bitsPerPixel = CGImageGetBitsPerPixel(cgimage);
    
    // This function returns the raw pixel values
    CFDataRef dataRef = CGDataProviderCopyData(CGImageGetDataProvider(cgimage));
    const UInt8 *rawPixelData = CFDataGetBytePtr(dataRef);
    
    // create settings object
    RecognizerSettings* settings;
    recognizerSettingsCreate(&settings);
    
    // set what is scanned
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

    // define license key
    recognizerSettingsSetLicenseKey(settings, "Add licensee here", "Add license key here");
    
    // create recognizer object
    Recognizer* recognizer;
    RecognizerErrorStatus status = recognizerCreate(&recognizer, settings);
    
    // Delete settings object
    recognizerSettingsDelete(&settings);
    
    if (status == RECOGNIZER_ERROR_STATUS_SUCCESS) {
        /** 2. Obtain the scan result */
        
        RecognizerResultList* resultList;
        
        // scan the raw grayscale image, other types are possible
        
        RawImageType imageType = RAW_IMAGE_TYPE_BGRA;
        if (bitsPerPixel == 8) {
            imageType = RAW_IMAGE_TYPE_GRAY;
        }
        
        RecognizerErrorStatus scanStatus = recognizerRecognizeFromRawImage(recognizer, &resultList, rawPixelData, width, height, bytesPerRow, imageType, 0, NULL);
        
        if (scanStatus == RECOGNIZER_ERROR_STATUS_SUCCESS) {
            /** 3. Get the results */
            
            size_t numResults;
            recognizerResultListGetNumOfResults(resultList, &numResults);
            
            NSMutableString* text = [[NSMutableString alloc] init];
            NSMutableString* bytes = [[NSMutableString alloc] init];
            
            for(size_t i = 0; i < numResults; ++i) {
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
                        
                        printf("Driver's license belongs to %s %s\n", firstName, lastName);

                        [text appendString:@"Type: US Driver's License\n"];
                        [text appendFormat:@"First name: %s, Last name: %s\n", firstName, lastName];
                        
                        /* obtain raw bytes */
                        const void* usdlBytes;
                        size_t usdlBytesSize = 0;
                        recognizerResultGetUSDLRawBinaryData(result, &usdlBytes, &usdlBytesSize);

                        [bytes appendFormat:@"Type: US Driver's License\n%@\n\n", [PPScanManager stringRepresentationOfData:[NSData dataWithBytes:usdlBytes length:usdlBytesSize]]];
                    } else {
                        printf("Invalid driver license result!\n");
                    }
                    
                } else {
                    const char* str;
                    BarcodeType barcodeType;
                    
                    /* obtain the type of the barcode */
                    status = recognizerResultGetBarcodeType(result, &barcodeType);
                    
                    if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
                        printf("Error obtaining barcode type: %s\n", recognizerErrorToString(status));
                        // handle error
                    }
                    
                    [text appendFormat:@"Type: %s\n", barcodeTypeToString(barcodeType)];
                    [bytes appendFormat:@"Type %s\n", barcodeTypeToString(barcodeType)];
                    
                    /* obtain string version of data in barcode */
                    status = recognizerResultGetBarcodeStringData(result, &str);
                    
                    if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
                        printf("Error obtaining barcode string data: %s\n", recognizerErrorToString(status));
                        // handle error
                    }
                    
                    printf("%s Result: %s\n", barcodeTypeToString(barcodeType), str);
                    NSString* barcodeText = [[NSString alloc] initWithCString:str encoding:NSUTF8StringEncoding];
                    if (!barcodeText) {
                        // if UTF8 wasn't correct encoding, try ASCII
                        barcodeText = [[NSString alloc] initWithCString:str encoding:NSASCIIStringEncoding];
                    }
                    
                    [text appendFormat:@"%@\n\n", barcodeText];
                    
                    /* obtain raw byte version of data in barcode */
                    const void* rawData;
                    size_t rawDataSize;
                    status = recognizerResultGetBarcodeRawData(result, &rawData, &rawDataSize);
                    if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
                        printf("Error obtaining barcode raw data: %s\n", recognizerErrorToString(status));
                        // handle error
                    }
                    
                    [bytes appendFormat:@"%@\n\n", [PPScanManager stringRepresentationOfData:[NSData dataWithBytes:rawData length:rawDataSize]]];
                }
            }
            
            scanResult.text = text;
            scanResult.bytes = bytes;
            
            // must delete the result list object (it will already be NULL if scanning has failed)
            recognizerResultListDelete(&resultList);
        } else {
            const char* statusString = recognizerErrorToString(scanStatus);
            printf("Recognize method returned status: %s\n", statusString);
            
            // handle error
        }
        
        /** 4. Delete recognizer object */
        recognizerDelete(&recognizer);
        
    } else {
        const char* statusString = recognizerErrorToString(status);
        printf("Recognizer create returned status: %s\n", statusString);
        
        // handle error
    }

    CFRelease(dataRef);
    
    return scanResult;
}

@end
