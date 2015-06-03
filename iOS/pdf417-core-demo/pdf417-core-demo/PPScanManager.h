//
//  PPScanner.h
//  pdf417-core-demo
//
//  Created by Jura on 06/12/13.
//  Copyright (c) 2013 PhotoPay. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "PPScanResult.h"

@interface PPScanManager : NSObject

+ (PPScanResult*)scanImage:(UIImage*)image;

@end
