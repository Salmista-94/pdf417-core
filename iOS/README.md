# Integration instructions

The package contains a static library built for pdf417.mobi barcode scanning library. Also, a demo app is provided as a reference for the integration.

## Integration steps

1. Drag the pdf417-core-archive into the Frameworks Group in your Xcode project. The package consists of code, headers, resources and everything it needs to function properly.

2. Include the following frameworks and libraries into your project:
	- AVFoundation
	- CoreMedia
	- AssetsLibrary
	- libiconv.dylib
	
3. See the [documentation](doc/index.html) for code examples and API reference. You are ready to start coding!

## Example for scanning UIImage

The complete tutorial and API reference is in [documentation](doc/index.html). Here is only part that explains how to extract bytes from `UIImage` in order to present them to RecognizerAPI.    
    
In this example, `image` is UIImage. Of course, you can scan any other image object, but you have to know width and height of the image, it's buffer with pixels, image type (is it grayscale, BGRA, or some other), and number of bytes per each pixel row.

```objective-c
CGImageRef cgimage = image.CGImage;
    
size_t width  = CGImageGetWidth(cgimage);
size_t height = CGImageGetHeight(cgimage);
    
size_t bytesPerRow = CGImageGetBytesPerRow(cgimage);
size_t bitsPerPixel = CGImageGetBitsPerPixel(cgimage);
    
// This function returns the raw pixel values
CFDataRef dataRef = CGDataProviderCopyData(CGImageGetDataProvider(cgimage));
const UInt8 *rawPixelData = CFDataGetBytePtr(dataRef);
    	
// scan the raw grayscale image, and other types are possible
RawImageType imageType = RAW_IMAGE_TYPE_BGRA;
if (bitsPerPixel == 8) {
  imageType = RAW_IMAGE_TYPE_GRAY;
}
   
RecognizerResultList *resultList;
RecognizerErrorStatus scanStatus = recognizerRecognizeFromRawImage(recognizer, &resultList, rawPixelData,width, height, bytesPerRow, imageType, 0, NULL);
   
if (scanStatus == RECOGNIZER_ERROR_STATUS_SUCCESS) {
  // get the results
  
  // must delete the result object
  recognizerResultListDelete(&result);
} else {
  const char* statusString = recognizerErrorToString(scanStatus);
  printf("Recognize method returned status: %s\n", statusString);
  
  // handle error
}
   
CFRelease(dataRef);
```
        
To see the tutorial with examples in settings initialization, check [documentation](doc/index.html).
        
## Demo application

You can use the provided demo application to test the barcode scanning library.

You can test it in two ways:

- By taking a photo and performing a scan on that photo

- By opening an image (for example in or Dropbox app), choosing "Open in..." button and selecting "pdf417-core-demo" app.
        
## Additional info
 
For any inquiries, additional information or instructions please contact us at <support@microblink.com>.
    