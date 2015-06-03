# Release notes

## 4.1.0
- new licence key format
- support for magnetic stripe standard in USDL barcodes
- improved parsing of some problematic US Driver's Licence barcodes

## 4.0.1
- fixed bug in new API that caused enabling of all ZXing readers if non-null `ZXingSettings` was set, but with everything disabled.
- if recognition result list is `NULL`, `recognizerResultListGetNumOfResults` sets its output parameter to `0`, instead of leaving it unchanged.
- calling `recognizerResultListDelete` with `NULL` result list no longer crashes

## 4.0.0
- new API that is more flexible in configuring various recognizers (see [documentation](doc/index.html) and demo apps for examples)
- custom implementation of Code128 and Code39 readers
- added API method for recognition of Aztec and DataMatrix 2D barcodes with ZXing recognizer
- added support for parsing US Driver's License barcodes (only if license key permits)
- various bugfixes in PDF417 recognition
- fixed bug in Code39 decoding
- lots of improvements in automatic scale detection in PDF417, Code39 and Code128 barcodes - much better performance on high resolution images

## 3.0.1
- fixed duplicate symbols linker error on iOS and Android when using ZXing library together with RecognizerApi

## 3.0.0
- support for obtaining multiple scan results from single image
- improvements in scanning algorithm
- API change - all recognition methods now return list of scan results
- support for defining information about device that executes the library (new function `recognizerSettingsSetDeviceInfo` replaces the old function `recognizerSettingsSetNumberOfProcessors`. See header code comments for detailed info).
- linux version is now distributed as dynamic library, just like windows and mac versions

## 2.0.0
- new license key format
- a simpler API for setting license key

## 1.3.3
- added .NET wrapper - initial version
- fixed frame quality estimation bug in Android

## 1.3.2
- Improvements in scanning uncertain barcodes
- Fixed crashing bug in uncertain barcode scanning

## 1.3.1
- added missing android files that were gitignored by mistake

## 1.3.0
- support for setting scanning ROI
- MacOS version is now distributed as framework

## 1.2.0
- support for automatic scale detection
- support for reading inverted barcodes
- support for reading barcodes that do not have quiet zone

## 1.1.0
- Added support for receiving callbacks from library
- Changed license format

## 1.0.1
- Adding trial license check

## 1.0.0
- Basic functionality